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
  private:
    // used for compatibility for gcc-4.3.6
    template< int i >
    struct TypeEvaluator
    {
      typedef Dune::FieldVector< K, i > Type;
    };

  public:
    // implementation type
    typedef FieldVectorTuple< K, Sizes... > derived_type;
    // raw tuple type
    typedef Dune::tuple< typename TypeEvaluator< Sizes >::Type... > tuple;

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
  private:
    friend class RawTuple< FieldVectorTuple< K, Sizes... > >;

    typename Base::tuple &raw () { return tuple_; }
    const typename Base::tuple &raw () const { return tuple_; }
#endif // #ifndef DOXYGEN

  private:
    typename Base::tuple tuple_;
  };



  // RawTuple< FieldVectorTuple >
  // ----------------------------

  template< class K, int... Sizes >
  struct RawTuple< FieldVectorTuple< K, Sizes... > >
  {
    typedef typename DenseVectorTupleTraits< FieldVectorTuple< K, Sizes... > >::tuple Type;

    static Type &raw ( FieldVectorTuple< K, Sizes... > &fieldVectorTuple )
    {
      return fieldVectorTuple.raw();
    }
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_FVECTOR_HH
