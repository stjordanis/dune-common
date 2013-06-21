#ifndef DUNE_COMMON_TUPLES_FVECTOR_HH
#define DUNE_COMMON_TUPLES_FVECTOR_HH

#include <cstddef>

#include <dune/common/fvector.hh>

#include <dune/common/tuples/densevector.hh>
#include <dune/common/tuples/foreach.hh>
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

  /** \ingroup Tuples_DenseVector
   *
   *  \brief Implementation of Dune::DenseVectorTuple.
   *         Each element of Dune::FieldVectorTuple is of
   *         type Dune::FieldVector.
   *
   *  \tparam  Field       Field type
   *  \tparam  Dimensions  Tuple of integral constants using \c int.
   *                       See Dune::IntegralConstantTuple for a
   *                       convenient construction.
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

#ifndef DOXYGEN
  protected:
    friend class DenseVectorTuple< FieldVectorTuple< Field, Dimensions > >;

    typename Base::tuple &raw () { return tuple_; }
    const typename Base::tuple &raw () const { return tuple_; }
#endif // #ifndef DOXYGEN

  private:
    typename Base::tuple tuple_;
  };



  // tuple_element for FieldVectorTuple
  // ----------------------------------

  template< std::size_t i, class Field, class Dimensions >
  struct tuple_element< i, FieldVectorTuple< Field, Dimensions > >
  {
    typedef typename tuple_element< i, typename DenseVectorTupleTraits< FieldVectorTuple< Field, Dimensions > >::tuple >::type type;
  };

  template< std::size_t i, class Field, class Dimensions >
  struct tuple_element< i, const FieldVectorTuple< Field, Dimensions > >
  {
    typedef typename tuple_element< i, typename DenseVectorTupleTraits< FieldVectorTuple< Field, Dimensions > >::tuple >::type type;
  };



  // get for FieldVectorTuple
  // ------------------------

  template< std::size_t i, class Field, class Dimensions >
  const typename tuple_element< i, FieldVectorTuple< Field, Dimensions > >::type &
  get ( const FieldVectorTuple< Field, Dimensions > &tuple ) throw()
  {
    return get< i >( static_cast< const typename DenseVectorTupleTraits< FieldVectorTuple< Field, Dimensions > >::tuple & >( tuple ) );
  }

  template< std::size_t i, class Field, class Dimensions >
  typename tuple_element< i, FieldVectorTuple< Field, Dimensions > >::type &
  get ( FieldVectorTuple< Field, Dimensions > &tuple ) throw()
  {
    return get< i >( static_cast< typename DenseVectorTupleTraits< FieldVectorTuple< Field, Dimensions > >::tuple & >( tuple ) );
  }



  // tuple_size for FieldVectorTuple
  // -------------------------------

  template< class Field, class Dimensions >
  struct tuple_size< FieldVectorTuple< Field, Dimensions > >
  {
    enum { value = tuple_size< typename DenseVectorTupleTraits< FieldVectorTuple< Field, Dimensions > >::tuple >::value };
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_FVECTOR_HH
