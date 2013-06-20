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
   *  \tparam  length  size of index tuple
   *  \tparam  init    start value for enumeration (optional, defaults to 0)
   *
\code
  typedef typename Dune::EnumerationTuple< 3, 1 >::Type Type;
  // 'Type' corresponds to:
  // Dune::tuple< Dune::integral_constant< int, 1 >,
  //              Dune::integral_constant< int, 2 >,
  //              Dune::integral_constant< int, 3 > >
\endcode
   *
   *  \note For <tt>length <= 0</tt> the resulting type is
   *        <tt>Type = Dune::tuple<></tt>.
   */
  template< int length, int init = 0 >
  class EnumerationTuple
  {
    template< int N, int M >
    struct Max
    {
      static const int value = (N >= M) ? N : M;
    };

    template< int N, int M, class... Args >
    struct Create
    {
      typedef typename Create< N, M+1, Args..., Dune::integral_constant< int, M > >::Type Type;
    };

    template< int N, int M >
    struct Create< N, M >
    {
      typedef typename Create< N, M+1, Dune::integral_constant< int, M > >::Type Type;
    };

    template< int N >
    struct Create< N, N >
    {
      typedef Dune::tuple<> Type;
    };

    template< int N, class NonEmpty, class... Args >
    struct Create< N, N, NonEmpty, Args... >
    {
      typedef Dune::tuple< NonEmpty, Args... > Type;
    };

  public:
    typedef typename Create< (Max< length, 0 >::value)+init, init >::Type Type;

  private:
    EnumerationTuple ();
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_ENUMERATION_HH
