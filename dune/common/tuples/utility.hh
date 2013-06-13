#ifndef DUNE_COMMON_TUPLES_UTILITY_HH
#define DUNE_COMMON_TUPLES_UTILITY_HH

#include <dune/common/typetraits.hh>

namespace Dune
{

  // Sample TypeEvaluators
  // ---------------------



  // AddRefTypeEvaluator
  // -------------------

  //! \c TypeEvaluator to turn a type \c T into a reference to \c T
  /**
   * This is suitable as the \c TypeEvaluator template parameter for
   * ForEachType and transformTuple().
   */
  template<class T>
  struct AddRefTypeEvaluator {
    typedef T& Type;
    static Type apply(T& t) { return t; }
  };



  // AddPtrTypeEvaluator
  // -------------------

  //! \c TypeEvaluator to turn a type \c T into a pointer to \c T
  /**
   * This is suitable as the \c TypeEvaluator template parameter for
   * ForEachType and transformTuple().
   */
  template<class T>
  struct AddPtrTypeEvaluator {
    typedef typename remove_reference<T>::type* Type;
    static Type apply(T& t) { return &t; }
  };

  // Specialization, in case the type is already a reference
  template<class T>
  struct AddPtrTypeEvaluator<T&> {
    typedef typename remove_reference<T>::type* Type;
    static Type apply(T& t) { return &t; }
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_UTILITY_HH
