#ifndef DUNE_COMMON_TUPLES_FOREACH_HH
#define DUNE_COMMON_TUPLES_FOREACH_HH

#include <dune/common/documentation.hh>
#include <dune/common/forloop.hh>
#include <dune/common/static_assert.hh>

#include <dune/common/tuples/modifiers.hh>
#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  // ForEachType
  // -----------

  /** @ingroup Tuples_MetaProgramming
   *
   *  @brief Helper template to clone the type definition of a tuple with the
   *  storage types replaced by a user-defined rule.
   *
   *  Suppose all storage types A_i in a tuple define a type A_i::B. You can
   *  build up a pair consisting of the types defined by A_i::B in the following
   *  way:
   *  \code
   *  template <class A>
   *  struct MyEvaluator {
   *    typedef typename A::B Type;
   *  };
   *
   *  typedef ForEachType<MyEvaluator, ATuple>::Type BTuple;
   *  \endcode
   *  Here, MyEvaluator is a helper struct that extracts the correct type from
   *  the storage types of the tuple defined by the tuple ATuple.
   *
   *  \sa AddRefTypeEvaluator, AddPtrTypeEvaluator, genericTransformTuple(),
   *      and transformTuple().
   */
  template <template <class> class TypeEvaluator, class TupleType>
  class ForEachType {
    dune_static_assert(AlwaysFalse<TupleType>::value, "Attempt to use the "
                       "unspecialized version of ForEachType.  ForEachType "
                       "needs to be specialized for each possible tuple "
                       "size.  Naturally the number of pre-defined "
                       "specializations is limited arbitrarily.  Maybe you "
                       "need to raise this limit by defining some more "
                       "specializations?");
    struct ImplementationDefined {};
  public:
    //! type of the transformed tuple
    typedef ImplementationDefined Type;
  };

#ifndef DOXYGEN
  template <template <class> class TE, class Tuple>
  struct ForEachType<TE, const Tuple> {
    typedef const typename ForEachType<TE, Tuple>::Type Type;
  };

  template <template <class> class TE>
  struct ForEachType<TE, tuple<> > {
    typedef tuple<> Type;
  };

  template <template <class> class TE, class T0>
  struct ForEachType<TE, tuple<T0> > {
    typedef tuple<typename TE<T0>::Type> Type;
  };

  template <template <class> class TE, class T0, class T1>
  struct ForEachType<TE, tuple<T0, T1> > {
    typedef tuple<typename TE<T0>::Type, typename TE<T1>::Type> Type;
  };

  template <template <class> class TE, class T0, class T1, class T2>
  struct ForEachType<TE, tuple<T0, T1, T2> > {
    typedef tuple<typename TE<T0>::Type, typename TE<T1>::Type,
        typename TE<T2>::Type> Type;
  };

  template <template <class> class TE, class T0, class T1, class T2, class T3>
  struct ForEachType<TE, tuple<T0, T1, T2, T3> > {
    typedef tuple<typename TE<T0>::Type, typename TE<T1>::Type,
        typename TE<T2>::Type, typename TE<T3>::Type> Type;
  };

  template <template <class> class TE, class T0, class T1, class T2, class T3,
      class T4>
  struct ForEachType<TE, tuple<T0, T1, T2, T3, T4> > {
    typedef tuple<typename TE<T0>::Type, typename TE<T1>::Type,
        typename TE<T2>::Type, typename TE<T3>::Type,
        typename TE<T4>::Type> Type;
  };

  template <template <class> class TE, class T0, class T1, class T2, class T3,
      class T4, class T5>
  struct ForEachType<TE, tuple<T0, T1, T2, T3, T4, T5> > {
    typedef tuple<typename TE<T0>::Type, typename TE<T1>::Type,
        typename TE<T2>::Type, typename TE<T3>::Type,
        typename TE<T4>::Type, typename TE<T5>::Type> Type;
  };

  template <template <class> class TE, class T0, class T1, class T2, class T3,
      class T4, class T5, class T6>
  struct ForEachType<TE, tuple<T0, T1, T2, T3, T4, T5, T6> > {
    typedef tuple<typename TE<T0>::Type, typename TE<T1>::Type,
        typename TE<T2>::Type, typename TE<T3>::Type,
        typename TE<T4>::Type, typename TE<T5>::Type,
        typename TE<T6>::Type> Type;
  };

  template <template <class> class TE, class T0, class T1, class T2, class T3,
      class T4, class T5, class T6, class T7>
  struct ForEachType<TE, tuple<T0, T1, T2, T3, T4, T5, T6, T7> > {
    typedef tuple<typename TE<T0>::Type, typename TE<T1>::Type,
        typename TE<T2>::Type, typename TE<T3>::Type,
        typename TE<T4>::Type, typename TE<T5>::Type,
        typename TE<T6>::Type, typename TE<T7>::Type> Type;
  };

  template <template <class> class TE, class T0, class T1, class T2, class T3,
      class T4, class T5, class T6, class T7, class T8>
  struct ForEachType<TE, tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8> > {
    typedef tuple<typename TE<T0>::Type, typename TE<T1>::Type,
        typename TE<T2>::Type, typename TE<T3>::Type,
        typename TE<T4>::Type, typename TE<T5>::Type,
        typename TE<T6>::Type, typename TE<T7>::Type,
        typename TE<T8>::Type> Type;
  };

  // template <template <class> class TE, class T0, class T1, class T2, class T3,
  //           class T4, class T5, class T6, class T7, class T8, class T9>
  // struct ForEachType<TE, tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > {
  //   typedef tuple<typename TE<T0>::Type, typename TE<T1>::Type,
  //                 typename TE<T2>::Type, typename TE<T3>::Type,
  //                 typename TE<T4>::Type, typename TE<T5>::Type,
  //                 typename TE<T6>::Type, typename TE<T7>::Type,
  //                 typename TE<T8>::Type, typename TE<T9>::Type> Type;
  // };
