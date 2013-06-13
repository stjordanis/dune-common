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

  /** \class NullPointerTuple
   *
   *  \brief Initialize a tuple of pointers with nullptr.
   *
   *  \tparam  Tuple  A tuple consisting of pointer types.
   *
   * Usage:
\code
  Tuple tuple = Dune::NullPointerTuple< Tuple >();
\endcode
   */
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

  /** \class NullPointerInitialiser
   *
   * \brief Compatibility class; forwards to NullPointerTuple.
   *
   */
  template <class Tuple>
  struct NullPointerInitialiser
  {
    typedef Tuple ResultType;

    static ResultType apply() DUNE_DEPRECATED
    {
      return NullPointerTuple< ResultType >();
    }
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_NULLPTR_HH
