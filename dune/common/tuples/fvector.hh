#ifndef DUNE_COMMON_TUPLES_FVECTOR_HH
#define DUNE_COMMON_TUPLES_FVECTOR_HH

#include <cstddef>

#include <dune/common/fvector.hh>

#include <dune/common/tuples/densevector.hh>
#include <dune/common/tuples/integralconstant.hh>
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

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_FVECTOR_HH