#endif // !defined(DOXYGEN)



#ifndef DOXYGEN
  namespace
  {
    template<int i, typename T1,typename F>
    struct Visitor
    {
      static inline void visit(F& func, T1& t1)
      {
        func.visit(get<tuple_size<T1>::value-i>(t1));
        Visitor<i-1,T1,F>::visit(func, t1);
      }
    };

    template<typename T1,typename F>
    struct Visitor<0,T1,F>
    {
      static inline void visit(F& func, T1& t1)
      {}
    };

    template<int i, typename T1, typename T2,typename F>
    struct PairVisitor
    {
      static inline void visit(F& func, T1& t1, T2& t2)
      {
        func.visit(get<tuple_size<T1>::value-i>(t1), get<tuple_size<T2>::value-i>(t2));
        PairVisitor<i-1,T1,T2,F>::visit(func, t1, t2);
      }
    };

    template<typename T1, typename T2, typename F>
    struct PairVisitor<0,T1,T2,F>
    {
      static inline void visit(F& func, T1& t1, T2& t2)
      {}
    };
  }
#endif // #ifndef DOXYGEN



  // ForEachValue
  // ------------

  /** @ingroup Tuples_MetaProgramming
   *
   *  @brief Helper template which implements iteration over all storage
   *  elements in a tuple.
   *
   *  Compile-time constructs that allows to process all elements in a tuple.
   *  The exact operation performed on an element is defined by a function
   *  object, which needs to implement a visit method which is applicable to
   *  all storage elements of a tuple.  Each tuple element is visited once, and
   *  the iteration is done in ascending order.
   *
   *  The following example implements a function object which counts the
   *  elements in a tuple
   *  \code
   *  template <class T>
   *  struct Counter {
   *  Counter() : result_(0) {}
   *
   *  template <class T>
   *  void visit(T& elem) { ++result_; }
   *
   *  int result_;
   *  };
   *  \endcode
   *  The number of elements in the tuple are stored in the member variable
   *  result_. The Counter can be used as follows, assuming a tuple t of type
   *  MyTuple is given:
   *  \code
   *  Counter c;
   *  ForEachValue<MyTuple> forEach(t);
   *
   *  forEach.apply(c);
   *  std::cout << "Number of elements is: " << c.result_ << std::endl;
   *  \endcode
   */
  template <class TupleType>
  class ForEachValue {
  public:
    //! \brief Constructor
    //! \param tuple The tuple which we want to process.
    ForEachValue(TupleType& tuple) : tuple_(tuple) {}

    //! \brief Applies a function object to each storage element of the tuple.
    //! \param f Function object.
    template <class Functor>
    void apply(Functor& f) const {
      Visitor<tuple_size<TupleType>::value,TupleType,Functor>::visit(f, tuple_);
    }
  private:
    TupleType& tuple_;
  };



  // ForEachValuePair
  // ----------------

  //- Definition ForEachValuePair class
  // Assertion: both tuples have the same length and the contained types are
  // compatible in the sense of the applied function object
  /** @ingroup Tuples_MetaProgramming
   *
   *  @brief Extension of ForEachValue to two tuples...
   *
   *  This class provides the framework to process two tuples at once. It works
   *  the same as ForEachValue, just that the corresponding function object
   *  takes one argument from the first tuple and one argument from the second.
   *
   *  \note You have to ensure that the two tuples you provide are compatible
   *  in the sense that they have the same length and that the objects passed
   *  to the function objects are related in meaningful way. The best way to
   *  enforce it is to build the second tuple from the existing first tuple
   *  using ForEachType.
   */
  template <class TupleType1, class TupleType2>
  class ForEachValuePair {
  public:
    //! Constructor
    //! \param t1 First tuple.
    //! \param t2 Second tuple.
    ForEachValuePair(TupleType1& t1, TupleType2& t2) :
      tuple1_(t1),
      tuple2_(t2)
    {}

    //! Applies the function object f to the pair of tuples.
    //! \param f The function object to apply on the pair of tuples.
    template <class Functor>
    void apply(Functor& f) {
      PairVisitor<tuple_size<TupleType1>::value,TupleType1,TupleType2,Functor>
      ::visit(f, tuple1_, tuple2_);
    }
  private:
    TupleType1& tuple1_;
    TupleType2& tuple2_;
  };



