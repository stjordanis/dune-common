#ifndef DUNE_COMMON_TUPLES_CONTAINSTYPE_HH
#define DUNE_COMMON_TUPLES_CONTAINSTYPE_HH

#include <cstddef>

#include <dune/common/typetraits.hh>

#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  // ContainsType
  // ------------

  /*
   * \brief Provide static check whether a given type is contained
   *        in tuple.
   *
   * \tparam  Tuple  tuple
   * \tparam  Type   type to search for
   *
   * \note Unlike FirstTypeIndex, this method will not result in
   *       an error, if the given type was not found in the tuple.
   */
  template< class Tuple, class Type,
            std::size_t N = Dune::tuple_size< Tuple >::value >
  struct ContainsType
  {
    static const bool value = ( Dune::is_same< typename Dune::tuple_element< N-1, Tuple >::type, Type >::value
                                || ContainsType< Tuple, Type, N-1 >::position );
  };

  template< class Tuple, class Type >
  struct ContainsType< Tuple, Type, 0 >
  {
    static const bool value = false;
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_CONTAINSTYPE_HH
