// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef __DUNE_CACHINGBASE_HH__
#define __DUNE_CACHINGBASE_HH__

#include "basefunctions.hh"

namespace Dune {

  /** @defgroup CachingBaseFunctionSet The CachingBaseFunctionSet
     @ingroup BaseFunction
     The CachingBaseFunctionSet is an special implementation of the interface
     BaseFunctionSetInterface. Like in FastBaseFunctionSet, the base function
     evaluations are cached.
     @{
   */

  //*************************************************************************
  //
  //  --CachingBaseFunctionSet
  //
  //! CachingBaseFunctionSet is the Implementation of a BaseFunctionSet.
  //! It stores the values at the quadrature points to speed up the evaluation
  //! of the base functions.
  //! All this pointer to base function and virtual method calling is not
  //! efficient. Therefor the values of the base function on given quadrature
  //! points is cache an on the next call of evaluate this is much faster.
  //
  // deriType is defined in basefunctions.hh
  //
  //*************************************************************************
  template<class FunctionSpaceType>
  class CachingBaseFunctionSet
    : public BaseFunctionSetDefault <FunctionSpaceType, CachingBaseFunctionSet<FunctionSpaceType> >
  {
    typedef typename FunctionSpaceType::Domain Domain;
    typedef typename FunctionSpaceType::Range Range;
    typedef typename FunctionSpaceType::JacobianRange JacobianRange;
    enum { DimDomain = FunctionSpaceType::DimDomain };
    enum { DimRange  = FunctionSpaceType::DimRange  };
  public:

    //! the BaseFunctionInterface type
    typedef BaseFunctionInterface<FunctionSpaceType> BaseFunctionInterfaceType;

    //! maximum number of different differentiation order
    enum { numDiffOrd = 1 };

    //! Constructor
    CachingBaseFunctionSet (FunctionSpaceType& fuspace,
                            ElementType eltype,
                            int nBaseFnc);

    //! Destructor
    ~CachingBaseFunctionSet();

    //! return the number of base fucntions for this BaseFunctionSet
    int getNumberOfBaseFunctions () const
    {
      return baseFunctionList_.size();
    }

    //! return the number of base fucntions for this BaseFunctionSet
    int getNumberOfDiffBaseFuncs () const
    {
      return getNumberOfBaseFunctions()/DimRange;
    }

    //! evaluate base function baseFunct with the given diffVariable and a
    //! point x and range phi
    template <int diffOrd>
    void evaluate ( int baseFunct,
                    const FieldVector<deriType, diffOrd> &diffVariable,
                    const Domain & x,  Range & phi ) const;

    //! evaluate base function baseFunct at a given quadrature point
    //! the identifier of the quadrature is stored to check , whether the
    //! qaudrature has changed and the values at the quadrature have to be
    //! calulated again
    template <int diffOrd, class QuadratureType>
    void evaluate ( int baseFunct,
                    const FieldVector<deriType, diffOrd> &diffVariable,
                    QuadratureType & quad, int quadPoint, Range & phi ) const;

    //! Alternative access to precomputed base function values.
    template <class QuadratureType>
    const std::vector<Range>& values(int baseFunct,
                                     const QuadratureType& quad) const;

    //! Alternative access to precomputed base function gradients.
    template <class QuadratureType>
    const std::vector<JacobianRange>& gradients(int baseFunct,
                                                const QuadratureType& quad) const;

    //! get a reference of the base function baseFunct
    //! this is the same concept for all basis, we have a number of
    //! base functions, and store the pointers in a vector
    const BaseFunctionInterface<FunctionSpaceType>&
    getBaseFunction( int baseFunct ) const
    {
      return (*baseFunctionList_[baseFunct]);
    }

    //! \brief Initialise the base function set for a given quadrature
    //! The base function set will only work for quadratures which were
    //! registered
    template <class QuadratureType>
    void registerQuadrature(const QuadratureType & quad);

  private:
    //- Local typedefs
    typedef std::map<IdentifierType, std::vector<std::vector<Range> > > RangeMap;
    typedef RangeMap::iterator RangeMapIterator;
    typedef RangeMap::const_iterator ConstRangeMapIterator;
    typedef std::map<IdentifierType,
        std::vector<std::vector<JacobianRange> > > JacobianMap;
    typedef JacobianMap::iterator JacobianMapIterator;
    typedef JacobianMap::const_iterator ConstJacobianMapIterator;

    //- Helper methods
    //! Extracts a gradient component from the base function
    const Range extractGradientComp(const JacobianRange& jr, int idx) const;

    //! number of basefunctions
    int numOfBaseFct_;

    //! vector which holds the base function pointers
    std::vector<BaseFunctionInterfaceType *> baseFunctionList_ ;

    //! map with cached values for base functions
    RangeMap vals_;

    //! map with cached values for base function gradients
    JacobianMap grads_;

  }; // end class CachingBaseFunctionSet


  /** @} end documentation group */

#include "cachingbase.cc"

} // end namespace Dune

#endif
