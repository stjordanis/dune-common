#ifndef DUNE_COMMON_TUPLES_NULLPTR_HH
#define DUNE_COMMON_TUPLES_NULLPTR_HH

#include <dune/common/deprecated.hh>
#include <dune/common/nullptr.hh>
#include <dune/common/static_assert.hh>
#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  // NullPointerTuple
  // ----------------

  /** \ingroup Tuples_Init
   *
   *  \brief Initialize a tuple of pointers with nullptr.
   *
   *  \tparam Tuple A tuple consisting of pointer types
   *
   *  Usage:
   *  @code
   *  Tuple tuple = Dune::NullPointerTuple< Tuple >();
   *  @endcode
   */
  template< class Tuple >
  class NullPointerTuple
  {
    static_assert( AlwaysFalse< Tuple >::value,
                   "NullPointerTuple only available for pointer tuples" );
  };

  template< class... T >
  class NullPointerTuple< Dune::tuple< T *... > >
  {
    template< class U >
    static U *null () { return nullptr; }

  public:
    operator Dune::tuple< T *... > () const
    {
      return Dune::tuple< T *... >( null< T >()... );
    }
  };



  // NullPointerInitialiser
  // ----------------------

  /** \ingroup Tuples_Init
   *
   *  \brief Initialize a tuple of pointers with nullptr.
   *
   *  \note This is a compatibility class; the new implementations
   *        forwards to Dune::NullPointerTuple.
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
