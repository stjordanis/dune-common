#ifndef DUNE_COMMON_TUPLES_TYPETRAITS_HH
#define DUNE_COMMON_TUPLES_TYPETRAITS_HH

#include <dune/common/typetraits.hh>

#include <dune/common/tuples/checkpredicate.hh>
#include <dune/common/tuples/tuples.hh>
#include <dune/common/tuples/uniqueelementtype.hh>

namespace Dune
{

  // TupleTypeTraits
  // ---------------

  /** \class TupleTypeTraits
   *
   *  \brief This class mimicks the Dune::TypeTraits class
   *  (see dune/common/typetraits.hh) for tuples.
   *
   *  \tparam  Tuple  Some tuple type
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
    static const bool isPointerTuple = CheckPredicate< Tuple, IsPointerPredicate >::value;

    typedef typename SelectType< isPointerTuple,
        typename ForEachType< IsPointerPredicate, Tuple >::Type,
        UniqueElementTypeTuple< Empty, tuple_size< Tuple >::value >
      >::Type PointeeTupleType;

    static const bool isReferenceTuple = CheckPredicate< Tuple, IsReferencePredicate >::value;

    typedef typename ForEachType< IsReferencePredicate, Tuple >::Type ReferredTupleType;
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_TYPETRAITS_HH
