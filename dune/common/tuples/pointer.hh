#ifndef DUNE_COMMON_TUPLES_POINTER_HH
#define DUNE_COMMON_TUPLES_POINTER_HH

#include <dune/common/static_assert.hh>
#include <dune/common/typetraits.hh>

#include <dune/common/tuples/checkpredicate.hh>
#include <dune/common/tuples/foreach.hh>
#include <dune/common/tuples/modifiers.hh>
#include <dune/common/tuples/transform.hh>
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
      static bool apply ( const Tuple &tuple )
      {
        return check_predicate_tuple< ValidPointerPredicate >( tuple );
      }
    };

  } // namespace

#endif // #ifndef DOXYGEN



  /** \ingroup Tuples_Init
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
  bool valid_pointer_tuple ( const Tuple &tuple )
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
   */
  template< class Tuple >
  class DereferenceTuple;

  template< class... T >
  class DereferenceTuple< Dune::tuple< T... > >
  {
    struct Functor
    {
      template< class U >
      struct TypeEvaluator
      {
        typedef typename TypeTraits< U >::PointeeType & Type;
      };

      template< class U >
      typename TypeEvaluator< U * >::Type operator() ( U *u ) const
      {
        return *u;
      }
    };

  public:
    typedef typename ForEachType< Functor::template TypeEvaluator, Dune::tuple< T... > >::Type Type;

    static Type apply ( Dune::tuple< T... > &tuple )
    {
      return genericTransformTuple( tuple, Functor() );
    }
  };



  // dereference_tuple
  // -----------------

  /** \ingroup Tuples_Init
   *
   *  \brief Dereference pointer tuple and return
   *         tuple of references.
   *  \tparam Tuple A tuple consisting of pointer types
   *
   *  \param[in]  tuple  tuple of pointers
   */
  template< class Tuple >
  typename Dune::DereferenceTuple< Tuple >::Type dereference_tuple ( Tuple &tuple )
  {
    return Dune::DereferenceTuple< Tuple >::apply( tuple );
  }

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_POINTER_HH