#ifndef DOXYGEN

  namespace
  {

    // ForEachElementHelper
    // --------------------

    template< template< class, class > class Operation,
              template< int, class > class Value,
              class Enumeration,
              int length = tuple_size< Enumeration >::value
            >
    class ForEachElementHelper
    : public Operation<
        Value< 0, Enumeration >,
        ForEachElementHelper< Operation, Value, typename PopFrontTuple< Enumeration >::type >
      >
    {};

    template< template< class, class > class Operation,
              template< int, class > class Value,
              class Enumeration
            >
    class ForEachElementHelper< Operation, Value, Enumeration, 1 >
    : public Value< 0, Enumeration >
    {};

    template< template< class, class > class Operation,
              template< int, class > class Value,
              class Enumeration
            >
    class ForEachElementHelper< Operation, Value, Enumeration, 0 >
    {
    public:
      template< class... Args >
      static void apply( const Args&... args )
      {}
    };

  } // namespace

#endif // #ifndef DOXYGEN



  // ForEachElement
  // --------------

  /** \ingroup Tuples_MetaProgramming
   *
   *  \tparam  Operation    See documentation below.
   *  \tparam  Enumeration  A tuple of integral_constant.
   *
   *  The first template parameter is expected to provide the
   *  following method:
   *  @code
   *  template< int i, class Enumeration >
   *  struct Operation
   *  {
   *    template< class... Arguments >
   *    static void apply ( Arguments &... arguments );
   *  };
   *  @endcode
   *
   *  \note For setting up a tuple of integral constants, see Dune::IntegralConstantTuple.
   *        For a enumeration (a sequence) of Dune::integral_constant types, see
   *        Dune::EnumerationTuple.
   *
   *  \note The case <tt>Dune::tuple_size< Enumeration >::value == 0</tt>
   *        is explicitely allowed.
   */
  template< template< int, class > class Operation, class Enumeration >
  class ForEachElement
  : public ForEachElementHelper< ForLoopHelper::Apply, Operation, Enumeration >
  {};

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_FOREACH_HH
