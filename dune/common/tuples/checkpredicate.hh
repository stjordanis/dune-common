#ifndef DUNE_COMMON_TUPLES_CHECKPREDICATE_HH
#define DUNE_COMMON_TUPLES_CHECKPREDICATE_HH

#include <dune/common/tuples/foreach.hh>
#include <dune/common/tuples/transform.hh>
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
   *  @code
   *  template< class T >
   *  struct Predicate
   *  {
   *    static const bool value = ...;
   *  };
   *  @endcode
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



#ifndef DOXYGEN

  namespace
  {

    // CheckPredicateHelper
    // --------------------

    template< class Tuple >
    class CheckPredicateHelper
    {
      template< class Predicate >
      struct CheckPredicateFunctor
      {
        explicit CheckPredicateFunctor ( const Predicate &predicate )
        : v_( true )
        {}

        template< class T >
        void visit ( T &x )
        {
          v_ &= predicate_( x );
        }

        operator bool() const { return v_; }

      private:
        bool v_;
        Predicate predicate_;
      };

      template< class T >
      struct ConstReferenceTypeEvaluator
      {
        typedef const T & Type;

        static Type apply( T &t ) { return t; }
      };

    public:
      template< class Predicate >
      static bool apply ( const Tuple &tuple, const Predicate &predicate = Predicate() )
      {
        typedef typename Dune::ForEachType< ConstReferenceTypeEvaluator, Tuple >::Type IntermediaryTuple;
        IntermediaryTuple intermediary = Dune::transformTuple< ConstReferenceTypeEvaluator >( tuple );
        Dune::ForEachValue< IntermediaryTuple > forEach( intermediary );
        CheckPredicateFunctor< Predicate > check( predicate );
        forEach.apply( check );
        return check;
      }
    };

  } // namespace

#endif // #ifndef DOXYGEN



  // check_predicate_tuple
  // ---------------------

  /** \ingroup Tuples_Algorithms
   *
   *  \brief Check a predicate for all tuple elements.
   *
   *  \tparam  Predicate  A predicate
   *  \tparam  Tuple      Some tuple type
   *
   *  The predicate must be a copyable object and is expected to provide
   *  the following method:
   *  @code
   *  struct Predicate
   *  {
   *    template< class T >
   *    bool operator() ( const &T x );
   *  };
   *  @endcode
   *
   *  \returns \b true if predicate applies to all elements in tuple, \b false otherwise
   */
  template< class Predicate, class Tuple >
  bool check_predicate_tuple ( const Tuple &tuple, const Predicate &predicate = Predicate() )
  {
    return CheckPredicateHelper< Tuple >::apply( tuple, predicate );
  }

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_CHECKPREDICATE_HH
