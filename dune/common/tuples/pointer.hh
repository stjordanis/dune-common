#ifndef DUNE_COMMON_TUPLES_POINTER_HH
#define DUNE_COMMON_TUPLES_POINTER_HH

#include <dune/common/static_assert.hh>
#include <dune/common/typetraits.hh>

#include <dune/common/tuples/checkpredicate.hh>
#include <dune/common/tuples/foreach.hh>
#include <dune/common/tuples/modifiers.hh>
#include <dune/common/tuples/tuples.hh>
#include <dune/common/tuples/typetraits.hh>

namespace Dune
{

  // PointerPairDeletor
  // ------------------

  /**
   * @brief Deletes all objects pointed to in a tuple of pointers.
   *
   * \warning Pointers cannot be set to NULL, so calling the Deletor twice
   * or accessing elements of a deleted tuple leads to unforeseeable results!
   */
  template <class Tuple>
  class PointerPairDeletor
  {
    struct Deletor {
      template<typename P> void visit(const P& p) { delete p; }
    };

  public:
    static void apply(Tuple& t) {
      static Deletor deletor;
      ForEachValue<Tuple>(t).apply(deletor);
    }
  };



#ifndef DOXYGEN

  namespace
  {

    // ValidPointerTupleHelper
    // -----------------------

    template< class Tuple >
    class ValidPointerTupleHelper
    {
      dune_static_assert( TupleTypeTraits< Tuple >::isPointerTuple,
                          "Can not check non-pointer tuple." );

      struct ValidPointerPredicate
      {
        template< class T >
        bool operator() ( T &x )
        {
          return x;
        }
      };

    public:
      static bool apply ( Tuple tuple )
      {
        return check_predicate_tuple< ValidPointerPredicate >( tuple );
      }
    };

  } // namespace

#endif // #ifndef DOXYGEN



  /** \ingroup Tuple_Init
   *
   *  \brief Check whether a pointer tuple can be dereferenced.
   *
   *  \tparam  Tuple  tuple of pointer types
   *
   *  \param[in]  tuple  tuple of pointers
   *
   *  \returns \b true if all elements of \c tuple are non-zero, \b false otherwise
   */
  template< class Tuple >
  bool valid_pointer_tuple ( Tuple tuple )
  {
    return ValidPointerTupleHelper< Tuple >::apply( tuple );
  }



#ifndef DOXYGEN

  namespace
  {

    // ReferenceTuple
    // --------------

    template< class Tuple >
    class ReferenceTuple
    {
      template< class T >
      struct ReferenceEvaluator
      {
        typedef typename Dune::TypeTraits< T >::ReferredType & Type;
      };

    public:
      typedef typename Dune::ForEachType< ReferenceEvaluator, Tuple >::Type Type;
    };

  }  // namespace

#endif // #ifndef DOXYGEN



  // DereferenceTuple
  // ----------------

  /** \ingroup Tuples_Init
   *
   *  \brief Dereference pointer tuple and return
   *         tuple of references.
   *
   *  \tparam Tuple A tuple consisting of pointer types
   *  \tparam Seed  implementation internal
   *  \tparam index implementation internal
   *  \tparam size  implementation internal
   */
  template< class Tuple,
            class Seed = Dune::tuple<>,
            int index = 0,
            int size = Dune::tuple_size< Tuple >::value
          >
  class DereferenceTuple
  {
    template< class, class, int, int > friend class DereferenceTuple;

    typedef typename Dune::TypeTraits< typename Dune::tuple_element< index, Tuple >::type >::PointeeType & AppendType;
    typedef typename Dune::PushBackTuple< Seed, AppendType >::type AccumulatedType;

    typedef DereferenceTuple< Tuple, AccumulatedType, (index+1), size > NextType;

  public:
    typedef typename Dune::ReferenceTuple<
        typename Dune::TupleTypeTraits< Tuple >::PointeeTupleType
      >::Type Type;

    static Type apply ( Tuple &tuple )
    {
      Seed seed;
      return append( tuple, seed );
    }

  private:
    static Type append ( Tuple &tuple, Seed &seed )
    {
      typename Dune::tuple_element< index, Tuple >::type pointer = Dune::get< index >( tuple );
      AppendType append = *pointer;
      AccumulatedType next = Dune::tuple_push_back< AppendType >( seed, append );
      return NextType::append( tuple, next );
    }
  };

  template< class Tuple,
            class Seed,
            int size
          >
  class DereferenceTuple< Tuple, Seed, size, size >
  {
    template< class, class, int, int > friend class DereferenceTuple;

  public:
    typedef typename Dune::ReferenceTuple<
        typename Dune::TupleTypeTraits< Tuple >::PointeeTupleType
      >::Type Type;

    dune_static_assert( (Dune::is_same< Seed, Type >::value), "Failed to dereference pointer tuple." );

    static Type apply ( Tuple & )
    {
      return Type();
    }

  private:
    static Seed append ( Tuple &tuple, Seed &seed ) { return seed; }
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_POINTER_HH
