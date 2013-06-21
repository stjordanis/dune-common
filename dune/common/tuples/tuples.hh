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
  using std::tuple_element;
  using std::get;
  using std::tuple_size;
  using std::tie;
  using std::make_tuple;

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_TUPLES_HH
