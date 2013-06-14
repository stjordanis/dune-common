#ifndef DUNE_COMMON_TUPLES_TYPETRAITS_HH
#define DUNE_COMMON_TUPLES_TYPETRAITS_HH

#include <dune/common/typetraits.hh>

#include <dune/common/tuples/checkpredicate.hh>
#include <dune/common/tuples/foreach.hh>
#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  // TupleTypeTraits
  // ---------------

  /** \ingroup Tuples_Traits
   *
   *  \brief This class mimicks the Dune::TypeTraits class
   *         for tuples.
   *
   *  \tparam  Tuple  Some Dune::tuple type.
   */
  template< class Tuple >
  class TupleTypeTraits
  {
    template< class T >
    struct IsPointerPredicate
    {
      static const bool value = TypeTraits< T >::isPointer;
      typedef typename TypeTraits< T >::PointeeType Type;
    };

    template< class T >
    struct IsReferencePredicate
    {
      static const bool value = TypeTraits< T >::isReference;
      typedef typename TypeTraits< T >::ReferredType Type;
    };

  public:
    //! \brief value is \b true, if each element in template argument tuple is of pointer type
    static const bool isPointerTuple = CheckPredicateTuple< Tuple, IsPointerPredicate >::value;
    //! \brief a Dune::tuple type; strip away pointer symbols in a pointer tuple
    typedef typename ForEachType< IsPointerPredicate, Tuple >::Type PointeeTupleType;

    //! \brief value is \b true, if each element in template argument tuple is of reference type
    static const bool isReferenceTuple = CheckPredicateTuple< Tuple, IsReferencePredicate >::value;
    //! \brief a Dune::tuple type; strip away reference symbols in a reference tuple
    typedef typename ForEachType< IsReferencePredicate, Tuple >::Type ReferredTupleType;
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_TYPETRAITS_HH
