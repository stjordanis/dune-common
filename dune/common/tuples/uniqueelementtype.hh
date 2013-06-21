#ifndef DUNE_COMMON_TUPLES_UNIQUEELEMENTTYPE_HH
#define DUNE_COMMON_TUPLES_UNIQUEELEMENTTYPE_HH

#include <dune/common/tuples/enumeration.hh>
#include <dune/common/tuples/foreach.hh>

namespace Dune
{

  // UniqueElementTypeTuple
  // ----------------------

  /* \class UniqueElementTypeTuple
   *
   * \brief Provide type of tuple of given length and unique
   *        element type.
   *
   * \tparam  T      type of each element in resulting tuple type
   * \tparam  size   size of resulting tuple type
   */
  template< class T, int size >
  class UniqueElementTypeTuple
  {
    template< class U >
    struct TypeEvaluator
    {
      typedef T Type;
    };

    typedef typename EnumerationTuple< int, size >::Type Enumeration;

  public:
    typedef typename ForEachType< TypeEvaluator, Enumeration >::Type Type;
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_UNIQUEELEMENTTYPE_HH
