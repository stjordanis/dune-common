#ifndef DUNE_COMMON_TUPLES_INTEGRALCONSTANT_HH
#define DUNE_COMMON_TUPLES_INTEGRALCONSTANT_HH

#include <climits>
#include <cstddef>

#include <dune/common/typetraits.hh>

#include <dune/common/tuples/modifiers.hh>
#include <dune/common/tuples/tuples.hh>

namespace Dune
{

#if HAVE_VARIADIC_TEMPLATES || DOXYGEN

  // IntegralConstantTuple
  // ---------------------

  /** \ingroup Tuples_Traits
   *
   *  \brief Convenient setup of a Dune::tuple consisting
   *         of Dune::integral_constant.
   *
\code
  typedef typename Dune::IntegralConstantTuple< int, 1, 2, 3 >::Type Type;
  // 'Type' corresponds to:
  // Dune::tuple< Dune::integral_constant< int, 1 >,
  //              Dune::integral_constant< int, 2 >,
  //              Dune::integral_constant< int, 3 > >
\endcode
   *
   *  \note This documentations covers the implementation
   *        of Dune::IntegralConstantTuple if variadic
   *        templates are available. There is a fallback
   *        implementations for older compilers, which limits
   *        the number of template values to 9.
   */
  template< class T, T... w >
  class IntegralConstantTuple;

  template< class T, T v, T... w >
  class IntegralConstantTuple< T, v, w... >
  {
  public:
    typedef typename PushFrontTuple< typename IntegralConstantTuple< T, w... >::Type, integral_constant< T, v > >::type Type;
  };

  template< class T >
  class IntegralConstantTuple< T >
  {
  public:
    typedef tuple<> Type;
  };

#else // #if HAVE_VARIADIC_TEMPLATES

  // IntegralConstantTupleTraits
  // ---------------------------

  template< class T >
  struct IntegralConstantTupleTraits;

  template<>
  struct IntegralConstantTupleTraits< int >
  {
    static const int unused = INT_MIN;
  };

  template<>
  struct IntegralConstantTupleTraits< unsigned int >
  {
    static const unsigned int unused = UINT_MAX;
  };

  template<>
  struct IntegralConstantTupleTraits< std::size_t >
  {
    static const std::size_t unused = std::size_t( -1 );
  };



  // IntegralConstantTuple
  // ---------------------

  template< class T, T v, T w1 = IntegralConstantTupleTraits< T >::unused,
                          T w2 = IntegralConstantTupleTraits< T >::unused,
                          T w3 = IntegralConstantTupleTraits< T >::unused,
                          T w4 = IntegralConstantTupleTraits< T >::unused,
                          T w5 = IntegralConstantTupleTraits< T >::unused,
                          T w6 = IntegralConstantTupleTraits< T >::unused,
                          T w7 = IntegralConstantTupleTraits< T >::unused,
                          T w8 = IntegralConstantTupleTraits< T >::unused >
  class IntegralConstantTuple
  {
    static const T unused = IntegralConstantTupleTraits< T >::unused;

    template< T u1, T u2, T u3, T u4, T u5, T u6, T u7, T u8, T u9, class Seed = tuple<> >
    class Create
    {
      typedef typename PushBackTuple< Seed, integral_constant< T, u1 > >::type Accumulated;

    public:
      typedef typename Create< u2, u3, u4, u5, u6, u7, u8, u9, unused, Accumulated>::Type Type;
    };

    template< class Seed >
    struct Create< unused, unused, unused, unused, unused, unused, unused, unused, unused, Seed >
    {
      typedef Seed Type;
    };

  public:
    typedef typename Create< v, w1, w2, w3, w4, w5, w6, w7, w8 >::Type Type;
  };

#endif // #if HAVE_VARIADIC_TEMPLATES

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_INTEGRALCONSTANT_HH
