#ifndef DUNE_COMMON_TUPLES_TYPETRAITS_HH
#define DUNE_COMMON_TUPLES_TYPETRAITS_HH

#include <dune/common/typetraits.hh>

#include <dune/common/tuples/checkpredicate.hh>
#include <dune/common/tuples/foreach.hh>
#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  // TupleTypeTraits
  // ---------------

  /** \ingroup Tuples_Traits
   *
   *  \brief This class mimicks the Dune::TypeTraits class
   *         for tuples.
   *
   *  \tparam  Tuple  Some Dune::tuple type.
   */
  template< class Tuple >
  class TupleTypeTraits
  {
    template< class T >
    struct IsPointerPredicate
    {
      static const bool value = TypeTraits< T >::isPointer;
      typedef typename TypeTraits< T >::PointeeType Type;
    };

    template< class T >
    struct IsReferencePredicate
    {
      static const bool value = TypeTraits< T >::isReference;
      typedef typename TypeTraits< T >::ReferredType Type;
    };

  public:
    //! \brief value is \b true, if each element in template argument tuple is of pointer type
    static const bool isPointerTuple = CheckPredicateTuple< Tuple, IsPointerPredicate >::value;
    //! \brief a Dune::tuple type; strip away pointer symbols in a pointer tuple
    typedef typename ForEachType< IsPointerPredicate, Tuple >::Type PointeeTupleType;

    //! \brief value is \b true, if each element in template argument tuple is of reference type
    static const bool isReferenceTuple = CheckPredicateTuple< Tuple, IsReferencePredicate >::value;
    //! \brief a Dune::tuple type; strip away reference symbols in a reference tuple
    typedef typename ForEachType< IsReferencePredicate, Tuple >::Type ReferredTupleType;
  };



  // RawTuple
  // --------

  /** \ingroup Tuples_Traits
   *
   *  \brief Access internal tuple type in wrappers.
   *
   *  \tparam  T  A Dune::tuple or a wrapper around a tuple.
   *
   *  Wrapper classes around a Dune::tuple may export the internal
   *  tuple type in non-consistent way. Template specalizations
   *  of Dune::RawTuple for wrappres solve this problem.
   *
   *  Dune::RawTuple exports the type of the wrapped tuple
   *  and allows accessing it as well:
   * \code
   * template< class T >
   * struct RawTuple
   * {
   *   typedef ImplementationDefined Type;
   *
   *   static Type &raw ( T &tuple );
   * };
   * \endcode
   *
   *  In order to behave like a Dune:tuple, Wrapper classes
   *  need to overload Dune::tuple_element, Dune::tuple_size,
   *  Dune::get, etc., which may become cumbersome.
   *  Instead, it should always be safe to call
   * \code
   *   tempalte< int i, class T >
   *   typename tuple_element< i, typename RawTuple< T >::Type >::type
   *   get ( T &tuple )
   *   {
   *     return get< i >( RawTuple< T >::raw( t ) );
   *   }
   * \endcode
   *
   * See Dune::raw_tuple for a convenient alternative call to <tt>RawTuple< T >::raw( t )</tt>.
   */
  template< class T >
  struct RawTuple;

  template< class T >
  struct RawTuple< const T >
  {
    typedef const typename RawTuple< T >::Type Type;

    static Type &raw ( const T &tuple )
    {
      return RawTuple< T >::raw( const_cast< T & >( tuple ) );
    }
  };

  template< class... T >
  struct RawTuple< Dune::tuple< T... > >
  {
    typedef Dune::tuple< T... > Type;

    static Type &raw ( Type &tuple ) { return tuple; }
  };



  // raw_tuple
  // ---------

  /** \ingroup Tuples_Traits
   *
   *  \brief Access internal tuple in wrappers.
   *
   *  \tparam  Tuple  A Dune::tuple or a wrapper around a tuple.
   *
   *  This convenience function is equivalent to
   *  <tt>Dune::RawTuple< Tuple >::raw()</tt>.
   */
  template< class Tuple >
  typename RawTuple< Tuple >::Type &raw_tuple ( Tuple &tuple )
  {
    return RawTuple< Tuple >::raw( tuple );
  }

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_TYPETRAITS_HH
