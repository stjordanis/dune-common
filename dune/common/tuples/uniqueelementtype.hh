#ifndef DUNE_COMMON_TUPLES_UNIQUEELEMENTTYPE_HH
#define DUNE_COMMON_TUPLES_UNIQUEELEMENTTYPE_HH

#include <dune/common/tupleutility.hh>

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
   * \tparam  Seed   implementation internal
   * \tparm   index  implementation internal
   */
  template< class T, int size, class Seed = Dune::tuple<>, int index = 0 >
  struct UniqueElementTypeTuple
  {
    typedef typename UniqueElementTypeTuple< T, size, typename Dune::PushBackTuple< Seed, T >::type, (index+1) >::Type Type;
  };

  template< class T, int size, class Seed >
  struct UniqueElementTypeTuple< T, size, Seed, size >
  {
    typedef Seed Type;
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_UNIQUEELEMENTTYPE_HH
