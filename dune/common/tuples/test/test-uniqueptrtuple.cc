#include <config.h>

#include <iostream>

#include <dune/common/exceptions.hh>
#include <dune/common/unused.hh>
#include <dune/common/tuples/typetraits.hh>

#include <dune/common/tuples/unique_ptr.hh>

using namespace Dune;

int main ( int argc, char **argv )
{
  typedef Dune::UniquePointerTuple< int, double > UniquePointerTuple;

  // create empty tuple
  UniquePointerTuple tuple;

  // assert that all elements are invalid
  if( tuple )
    DUNE_THROW( InvalidStateException, "Cast to bool failed" );

  // get deleleter tuple
  auto deleters DUNE_UNUSED = tuple.get_deleters();

  // get pointers
  typedef UniquePointerTuple::PointerTuple PointerTuple;
  PointerTuple pointers = tuple.get();
  if( valid_pointer_tuple( pointers ) )
    DUNE_THROW( InvalidStateException, "Got non-zero initialized pointers" );

  // initialize unique_ptr tuple with newly created elements
  tuple.reset( make_tuple( new int, new double ) );
  if( !tuple )
    DUNE_THROW( InvalidStateException, "Cast to bool failed" );

  // access elements
  tuple_element< 0, RawTuple< UniquePointerTuple >::Type >::type &pi
    = get< 0 >( raw_tuple( tuple ) );
  tuple_element< 1, RawTuple< UniquePointerTuple >::Type >::type &pd
    = get< 1 >( raw_tuple( tuple ) );

  // assign values
  *pi = 1;
  *pd = 11.;

  // create another tuple from tuple of pointers
  PointerTuple pointers2( new int, new double );
  *get< 0 >( pointers2 ) = 2;
  *get< 1 >( pointers2 ) = 22.;
  UniquePointerTuple tuple2( pointers2 );

  // swap elements
  tuple.swap( tuple2 );
  RawTuple< UniquePointerTuple >::Type &raw = raw_tuple( tuple2 );
  tuple.swap( raw );

  // test move constructor
  UniquePointerTuple tuple3( std::move( tuple2 ) );

  // check resetting
  tuple.reset();
  if( tuple )
    DUNE_THROW( InvalidStateException, "Cast to bool failed" );

  return 0;
}
