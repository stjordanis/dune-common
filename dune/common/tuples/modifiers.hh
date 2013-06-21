#ifndef DUNE_COMMON_TUPLES_MODIFIERS_HH
#define DUNE_COMMON_TUPLES_MODIFIERS_HH

#include <dune/common/documentation.hh>
#include <dune/common/static_assert.hh>

#include <dune/common/tuples/enumeration.hh>
#include <dune/common/tuples/subtuple.hh>
#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  /** @addtogroup Tuples_MetaProgramming
   *
   * @{
   */

  // PushBackTuple
  // -------------

  /** \brief Helper template to append a type to a tuple
   *
   *  \tparam Tuple The tuple type to extend
   *  \tparam T     The type to be appended to the tuple
   */
  template< class Tuple, class T >
  struct PushBackTuple
  {
    static_assert( AlwaysFalse< Tuple >::value,
                   "PushBackTuple may only be used on Dune::tuple." );

    /**
     * \brief For all specializations this is the type of a tuple with T appended.
     *
     * Suppose you have Tuple=tuple<T1, T2, ..., TN> then
     * this type is tuple<T1, T2, ..., TN, T>.
     */
    typedef ImplementationDefined Type;
  };

#ifndef DOXYGEN
  template< class T, class... U >
  struct PushBackTuple< Dune::tuple< U... >, T >
  {
    typedef Dune::tuple< U..., T > Type;

    static Type apply ( const Dune::tuple< U... > &tuple, T t )
    {
      typedef typename EnumerationTuple< std::size_t, sizeof...( U ) >::Type Enumeration;
      return apply( tuple, t, Enumeration() );
    }

  private:
    template< class... I >
    static Type apply ( const Dune::tuple< U... > &tuple, T t, Dune::tuple< I... > )
    {
      return Type( Dune::get< I::value >( tuple )..., t );
    }
  };
#endif // #ifndef DOXYGEN



  // tuple_push_back
  // ---------------

  /** \brief Create new tuple instance from given tuple and element
   *         to append at the end.
   */
  template< class T, class... U >
  inline Dune::tuple< U..., T >
  tuple_push_back ( const Dune::tuple< U... > &tuple, T t )
  {
    return PushBackTuple< Dune::tuple< U... >, T >::apply( tuple, t );
  }



  // PushFrontTuple
  // --------------

  /** \brief Helper template to prepend a type to a tuple
   *
   *  \tparam Tuple The tuple type to extend
   *  \tparam T     The type to be prepended to the tuple
   */
  template< class Tuple, class T >
  struct PushFrontTuple
  {
    static_assert( AlwaysFalse< Tuple >::value,
                   "PushFrontTuple may only be used on Dune::tuple." );

    /**
     * \brief For all specializations this is the type of a tuple with T prepended.
     *
     * Suppose you have Tuple=tuple<T1, T2, ..., TN> then
     * this type is tuple<T, T1, T2, ..., TN>.
     */
    typedef ImplementationDefined Type;
  };

#ifndef DOXYGEN
  template< class T, class... U >
  struct PushFrontTuple< Dune::tuple< U... >, T >
  {
    typedef Dune::tuple< T, U... > Type;

    static Type apply ( const Dune::tuple< U... > &tuple, T t )
    {
      typedef typename EnumerationTuple< std::size_t, sizeof...( U ) >::Type Enumeration;
      return apply( tuple, t, Enumeration() );
    }

  private:
    template< class... I >
    static Type apply ( const Dune::tuple< U... > &tuple, T t, Dune::tuple< I... > )
    {
      return Type( t, Dune::get< I::value >( tuple )... );
    }
  };
#endif // #ifndef DOXYGEN



  // tuple_push_front
  // ----------------

  /** \brief Create new tuple instance from given tuple and element
   *         to append at the beginning.
   */
  template< class T, class... U >
  inline Dune::tuple< T, U... >
  tuple_push_front ( const Dune::tuple< U... > &tuple, T t )
  {
    return PushFrontTuple< Dune::tuple< U... >, T >::apply( tuple, t );
  }



  // PopBackTuple
  // ------------

  /** \brief Remove last element from tuple type.
   *
   *  \tparam Tuple Tuple type to be modified.
   *  \tparam size  (implementation internal)
   */
  template< class Tuple, int size = Dune::tuple_size< Tuple >::value >
  class PopBackTuple
  {
    typedef typename Dune::EnumerationTuple< int, (Dune::tuple_size< Tuple >::value - 1), 0 >::Type Enumeration;

  public:
    typedef typename Dune::SubTuple< Enumeration, Tuple >::Type Type;
  };



  // tuple_pop_back
  // --------------

  /** \brief Remove last element from tuple */
  template< class Tuple >
  inline typename PopBackTuple< Tuple >::Type tuple_pop_back ( const Tuple &tuple )
  {
    typedef typename Dune::EnumerationTuple< int, (Dune::tuple_size< Tuple >::value - 1), 0 >::Type Enumeration;
    return Dune::sub_tuple< Enumeration >( tuple );
  }



  // PopFrontTuple
  // -------------

  /** \brief Remove first element from tuple type.
   *
   *  \tparam Tuple Tuple type to be modified.
   */
  template< class Tuple >
  class PopFrontTuple
  {
    typedef typename Dune::EnumerationTuple< int, (Dune::tuple_size< Tuple >::value - 1), 1 >::Type Enumeration;

  public:
    typedef typename Dune::SubTuple< Enumeration, Tuple >::Type Type;
  };



  // tuple_pop_front
  // ---------------

  /** \brief Remove first element from tuple */
  template< class Tuple >
  inline typename PopFrontTuple< Tuple >::Type tuple_pop_front ( const Tuple &tuple )
  {
    typedef typename Dune::EnumerationTuple< int, (Dune::tuple_size< Tuple >::value - 1), 0 >::Type Enumeration;
    return Dune::sub_tuple< Enumeration >( tuple );
  }



  // ReduceTuple
  // -----------

  /** \brief Apply reduce with meta binary function to template
   *
   *  For a tuple\<T0,T1,...,TN-1,TN,...\> the exported result is
   *
   *  F\< ... F\< F\< F\<Seed,T0\>\::type, T1\>\::type, T2\>\::type,  ... TN-1\>\::type
   *
   *  \tparam F Binary meta function
   *  \tparam Tuple Apply reduce operation to this tuple
   *  \tparam Seed Initial value for reduce operation
   *  \tparam N Reduce the first N tuple elements
   */
  template<
      template <class, class> class F,
      class Tuple,
      class Seed=tuple<>,
      int N=tuple_size<Tuple>::value>
  struct ReduceTuple
  {
    typedef typename ReduceTuple<F, Tuple, Seed, N-1>::type Accumulated;
    typedef typename tuple_element<N-1, Tuple>::type Value;

    //! Result of the reduce operation
    typedef typename F<Accumulated, Value>::type type;
  };

#ifndef DOXYGEN
  template< template <class, class> class F, class Tuple, class Seed>
  struct ReduceTuple<F, Tuple, Seed, 0>
  {
    //! Result of the reduce operation
    typedef Seed type;
  };
#endif // #ifndef DOXYGEN



  // JoinTuples
  // ----------

  /** \brief Join two tuples
   *
   *  For Head=tuple<T0,...,TN> and Tail=tuple<S0,...,SM>
   *  the exported result is tuple<T0,..,TN,S0,...,SM>.
   *
   *  \tparam Head Head of resulting tuple
   *  \tparam Tail Tail of resulting tuple
   */
  template<class Head, class Tail>
  struct JoinTuples
  {
    //! Result of the join operation
    typedef typename ReduceTuple< PushBackTuple, Tail, Head>::type type;
  };



  // FlattenTuple
  // ------------

  /** \brief Flatten a tuple of tuples
   *
   *  This flattens a tuple of tuples tuple<tuple<T0,...,TN>, tuple<S0,...,SM> >
   *  and exports tuple<T0,..,TN,S0,...,SM>.
   *
   *  \tparam TupleTuple A tuple of tuples
   */
  template<class TupleTuple>
  struct FlattenTuple
  {
    //! Result of the flatten operation
    typedef typename ReduceTuple< JoinTuples, TupleTuple>::type type;
  };

  /** @} */

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_MODIFIERS_HH
