#ifndef DUNE_COMMON_TUPLES_CHECKPREDICATE_HH
#define DUNE_COMMON_TUPLES_CHECKPREDICATE_HH

#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  // CheckPredicate
  // --------------

  /** \ingroup Tuples_Algorithms
   *
   *  \brief Check a static predicate for all tuple elements.
   *
   *  \tparam  Tuple      Some tuple type
   *  \tparam  Predicate  Unary static predicate
   *  \tparam  N          Implementation internal
   *
   *  The predicate is expected to provide a static boolean member
   *  \c v, i.e.,
\code
  template< class T >
  struct Predicate
  {
    static const bool value = ...;
  };
\endcode
   */
  template< class Tuple, template< class > class Predicate,
            int N = Dune::tuple_size< Tuple >::value >
  struct CheckPredicate
  {
    static const bool value = ( Predicate< typename Dune::tuple_element< N-1, Tuple >::type >::value
                                && CheckPredicate< Tuple, Predicate, (N-1) >::value );
  };

  template< class Tuple, template< class > class Predicate >
  struct CheckPredicate< Tuple, Predicate, 0 >
  {
    static const bool value = true;
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_CHECKPREDICATE_HH
