#ifndef DUNE_COMMON_TUPLES_FVECTOR_HH
#define DUNE_COMMON_TUPLES_FVECTOR_HH

#include <cstddef>

#include <dune/common/fvector.hh>

#include <dune/common/tuples/densevector.hh>
#include <dune/common/tuples/uniqueelementtype.hh>

namespace Dune
{

  // Internal forward declaration
  // ----------------------------

  template< class K, int... Sizes >
  class FieldVectorTuple;



  // DenseVectorTupleTraits
  // ----------------------

  template< class K, int... Sizes >
  struct DenseVectorTupleTraits< FieldVectorTuple< K, Sizes... > >
  {
    // implementation type
    typedef FieldVectorTuple< K, Sizes... > derived_type;

    // raw tuple type
    typedef Dune::tuple< Dune::FieldVector< K, Sizes >... > tuple;

    // field type
    typedef K field_type;
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
   *  \tparam  K      Field type
   *  \tparam  Sizes  List of integer values (variadic template parameter list)
   */
  template< class K, int... Sizes >
  class FieldVectorTuple
  : public DenseVectorTuple< FieldVectorTuple< K, Sizes... > >
  {
    typedef DenseVectorTuple< FieldVectorTuple< K, Sizes... > > Base;

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
    //! \brief export template parameters \c Sizes
    typedef tuple< Dune::integral_constant< int, Sizes >... > Dimensions;
    //! \brief overall dimension of dense vector tuple
    static const int dimension = Accumulate< Dimensions >::value;

    using Base::operator=;

    FieldVectorTuple () {}

    explicit FieldVectorTuple ( const typename Base::field_type &alpha )
    {
      (*this) = alpha;
    }

#ifndef DOXYGEN
  protected:
    friend class DenseVectorTuple< FieldVectorTuple< K, Sizes... > >;

    typename Base::tuple &raw () { return tuple_; }
    const typename Base::tuple &raw () const { return tuple_; }
#endif // #ifndef DOXYGEN

  private:
    typename Base::tuple tuple_;
  };



  // tuple_element for FieldVectorTuple
  // ----------------------------------

  template< std::size_t i, class K, int... Sizes >
  struct tuple_element< i, FieldVectorTuple< K, Sizes... > >
  {
    typedef typename tuple_element< i, typename DenseVectorTupleTraits< FieldVectorTuple< K, Sizes... > >::tuple >::type type;
  };

  template< std::size_t i, class K, int... Sizes >
  struct tuple_element< i, const FieldVectorTuple< K, Sizes... > >
  {
    typedef typename tuple_element< i, typename DenseVectorTupleTraits< FieldVectorTuple< K, Sizes... > >::tuple >::type type;
  };



  // get for FieldVectorTuple
  // ------------------------

  template< std::size_t i, class K, int... Sizes >
  const typename tuple_element< i, FieldVectorTuple< K, Sizes... > >::type &
  get ( const FieldVectorTuple< K, Sizes... > &tuple ) throw()
  {
    return get< i >( static_cast< const typename DenseVectorTupleTraits< FieldVectorTuple< K, Sizes... > >::tuple & >( tuple ) );
  }

  template< std::size_t i, class K, int... Sizes >
  typename tuple_element< i, FieldVectorTuple< K, Sizes... > >::type &
  get ( FieldVectorTuple< K, Sizes... > &tuple ) throw()
  {
    return get< i >( static_cast< typename DenseVectorTupleTraits< FieldVectorTuple< K, Sizes... > >::tuple & >( tuple ) );
  }



  // tuple_size for FieldVectorTuple
  // -------------------------------

  template< class K, int... Sizes >
  struct tuple_size< FieldVectorTuple< K, Sizes... > >
  {
    enum { value = tuple_size< typename DenseVectorTupleTraits< FieldVectorTuple< K, Sizes... > >::tuple >::value };
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_FVECTOR_HH
