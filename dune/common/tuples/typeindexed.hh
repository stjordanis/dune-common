#ifndef DUNE_COMMON_TUPLES_TYPEINDEXED_HH
#define DUNE_COMMON_TUPLES_TYPEINDEXED_HH

#include <dune/common/tuples/firstindex.hh>
#include <dune/common/tuples/tuples.hh>

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

    //! \brief cast to Tuple
    operator Tuple & () { return tuple_; }

    //! \brief cast to const Tuple
    operator const Tuple & () const { return tuple_; }

  private:
    Tuple tuple_;
  };



  // tuple_element
  // -------------

  template< std::size_t i, class Tuple, class Types >
  struct tuple_element< i, TypeIndexedTuple< Tuple, Types > >
  {
    typedef typename tuple_element< i, Tuple >::type type;
  };

  template< std::size_t i, class Tuple, class Types >
  struct tuple_element< i, const TypeIndexedTuple< Tuple, Types > >
  {
    typedef typename tuple_element< i, Tuple >::type type;
  };



  // get for TypeIndexedTuple
  // ------------------------

  template< std::size_t i, class Tuple, class Types >
  const typename tuple_element< i, TypeIndexedTuple< Tuple, Types > >::type &
  get ( const TypeIndexedTuple< Tuple, Types > &tuple ) throw()
  {
    return get< i >( static_cast< const Tuple & >( tuple ) );
  }

  template< std::size_t i, class Tuple, class Types >
  typename tuple_element< i, TypeIndexedTuple< Tuple, Types > >::type &
  get ( TypeIndexedTuple< Tuple, Types > &tuple ) throw()
  {
    return get< i >( static_cast< Tuple & >( tuple ) );
  }



  // tuple_size for TypeIndexedTuple
  // -------------------------------

  template< class Tuple, class Types >
  struct tuple_size< TypeIndexedTuple< Tuple, Types > >
  {
    enum { value = tuple_size< Tuple >::value };
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_TYPEINDEXED_HH
