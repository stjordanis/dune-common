// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_FUNCTIONS_COMMON_SHARED_FROM_THIS_HH
#define DUNE_FUNCTIONS_COMMON_SHARED_FROM_THIS_HH

#include <memory>
#include <dune/common/shared_ptr.hh>

namespace Dune {

  /* \brief Mixin class similar to std::enable_shared_from_this, but compatible with stack-allocated objects.
   *
   * This mixin class extends std::enable_shared_from_this to make it compatible with objects
   * created on the stack. The standard version of shared_from_this() throws an exception of type
   * std::bad_weak_ptr if no shared_ptr has been attached to the object yet, which interacts badly
   * with objects living on the stack, as there is no shared_ptr owning such an object.
   * This implementation will instead create a new shared_ptr with stackobject_to_shared_ptr() and
   * transfer ownership of itself to that shared_ptr, making it possible to hand out shared_ptrs to
   * stack-allocated objects.

   * \warning The internally generated shared_ptr will become the owner of this object. Make sure to *always*
   *          manually create an external shared_ptr for heap-allocated objects before calling shared_from_this()
   *          for the first time. Otherwise you *will* create a resource leak. If you want to be on the safe
   *          side, make sure to always create heap-allocated ojects with make_shared() or allocate_shared().
   */
  template<typename T>
  class stack_compatible_enable_shared_from_this
    : public std::enable_shared_from_this<T>
  {
  public:

    /** \brief Get a shared_ptr to this object to obtain shared ownership.
     *
     * \warning The semantics of this method are slightly different from the default version
     *          in the C++ standard. Notably, the standard version of shared_from_this() throws
     *          an exception of type std::bad_weak_ptr if no shared_ptr has been attached to the
     *          object. In contrast, this implementation will switch to non-managed mode if you
     *          call shared_from_this() without an attached shared_ptr. This change makes it
     *          possible to call shared_from_this() on stack-allocated objects.
     */
    shared_ptr<T> shared_from_this()
    {
      try
        {
          return std::enable_shared_from_this<T>::shared_from_this();
        }
      catch (std::bad_weak_ptr&)
        {
          _local_ptr = stackobject_to_shared_ptr(*static_cast<T*>(this));
          return _local_ptr;
        }
    }

    /** \brief Get a shared_ptr to this object to obtain shared ownership.
     *
     * \warning The semantics of this method are slightly different from the default version
     *          in the C++ standard. Notably, the standard version of shared_from_this() throws
     *          an exception of type std::bad_weak_ptr if no shared_ptr has been attached to the
     *          object. In contrast, this implementation will switch to non-managed mode if you
     *          call shared_from_this() without an attached shared_ptr. This change makes it
     *          possible to call shared_from_this() on stack-allocated objects.
     */
    shared_ptr<const T> shared_from_this() const
    {
      try
        {
          return std::enable_shared_from_this<T>::shared_from_this();
        }
      catch (std::bad_weak_ptr&)
        {
          _local_ptr = stackobject_to_shared_ptr(*const_cast<T*>(static_cast<const T*>(this)));
          return _local_ptr;
        }
    }

  private:

    mutable shared_ptr<T> _local_ptr;
  };

} // end of namespace Dune

#endif // DUNE_FUNCTIONS_COMMON_SHARED_FROM_THIS_HH
