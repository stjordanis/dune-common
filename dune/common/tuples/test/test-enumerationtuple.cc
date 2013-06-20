#include <config.h>

#include <iostream>

#include <dune/common/forloop.hh>
#include <dune/common/tuples/enumeration.hh>
#include <dune/common/tuples/integralconstant.hh>
#include <dune/common/tuples/forloop.hh>
#include <dune/common/tuples/tuples.hh>


template< int i, class Enumeration >
struct Print
{
  template< class Tuple >
  static void apply ( const Tuple &tuple, std::ostream &out )
  {
    out << Dune::get< Dune::tuple_element< i, Enumeration >::type::value >( tuple );

    if( i < Dune::tuple_size< Enumeration >::value-1 )
      out << ", ";
  }
};


template< class Enumeration, class Tuple >
void print ( const Tuple &tuple, std::ostream &out = std::cout )
{
  out << "(";
  Dune::TupleForLoop< Print, Enumeration >::apply( tuple, out );
  out << ")" << std::endl;
}


template< int i >
struct CallToPrint
{
  template< class Tuple >
  static void apply ( const Tuple &tuple )
  {
    print< typename Dune::EnumerationTuple< i >::Type >( tuple );
  }
};


int main ( int argc, char **argv )
{
  typedef Dune::IntegralConstantTuple< int, 1, 2, 3, 4 >::Type IntegralConstantTuple;
  IntegralConstantTuple tuple;

  Dune::ForLoop< CallToPrint, -1, Dune::tuple_size< IntegralConstantTuple>::value >::apply( tuple );

  return 0;
}
