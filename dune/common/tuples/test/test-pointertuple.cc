#include <config.h>

#include <iostream>

#include <dune/common/exceptions.hh>
#include <dune/common/forloop.hh>
#include <dune/common/parallel/mpihelper.hh>

#include <dune/common/tuples/nullptr.hh>
#include <dune/common/tuples/pointer.hh>
#include <dune/common/tuples/transform.hh>
#include <dune/common/tuples/tuples.hh>
#include <dune/common/tuples/utility.hh>

#include "printtuple.hh"


// get references from tuple
template< class Tuple >
typename Dune::ForEachType< Dune::AddPtrTypeEvaluator, Tuple >::Type initialize ( Tuple &tuple )
{
  return Dune::transformTuple< Dune::AddPtrTypeEvaluator >( tuple );
}


int main ( int argc, char **argv )
{
  Dune::MPIHelper::instance( argc, argv );

  typedef Dune::tuple< double *, int * > PointerTuple;
  if( !Dune::TupleTypeTraits< PointerTuple >::isPointerTuple )
    DUNE_THROW( Dune::InvalidStateException, "Tuple contains non-pointer types." );

  // create nullptr-initialized tuple
  PointerTuple pointerTuple = Dune::NullPointerTuple< PointerTuple >();
  if( Dune::valid_pointer_tuple( pointerTuple ) )
    DUNE_THROW( Dune::InvalidStateException, "Null-pointer tuple must be invalid." );

  // strip off pointers from tuple and create instance
  typedef Dune::TupleTypeTraits< PointerTuple >::PointeeTupleType Tuple;
  Tuple tuple( sqrt(2.), 1 );
  print( tuple );

  // re-initialize pointer tuple with valid entries
  pointerTuple = initialize( tuple );
  if( !Dune::valid_pointer_tuple( pointerTuple ) )
    DUNE_THROW( Dune::InvalidStateException, "Non-nullptr tuple must be valid." );

  // dereference pointer tuple
  Dune::DereferenceTuple< PointerTuple >::Type referenceTuple
    = Dune::DereferenceTuple< PointerTuple >::apply( pointerTuple );

  // print results
  print( referenceTuple );

  return 0;
}
