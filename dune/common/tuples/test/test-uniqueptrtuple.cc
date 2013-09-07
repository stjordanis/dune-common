#include <config.h>

#include <iostream>

#include <dune/common/exceptions.hh>
#include <dune/common/unused.hh>
#include <dune/common/tuples/typetraits.hh>

#include <dune/common/tuples/unique_ptr.hh>

int main ( int argc, char **argv )
{
  typedef Dune::UniquePointerTuple< int, double > UniquePointerTuple;

  UniquePointerTuple tuple;

  if( tuple )
    DUNE_THROW( Dune::InvalidStateException, "Cast to bool failed" );

  auto deleters DUNE_UNUSED = tuple.get_deleters();

  typedef UniquePointerTuple::PointerTuple PointerTuple;
  PointerTuple pointers = tuple.get();
  if( Dune::valid_pointer_tuple( pointers ) )
    DUNE_THROW( Dune::InvalidStateException, "Got non-zero initialized pointers" );

  tuple.reset( Dune::make_tuple( new int, new double ) );
  if( !tuple )
    DUNE_THROW( Dune::InvalidStateException, "Cast to bool failed" );

  Dune::tuple_element< 0, UniquePointerTuple >::type &pi
    = Dune::get< 0 >( tuple );
  Dune::tuple_element< 1, UniquePointerTuple >::type &pd
    = Dune::get< 1 >( tuple );

  *pi = 1;
  *pd = 11.;

  pointers = tuple.get();

  PointerTuple pointers2( new int, new double );
  *Dune::get< 0 >( pointers2 ) = 2;
  *Dune::get< 1 >( pointers2 ) = 22.;
  UniquePointerTuple tuple2( pointers2 );

  tuple.swap( tuple2 );
  Dune::RawTuple< UniquePointerTuple >::Type &raw = Dune::raw_tuple( tuple2 );
  tuple.swap( raw );

  UniquePointerTuple tuple3( std::move( tuple2 ) );

  tuple.reset();
  if( tuple )
    DUNE_THROW( Dune::InvalidStateException, "Cast to bool failed" );

  return 0;
}
