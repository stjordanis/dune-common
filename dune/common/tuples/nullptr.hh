#ifndef DUNE_COMMON_TUPLES_NULLPTR_HH
#define DUNE_COMMON_TUPLES_NULLPTR_HH

#include <dune/common/deprecated.hh>
#include <dune/common/nullptr.hh>
#include <dune/common/static_assert.hh>

#include <dune/common/tuples/instantiate.hh>
#include <dune/common/tuples/typetraits.hh>

namespace Dune
{

  // NullPointerTuple
  // ----------------

  template< class Tuple >
  class NullPointerTuple
  {
    dune_static_assert( Dune::TupleTypeTraits< Tuple >::isPointerTuple,
                        "Argument tuple contains non-pointer types." );

  public:
    operator Tuple () const
    {
      return Dune::InstantiateTuple< Tuple >::apply( nullptr );
    }
  };



  // NullPointerInitialiser
  // ----------------------

  /**
   * @brief A helper template that initializes a tuple consisting of pointers
   * to NULL.
   *
   * A tuple of NULL pointers may be useful when you use a tuple of pointers
   * in a class which you can only initialise in a later stage.
   */
  template <class Tuple>
  struct NullPointerInitialiser
  {
    typedef Tuple ResultType;

    static ResultType apply() DUNE_DEPRECATED { return NullPointerTuple< ResultType >(); }
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_NULLPTR_HH
