// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <iostream>
#include <dune/common/parallel/mpihelper.hh> // An initializer of MPI
#include <dune/common/exceptions.hh> // We use exceptions
#include <dune/common/test/testsuite.hh>

#include <dune/common/std/variant.hh>

// some non-default constructible type
struct F {
  int i;
  F() = delete;
  F(int j) :
    i(j) {}
};

Dune::TestSuite testVariant() {
  using namespace Dune;
  TestSuite suite;

  int i = 42;
  double d = 3.14;
  F f(13);
  using V = std::vector<int>;

  auto variant = Std::variant<int, double, F, V>();

  suite.check(Std::variant_size_v(variant) == 4, "Test variant_size_v");


  variant = d;
  suite.check(Std::holds_alternative<double>(variant), "Test holds_alternative");

  variant = f;
  suite.check(Std::holds_alternative<F>(variant), "Test holds_alternative");

  variant = i;
  suite.check(Std::holds_alternative<int>(variant), "Test holds_alternative");

  suite.check(Std::get<int>(variant) == i, "Test get<Type>");
  suite.check(Std::get<0>(variant) == i, "Test get<Index>");

  suite.check(Std::get_if<int>(variant) != nullptr, "Test get_if on right type");
  suite.check(Std::get_if<double>(variant) == nullptr, "Test get_if on wrong type");

  suite.check(Std::get_if<0>(variant) != nullptr, "Test get_if on right index");
  suite.check(Std::get_if<1>(variant) == nullptr, "Test get_if on wrong index");

  // test if get<Type> throws if one tries to get the wrong type:
  try {
    // currently hold type is still int, so double should throw
    Std::get<double>(variant);
    suite.check(false, "Test get<Type> on wrong type should have thrown");
  }
  catch (...) {
    suite.check(true, "Test get<Type> on wrong type has thrown");
  }


  variant = V(1);
  suite.check(Std::get<V>(variant).size() == 1, "Test with non-trivial type");

  variant = f;

  suite.check(variant.index() == 2, "Test index()"); // we're at type F, which has position 2

  // Demonstrate visit concept and using vector as an example of a non-POD type
  using V2 = std::vector<double>;
  Std::variant<V, V2> variant2;
  variant2 = V(1);
  auto size = [](auto&& v) {return v.size();};
  suite.check(Std::visit(size, variant2)== 1, "Test visit");
  variant2 = V2(2);
  suite.check(Std::visit(size, variant2)== 2, "Test visit");

  // try on a const reference:
  const auto& constv2 = variant2;
  suite.check(Std::visit(size, constv2)== 2, "Test const visit");
  suite.check(Std::get_if<V2>(constv2) != nullptr, "Test const get_if");


  return suite;
}

int main(int argc, char** argv)
{
  try{
    // Maybe initialize MPI
    Dune::MPIHelper::instance(argc, argv);

    Dune::TestSuite suite;
    suite.subTest(testVariant());

    return suite.exit();
  }
  catch (Dune::Exception &e){
    std::cerr << "Dune reported error: " << e << std::endl;
  }
  catch (...){
    std::cerr << "Unknown exception thrown!" << std::endl;
  }
}
