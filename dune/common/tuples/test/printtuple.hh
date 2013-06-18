#ifndef DUNE_COMMON_TUPLES_TEST_PRINTTUPLE_HH
#define DUNE_COMMON_TUPLES_TEST_PRINTTUPLE_HH

#include <iosfwd>

#include <dune/common/forloop.hh>
#include <dune/common/tuples/tuples.hh>

template< int i >
struct Print
{
  template< class Tuple >
  static void apply ( const Tuple &tuple, std::ostream &out )
  {
    out << Dune::get< i >( tuple );

    if( i < Dune::tuple_size< Tuple >::value-1 )
      out << ", ";
  }
};

template< class Tuple >
void print ( const Tuple &tuple, std::ostream &out = std::cout )
{
  out << "(";
  Dune::ForLoop< Print, 0, Dune::tuple_size< Tuple >::value-1 >::apply( tuple, out );
  out << ")" << std::endl;
}

#endif // #ifndef DUNE_COMMON_TUPLES_TEST_PRINTTUPLE_HH
