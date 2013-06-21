#ifndef DUNE_COMMON_TUPLES_ENUMERATION_HH
#define DUNE_COMMON_TUPLES_ENUMERATION_HH

#include <dune/common/tuples/tuples.hh>
#include <dune/common/typetraits.hh>

namespace Dune
{

  // EnumerationTuple
  // ----------------

  /** \ingroup Tuples_Traits
   *
   *  \brief Convenient setup of an enumeration of
   *         Dune::integral_constant in a tuple.
   *
   *  \tparam  T       integer type
   *  \tparam  length  size of index tuple
   *  \tparam  init    start value for enumeration (optional, defaults to 0)
   *
\code
  typedef typename Dune::EnumerationTuple< int, 3, 1 >::Type Type;
  // 'Type' corresponds to:
  // Dune::tuple< Dune::integral_constant< int, 1 >,
  //              Dune::integral_constant< int, 2 >,
  //              Dune::integral_constant< int, 3 > >
\endcode
   *
   *  \note For <tt>length <= 0</tt> the resulting type is
   *        <tt>Type = Dune::tuple<></tt>.
   */
  template< class T, T length, T init = 0 >
  class EnumerationTuple
  {
    template< T n, T m >
    struct Max
    {
      static const T value = (n >= m) ? n : m;
    };

    template< T n, T m, class... Args >
    struct Create
    {
      typedef typename Create< n, m+1, Args..., Dune::integral_constant< T, m > >::Type Type;
    };

    template< T n, class... Args >
    struct Create< n, n, Args... >
    {
      typedef Dune::tuple< Args... > Type;
    };

  public:
    typedef typename Create< (Max< length, 0 >::value)+init, init >::Type Type;

  private:
    EnumerationTuple ();
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_ENUMERATION_HH
