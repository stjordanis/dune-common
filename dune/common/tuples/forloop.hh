#ifndef DUNE_COMMON_TUPLES_FORLOOP_HH
#define DUNE_COMMON_TUPLES_FORLOOP_HH

#include <dune/common/forloop.hh>
#include <dune/common/tuples/modifiers.hh>
#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  // GenericTupleForLoop
  // -------------------

  template< template< class, class > class Operation,
            template< int, class > class Value,
            class Enumeration,
            int length = tuple_size< Enumeration >::value
          >
  class GenericTupleForLoop
  : public Operation<
      Value< 0, Enumeration >,
      GenericTupleForLoop< Operation, Value, typename PopFrontTuple< Enumeration >::type >
    >
  {};

  template< template< class, class > class Operation,
            template< int, class > class Value,
            class Enumeration
          >
  class GenericTupleForLoop< Operation, Value, Enumeration, 1 >
  : public Value< 0, Enumeration >
  {};

  template< template< class, class > class Operation,
            template< int, class > class Value,
            class Enumeration
          >
  class GenericTupleForLoop< Operation, Value, Enumeration, 0 >
  {
  public:
    template< class... Args >
    static void apply( const Args&... args )
    {}
  };



  // TupleForLoop
  // ------------

  template< template< int, class > class Operation, class Enumeration >
  class TupleForLoop
    : public GenericTupleForLoop< ForLoopHelper::Apply, Operation, Enumeration >
  {};

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_FORLOOP_HH
