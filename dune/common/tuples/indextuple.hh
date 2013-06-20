#ifndef DUNE_COMMON_TUPLES_INDEXTUPLE_HH
#define DUNE_COMMON_TUPLES_INDEXTUPLE_HH

#include <dune/common/typetraits.hh>
#include <dune/common/tuples/namespace.hh>
#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  // IndexTuple
  // ----------

  /** \ingroup Tuples_Traits
   *
   *  \brief Convenient setup of an enumeration of
   *         Dune::integral_constant in a tuple.
   *
   *  \tparam  length  size of index tuple
   *  \tparam  init    start value for enumeration (optional, defaults to 0)
   *
\code
  typedef typename Dune::IndexTuple< 3, 1 >::Type Type;
  // 'Type' corresponds to:
  // Dune::tuple< Dune::integral_constant< int, 1 >,
  //              Dune::integral_constant< int, 2 >,
  //              Dune::integral_constant< int, 3 > >
\endcode
   */
  template< int length, int init = 0 >
  class IndexTuple
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

  private:
    IndexTuple ();
  };

} // namespace Dune



DUNE_OPEN_TUPLE_NAMESPACE

  // tuple_element for IndexTuple
  // ----------------------------

  template< size_t i, int length, int init >
  struct tuple_element< i, Dune::IndexTuple< length, init > >
  {
    typedef Dune::integral_constant< int, init+i > type;
  };



  // tuple_size for IndexTuple
  // -------------------------

  template< int length, int init >
  struct tuple_size< Dune::IndexTuple< length, init > >
  {
    enum { value = (length >= 0) ? length : 0 };
  };

DUNE_CLOSE_TUPLE_NAMESPACE

#endif // #ifndef DUNE_COMMON_TUPLES_INDEXTUPLE_HH
