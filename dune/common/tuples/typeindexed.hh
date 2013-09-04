#ifndef DUNE_COMMON_TUPLES_TYPEINDEXED_HH
#define DUNE_COMMON_TUPLES_TYPEINDEXED_HH

#include <dune/common/tuples/firstindex.hh>
#include <dune/common/tuples/tuples.hh>
#include <dune/common/tuples/typetraits.hh>

namespace Dune
{

  // TypeIndexedTuple
  // ----------------

  /** \class TypeIndexedTuple
   *
   *  \brief Access tuple entries via their type.
   *
   *  \tparam  Tuple  Some tuple type
   *  \tparam  Types  A tuple of unique types
   */
  template< class Tuple, class Types >
  class TypeIndexedTuple
  {
    template< class T >
    struct Position
    {
      static const int value = Dune::FirstTypeIndex< Types, T >::value;
    };

  public:
    template< class T >
    struct Value
    {
      typedef typename Dune::tuple_element< Position< T >::value, Tuple >::type Type;
    };

    explicit TypeIndexedTuple ( const Tuple &tuple = Tuple() )
    : tuple_( tuple )
    {}

    //! \brief please doc me
    template< class T >
    typename Value< T >::Type &at ()
    {
      return Dune::get< Position< T >::value >( tuple_ );
    }

    //! \brief please doc me
    template< class T >
    const typename Value< T >::Type &at () const
    {
      return Dune::get< Position< T >::value >( tuple_ );
    }

    //! \brief please doc me
    template< class T >
    typename Value< T >::Type &operator[] ( const T & )
    {
      return at< T >();
    }

    //! \brief please doc me
    template< class T >
    const typename Value< T >::Type &operator[] ( const T & ) const
    {
      return at< T >();
    }

  private:
    friend class RawTuple< TypeIndexedTuple< Tuple, Types > >;

    Tuple tuple_;
  };



  // RawTuple< TypeIndexedTuple >
  // ----------------------------

  template< class Tuple, class Types >
  struct RawTuple< TypeIndexedTuple< Tuple, Types > >
  {
    typedef Tuple Type;

    static Type &raw ( TypeIndexedTuple< Tuple, Types > &typeIndexedTuple )
    {
      return typeIndexedTuple.tuple_;
    }
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_TYPEINDEXED_HH
