#ifndef DUNE_COMMON_TUPLES_UNIQUE_PTR_HH
#define DUNE_COMMON_TUPLES_UNIQUE_PTR_HH

#include <cstddef>
#include <memory>

#include <dune/common/tuples/enumeration.hh>
#include <dune/common/tuples/foreach.hh>
#include <dune/common/tuples/pointer.hh>
#include <dune/common/tuples/tuples.hh>
#include <dune/common/tuples/typetraits.hh>

namespace Dune
{

  // UniquePointerTuple
  // ------------------

  /** \ingroup Tuples_Init
   *
   *  \brief A tuple of std::unique_ptr to different types.
   *
   *  \tparam  T  Variadic list of elements
   */
  template< class... T >
  class UniquePointerTuple
  {
    typedef typename EnumerationTuple< int, sizeof...( T ) >::Type Enumeration;

    struct ResetFunctor
    {
      template< class U >
      static void visit ( std::unique_ptr< U > &up, U *rp  = nullptr )
      {
        up.reset( rp );
      }
    };

    struct SwapFunctor
    {
      template< class U >
      static void visit ( std::unique_ptr< U > &p1, std::unique_ptr< U > &p2 )
      {
        p1.swap( p2 );
      }
    };

  public:
    //! \brief element tuple
    typedef tuple< typename std::unique_ptr< T >::element_type... > ElementTuple;
    //! \brief pointer tuple
    typedef tuple< typename std::unique_ptr< T >::pointer... > PointerTuple;
    //! \brief deleter tuple
    typedef tuple< typename std::unique_ptr< T >::deleter_type... > DeleterTuple;

    //! \brief emtpy constructor
    UniquePointerTuple () {}

    //! \brief construct form pointers
    explicit UniquePointerTuple ( T *... pointers )
    : tuple_( make_tuple( pointers... ) )
    {}

    //! \brief construct form pointers
    explicit UniquePointerTuple ( PointerTuple pointers )
    {
      reset( pointers );
    }

    //! \brief move constructor
    UniquePointerTuple ( UniquePointerTuple &&other )
    : tuple_( std::move( other.tuple_ ) )
    {}

    //! \brief move constructor
    UniquePointerTuple ( tuple< typename std::unique_ptr< T >&&... > other )
    : tuple_( std::move( other ) )
    {}

    //! \brief assignment from rvalue reference
    UniquePointerTuple &operator= ( UniquePointerTuple &&other )
    {
      tuple_ = std::move( other.tuple_ );
      return *this;
    }

    //! \brief assignment from rvalue reference
    UniquePointerTuple &operator= ( tuple< typename std::unique_ptr< T >&&... > other )
    {
      tuple_ = std::move( other );
      return *this;
    }

  private:
    UniquePointerTuple ( const UniquePointerTuple & );
    UniquePointerTuple &operator= ( const UniquePointerTuple &other );

  public:
    //! \brief get tuple of pointers
    PointerTuple get () const { return get( Enumeration() ); }

    //! \brief get deleters
    tuple< typename std::unique_ptr< T >::deleter_type &... > get_deleters ()
    {
      return get_deleters( Enumeration() );
    }

    //! \brief get deleters
    tuple< const typename std::unique_ptr< T >::deleter_type &... > get_deleters () const
    {
      return get_deleters( Enumeration() );
    }

    //! \brief check if all pointers are not empty
    operator bool () const
    {
      return valid_pointer_tuple( get() );
    }

    //! \brief release pointers
    PointerTuple release () { return release( Enumeration() ); }

    //! \brief reset pointers
    void reset ( PointerTuple pointers )
    {
      ForEachValuePair< tuple< std::unique_ptr< T >... > , PointerTuple >forEach( tuple_, pointers );
      ResetFunctor functor;
      forEach.apply( functor );
    }

    //! \brief reset pointers
    void reset ()
    {
      ForEachValue< tuple< std::unique_ptr< T >... > >forEach( tuple_ );
      ResetFunctor functor;
      forEach.apply( functor );
    }

    //! \brief swap contents
    void swap ( UniquePointerTuple &other ) { swap( tuple_, other.tuple_ ); }

    //! \brief swap contents
    void swap ( tuple< std::unique_ptr< T > ... > &other )
    {
      swap( tuple_, other );
    }

    //! \brief cast to primitive tuple
    operator tuple< std::unique_ptr< T >... > & () { return tuple_; }
    //! \brief cast to primitive tuple
    operator const tuple< std::unique_ptr< T >... > & () const { return tuple_; }

  private:
    template< class... I >
    PointerTuple get ( tuple< I... > ) const
    {
      return make_tuple( Dune::get< I::value >( tuple_ ).get()... );
    }

    template< class... I >
    tuple< typename std::unique_ptr< T >::deleter_type &... > get_deleters ( tuple< I... > )
    {
      return forward_as_tuple( Dune::get< I::value >( tuple_ ).get_deleter()... );
    }

    template< class... I >
    tuple< const typename std::unique_ptr< T >::deleter_type &... > get_deleters ( tuple< I... > ) const
    {
      return forward_as_tuple( Dune::get< I::value >( tuple_ ).get_deleter()... );
    }

    template< class... I >
    PointerTuple release ( tuple< I... > )
    {
      return make_tuple( Dune::get< I::value >( tuple_ ).release()... );
    }

    void swap ( tuple< std::unique_ptr< T >... > &t1,
                tuple< std::unique_ptr< T >... > &t2 ) const
    {
      typedef tuple< std::unique_ptr< T >... > TupleType;
      ForEachValuePair< TupleType, TupleType > forEach( t1, t2 );
      SwapFunctor functor;
      forEach.apply( functor );
    }

    tuple< std::unique_ptr< T >... > tuple_;
  };



  // RawTuple< UniquePointerTuple >
  // ------------------------------

  template< class... T >
  struct RawTuple< UniquePointerTuple< T... > >
  {
    typedef Dune::tuple< std::unique_ptr< T >... > Type;

    static Type &raw ( Type &tuple )
    {
      return static_cast< Type & >( tuple );
    }
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_UNIQUE_PTR_HH
