#ifndef DUNE_COMMON_IDENTITYMATRIX_HH
#define DUNE_COMMON_IDENTITYMATRIX_HH

#include <dune/common/fmatrix.hh>
#include <dune/common/ftraits.hh>
#include <dune/common/math.hh>
#include <dune/common/std/constexpr.hh>

namespace Dune
{

  // IdentityMatrix
  // --------------

  template< class K, int N >
  struct IdentityMatrix
  {
    typedef K field_type;
    typedef std::size_t size_type;

    DUNE_CONSTEXPR size_type rows () const { return N; }
    DUNE_CONSTEXPR size_type cols () const { return N; }

    template< class X, class Y >
    void mv ( const X &x, Y &y ) const
    {
      y = x;
    }

    template< class X, class Y >
    void mtv ( const X &x, Y &y ) const
    {
      y = x;
    }

    template< class X, class Y >
    void umv ( const X &x, Y &y ) const
    {
      y += x;
    }

    template< class X, class Y >
    void umtv ( const X &x, Y &y ) const
    {
      y += x;
    }

    template< class X, class Y >
    void umhv ( const X &x, Y &y ) const
    {
      y += x;
    }

    template< class X, class Y >
    void mmv ( const X &x, Y &y ) const
    {
      y -= x;
    }

    template< class X, class Y >
    void mmtv ( const X &x, Y &y ) const
    {
      y -= x;
    }

    template< class X, class Y >
    void mmhv ( const X &x, Y &y ) const
    {
      y -= x;
    }

    template< class X, class Y >
    void usmv ( const field_type &alpha, const X &x, Y &y ) const
    {
      y.axpy( alpha, x );
    }

    template< class X, class Y >
    void usmtv ( const field_type &alpha, const X &x, Y &y ) const
    {
      y.axpy( alpha, x );
    }

    template< class X, class Y >
    void usmhv ( const field_type &alpha, const X &x, Y &y ) const
    {
      y.axpy( alpha, x );
    }

    typename FieldTraits< field_type >::real_type frobenius_norm () const
    {
      return std::sqrt( frobenius_norm2() );
    }

    typename FieldTraits< field_type >::real_type frobenius_norm2 () const
    {
      return FieldTraits< field_type >::real_type( N );
    }

    typename FieldTraits< field_type >::real_type infinity_norm () const
    {
      return FieldTraits< field_type >::real_type( 1 );
    }

    typename FieldTraits< field_type >::real_type infinity_norm_real () const
    {
      return FieldTraits< field_type >::real_type( 1 );
    }

    operator FieldMatrix< field_type, N, N > () const
    {
      FieldMatrix< field_type, N, N > fieldMatrix( 0 );
      for( int i = 0; i < N; ++i )
        fieldMatrix[ i ][ i ] = field_type( 1 );
      return fieldMatrix;
    }
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_IDENTITYMATRIX_HH
