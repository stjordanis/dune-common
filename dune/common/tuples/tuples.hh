// -*- tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=8 sw=2 sts=2:
#ifndef DUNE_COMMON_TUPLES_TUPLES_HH
#define DUNE_COMMON_TUPLES_TUPLES_HH

#include <tuple>

#include <dune/common/static_assert.hh>
#include <dune/common/typetraits.hh>

namespace Dune
{

  /** @addtogroup Common
   *
   * @{
   */

  template<class T>
  struct TupleAccessTraits
  {
    typedef typename ConstantVolatileTraits<T>::ConstType& ConstType;
    typedef T& NonConstType;
    typedef const typename ConstantVolatileTraits<T>::UnqualifiedType& ParameterType;
  };

  template<class T>
  struct TupleAccessTraits<T*>
  {
    typedef typename ConstantVolatileTraits<T>::ConstType* ConstType;
    typedef T* NonConstType;
    typedef T* ParameterType;
  };

  template<class T>
  struct TupleAccessTraits<T&>
  {
    typedef T& ConstType;
    typedef T& NonConstType;
    typedef T& ParameterType;
  };



  using std::tuple;
  using std::make_tuple;
  using std::forward_as_tuple;
  using std::tie;
  using std::tuple_cat;



  // tuple_element
  // -------------

  template< std::size_t I, class Tuple >
  struct tuple_element
  {
    typedef typename std::tuple_element< I, Tuple >::type type;
  };

  template< std::size_t I, class Tuple >
  struct tuple_element< I, const Tuple >
  {
    typedef typename std::tuple_element< I, Tuple >::type type;
  };



  // tuple_size
  // ----------

  template< class Tuple >
  struct tuple_size
  : public std::tuple_size< Tuple >
  {};



  // get
  // ---

  template< std::size_t I, class Tuple >
  const typename tuple_element< I, Tuple >::type &get ( const Tuple &tuple ) throw()
  {
    return std::get< I >( tuple );
  }

  template< std::size_t I, class Tuple >
  typename tuple_element< I, Tuple >::type &get ( Tuple &tuple ) throw()
  {
    return std::get< I >( tuple );
  }

  /** @} */

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_TUPLES_HH
