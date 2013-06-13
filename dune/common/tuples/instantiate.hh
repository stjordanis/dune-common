#ifndef DUNE_COMMON_TUPLES_INSTANTIATE_HH
#define DUNE_COMMON_TUPLES_INSTANTIATE_HH

#include <dune/common/tuples/modifiers.hh>
#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  // InstantiateTuple
  // ----------------

  /** \brief Instantiate a tuple of elements with identical, simple
   *         constructors.
   *
   *  \tparam  Tuple  tuple type
   *  \tparam  Seed   internal template argument
   *  \tparam  len    internal template argument
   *
   * Sample usage:
\code
  Tuple tuple = InstantiateTuple< Tuple >::apply( key );
\endcode
   *  Note that the tuple object is required to be copyable.
   */
  template< class Tuple,
            class Seed = Dune::tuple<>,
            int len = Dune::tuple_size< Tuple >::value
          >
  struct InstantiateTuple
  {
    /** \brief create tuple instance
     *
     *  \tparam  Key    type of argument to be passed to each constructor
     *
     *  \param[in]  key  argument passed to each constructor
     */
    template< class Key >
    static Tuple apply ( const Key &key = Key() )
    {
      Seed seed;
      return append( key, seed );
    }

  private:
    template< class, class, int > friend class InstantiateTuple;

    template< class Key >
    static Tuple append ( const Key &key, Seed &seed )
    {
      static const int index = Dune::tuple_size< Tuple >::value - len;

      typedef typename Dune::tuple_element< index, Tuple >::type AppendType;
      typedef typename Dune::PushBackTuple< Seed, AppendType >::type AccumulatedType;

      AccumulatedType next = Dune::tuple_push_back< AppendType >( seed, AppendType( key ) );
      return InstantiateTuple< Tuple, AccumulatedType, len-1 >::append( key, next );
    }
  };

  template< class Tuple, class Seed >
  struct InstantiateTuple< Tuple, Seed, 0 >
  {
    template< class Key >
    static Tuple apply ( const Key &key = Key() ) { return Tuple(); }

  private:
    template< class, class, int > friend class InstantiateTuple;

    template< class Key >
    static Seed append ( const Key &key, Seed &seed ) { return seed; }
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_INSTANTIATE_HH
