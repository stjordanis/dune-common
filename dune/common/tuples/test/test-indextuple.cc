#include <config.h>

#include <iostream>

#include <dune/common/forloop.hh>
#include <dune/common/tuples/indextuple.hh>
#include <dune/common/tuples/tuples.hh>


template< class Tuple, bool empty = ( Dune::tuple_size< Tuple >::value <= 0 ) >
struct PrintIndexTuple;

template< class Tuple >
struct PrintIndexTuple< Tuple, true >
{
  static void apply( std::ostream &out = std::cout )
  {
    out << "()" << std::endl;
  }
};

template< class Tuple >
class PrintIndexTuple< Tuple, false >
{
  template< int i >
  struct Operation
  {
    static void apply ( std::ostream &out )
    {
      out << Dune::tuple_element< i, Tuple >::type::value;

      if( i < Dune::tuple_size< Tuple >::value-1 )
        out << ", ";
    }
  };

public:
  static void apply( std::ostream &out = std::cout )
  {
    out << "(";
    Dune::ForLoop< Operation, 0, Dune::tuple_size< Tuple >::value-1 >::apply( out );
    out << ")" << std::endl;
  }
};


int main ( int argc, char **argv )
{
  typedef Dune::IndexTuple< 3, 1 > IndexTuple;
  PrintIndexTuple< IndexTuple >::apply();
  return 0;
}
