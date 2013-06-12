#ifndef DUNE_COMMON_TUPLES_TYPEINDEXED_HH
#define DUNE_COMMON_TUPLES_TYPEINDEXED_HH

#include <dune/common/tuples/namespace.hh>
#include <dune/common/tuples/tuples.hh>
#include <dune/common/tuples/tupleutility.hh>

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



  // get for TypeIndexedTuple
  // ------------------------

  template< int i, class Tuple, class Types >
  typename tuple_element< i, Tuple >::type &
  get ( Dune::TypeIndexedTuple< Tuple, Types > &tuple )
  {
    return get< i >( static_cast< Tuple & >( tuple ) );
  }

  template< int i, class Tuple, class Types >
  const typename tuple_element< i, Tuple >::type &
  get ( const Dune::TypeIndexedTuple< Tuple, Types > &tuple )
  {
    return get< i >( static_cast< const Tuple & >( tuple ) );
  }

} // namespace Dune



// Some Specializations for Tuple Access
// -------------------------------------

DUNE_OPEN_TUPLE_NAMESPACE

  // tuple_element for TypeIndexedTuple
  // ----------------------------------

  template< size_t i, class Tuple, class Types >
  struct tuple_element< i, Dune::TypeIndexedTuple< Tuple, Types > >
  {
    typedef typename tuple_element< i, Tuple >::type type;
  };

DUNE_CLOSE_TUPLE_NAMESPACE

#endif // #ifndef DUNE_COMMON_TUPLES_TYPEINDEXED_HH
