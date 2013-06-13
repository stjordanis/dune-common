#ifndef DUNE_COMMON_TUPLES_ACCESS_HH
#define DUNE_COMMON_TUPLES_ACCESS_HH

#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  // AtType
  // ------

  //- Reverse element access
  /**
   * @brief Type for reverse element access.
   *
   * Counterpart to ElementType for reverse element access.
   */
  template <int N, class Tuple>
  struct AtType {
    typedef typename tuple_element<tuple_size<Tuple>::value - N - 1,
        Tuple>::type Type;
  };



  // At
  // --

  /**
   * @brief Reverse element access.
   *
   * While Element<...> gives you the arguments beginning at the front of a
   * tuple, At<...> starts at the end, which may be more convenient, depending
   * on how you built your tuple.
   */
  template <int N>
  struct At
  {

    template<typename Tuple>
    static
    typename TupleAccessTraits<typename AtType<N, Tuple>::Type>::NonConstType
    get(Tuple& t)
    {
      return Dune::get<tuple_size<Tuple>::value - N - 1>(t);
    }

    template<typename Tuple>
    static
    typename TupleAccessTraits<typename AtType<N, Tuple>::Type>::ConstType
    get(const Tuple& t)
    {
      return Dune::get<tuple_size<Tuple>::value - N - 1>(t);
    }
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_ACCESS_HH
