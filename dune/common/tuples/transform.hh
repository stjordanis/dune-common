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
   * \param tuple The tuple value to transform.
   * \param functor The functor to use to transform the values.
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



  // TransformTupleFunctor
  // ---------------------

  /** \brief helper class to implement transformTuple()
   *
   * \tparam TE   type evaluator class template
   * \tparam Args types of extra arguments to pass to \c TE<T>::apply()
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
  template< template< class > class TE, class ...Args >
  struct TransformTupleFunctor
  {
    //! export the type evaluator class template for genericTransformTuple()
    template< class T >
    struct TypeEvaluator
    : public TE< T >
    {};

    explicit TransformTupleFunctor ( const Dune::tuple< Args... > &args )
    : args_( args )
    {}

    explicit TransformTupleFunctor ( Args &...args )
    : args_( args... )
    {}

    /** \brief call \c TE< T >::apply( t, args... )
     *
     * This calls the static apply method of the TypeEvaluator class
     * template.
     *
     * \note There is no need to overload \c operator() with a \c const \c T&
     *       argument, since genericTransformTuple() will always use an lvalue
     *       argument.
     */
    template< class T >
    typename TypeEvaluator< T >::Type operator() ( T &t ) const
    {
      typedef typename EnumerationTuple< std::size_t, sizeof...( Args ) >::Type Enumeration;
      return apply( t, Enumeration() );
    }

  private:
    template< class T, class ...I >
    typename TypeEvaluator< T >::Type apply ( T &t, Dune::tuple< I... > ) const
    {
      return TypeEvaluator< T >::apply( t, Dune::get< I::value >( args_ )... );
    }

    Dune::tuple< Args... > args_;
  };



  // makeTransformTupleFunctor
  // -------------------------

  /** \brief syntactic sugar for creation of TransformTupleFunctor objects
   *
   * \code
   * #include <dune/common/tuples/transform.hh>
   * \endcode
   * \tparam  TE    type evaluator class template
   * \tparam  Args  types of extra arguments to pass to \c TE<T>::apply()
   *
   * \note It should not be necessary to specify the Args template parameters
   *       explicitly since they can be deduced by the compiler.
   *
   * \param  args  arguments to save references to in the TransformTupleFunctor
   */
  template< template< class > class TE, class ...Args >
  TransformTupleFunctor< TE, Args... >
  makeTransformTupleFunctor( Args &...args )
  {
    return TransformTupleFunctor< TE, Args... >( args... );
  }



  //! transform a tuple's value according to a user-supplied policy
  /**
   * \code
   * #include <dune/common/utility.hh>
   * \endcode
   * This function provides functionality similiar to genericTransformTuple(),
   * although less general and closer in spirit to ForEachType.
   *
   * \tparam  TE     type evaluator class template
   * \tparam  Tuple  type of tuple to transform
   * \tparam  Args   types of extra arguments to the transformation function
   *
   * \note The \c Tuple and \c Args template arguments can be deduced from the
   *       function arguments, so they can usually be omitted.
   *
   * \param  tuple  tuple to be transformed
   * \param  args   extra arguments to the transformation function
   *
   * The \c TE class template should be suitable as the \c TE template argument
   * for TransformTupleFunctor.
   * It has the following form (an extension of the \c TypeEvaluator template
   * argument of ForEachType):
   * \code
   * template< class T >
   * struct TypeEvaluator
   * {
   *   typedef UserDefined Type;
   *
   *   template< class ...Args >
   *   static Type apply ( T &t, Args &...args );
   * };
   * \endcode
   * For any given element type \c T of the tuple, the TypeEvaluator template
   * class should provide a member typedef \c Type which determines the type
   * of the transformed value and a static function \c apply(), taking the
   * value of the tuple element \c t and the extra arguments given to
   * transformTuple(). The signature of \c apply() does not have to match the
   * one given above exactly, as long as it can be called with the appropriate
   * number or arguments.
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
  template< template< class > class TE, class Tuple, class ...Args >
  typename remove_const< typename ForEachType< TE, Tuple >::Type >::type
  transformTuple ( Tuple &tuple, Args &...args )
  {
    return genericTransformTuple( tuple, makeTransformTupleFunctor< TE >( args... ) );
  }

  /** @} */

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_TRANSFORM_HH
