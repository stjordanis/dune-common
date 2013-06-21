#ifndef DUNE_COMMON_TUPLES_SUBTUPLE_HH
#define DUNE_COMMON_TUPLES_SUBTUPLE_HH

#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  // SubTuple
  // --------

  /** \ingroup Tuples_MetaProgramming
   *
   *  \brief Create a new tuple type by choosing its element types from
   *         another tuple.
   *
   *  \tparam  Positions  A tuple of integral constants using \c int
   *                      denoting the positions of the types to be extracted
   *                      from Tuple.
   *  \tparam  Tuple      A tuple type.
   *
   *  Usage (using Dune::IntegralConstantTuple):
   *  @code
   *  typedef typename Dune::IntegralConstantTuple< int, 1, 2, 3, 4 >::Type Tuple;
   *  typedef typename Dune::IntegralConstantTuple< int, 2, 1 >::Type Positions;
   *  // resulting type is equal to: Dune::IntegralConstantTuple< int, 3, 2 >::Type
   *  typedef typename Dune::SubTuple< Tuple, Positions >::Type SubTuple;
   *
   *  Tuple tuple;
   *  SubTuple subtuple = Dune::SubTuple< Positions, Tuple >::apply( tuple );
   *  @endcode
   *
   *  See Dune::sub_tuple for a convenient alternative.
   */
  template< class Positions, class Tuple >
  struct SubTuple;

  template< class Tuple, class... P >
  struct SubTuple< Dune::tuple< P... >, Tuple >
  {
    typedef Dune::tuple< typename Dune::tuple_element< P::value, Tuple >::type... > Type;

    static Type apply ( const Tuple &tuple )
    {
      return Type( Dune::get< P::value >( tuple )... );
    }
  };



  // sub_tuple
  // ---------

  /** \ingroup Tuples_MetaProgramming
   *
   *  \brief Create a new tuple instance by choosing its element from
   *         another tuple (cf. Dune::SubTuple).
   *
   *  \tparam  Tuple      A tuple type.
   *  \tparam  Positions  A tuple of integral constants using \c int
   *
   *  Usage (using Dune::IntegralConstantTuple):
   *  @code
   *  typedef typename Dune::IntegralConstantTuple< int, 1, 2, 3, 4 >::Type Tuple;
   *  typedef typename Dune::IntegralConstantTuple< int, 2, 1 >::Type Positions;
   *
   *  Tuple tuple;
   *  typename Dune::SubTuple< Tuple, Positions >::Type subTuple = Dune::sub_tuple< Positions >( tuple );
   *  @endcode
   */
  template< class Positions, class Tuple >
  static typename SubTuple< Positions, Tuple >::Type sub_tuple ( const Tuple &tuple )
  {
    return SubTuple< Positions, Tuple >::apply( tuple );
  }

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_SUBTUPLE_HH
