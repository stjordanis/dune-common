#include <config.h>

#include <cassert>
#include <iostream>

#include <dune/common/forloop.hh>

#include <dune/common/tuples/densevector.hh>
#include <dune/common/tuples/fvector.hh>
#include <dune/common/tuples/integralconstant.hh>
#include <dune/common/tuples/tuples.hh>

template< class ct, int dim >
struct Assign
{
  typedef Dune::FieldVector< ct, dim > FieldVectorType;
  typedef typename FieldVectorType::field_type field_type;
  typedef typename FieldVectorType::size_type size_type;

  Assign ( const FieldVectorType &x ) : x_( &x ) {}

  void operator() ( field_type &x, size_type i ) const
  {
    x = (*x_)[ i ];
  }

private:
  const FieldVectorType *x_;
};



int main ( int argc, char **argv )
{
  typedef Dune::IntegralConstantTuple< int, 1, 2, 3 >::Type Dimensions;
  typedef Dune::FieldVectorTuple< double, Dimensions > FieldVectorTuple;

  typedef FieldVectorTuple::field_type field_type;
  typedef FieldVectorTuple::size_type size_type;

  static const int dimension = FieldVectorTuple::Dimension::value;

  // creation
  FieldVectorTuple t1, t2;

  // check size
  size_type size = t1.size();
  assert( int( size ) == dimension );

  // assignment
  t1 = t2;
  t1 += t2;
  t1 -= t2;

  // operations for scalar value
  field_type alpha( 2 );
  t1 = alpha;
  t1 += alpha;
  t1 -= alpha;
  t1 *= alpha;
  t1 /= alpha;

  // for_each
  Dune::FieldVector< double, dimension > vector( 1 );
  for( size_type i = 0; i < size; ++i )
    vector[ i ] = i;
  Assign< double, dimension > function( vector );
  t1.for_each( function );

  // check norms
  t1.one_norm();
  t1.one_norm_real();
  t1.two_norm();
  t1.two_norm2();
  t1.infinity_norm();
  t1.infinity_norm_real();

  return 0;
}
