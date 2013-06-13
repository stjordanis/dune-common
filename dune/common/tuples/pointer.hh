#ifndef DUNE_COMMON_TUPLES_POINTER_HH
#define DUNE_COMMON_TUPLES_POINTER_HH

#include <dune/common/static_assert.hh>

#include <dune/common/tuples/foreach.hh>
#include <dune/common/tuples/tuples.hh>

namespace Dune
{

  // NullPointerInitialiser
  // ----------------------

  /**
   * @brief A helper template that initializes a tuple consisting of pointers
   * to NULL.
   *
   * A tuple of NULL pointers may be useful when you use a tuple of pointers
   * in a class which you can only initialise in a later stage.
   */
  template <class Tuple>
  class NullPointerInitialiser {
    dune_static_assert(AlwaysFalse<Tuple>::value, "Attempt to use the "
                       "unspecialized version of NullPointerInitialiser.  "
                       "NullPointerInitialiser needs to be specialized for "
                       "each possible tuple size.  Naturally the number of "
                       "pre-defined specializations is limited arbitrarily.  "
                       "Maybe you need to raise this limit by defining some "
                       "more specializations?  Also check that the tuple this "
                       "is applied to really is a tuple of pointers only.");
  public:
    //! export the type of the tuples
    typedef Tuple ResultType;
    //! generate a zero-initialized tuple
    static ResultType apply();
  };

#ifndef DOXYGEN
  template<class Tuple>
  struct NullPointerInitialiser<const Tuple>
    : public NullPointerInitialiser<Tuple>
  {
    typedef const Tuple ResultType;
  };

  template<>
  struct NullPointerInitialiser<tuple<> > {
    typedef tuple<> ResultType;
    static ResultType apply() {
      return ResultType();
    }
  };

  template<class T0>
  struct NullPointerInitialiser<tuple<T0*> > {
    typedef tuple<T0*> ResultType;
    static ResultType apply() {
      return ResultType(static_cast<T0*>(0));
    }
  };

  template<class T0, class T1>
  struct NullPointerInitialiser<tuple<T0*, T1*> > {
    typedef tuple<T0*, T1*> ResultType;
    static ResultType apply() {
      return ResultType(static_cast<T0*>(0), static_cast<T1*>(0));
    }
  };

  template<class T0, class T1, class T2>
  struct NullPointerInitialiser<tuple<T0*, T1*, T2*> > {
    typedef tuple<T0*, T1*, T2*> ResultType;
    static ResultType apply() {
      return ResultType(static_cast<T0*>(0), static_cast<T1*>(0),
                        static_cast<T2*>(0));
    }
  };

  template<class T0, class T1, class T2, class T3>
  struct NullPointerInitialiser<tuple<T0*, T1*, T2*, T3*> > {
    typedef tuple<T0*, T1*, T2*, T3*> ResultType;
    static ResultType apply() {
      return ResultType(static_cast<T0*>(0), static_cast<T1*>(0),
                        static_cast<T2*>(0), static_cast<T3*>(0));
    }
  };

  template<class T0, class T1, class T2, class T3, class T4>
  struct NullPointerInitialiser<tuple<T0*, T1*, T2*, T3*, T4*> > {
    typedef tuple<T0*, T1*, T2*, T3*, T4*> ResultType;
    static ResultType apply() {
      return ResultType(static_cast<T0*>(0), static_cast<T1*>(0),
                        static_cast<T2*>(0), static_cast<T3*>(0),
                        static_cast<T4*>(0));
    }
  };

  template<class T0, class T1, class T2, class T3, class T4, class T5>
  struct NullPointerInitialiser<tuple<T0*, T1*, T2*, T3*, T4*, T5*> > {
    typedef tuple<T0*, T1*, T2*, T3*, T4*, T5*> ResultType;
    static ResultType apply() {
      return ResultType(static_cast<T0*>(0), static_cast<T1*>(0),
                        static_cast<T2*>(0), static_cast<T3*>(0),
                        static_cast<T4*>(0), static_cast<T5*>(0));
    }
  };

  template<class T0, class T1, class T2, class T3, class T4, class T5,
      class T6>
  struct NullPointerInitialiser<tuple<T0*, T1*, T2*, T3*, T4*, T5*, T6*> > {
    typedef tuple<T0*, T1*, T2*, T3*, T4*, T5*, T6*> ResultType;
    static ResultType apply() {
      return ResultType(static_cast<T0*>(0), static_cast<T1*>(0),
                        static_cast<T2*>(0), static_cast<T3*>(0),
                        static_cast<T4*>(0), static_cast<T5*>(0),
                        static_cast<T6*>(0));
    }
  };

  template<class T0, class T1, class T2, class T3, class T4, class T5,
      class T6, class T7>
  struct NullPointerInitialiser<tuple<T0*, T1*, T2*, T3*, T4*, T5*, T6*,
          T7*> > {
    typedef tuple<T0*, T1*, T2*, T3*, T4*, T5*, T6*, T7*> ResultType;
    static ResultType apply() {
      return ResultType(static_cast<T0*>(0), static_cast<T1*>(0),
                        static_cast<T2*>(0), static_cast<T3*>(0),
                        static_cast<T4*>(0), static_cast<T5*>(0),
                        static_cast<T6*>(0), static_cast<T7*>(0));
    }
  };

  template<class T0, class T1, class T2, class T3, class T4, class T5,
      class T6, class T7, class T8>
  struct NullPointerInitialiser<tuple<T0*, T1*, T2*, T3*, T4*, T5*, T6*,
          T7*, T8*> > {
    typedef tuple<T0*, T1*, T2*, T3*, T4*, T5*, T6*, T7*, T8*> ResultType;
    static ResultType apply() {
      return ResultType(static_cast<T0*>(0), static_cast<T1*>(0),
                        static_cast<T2*>(0), static_cast<T3*>(0),
                        static_cast<T4*>(0), static_cast<T5*>(0),
                        static_cast<T6*>(0), static_cast<T7*>(0),
                        static_cast<T8*>(0));
    }
  };

  // template<class T0, class T1, class T2, class T3, class T4, class T5,
  //          class T6, class T7, class T8, class T9>
  // struct NullPointerInitialiser<tuple<T0*, T1*, T2*, T3*, T4*, T5*, T6*,
  //                                     T7*, T8*, T9*> > {
  //   typedef tuple<T0*, T1*, T2*, T3*, T4*, T5*, T6*, T7*, T8*, T9*> ResultType;
  //   static ResultType apply() {
  //     return ResultType(static_cast<T0*>(0), static_cast<T1*>(0),
  //                       static_cast<T2*>(0), static_cast<T3*>(0),
  //                       static_cast<T4*>(0), static_cast<T5*>(0),
  //                       static_cast<T6*>(0), static_cast<T7*>(0),
  //                       static_cast<T8*>(0), static_cast<T9*>(0));
  //   }
  // };
#endif // !defined(DOXYGEN)



  // PointerPairDeletor
  // ------------------

  /**
   * @brief Deletes all objects pointed to in a tuple of pointers.
   *
   * \warning Pointers cannot be set to NULL, so calling the Deletor twice
   * or accessing elements of a deleted tuple leads to unforeseeable results!
   */
  template <class Tuple>
  class PointerPairDeletor
  {
    struct Deletor {
      template<typename P> void visit(const P& p) { delete p; }
    };

  public:
    static void apply(Tuple& t) {
      static Deletor deletor;
      ForEachValue<Tuple>(t).apply(deletor);
    }
  };

} // namespace Dune

#endif // #ifndef DUNE_COMMON_TUPLES_POINTER_HH
