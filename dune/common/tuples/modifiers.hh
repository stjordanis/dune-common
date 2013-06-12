#ifndef DUNE_COMMON_TUPLES_MODIFIERS_HH
#define DUNE_COMMON_TUPLES_MODIFIERS_HH

#include <dune/common/static_assert.hh>

#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  /** \class PushBackTuple
   *
   *  \brief Helper template to append a type to a tuple
   *
   *  \tparam Tuple The tuple type to extend
   *  \tparam T     The type to be appended to the tuple
   *
   *  With variadic templates the generic specialization would be:
   *
   *  \code
   *  template<class... TupleArgs, class T>
   *  struct PushBackTuple<typename Dune::tuple<TupleArgs...>, T>
   *  {
   *    typedef typename Dune::tuple<TupleArgs..., T> type;
   *  };
   *  \endcode
   *
   */
  template< class Tuple, class T>
  struct PushBackTuple
  {
    dune_static_assert(AlwaysFalse<Tuple>::value, "Attempt to use the "
                       "unspecialized version of PushBackTuple.  "
                       "PushBackTuple needs to be specialized for "
                       "each possible tuple size.  Naturally the number of "
                       "pre-defined specializations is limited arbitrarily.  "
                       "Maybe you need to raise this limit by defining some "
                       "more specializations?");

    /**
     * \brief For all specializations this is the type of a tuple with T appended.
     *
     * Suppose you have Tuple=tuple<T1, T2, ..., TN> then
     * this type is tuple<T1, T2, ..., TN, T>.
     */
    typedef Tuple type;
  };


#ifndef DOXYGEN

  template<class T>
  struct PushBackTuple< Dune::tuple<>, T>
  {
    typedef typename Dune::tuple<T> type;
  };

  template< class T1, class T>
  struct PushBackTuple< Dune::tuple<T1>, T>
  {
    typedef typename Dune::tuple<T1, T> type;
  };

  template< class T1, class T2, class T>
  struct PushBackTuple< Dune::tuple<T1, T2>, T>
  {
    typedef typename Dune::tuple<T1, T2, T> type;
  };

  template< class T1, class T2, class T3, class T>
  struct PushBackTuple< Dune::tuple<T1, T2, T3>, T>
  {
    typedef typename Dune::tuple<T1, T2, T3, T> type;
  };

  template< class T1, class T2, class T3, class T4, class T>
  struct PushBackTuple< Dune::tuple<T1, T2, T3, T4>, T>
  {
    typedef typename Dune::tuple<T1, T2, T3, T4, T> type;
  };

  template< class T1, class T2, class T3, class T4, class T5, class T>
  struct PushBackTuple< Dune::tuple<T1, T2, T3, T4, T5>, T>
  {
    typedef typename Dune::tuple<T1, T2, T3, T4, T5, T> type;
  };

  template< class T1, class T2, class T3, class T4, class T5, class T6, class T>
  struct PushBackTuple< Dune::tuple<T1, T2, T3, T4, T5, T6>, T>
  {
    typedef typename Dune::tuple<T1, T2, T3, T4, T5, T6, T> type;
  };

  template< class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T>
  struct PushBackTuple< Dune::tuple<T1, T2, T3, T4, T5, T6, T7>, T>
  {
    typedef typename Dune::tuple<T1, T2, T3, T4, T5, T6, T7, T> type;
  };

  template< class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T>
  struct PushBackTuple< Dune::tuple<T1, T2, T3, T4, T5, T6, T7, T8>, T>
  {
    typedef typename Dune::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T> type;
  };

#endif



  /** \class PushFrontTuple
   *
   *  \brief Helper template to prepend a type to a tuple
   *
   *  \tparam Tuple The tuple type to extend
   *  \tparam T     The type to be prepended to the tuple
   *
   *  With variadic templates the generic specialization would be:
   *
   *  \code
   *  template<class... TupleArgs, class T>
   *  struct PushFrontTuple<typename Dune::tuple<TupleArgs...>, T>
   *  {
   *    typedef typename Dune::tuple<T, TupleArgs...> type;
   *  };
   *  \endcode
   *
   */
  template< class Tuple, class T>
  struct PushFrontTuple
  {
    dune_static_assert(AlwaysFalse<Tuple>::value, "Attempt to use the "
                       "unspecialized version of PushFrontTuple.  "
                       "PushFrontTuple needs to be specialized for "
                       "each possible tuple size.  Naturally the number of "
                       "pre-defined specializations is limited arbitrarily.  "
                       "Maybe you need to raise this limit by defining some "
                       "more specializations?");

    /**
     * \brief For all specializations this is the type of a tuple with T prepended.
     *
     * Suppose you have Tuple=tuple<T1, T2, ..., TN> then
     * this type is tuple<T, T1, T2, ..., TN>.
     */
    typedef Tuple type;
  };


#ifndef DOXYGEN

  template<class T>
  struct PushFrontTuple< Dune::tuple<>, T>
  {
    typedef typename Dune::tuple<T> type;
  };

  template< class T1, class T>
  struct PushFrontTuple< Dune::tuple<T1>, T>
  {
    typedef typename Dune::tuple<T, T1> type;
  };

  template< class T1, class T2, class T>
  struct PushFrontTuple< Dune::tuple<T1, T2>, T>
  {
    typedef typename Dune::tuple<T, T1, T2> type;
  };

  template< class T1, class T2, class T3, class T>
  struct PushFrontTuple< Dune::tuple<T1, T2, T3>, T>
  {
    typedef typename Dune::tuple<T, T1, T2, T3> type;
  };

  template< class T1, class T2, class T3, class T4, class T>
  struct PushFrontTuple< Dune::tuple<T1, T2, T3, T4>, T>
  {
    typedef typename Dune::tuple<T, T1, T2, T3, T4> type;
  };

  template< class T1, class T2, class T3, class T4, class T5, class T>
  struct PushFrontTuple< Dune::tuple<T1, T2, T3, T4, T5>, T>
  {
    typedef typename Dune::tuple<T, T1, T2, T3, T4, T5> type;
  };

  template< class T1, class T2, class T3, class T4, class T5, class T6, class T>
  struct PushFrontTuple< Dune::tuple<T1, T2, T3, T4, T5, T6>, T>
  {
    typedef typename Dune::tuple<T, T1, T2, T3, T4, T5, T6> type;
  };

  template< class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T>
  struct PushFrontTuple< Dune::tuple<T1, T2, T3, T4, T5, T6, T7>, T>
  {
    typedef typename Dune::tuple<T, T1, T2, T3, T4, T5, T6, T7> type;
  };

  template< class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T>
  struct PushFrontTuple< Dune::tuple<T1, T2, T3, T4, T5, T6, T7, T8>, T>
  {
    typedef typename Dune::tuple<T, T1, T2, T3, T4, T5, T6, T7, T8> type;
  };

#endif

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_MODIFIERS_HH
