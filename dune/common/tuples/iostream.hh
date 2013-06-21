// -*- tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=8 sw=2 sts=2:
#ifndef DUNE_COMMON_TUPLES_IOSTREAM_HH
#define DUNE_COMMON_TUPLES_IOSTREAM_HH

#include <iostream>

#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  /** @addtogroup Common
   *
   * @{
   */

  template<int i>
  struct tuple_writer
  {
    template<class T>
    static std::ostream& put(std::ostream& os, const T& t, const char* delim=", ")
    {
      return tuple_writer<i-1>::put(os,t,delim)<<delim<<Dune::get<i-1>(t);
    }

    template< class T >
    static std::istream &get ( std::istream &is, T &t, const char *delim = "," )
    {
      tuple_writer< i-1 >::get( is, t, delim );
      for( const char *it = delim; is && (*it != 0); ++it )
      {
        char c = 0;
        is >> c;
        if( c != *it )
          is.setstate( std::ios::failbit );
      }
      return is >> Dune::get< i-1 >( t );
    }
  };

  template<>
  struct tuple_writer<1>
  {
    template<class T>
    static std::ostream& put(std::ostream& os, const T& t, const char* delim=", ")
    {
      return os<<Dune::get<0>(t);
    }

    template< class T >
    static std::istream &get ( std::istream &is, T &t, const char *delim = ", " )
    {
      return is >> Dune::get< 0 >( t );
    }
  };

  template<>
  struct tuple_writer<0>
  {
    template<class T>
    static std::ostream& put(std::ostream& os, const T& t, const char* delim=", ")
    {
      return os;
    }

    template< class T >
    static std::istream &get ( std::istream &is, T &t, const char *delim = ", " )
    {
      return is;
    }
  };



  /** \brief Print a tuple */
  template< class charT, class traits, class... T >
  inline std::basic_ostream< charT, traits > &
  write_tuple ( std::basic_ostream< charT, traits > &os,
                const Dune::tuple< T... > &tuple,
                const char *delim = ", " )
  {
    return tuple_writer< sizeof...( T ) >::put( os, tuple, delim );
  }

  /** \brief Print a tuple */
  template< class charT, class traits, class... T >
  inline std::basic_ostream< charT, traits > &
  operator<< ( std::basic_ostream< charT, traits > &os,
               const Dune::tuple< T... > &tuple )
  {
    return write_tuple( os, tuple );
  }



  /** \brief Read a tuple */
  template< class charT, class traits, class... T >
  inline std::basic_istream< charT, traits > &
  read_tuple ( std::basic_istream< charT, traits > &is,
               Dune::tuple< T... > &tuple,
               const char *delim = ", " )
  {
    return tuple_writer< sizeof...( T ) >::get( is, tuple, delim );
  }

  /** \brief Read a tuple */
  template< class charT, class traits, class... T >
  inline std::basic_istream< charT, traits > &
  operator>> ( std::basic_istream< charT, traits > &is,
               Dune::tuple< T... > &tuple )
  {
    return read_tuple( is, tuple );
  }

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_IOSTREAM_HH
