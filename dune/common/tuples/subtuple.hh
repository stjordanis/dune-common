#ifndef DUNE_COMMON_TUPLES_SUBTUPLE_HH
#define DUNE_COMMON_TUPLES_SUBTUPLE_HH

#include <dune/common/tuples/modifiers.hh>
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
   *  \tparam  Tuple      A tuple type.
   *  \tparam  Positions  A tuple of integral constants using \c int
   *                      denoting the positions of the types to be extracted
   *                      from Tuple.
   *  \tparam  Seed       (implementation internal)
   *  \tparam  index      (implementation internal)
   *  \tparam  size       (implementation internal)
   *
   *  Usage (using Dune::IntegralConstantTuple):
   *  @code
   *  typedef typename Dune::IntegralConstantTuple< int, 1, 2, 3, 4 >::Type Tuple;
   *  typedef typename Dune::IntegralConstantTuple< int, 2, 1 >::Type Positions;
   *  // resulting type is equal to: Dune::IntegralConstantTuple< int, 3, 2 >::Type
   *  typedef typename Dune::SubTuple< Tuple, Positions >::Type SubTuple;
   *
   *  Tuple tuple;
   *  SubTuple subtuple = Dune::SubTuple< Tuple, Positions >::apply( tuple );
   *  @endcode
   *
   *  See Dune::sub_tuple for a convenient alternative.
   */
  template< class Tuple, class Positions,
            class Seed = tuple<>, int index = 0,
            int size = tuple_size< Positions >::value >
  class SubTuple
  {
    template< class, class, class, int, int > friend class SubTuple;

    // get pass number for element to append from mapping
    static const int position = tuple_element< index, Positions >::type::value;

    // add type to seed
    typedef typename tuple_element< position, Tuple >::type AppendType;
    typedef typename PushBackTuple< Seed, AppendType >::type AccumulatedType;
    typedef SubTuple< Tuple, Positions, AccumulatedType, (index+1), size > NextType;

    static typename NextType::Type append ( const Tuple &tuple, Seed &seed )
    {
      AppendType append = get< position >( tuple );
      AccumulatedType next = tuple_push_back( seed, append );
      return NextType::append( tuple, next );
    }

  public:
    typedef typename NextType::Type Type;

    static Type apply ( const Tuple &tuple )
    {
      Seed seed;
      return append( tuple, seed );
    }
  };

#ifndef DOXYGEN
  template< class Tuple, class Positions, class Seed, int size >
  class SubTuple< Tuple, Positions, Seed, size, size >
  {
    template< class, class, class, int, int > friend class SubTuple;

    static Seed append ( const Tuple &tuple, Seed &seed ) { return seed; }

  public:
    typedef Seed Type;

    static Type apply ( const Tuple & ) { return Type(); }
  };
#endif // #ifndef DOXYGEN



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
  static typename SubTuple< Tuple, Positions >::Type sub_tuple ( const Tuple &tuple )
  {
    return SubTuple< Tuple, Positions >::apply( tuple );
  }

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_SUBTUPLE_HH
