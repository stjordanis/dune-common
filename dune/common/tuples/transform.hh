#ifndef DUNE_COMMON_TUPLES_TRANSFORM_HH
#define DUNE_COMMON_TUPLES_TRANSFORM_HH

#include <dune/common/typetraits.hh>

#include <dune/common/tuples/enumeration.hh>
#include <dune/common/tuples/foreach.hh>
#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  /** @addtogroup Tuples_MetaProgramming
   *
   * @{
   */

  //////////////////////////////////////////////////////////////////////
  //
  // genericTransformTuple stuff
  //

  // genericTransformTuple() needs to be overloaded for each tuple size (we
  // limit ourselves to tuple_size <= 10 here).  For a given tuple size it
  // needs to be overloaded for all combinations of const and non-const
  // argument references.  (On the one hand, we want to allow modifyable
  // arguments, so const references alone are not sufficient.  On the other
  // hand, we also want to allow rvalues (literals) as argument, which do not
  // bind to non-const references.)
  //
  // We can half the number of specializations required by introducing a
  // function genericTransformTupleBackend(), which is overloaded for each
  // tuple size and for const and non-const tuple arguments; the functor
  // argument is always given as as (non-const) reference.  When
  // genericTransformTupleBackend() is called, the type of the Functor template
  // parameter is the deduced as either "SomeType" or "const SomeType",
  // depending on whether the function argument is a non-const or a const
  // lvalue of type "SomeType".  As explained above, this does not work for
  // rvalues (i.e. literals).
  //
  // To make it work for literals of functors as well, we wrap the call to
  // genericTransformTupleBackend() in a function genericTransformTuple().
  // genericTransformTuple() needs to be overloaded for non-const and const
  // tuples and functors -- 4 overloads only.  Inside genericTransformTuple()
  // the functor is an lvalue no matter whether the argument was an lvalue or
  // an rvalue.  There is no need need to overload genericTransformTuple() for
  // all tuple sizes -- this is done by the underlying
  // genericTransformTupleBackend().

  // genericTransformTupleBackend() is an implementation detail -- hide it
  // from Doxygen
#ifndef DOXYGEN
  template< class Tuple, class Functor, class ...T >
  typename ForEachType< Functor::template TypeEvaluator, Tuple >::Type
  genericTransformTupleBackend ( Tuple &tuple, Functor &functor, Dune::tuple< T... > )
  {
    typedef typename ForEachType< Functor::template TypeEvaluator, Tuple >::Type Result;
    return Result( functor( Dune::get< T::value >( tuple ) )... );
  }

  template< class Tuple, class Functor, class ...T >
  typename ForEachType< Functor::template TypeEvaluator, Tuple >::Type
  genericTransformTupleBackend ( const Tuple &tuple, Functor &functor, Dune::tuple< T... > )
  {
    typedef typename ForEachType< Functor::template TypeEvaluator, Tuple >::Type Result;
    return Result( functor( Dune::get< T::value >( tuple ) )... );
  }
#endif // ! defined(DOXYGEN)



  // genericTransformTuple
  // ---------------------

  //! transform a tuple object into another tuple object
  /**
   * \code
   * #include <dune/common/utility.hh>
   * \endcode
   * This function does for the value of a tuple what ForEachType does for the
   * type of a tuple: it transforms the value using a user-provided policy
   * functor.
   *
   * \param t The tuple value to transform.
   * \param f The functor to use to transform the values.
   *
   * The functor should have the following form:
   * \code
   * struct Functor {
   *   template<class> struct TypeEvaluator {
   *     typedef user-defined Type;
   *   };
   *
   *   template<class T>
   *   typename TypeEvaluator<T>::Type operator()(T& val);
   *
   *   template<class T>
   *   typename TypeEvaluator<T>::Type operator()(T& val) const;
   *
   *   template<class T>
   *   typename TypeEvaluator<T>::Type operator()(const T& val);
   *
   *   template<class T>
   *   typename TypeEvaluator<T>::Type operator()(const T& val) const;
   * };
   * \endcode
   * The member class template \c TypeEvaluator should be a class template
   * suitable as the \c TypeEvaluator template parameter for ForEachType.  The
   * function call operator \c operator() is used to transform the value; only
   * the signatures of \c operator() which are actually used must be present.
   *
   * There are overloaded definitions of genericTransformTuple() wich take
   * constant tuple and functor arguments so rvalues are permissible as
   * arguments here.  These overloaded definitions are not documented
   * separately.
   */
  template< class Tuple, class Functor >
  typename ForEachType< Functor::template TypeEvaluator, Tuple >::Type
  genericTransformTuple ( Tuple &tuple, Functor &functor )
  {
    typedef typename EnumerationTuple< std::size_t, tuple_size< Tuple >::value >::Type Enumeration;
    return genericTransformTupleBackend( tuple, functor, Enumeration() );
  }

#ifndef DOXYGEN
  template< class Tuple, class Functor >
  typename ForEachType< Functor::template TypeEvaluator, Tuple >::Type
  genericTransformTuple ( const Tuple &tuple, Functor &functor )
  {
    typedef typename EnumerationTuple< std::size_t, tuple_size< Tuple >::value >::Type Enumeration;
    return genericTransformTupleBackend( tuple, functor, Enumeration() );
  }

  template< class Tuple, class Functor >
  typename ForEachType< Functor::template TypeEvaluator, Tuple >::Type
  genericTransformTuple ( Tuple &tuple, const Functor &functor )
  {
    typedef typename EnumerationTuple< std::size_t, tuple_size< Tuple >::value >::Type Enumeration;
    return genericTransformTupleBackend( tuple, functor, Enumeration() );
  }

  template< class Tuple, class Functor >
  typename ForEachType< Functor::template TypeEvaluator, Tuple >::Type
  genericTransformTuple ( const Tuple &tuple, const Functor &functor )
  {
    typedef typename EnumerationTuple< std::size_t, tuple_size< Tuple >::value >::Type Enumeration;
    return genericTransformTupleBackend( tuple, functor, Enumeration() );
  }
#endif // #ifndef DOXYGEN



  ////////////////////////////////////////////////////////////////////////
  //
  // transformTuple() related stuff
  //

  //! helper class to implement transformTuple()
  /**
   * \tparam TE TypeEvaluator class template.
   * \tparam An Type of extra arguments to pass to \c TE<T>::apply().  \c void
   *            means "no argument".  Only trailing arguments may be void.
   *
   * This class stores references to a number of arguments it receives in the
   * constructor.  Later, its function call operator \c operator() may be
   * called with a parameter \c t of type \c T.  \c operator() will then call
   * the static method \c TE<T>::apply(t,args...), where \c args... is the
   * sequence of arguments the object was constructed with.  \c operator()
   * will convert the result to type \c TE<T>::Type and return it.
   *
   * \c TE should be an extended version of the \c TypeEvaluator class
   * template parameter of ForEachType, for instance:
   * \code
   * template <class T>
   * struct TypeEvaluator {
   *   typedef T* Type;
   *   static Type apply(T& t, void* a0) {
   *     return t ? &t : static_cast<T*>(a0);
   *   }
   * };
   * \endcode
   * This example is for a TransformTupleFunctor with one argument, i.e. \c
   * A0!=void and all other \c An=void.  For the type transformation, it will
   * transform a value of some type T into a pointer to T.  For the value
   * transformation, it will take a reference to a value of type T and return
   * the pointer to that value, unless the value evaluates to false in boolean
   * context.  If the value evaluates to false, it will instead return the
   * pointer from the extra argument.
   */
  template<template<class> class TE, class A0 = void, class A1 = void,
      class A2 = void, class A3 = void, class A4 = void, class A5 = void,
      class A6 = void, class A7 = void, class A8 = void, class A9 = void>
  class TransformTupleFunctor {
    A0& a0; A1& a1; A2& a2; A3& a3; A4& a4; A5& a5; A6& a6; A7& a7; A8& a8;
    A9& a9;

  public:
    //! export the TypeEvaluator template class for genericTransformTuple()
    template<class T> struct TypeEvaluator : public TE<T> {};

    //! constructor
    /**
     * The actual number of arguments varies between specializations, the
     * actual number of arguments here is equal to the number of non-\c void
     * class template arguments \c An.
     */
    TransformTupleFunctor(A0& a0_, A1& a1_, A2& a2_, A3& a3_, A4& a4_, A5& a5_,
                          A6& a6_, A7& a7_, A8& a8_, A9& a9_)
      : a0(a0_), a1(a1_), a2(a2_), a3(a3_), a4(a4_), a5(a5_), a6(a6_), a7(a7_),
        a8(a8_), a9(a9_)
    { }

    //! call \c TE<T>::apply(t,args...)
    /**
     * This calls the static apply method of the TypeEvaluator class
     * template.
     *
     * \note There is no need to overload \c operator() with at \c const \c T&
     * argument, since genericTransformTuple() will always use an lvalue
     * argument.
     */
    template<class T>
    typename TE<T>::Type operator()(T& t) const {
      return TE<T>::apply(t, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
    }
  };

  //! syntactic sugar for creation of TransformTupleFunctor objects
  /**
   * \code
   * #include <dune/common/utility.hh>
   * \endcode
   * \tparam TE TypeEvaluator class template.
   * \tparam A0 Type of extra arguments to pass to \c TE<T>::apply().  It
   *            should not be necessary to specify these template parameters
   *            explicitly since they can be deduced.
   * \tparam A1 Type of extra arguments to pass to \c TE<T>::apply().  It
   *            should not be necessary to specify these template parameters
   *            explicitly since they can be deduced.
   * \tparam A2 Type of extra arguments to pass to \c TE<T>::apply().  It
   *            should not be necessary to specify these template parameters
   *            explicitly since they can be deduced.
   * \tparam A3 Type of extra arguments to pass to \c TE<T>::apply().  It
   *            should not be necessary to specify these template parameters
   *            explicitly since they can be deduced.
   * \tparam A4 Type of extra arguments to pass to \c TE<T>::apply().  It
   *            should not be necessary to specify these template parameters
   *            explicitly since they can be deduced.
   * \tparam A5 Type of extra arguments to pass to \c TE<T>::apply().  It
   *            should not be necessary to specify these template parameters
   *            explicitly since they can be deduced.
   * \tparam A6 Type of extra arguments to pass to \c TE<T>::apply().  It
   *            should not be necessary to specify these template parameters
   *            explicitly since they can be deduced.
   * \tparam A7 Type of extra arguments to pass to \c TE<T>::apply().  It
   *            should not be necessary to specify these template parameters
   *            explicitly since they can be deduced.
   * \tparam A8 Type of extra arguments to pass to \c TE<T>::apply().  It
   *            should not be necessary to specify these template parameters
   *            explicitly since they can be deduced.
   * \tparam A9 Type of extra arguments to pass to \c TE<T>::apply().  It
   *            should not be necessary to specify these template parameters
   *            explicitly since they can be deduced.
   *
   * \param a0 Arguments to save references to in the TransformTupleFunctor.
   * \param a1 Arguments to save references to in the TransformTupleFunctor.
   * \param a2 Arguments to save references to in the TransformTupleFunctor.
   * \param a3 Arguments to save references to in the TransformTupleFunctor.
   * \param a4 Arguments to save references to in the TransformTupleFunctor.
   * \param a5 Arguments to save references to in the TransformTupleFunctor.
   * \param a6 Arguments to save references to in the TransformTupleFunctor.
   * \param a7 Arguments to save references to in the TransformTupleFunctor.
   * \param a8 Arguments to save references to in the TransformTupleFunctor.
   * \param a9 Arguments to save references to in the TransformTupleFunctor.
   *
   * There are overloads of this function (not documented separately) for any
   * number of arguments, up to an implementation-defined arbitrary limit.
   * The number of arguments given determines the number of non-\c void
   * template arguments in the type of the returned TransformTupleFunctor.
   */
  template<template<class> class TE, class A0, class A1, class A2, class A3,
      class A4, class A5, class A6, class A7, class A8, class A9>
  TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9>
  makeTransformTupleFunctor(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5,
                            A6& a6, A7& a7, A8& a8, A9& a9) {
    return TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9>
             (a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
  }

#ifndef DOXYGEN
  // 0 argument
  template<template<class> class TE>
  struct TransformTupleFunctor<TE>
  {
    template<class T> struct TypeEvaluator : public TE<T> {};

    template<class T>
    typename TE<T>::Type operator()(T& t) const {
      return TE<T>::apply(t);
    }
  };
  template<template<class> class TE>
  TransformTupleFunctor<TE>
  makeTransformTupleFunctor() {
    return TransformTupleFunctor<TE>
             ();
  }

  // 1 argument
  template<template<class> class TE, class A0>
  class TransformTupleFunctor<TE, A0>
  {
    A0& a0;

  public:
    template<class T> struct TypeEvaluator : public TE<T> {};

    TransformTupleFunctor(A0& a0_)
      : a0(a0_)
    { }

    template<class T>
    typename TE<T>::Type operator()(T& t) const {
      return TE<T>::apply(t, a0);
    }
  };
  template<template<class> class TE, class A0>
  TransformTupleFunctor<TE, A0>
  makeTransformTupleFunctor(A0& a0) {
    return TransformTupleFunctor<TE, A0>
             (a0);
  }

  // 2 argument
  template<template<class> class TE, class A0, class A1>
  class TransformTupleFunctor<TE, A0, A1>
  {
    A0& a0; A1& a1;

  public:
    template<class T> struct TypeEvaluator : public TE<T> {};

    TransformTupleFunctor(A0& a0_, A1& a1_)
      : a0(a0_), a1(a1_)
    { }

    template<class T>
    typename TE<T>::Type operator()(T& t) const {
      return TE<T>::apply(t, a0, a1);
    }
  };
  template<template<class> class TE, class A0, class A1>
  TransformTupleFunctor<TE, A0, A1>
  makeTransformTupleFunctor(A0& a0, A1& a1) {
    return TransformTupleFunctor<TE, A0, A1>
             (a0, a1);
  }

  // 3 arguments
  template<template<class> class TE, class A0, class A1, class A2>
  class TransformTupleFunctor<TE, A0, A1, A2>
  {
    A0& a0; A1& a1; A2& a2;

  public:
    template<class T> struct TypeEvaluator : public TE<T> {};

    TransformTupleFunctor(A0& a0_, A1& a1_, A2& a2_)
      : a0(a0_), a1(a1_), a2(a2_)
    { }

    template<class T>
    typename TE<T>::Type operator()(T& t) const {
      return TE<T>::apply(t, a0, a1, a2);
    }
  };
  template<template<class> class TE, class A0, class A1, class A2>
  TransformTupleFunctor<TE, A0, A1, A2>
  makeTransformTupleFunctor(A0& a0, A1& a1, A2& a2) {
    return TransformTupleFunctor<TE, A0, A1, A2>
             (a0, a1, a2);
  }

  // 4 arguments
  template<template<class> class TE, class A0, class A1, class A2, class A3>
  class TransformTupleFunctor<TE, A0, A1, A2, A3>
  {
    A0& a0; A1& a1; A2& a2; A3& a3;

  public:
    template<class T> struct TypeEvaluator : public TE<T> {};

    TransformTupleFunctor(A0& a0_, A1& a1_, A2& a2_, A3& a3_)
      : a0(a0_), a1(a1_), a2(a2_), a3(a3_)
    { }

    template<class T>
    typename TE<T>::Type operator()(T& t) const {
      return TE<T>::apply(t, a0, a1, a2, a3);
    }
  };
  template<template<class> class TE, class A0, class A1, class A2, class A3>
  TransformTupleFunctor<TE, A0, A1, A2, A3>
  makeTransformTupleFunctor(A0& a0, A1& a1, A2& a2, A3& a3) {
    return TransformTupleFunctor<TE, A0, A1, A2, A3>
             (a0, a1, a2, a3);
  }

  // 5 arguments
  template<template<class> class TE, class A0, class A1, class A2, class A3,
      class A4>
  class TransformTupleFunctor<TE, A0, A1, A2, A3, A4>
  {
    A0& a0; A1& a1; A2& a2; A3& a3; A4& a4;

  public:
    template<class T> struct TypeEvaluator : public TE<T> {};

    TransformTupleFunctor(A0& a0_, A1& a1_, A2& a2_, A3& a3_, A4& a4_)
      : a0(a0_), a1(a1_), a2(a2_), a3(a3_), a4(a4_)
    { }

    template<class T>
    typename TE<T>::Type operator()(T& t) const {
      return TE<T>::apply(t, a0, a1, a2, a3, a4);
    }
  };
  template<template<class> class TE, class A0, class A1, class A2, class A3,
      class A4>
  TransformTupleFunctor<TE, A0, A1, A2, A3, A4>
  makeTransformTupleFunctor(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4) {
    return TransformTupleFunctor<TE, A0, A1, A2, A3, A4>
             (a0, a1, a2, a3, a4);
  }

  // 6 arguments
  template<template<class> class TE, class A0, class A1, class A2, class A3,
      class A4, class A5>
  class TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5>
  {
    A0& a0; A1& a1; A2& a2; A3& a3; A4& a4; A5& a5;

  public:
    template<class T> struct TypeEvaluator : public TE<T> {};

    TransformTupleFunctor(A0& a0_, A1& a1_, A2& a2_, A3& a3_, A4& a4_, A5& a5_)
      : a0(a0_), a1(a1_), a2(a2_), a3(a3_), a4(a4_), a5(a5_)
    { }

    template<class T>
    typename TE<T>::Type operator()(T& t) const {
      return TE<T>::apply(t, a0, a1, a2, a3, a4, a5);
    }
  };
  template<template<class> class TE, class A0, class A1, class A2, class A3,
      class A4, class A5>
  TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5>
  makeTransformTupleFunctor(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5) {
    return TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5>
             (a0, a1, a2, a3, a4, a5);
  }

  // 7 arguments
  template<template<class> class TE, class A0, class A1, class A2, class A3,
      class A4, class A5, class A6>
  class TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5, A6>
  {
    A0& a0; A1& a1; A2& a2; A3& a3; A4& a4; A5& a5; A6& a6;

  public:
    template<class T> struct TypeEvaluator : public TE<T> {};

    TransformTupleFunctor(A0& a0_, A1& a1_, A2& a2_, A3& a3_, A4& a4_, A5& a5_,
                          A6& a6_)
      : a0(a0_), a1(a1_), a2(a2_), a3(a3_), a4(a4_), a5(a5_), a6(a6_)
    { }

    template<class T>
    typename TE<T>::Type operator()(T& t) const {
      return TE<T>::apply(t, a0, a1, a2, a3, a4, a5, a6);
    }
  };
  template<template<class> class TE, class A0, class A1, class A2, class A3,
      class A4, class A5, class A6>
  TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5, A6>
  makeTransformTupleFunctor(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5,
                            A6& a6) {
    return TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5, A6>
             (a0, a1, a2, a3, a4, a5, a6);
  }

  // 8 arguments
  template<template<class> class TE, class A0, class A1, class A2, class A3,
      class A4, class A5, class A6, class A7>
  class TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5, A6, A7>
  {
    A0& a0; A1& a1; A2& a2; A3& a3; A4& a4; A5& a5; A6& a6; A7& a7;

  public:
    template<class T> struct TypeEvaluator : public TE<T> {};

    TransformTupleFunctor(A0& a0_, A1& a1_, A2& a2_, A3& a3_, A4& a4_, A5& a5_,
                          A6& a6_, A7& a7_)
      : a0(a0_), a1(a1_), a2(a2_), a3(a3_), a4(a4_), a5(a5_), a6(a6_), a7(a7_)
    { }

    template<class T>
    typename TE<T>::Type operator()(T& t) const {
      return TE<T>::apply(t, a0, a1, a2, a3, a4, a5, a6, a7);
    }
  };
  template<template<class> class TE, class A0, class A1, class A2, class A3,
      class A4, class A5, class A6, class A7>
  TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5, A6, A7>
  makeTransformTupleFunctor(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5,
                            A6& a6, A7& a7) {
    return TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5, A6, A7>
             (a0, a1, a2, a3, a4, a5, a6, a7);
  }

  // 9 arguments
  template<template<class> class TE, class A0, class A1, class A2, class A3,
      class A4, class A5, class A6, class A7, class A8>
  class TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5, A6, A7, A8>
  {
    A0& a0; A1& a1; A2& a2; A3& a3; A4& a4; A5& a5; A6& a6; A7& a7; A8& a8;

  public:
    template<class T> struct TypeEvaluator : public TE<T> {};

    TransformTupleFunctor(A0& a0_, A1& a1_, A2& a2_, A3& a3_, A4& a4_, A5& a5_,
                          A6& a6_, A7& a7_, A8& a8_)
      : a0(a0_), a1(a1_), a2(a2_), a3(a3_), a4(a4_), a5(a5_), a6(a6_), a7(a7_),
        a8(a8_)
    { }

    template<class T>
    typename TE<T>::Type operator()(T& t) const {
      return TE<T>::apply(t, a0, a1, a2, a3, a4, a5, a6, a7, a8);
    }
  };
  template<template<class> class TE, class A0, class A1, class A2, class A3,
      class A4, class A5, class A6, class A7, class A8>
  TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5, A6, A7, A8>
  makeTransformTupleFunctor(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5,
                            A6& a6, A7& a7, A8& a8) {
    return TransformTupleFunctor<TE, A0, A1, A2, A3, A4, A5, A6, A7, A8>
             (a0, a1, a2, a3, a4, a5, a6, a7, a8);
  }
#endif // ! defined(DOXYGEN)

  //! transform a tuple's value according to a user-supplied policy
  /**
   * \code
   * #include <dune/common/utility.hh>
   * \endcode
   * This function provides functionality similiar to genericTransformTuple(),
   * although less general and closer in spirit to ForEachType.
   *
   * \tparam TypeEvaluator Used as the \c TE template argument to
   *                       TransformTupleFunctor internally.
   * \tparam Tuple         Type of the tuple to transform.
   * \tparam A0            Types of extra argument to call the transformation
   *                       function with.
   * \tparam A1            Types of extra argument to call the transformation
   *                       function with.
   * \tparam A2            Types of extra argument to call the transformation
   *                       function with.
   * \tparam A3            Types of extra argument to call the transformation
   *                       function with.
   * \tparam A4            Types of extra argument to call the transformation
   *                       function with.
   * \tparam A5            Types of extra argument to call the transformation
   *                       function with.
   * \tparam A6            Types of extra argument to call the transformation
   *                       function with.
   * \tparam A7            Types of extra argument to call the transformation
   *                       function with.
   * \tparam A8            Types of extra argument to call the transformation
   *                       function with.
   * \tparam A9            Types of extra argument to call the transformation
   *                       function with.
   *
   * \note The \c Tuple and \c An template arguments can be deduced from the
   *       function arguments, so they can usually be omitted.
   *
   * \param orig Tuple value to be transformed.
   * \param a0   Extra argument values to provide to the transformation
   *             function.
   * \param a1   Extra argument values to provide to the transformation
   *             function.
   * \param a2   Extra argument values to provide to the transformation
   *             function.
   * \param a3   Extra argument values to provide to the transformation
   *             function.
   * \param a4   Extra argument values to provide to the transformation
   *             function.
   * \param a5   Extra argument values to provide to the transformation
   *             function.
   * \param a6   Extra argument values to provide to the transformation
   *             function.
   * \param a7   Extra argument values to provide to the transformation
   *             function.
   * \param a8   Extra argument values to provide to the transformation
   *             function.
   * \param a9   Extra argument values to provide to the transformation
   *             function.
   *
   * This function is overloaded for any number of extra arguments, up to an
   * implementation-defined arbitrary limit.  The overloads are not documented
   * separately.
   *
   * The \c TypeEvaluator class template should be suitable as the \c TE
   * template argument for TransformTupleFunctor.  It has the following form
   * (an extension of the \c TypeEvaluator template argument of ForEachType):
   * \code
   * template <class T>
   * struct TypeEvaluator {
   *   typedef UserDefined Type;
   *
   *   template<class A0, class A1, class A2, class A3, class A4, class A5,
   *            class A6, class A7, class A8, class A9>
   *   static Type apply(T& t, A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5,
   *                     A6& a6, A7& a7, A8& a8, A9& a9);
   * };
   * \endcode
   * For any given element type \c T of the tuple, the TypeEvaluator template
   * class should provide a member typedef \c Type which determines the type
   * of the transformed value and a static function \c apply(), taking the
   * value of the tuple element \c t and the extra arguments given to
   * transformTuple().  The signature of \c apply() does not have to match the
   * one given above exactly, as long as it can be called that way.
   *
   * \note Since transformTuple() takes non-const references to the extra
   *       arguments, it will only bind to lvalue extra arguments, unless you
   *       specify the corresconding template parameter as \c const \c
   *       SomeType.  Specifically this meands that you cannot simply use
   *       literals or function return values as extra arguments. Providing
   *       overloads for all possible combinations of rvalue and lvalue extra
   *       arguments would result in \f$2^{n+1}-1\f$ overloads where \f$n\f$
   *       is the implementation defined limit in the number of extra
   *       arguments.
   *
   * \sa genericTransforTuple(), ForEachType, AddRefTypeEvaluator, and
   *     AddPtrTypeEvaluator.
   */
  template<template<class> class TypeEvaluator, class Tuple, class A0,
      class A1, class A2, class A3, class A4, class A5, class A6,
      class A7, class A8, class A9>
  typename remove_const<typename ForEachType<TypeEvaluator, Tuple>::Type>::type
  transformTuple(Tuple& orig, A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5,
                 A6& a6, A7& a7, A8& a8, A9& a9) {
    return genericTransformTuple
             ( orig,
             makeTransformTupleFunctor<TypeEvaluator>(a0, a1, a2, a3, a4, a5, a6,
                                                      a7, a8, a9));
  }

#ifndef DOXYGEN
  // 0 extra arguments
  template<template<class> class TypeEvaluator, class Tuple>
  typename remove_const<typename ForEachType<TypeEvaluator, Tuple>::Type>::type
  transformTuple(Tuple& orig) {
    return genericTransformTuple
             ( orig,
             makeTransformTupleFunctor<TypeEvaluator>());
  }

  // 1 extra argument
  template<template<class> class TypeEvaluator, class Tuple, class A0>
  typename remove_const<typename ForEachType<TypeEvaluator, Tuple>::Type>::type
  transformTuple(Tuple& orig, A0& a0) {
    return genericTransformTuple
             ( orig,
             makeTransformTupleFunctor<TypeEvaluator>(a0));
  }

  // 2 extra arguments
  template<template<class> class TypeEvaluator, class Tuple, class A0,
      class A1>
  typename remove_const<typename ForEachType<TypeEvaluator, Tuple>::Type>::type
  transformTuple(Tuple& orig, A0& a0, A1& a1) {
    return genericTransformTuple
             ( orig,
             makeTransformTupleFunctor<TypeEvaluator>(a0, a1));
  }

  // 3 extra arguments
  template<template<class> class TypeEvaluator, class Tuple, class A0,
      class A1, class A2>
  typename remove_const<typename ForEachType<TypeEvaluator, Tuple>::Type>::type
  transformTuple(Tuple& orig, A0& a0, A1& a1, A2& a2) {
    return genericTransformTuple
             ( orig,
             makeTransformTupleFunctor<TypeEvaluator>(a0, a1, a2));
  }

  // 4 extra arguments
  template<template<class> class TypeEvaluator, class Tuple, class A0,
      class A1, class A2, class A3>
  typename remove_const<typename ForEachType<TypeEvaluator, Tuple>::Type>::type
  transformTuple(Tuple& orig, A0& a0, A1& a1, A2& a2, A3& a3) {
    return genericTransformTuple
             ( orig,
             makeTransformTupleFunctor<TypeEvaluator>(a0, a1, a2, a3));
  }

  // 5 extra arguments
  template<template<class> class TypeEvaluator, class Tuple, class A0,
      class A1, class A2, class A3, class A4>
  typename remove_const<typename ForEachType<TypeEvaluator, Tuple>::Type>::type
  transformTuple(Tuple& orig, A0& a0, A1& a1, A2& a2, A3& a3, A4& a4) {
    return genericTransformTuple
             ( orig,
             makeTransformTupleFunctor<TypeEvaluator>(a0, a1, a2, a3, a4));
  }

  // 6 extra arguments
  template<template<class> class TypeEvaluator, class Tuple, class A0,
      class A1, class A2, class A3, class A4, class A5>
  typename remove_const<typename ForEachType<TypeEvaluator, Tuple>::Type>::type
  transformTuple(Tuple& orig, A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5) {
    return genericTransformTuple
             ( orig,
             makeTransformTupleFunctor<TypeEvaluator>(a0, a1, a2, a3, a4, a5));
  }

  // 7 extra arguments
  template<template<class> class TypeEvaluator, class Tuple, class A0,
      class A1, class A2, class A3, class A4, class A5, class A6>
  typename remove_const<typename ForEachType<TypeEvaluator, Tuple>::Type>::type
  transformTuple(Tuple& orig, A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5,
                 A6& a6) {
    return genericTransformTuple
             ( orig,
             makeTransformTupleFunctor<TypeEvaluator>(a0, a1, a2, a3, a4, a5, a6));
  }

  // 8 extra arguments
  template<template<class> class TypeEvaluator, class Tuple, class A0,
      class A1, class A2, class A3, class A4, class A5, class A6,
      class A7>
  typename remove_const<typename ForEachType<TypeEvaluator, Tuple>::Type>::type
  transformTuple(Tuple& orig, A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5,
                 A6& a6, A7& a7) {
    return genericTransformTuple
             ( orig,
             makeTransformTupleFunctor<TypeEvaluator>(a0, a1, a2, a3, a4, a5, a6,
                                                      a7));
  }

  // 9 extra arguments
  template<template<class> class TypeEvaluator, class Tuple, class A0,
      class A1, class A2, class A3, class A4, class A5, class A6,
      class A7, class A8>
  typename remove_const<typename ForEachType<TypeEvaluator, Tuple>::Type>::type
  transformTuple(Tuple& orig, A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5,
                 A6& a6, A7& a7, A8& a8) {
    return genericTransformTuple
             ( orig,
             makeTransformTupleFunctor<TypeEvaluator>(a0, a1, a2, a3, a4, a5, a6,
                                                      a7, a8));
  }
#endif // not defined(DOXYGEN)

  /** @} */

}

#endif // #ifndef DUNE_COMMON_TUPLES_TRANSFORM_HH
