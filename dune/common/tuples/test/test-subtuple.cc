#include <config.h>

#include <iostream>

#include <dune/common/exceptions.hh>
#include <dune/common/forloop.hh>

#include <dune/common/tuples/containstype.hh>
#include <dune/common/tuples/firstindex.hh>
#include <dune/common/tuples/integralconstant.hh>
#include <dune/common/tuples/subtuple.hh>


template< class Tuple >
struct Find
{

  template< int i >
  struct Functor
  {
    static void apply ()
    {
      static const bool contains = Dune::ContainsType< Tuple, typename Dune::tuple_element< i, Tuple >::type >::value;
      if( !contains )
        DUNE_THROW( Dune::InvalidStateException, "Could not find type" );
    }
  };
};


template< int i >
struct Print
{
  template< class Tuple >
  static void apply ( const Tuple &tuple, std::ostream &out )
  {
    out << Dune::tuple_element< i, Tuple >::type::value;

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


int main ( int argc, char **argv )
{
  typedef Dune::IntegralConstantTuple< int, 1, 2, 3, 4 >::Type Tuple;

  Tuple tuple;
  std::cout << "tuple = ";
  print( tuple, std::cout );

  Dune::ForLoop< Find< Tuple >::Functor, 0, Dune::tuple_size< Tuple >::value-1 >::apply();
  if( Dune::ContainsType< Tuple, double >::value )
    DUNE_THROW( Dune::InvalidStateException,
                "Found type that should not have been contained in tuple." );

  typedef Dune::IntegralConstantTuple< int, 2, 1 >::Type Positions;
  std::cout << "  using ";
  print( Positions(), std::cout );

  typedef Dune::SubTuple< Tuple, Positions >::Type SubTuple;
  SubTuple subtuple = Dune::sub_tuple< Positions >( tuple );
  std::cout << "results in subtuple = ";
  print( subtuple, std::cout );

  Dune::FirstTypeIndexTuple< Tuple, SubTuple >::Type positions;  // equals Positions()
  std::cout << "  found at = ";
  print( positions, std::cout );

  return 0;
}
