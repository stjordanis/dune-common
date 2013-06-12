#ifndef DUNE_COMMON_TUPLES_FVECTOR_HH
#define DUNE_COMMON_TUPLES_FVECTOR_HH

#include <cstddef>

#include <dune/common/fvector.hh>

#include <dune/common/tuples/densevector.hh>
#include <dune/common/tuples/integralconstant.hh>
#include <dune/common/tuples/namespace.hh>
#include <dune/common/tuples/tupleutility.hh>
#include <dune/common/tuples/uniqueelementtype.hh>

namespace Dune
{

  // Internal forward declaration
  // ----------------------------

  template< class Field, class Dimensions >
  class FieldVectorTuple;



  // DenseVectorTupleTraits
  // ----------------------

  template< class Field, class Dimensions >
  class DenseVectorTupleTraits< FieldVectorTuple< Field, Dimensions > >
  {
    template< class T >
    struct TypeEvaluator
    {
      typedef FieldVector< Field, T::value > Type;
    };

  public:
    // implementation type
    typedef FieldVectorTuple< Field, Dimensions > derived_type;
    // raw tuple type
    typedef typename ForEachType< TypeEvaluator, Dimensions >::Type tuple;

    // field type
    typedef Field field_type;
    // size type
    typedef std::size_t size_type;
    // field type tuple of equal length
    typedef typename UniqueElementTypeTuple< field_type, tuple_size< tuple >::value >::Type field_type_tuple;
  };



  // FieldVectorTuple
  // ----------------

  /** \class FieldVectorTuple
   *
   *  \brief Provide type of Dune::tuple consisting of
   *         Dune::FieldVector and given dimensions
   *
   *  \tparam  Field       field type
   *  \tparam  Dimensions  tuple of integral constants
     */
  template< class Field, class Dimensions >
  class FieldVectorTuple
  : public DenseVectorTuple< FieldVectorTuple< Field, Dimensions > >
  {
    typedef DenseVectorTuple< FieldVectorTuple< Field, Dimensions > > Base;

    template< class IntegralConstantTuple, int init = 0, std::size_t length = tuple_size< IntegralConstantTuple >::value >
    class Accumulate
    {
      static const int next = init+tuple_element< length-1, IntegralConstantTuple >::type::value;

    public:
      static const int value = Accumulate< IntegralConstantTuple, next, length-1 >::value;
    };

    template< class IntegralConstantTuple, int init >
    struct Accumulate< IntegralConstantTuple, init, 0 >
    {
      static const int value = init;
    };

  public:
    struct Dimension
    {
      //! \brief export template parameter \c Dimensions
      typedef Dimensions Type;
      //! \brief overall dimension of dense vector tuple
      static const int value = Accumulate< Type >::value;
    };

    using Base::operator=;

    FieldVectorTuple () {}

    explicit FieldVectorTuple ( const typename Base::field_type &alpha )
    {
      (*this) = alpha;
    }

  protected:
    friend class DenseVectorTuple< FieldVectorTuple< Field, Dimensions > >;

    typename Base::tuple &raw () { return tuple_; }
    const typename Base::tuple &raw () const { return tuple_; }

  private:
    typename Base::tuple tuple_;
  };



  // get for FieldVectorTuple
  // ------------------------

  template< int i, class Field, class Dimensions >
  typename tuple_element< i, typename DenseVectorTupleTraits< FieldVectorTuple< Field, Dimensions > >::tuple >::type &
  get ( Dune::FieldVectorTuple< Field, Dimensions > &tuple )
  {
    return get< i >( static_cast< typename DenseVectorTupleTraits< FieldVectorTuple< Field, Dimensions > >::tuple & >( tuple ) );
  }

  template< int i, class Field, class Dimensions >
  const typename tuple_element< i, typename DenseVectorTupleTraits< FieldVectorTuple< Field, Dimensions > >::tuple >::type &
  get ( const Dune::FieldVectorTuple< Field, Dimensions > &tuple )
  {
    return get< i >( static_cast< const typename DenseVectorTupleTraits< FieldVectorTuple< Field, Dimensions > >::tuple & >( tuple ) );
  }

} // namespace Dune



DUNE_OPEN_TUPLE_NAMESPACE

  // tuple_element for FieldectorTuple
  // ---------------------------------

  template< size_t i, class Field, class Dimensions >
  struct tuple_element< i, Dune::FieldVectorTuple< Field, Dimensions > >
  {
    typedef typename tuple_element< i, typename Dune::DenseVectorTupleTraits< Dune::FieldVectorTuple< Field, Dimensions > >::tuple >::type type;
  };



  // tuple_size for FieldVectorTuple
  // -------------------------------

  template< class Field, class Dimensions >
  struct tuple_size< Dune::FieldVectorTuple< Field, Dimensions > >
  {
    enum { value = Dune::FieldVectorTuple< Field, Dimensions >::tuple_size };
  };

DUNE_CLOSE_TUPLE_NAMESPACE

#endif // #ifndef DUNE_COMMON_TUPLES_FVECTOR_HH
