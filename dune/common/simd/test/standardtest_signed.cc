#include <config.h>

#include <dune/common/simd/test.hh>
#include <dune/common/simd/test/standardtest.hh>

namespace Dune {
  namespace Simd {

    template void UnitTest::checkSimdType<short             >();
    template void UnitTest::checkSimdType<int               >();
    template void UnitTest::checkSimdType<long              >();
    template void UnitTest::checkSimdType<long long         >();

  } // namespace Simd
} // namespace Dune