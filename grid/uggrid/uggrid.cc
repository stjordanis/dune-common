// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#include "config.h"
#include <dune/grid/uggrid.hh>

/** \todo Remove the following two includes once getAllSubfaces... is gone */
#include <dune/common/tuples.hh>
#include <dune/common/sllist.hh>
#include <dune/common/stdstreams.hh>

// *********************************************************************
//
//  -- UGGridLevelIteratorFactory
//
// *********************************************************************

namespace Dune {

  /** \brief Default implementation, just throws an exception */
  template <int codim, PartitionIteratorType PiType, class GridImp>
  class UGGridLevelIteratorFactory
  {
  public:
    static inline
    UGGridLevelIterator<codim,PiType,GridImp> getIterator(typename UGTypes<GridImp::dimension>::GridType* theGrid, int level) {
      DUNE_THROW(GridError, "Unknown LevelIterator requested");
    }

  };

  // //////////////////////////////////////////////////////////////////////
  //   Specializations for the element iterator-factories
  //   There is no overlap.  Therefore, All_Partition and Ghost_Partition
  //   loop over _all_ elements, and the remaining PartitionTypes only
  //   over the interior ones.
  // //////////////////////////////////////////////////////////////////////
  template <class GridImp>
  class UGGridLevelIteratorFactory<0,All_Partition,GridImp>
  {
  public:
    static inline
    UGGridLevelIterator<0,All_Partition,GridImp> getIterator(typename UGTypes<GridImp::dimension>::GridType* theGrid, int level) {

      return UGGridLevelIterator<0,All_Partition,GridImp>(UG_NS<GridImp::dimension>::PFirstElement(theGrid), level);
    }

  };

  template <class GridImp>
  class UGGridLevelIteratorFactory<0,Ghost_Partition,GridImp>
  {
  public:
    static inline
    UGGridLevelIterator<0,Ghost_Partition,GridImp> getIterator(typename UGTypes<GridImp::dimension>::GridType* theGrid, int level) {

      UGGridLevelIterator<0,Ghost_Partition,GridImp> it(level);
      it.setToTarget(UG_NS<GridImp::dimension>::PFirstElement(theGrid), level);
      return it;
    }

  };

  template <PartitionIteratorType PiType,class GridImp>
  class UGGridLevelIteratorFactory<0,PiType,GridImp>
  {
  public:
    static inline
    UGGridLevelIterator<0,PiType,GridImp> getIterator(typename UGTypes<GridImp::dimension>::GridType* theGrid, int level) {

      UGGridLevelIterator<0,PiType,GridImp> it(level);
      it.setToTarget(UG_NS<GridImp::dimension>::FirstElement(theGrid), level);
      return it;
    }

  };



  template <class GridImp>
  class UGGridLevelIteratorFactory<2,All_Partition,GridImp>
  {
  public:
    static inline
    UGGridLevelIterator<2,All_Partition,GridImp> getIterator(typename UGTypes<GridImp::dimension>::GridType* theGrid, int level) {

      return UGGridLevelIterator<2,All_Partition,GridImp>(UG_NS<2>::PFirstNode(theGrid), level);
    }

  };

  template <class GridImp>
  class UGGridLevelIteratorFactory<3,All_Partition,GridImp>
  {
  public:
    static inline
    UGGridLevelIterator<3,All_Partition,GridImp> getIterator(typename UGTypes<GridImp::dimension>::GridType* theGrid, int level) {

      return UGGridLevelIterator<3,All_Partition,GridImp>(UG_NS<3>::FirstNode(theGrid), level);
    }
  };

}  // end namespace Dune


using namespace Dune;

//***********************************************************************
//
// --UGGrid
// --Grid
//
//***********************************************************************

template<> int Dune::UGGrid < 2, 2 >::numOfUGGrids = 0;
template<> int Dune::UGGrid < 3, 3 >::numOfUGGrids = 0;

template<> bool Dune::UGGrid < 2, 2 >::useExistingDefaultsFile = false;
template<> bool Dune::UGGrid < 3, 3 >::useExistingDefaultsFile = false;


template < int dim, int dimworld >
inline Dune::UGGrid < dim, dimworld >::UGGrid() : multigrid_(NULL),
                                                  leafIndexSet_(*this),
                                                  globalIdSet_(*this),
                                                  localIdSet_(*this),
                                                  refinementType_(LOCAL),
                                                  omitGreenClosure_(false)
{
  init(500, 10);
}

template < int dim, int dimworld >
inline Dune::UGGrid < dim, dimworld >::UGGrid(unsigned int heapSize, unsigned envHeapSize)
  : multigrid_(NULL),
    leafIndexSet_(*this),
    globalIdSet_(*this),
    localIdSet_(*this),
    refinementType_(LOCAL),
    omitGreenClosure_(false)
{
  init(heapSize, envHeapSize);
}

template < int dim, int dimworld >
inline void Dune::UGGrid < dim, dimworld >::init(unsigned int heapSize, unsigned envHeapSize)
{
  heapsize = heapSize;

  if (numOfUGGrids==0) {

    useExistingDefaultsFile = false;

    if (access("defaults", F_OK) == 0) {

      dverb << "Using existing UG defaults file" << std::endl;
      useExistingDefaultsFile = true;

    } else {

      // Pass the explicitly given environment heap size
      // This is only possible by passing a pseudo 'defaults'-file
      FILE* fp = fopen("defaults", "w");
      fprintf(fp, "envmemory %d000000\n", envHeapSize);
      fprintf(fp, "mutelevel -1001\n");
      fclose(fp);

    }

    // Init the UG system
    int argc = 1;
    char* arg = {"dune.exe"};
    char** argv = &arg;


    UG_NS<dimworld>::InitUg(&argc, &argv);

  }

  // Create a dummy problem
  typename UG_NS<dim>::CoeffProcPtr coeffs[1];
  typename UG_NS<dim>::UserProcPtr upp[1];

  upp[0] = NULL;
  coeffs[0] = NULL;

  // Create unique problem name
  static unsigned int nameCounter = 0;
  std::stringstream numberAsAscii;
  numberAsAscii << nameCounter;
  name_ = "DuneUGGrid_" + numberAsAscii.str();

  std::string problemName = name_ + "_Problem";

  if (UG_NS<dimworld>::CreateBoundaryValueProblem(problemName.c_str(), 1,coeffs,1,upp) == NULL)
    DUNE_THROW(GridError, "UG" << dim << "d::CreateBoundaryValueProblem() returned and error code!");

  if (numOfUGGrids==0) {

    char* nfarg = "newformat DuneFormat";
    if (UG_NS<dim>::CreateFormatCmd(1, &nfarg))
      DUNE_THROW(GridError, "UG" << dim << "d::CreateFormat() returned and error code!");

  }

  numOfUGGrids++;

  extra_boundary_data_ = 0;

  dverb << "UGGrid<" << dim << "," << dimworld <<"> with name "
        << name_ << " created!" << std::endl;

  nameCounter++;
}

template < int dim, int dimworld >
inline Dune::UGGrid < dim, dimworld >::~UGGrid()
{
  if (extra_boundary_data_)
    free(extra_boundary_data_);

  if (multigrid_) {
    // Set UG's currBVP variable to the BVP corresponding to this
    // grid.  This is necessary if we have more than one UGGrid in use.
    // DisposeMultiGrid will crash if we don't do this
    std::string BVPName = name() + "_Problem";
    void* thisBVP = UG_NS<dim>::BVP_GetByName(BVPName.c_str());

    if (thisBVP == NULL)
      DUNE_THROW(GridError, "Couldn't find grid's own boundary value problem!");

    UG_NS<dim>::Set_Current_BVP((void**)thisBVP);
    UG_NS<dim>::DisposeMultiGrid(multigrid_);
  }

  numOfUGGrids--;

  // Shut down UG if this was the last existing UGGrid object
  if (numOfUGGrids == 0) {

    UG_NS<dim>::ExitUg();

    // remove defaults file, if we wrote one on startup
    if (!useExistingDefaultsFile)
      system("rm defaults");

  }

};

template < int dim, int dimworld >
inline int Dune::UGGrid < dim, dimworld >::maxlevel() const
{
  assert(multigrid_);
  return multigrid_->topLevel;
}



template<int dim, int dimworld>
template<int codim>
typename Dune::UGGrid<dim,dimworld>::Traits::template Codim<codim>::LevelIterator
Dune::UGGrid<dim, dimworld>::lbegin (int level) const
{
  assert(multigrid_);
  typename UGTypes<dim>::GridType* theGrid = multigrid_->grids[level];

  if (!theGrid)
    DUNE_THROW(GridError, "LevelIterator in nonexisting level " << level << " requested!");

  return UGGridLevelIteratorFactory<codim,All_Partition, const UGGrid<dim,dimworld> >::getIterator(theGrid, level);
}

template<int dim, int dimworld>
template<int codim, Dune::PartitionIteratorType PiType>
inline typename Dune::UGGrid<dim,dimworld>::Traits::template Codim<codim>::template Partition<PiType>::LevelIterator
Dune::UGGrid<dim, dimworld>::lbegin (int level) const
{
  assert(multigrid_);
  typename UGTypes<dim>::GridType* theGrid = multigrid_->grids[level];

  if (!theGrid)
    DUNE_THROW(GridError, "LevelIterator in nonexisting level " << level << " requested!");

  return UGGridLevelIteratorFactory<codim,PiType, const UGGrid<dim,dimworld> >::getIterator(theGrid, level);
}

template < int dim, int dimworld >
template<int codim>
typename Dune::UGGrid<dim,dimworld>::Traits::template Codim<codim>::LevelIterator
Dune::UGGrid < dim, dimworld >::lend (int level) const
{
  return UGGridLevelIterator<codim,All_Partition, const UGGrid<dim,dimworld> >(level);
}

template < int dim, int dimworld >
template<int codim, Dune::PartitionIteratorType PiType>
inline typename Dune::UGGrid<dim,dimworld>::Traits::template Codim<codim>::template Partition<PiType>::LevelIterator
Dune::UGGrid < dim, dimworld >::lend (int level) const
{
  return UGGridLevelIterator<codim,PiType, const UGGrid<dim,dimworld> >(level);
}

template < int dim, int dimworld >
inline int Dune::UGGrid < dim, dimworld >::size (int level, int codim) const
{
#ifndef ModelP
  switch (codim) {
  case 0 :
    return multigrid_->grids[level]->nElem[0];
  case dim :
    return multigrid_->grids[level]->nNode[0];
  default :
    DUNE_THROW(GridError, "UGGrid<" << dim << ", " << dimworld
                                    << ">::size(int level, int codim) is only implemented"
                                    << " for codim==0 and codim==dim!");
  }
#else

  int numberOfElements = 0;

  if(codim == 0)
  {
    typename Traits::template Codim<0>::LevelIterator it = lbegin<0>(level);
    typename Traits::template Codim<0>::LevelIterator endit = lend<0>(level);
    for (; it != endit; ++it)
      numberOfElements++;

  } else
  if(codim == dim)
  {
    typename Traits::template Codim<dim>::LevelIterator it    = lbegin<dim>(level);
    typename Traits::template Codim<dim>::LevelIterator endit = lend<dim>(level);
    for(; it != endit; ++it)
      numberOfElements++;
  }
  else
  {
    DUNE_THROW(GridError, "UGGrid<" << dim << ", " << dimworld
                                    << ">::size(int level, int codim) is only implemented"
                                    << " for codim==0 and codim==dim!");
  }

  return numberOfElements;
#endif
}


template < int dim, int dimworld >
void Dune::UGGrid < dim, dimworld >::makeNewUGMultigrid()
{
  //configure @PROBLEM $d @DOMAIN;
  std::string configureArgs[2] = {"configure " + name() + "_Problem", "d " + name() + "_Domain"};
  const char* configureArgs_c[2] = {configureArgs[0].c_str(), configureArgs[1].c_str()};

  if (UG_NS<dim>::ConfigureCommand(2, configureArgs_c))
    DUNE_THROW(GridError, "Calling UG" << dim << "d::ConfigureCommand failed!");

  //new @PROBLEM $b @PROBLEM $f @FORMAT $h @HEAP;
  char* newArgs[4];
  for (int i=0; i<4; i++)
    newArgs[i] = (char*)::malloc(50*sizeof(char));

  sprintf(newArgs[0], "new %s", name().c_str());

  sprintf(newArgs[1], "b %s_Problem", name().c_str());
  sprintf(newArgs[2], "f DuneFormat");
  sprintf(newArgs[3], "h %dM", heapsize);

  if (UG_NS<dim>::NewCommand(4, newArgs))
    DUNE_THROW(GridError, "UGGrid::makeNewMultigrid failed!");

  for (int i=0; i<4; i++)
    free(newArgs[i]);

  // Get a direct pointer to the newly created multigrid
  multigrid_ = UG_NS<dim>::GetMultigrid(name().c_str());
  if (!multigrid_)
    DUNE_THROW(GridError, "UGGrid::makeNewMultigrid failed!");
}

template < int dim, int dimworld >
bool Dune::UGGrid < dim, dimworld >::mark(int refCount,
                                          typename Traits::template Codim<0>::EntityPointer & e )
{
  // No refinement requested
  if (refCount==0)
    return false;

  typename TargetType<0,dim>::T* target = getRealEntity<0>(*e).target_;

  // Check whether element can be marked for refinement
  if (!EstimateHere(target))
    return false;

  if (refCount==1) {
    if (UG_NS<dim>::MarkForRefinement(target,
                                      UG_NS<dim>::RED,      // red refinement rule
                                      0)      // no user data
        ) DUNE_THROW(GridError, "UG" << dim << "d::MarkForRefinement returned error code!");

    return true;
  } else if (refCount==-1) {

    if (UG_NS<dim>::MarkForRefinement(target,
                                      UG_NS<dim>::COARSE,      // coarsen the element
                                      0)      // no user data
        ) DUNE_THROW(GridError, "UG" << dim << "d::MarkForRefinement returned error code!");

    return true;
  } else
    DUNE_THROW(GridError, "UGGrid only supports refCount values -1, 0, and for mark()!");

}

template < int dim, int dimworld >
bool Dune::UGGrid < dim, dimworld >::mark(typename Traits::template Codim<0>::EntityPointer & e,
                                          typename UG_NS<dim>::RefinementRule rule
                                          )
{
  typename TargetType<0,dim>::T* target = getRealEntity<0>(*e).target_;

  if (!UG_NS<dim>::isLeaf(target))
    return false;

  return UG_NS<dim>::MarkForRefinement(target,
                                       rule,
                                       0);   // no user data

}

template < int dim, int dimworld >
bool Dune::UGGrid < dim, dimworld >::adapt()
{

  int rv;
  int mode;

  assert(multigrid_);

  // Set UG's currBVP variable to the BVP corresponding to this
  // grid.  This is necessary if we have more than one UGGrid in use.
  std::string BVPName = name() + "_Problem";
  void* thisBVP = UG_NS<dim>::BVP_GetByName(BVPName.c_str());

  if (thisBVP == NULL)
    DUNE_THROW(GridError, "Couldn't find grid's own boundary value problem!");

  UG_NS<dim>::Set_Current_BVP((void**)thisBVP);

  mode = UG_NS<dim>::GM_REFINE_TRULY_LOCAL;

  if (refinementType_==COPY)
    mode = mode | UG_NS<dim>::GM_COPY_ALL;

  if (omitGreenClosure_)
    mode = mode | UG_NS<dim>::GM_REFINE_NOT_CLOSED;

  // I don't really know what this means
  int seq = UG_NS<dim>::GM_REFINE_PARALLEL;

  // I don't really know what this means either
  int mgtest = UG_NS<dim>::GM_REFINE_NOHEAPTEST;

  rv = AdaptMultiGrid(multigrid_,mode,seq,mgtest);

  if (rv!=0)
    DUNE_THROW(GridError, "UG::adapt() returned with error code " << rv);

  // Renumber everything
  setIndices();

  /** \bug Should return true only if at least one element has actually
      been refined */
  return true;
}

template <int dim, int dimworld>
void Dune::UGGrid <dim, dimworld>::postAdapt()
{
  for (int i=0; i<=maxlevel(); i++) {

    typename Traits::template Codim<0>::LevelIterator eIt    = lbegin<0>(i);
    typename Traits::template Codim<0>::LevelIterator eEndIt = lend<0>(i);

    for (; eIt!=eEndIt; ++eIt)
      UG_NS<dim>::WriteCW(getRealEntity<0>(*eIt).target_, UG_NS<dim>::NEWEL_CE, 0);

  }
}


template <int dim, int dimworld>
void Dune::UGGrid <dim, dimworld>::adaptWithoutClosure()
{
  bool omitClosureBackup = omitGreenClosure_;
  omitGreenClosure_ = true;
  adapt();
  omitGreenClosure_ = omitClosureBackup;
}

template < int dim, int dimworld >
void Dune::UGGrid < dim, dimworld >::globalRefine(int n)
{
  for (int i=0; i<n; i++) {

    // mark all entities for grid refinement
    typename Traits::template Codim<0>::LevelIterator iIt    = lbegin<0>(maxlevel());
    typename Traits::template Codim<0>::LevelIterator iEndIt = lend<0>(maxlevel());

    for (; iIt!=iEndIt; ++iIt)
      mark(1, iIt);

    this->preAdapt();
    adapt();
    this->postAdapt();

  }

}

template <int dim, int dimworld>
void Dune::UGGrid<dim,dimworld>::getChildrenOfSubface(typename Traits::template Codim<0>::EntityPointer & e,
                                                      int elementSide,
                                                      int maxl,
                                                      Array<typename Dune::UGGridEntityPointer<0, UGGrid> >& childElements,
                                                      Array<unsigned char>& childElementSides) const
{

  typedef typename TargetType<0,dim>::T ElementType;
  typedef Tuple<ElementType*,int, int> ListEntryType;

  SLList<ListEntryType> list;

  // The starting level
  int level = e->level();

  const int MAX_SONS = 30;    // copied from UG

  // //////////////////////////////////////////////////////////////////////
  //   Change the input face number from Dune numbering to UG numbering
  // //////////////////////////////////////////////////////////////////////

  switch (e->geometry().type()) {
  case cube :

    if (dim==3) {
      // Dune numbers the faces of a hexahedron differently than UG.
      // The following two lines do the transformation
      const int renumbering[6] = {4, 2, 1, 3, 0, 5};
      elementSide = renumbering[elementSide];
    } else {      // Quadrilateral
      const int renumbering[4] = {3, 1, 0, 2};
      elementSide = renumbering[elementSide];
    }
    break;

  case simplex :

    if (dim==3) {      // Tetrahedron

      // Dune numbers the faces of a tetrahedron differently than UG.
      // The following two lines do the transformation
      const int renumbering[4] = {1, 2, 3, 0};
      elementSide = renumbering[elementSide];

    } else {          // Triangle

      // Dune numbers the faces of a triangle differently from UG.
      // The following two lines do the transformation
      const int renumbering[3] = {1, 2, 0};
      elementSide = renumbering[elementSide];

    }

    break;

  case vertex :
  case prism :
  case pyramid :
    // Do nothing
    break;

  default :
    DUNE_THROW(NotImplemented, "Unknown element type '"
               << e->geometry().type() << "'found!");
  }

  // ///////////////
  //   init list
  // ///////////////
  if (level < maxl) {

    ElementType* theElement = getRealEntity<0>(*e).target_;

    int Sons_of_Side = 0;
    ElementType* SonList[MAX_SONS];
    int SonSides[MAX_SONS];

    int rv = Get_Sons_of_ElementSide(theElement,
                                     elementSide,      // needs to be renumbered!
                                     &Sons_of_Side,
                                     SonList,          // the output elements
                                     SonSides,         // Output element side numbers
                                     true,            // Element sons are not precomputed
                                     true);            // ioflag: I have no idea what this is supposed to do
    if (rv!=0)
      DUNE_THROW(GridError, "Get_Sons_of_ElementSide returned with error value " << rv);

    for (int i=0; i<Sons_of_Side; i++)
      list.push_back(ListEntryType(SonList[i],SonSides[i], level+1));

  }

  // //////////////////////////////////////////////////
  //   Traverse and collect all children of the side
  // //////////////////////////////////////////////////

  typename SLList<ListEntryType>::iterator f = list.begin();
  for (; f!=list.end(); ++f) {

    ElementType* theElement = Element<0>::get(*f);
    int side                 = Element<1>::get(*f);
    level                    = Element<2>::get(*f);

    int Sons_of_Side = 0;
    ElementType* SonList[MAX_SONS];
    int SonSides[MAX_SONS];

    if (level < maxl) {

      Get_Sons_of_ElementSide(theElement,
                              side,             // Input element side number
                              &Sons_of_Side,       // Number of topological sons of the element side
                              SonList,            // Output elements
                              SonSides,           // Output element side numbers
                              true,
                              true);

      for (int i=0; i<Sons_of_Side; i++)
        list.push_back(ListEntryType(SonList[i],SonSides[i], level+1));

    }

  }

  // //////////////////////////////
  //   Extract result from stack
  // //////////////////////////////
  childElements.resize(list.size());
  childElementSides.resize(list.size());

  int i=0;
  for (f = list.begin(); f!=list.end(); ++f, ++i) {

    int side = Element<1>::get(*f);

    // Dune numbers the faces of several elements differently than UG.
    // The following switch does the transformation
    switch (e->geometry().type()) {
    case cube :

      if (dim==3) {        // hexahedron
        const int renumbering[6] = {4, 2, 1, 3, 0, 5};
        side = renumbering[side];
      } else {        // Quadrilateral
        const int renumbering[4] = {2, 1, 3, 0};
        side = renumbering[side];
      }
      break;

    case simplex :

      if (dim==3) {
        const int renumbering[4] = {3, 0, 1, 2};
        side = renumbering[side];
      } else {
        const int renumbering[3] = {2, 0, 1};
        side = renumbering[side];
      }
      break;

    case vertex :
    case prism :
    case pyramid :
      // Do nothing
      break;

    default :
      DUNE_THROW(NotImplemented, "Unknown element type '"
                 << e->geometry().type() << "'found!");
    }

    childElements[i].setToTarget(Element<0>::get(*f), Element<2>::get(*f));
    childElementSides[i] = side;

  }

}

template < int dim, int dimworld >
void Dune::UGGrid < dim, dimworld >::loadBalance(int strategy, int minlevel, int depth, int maxlevel, int minelement)
{
  /** \todo Test for valid arguments */
  std::string argStrings[4];
  std::stringstream numberAsAscii[4];

  numberAsAscii[0] << strategy;
  argStrings[0] = "lb " + numberAsAscii[0].str();

  numberAsAscii[1] << minlevel;
  argStrings[1] = "c " + numberAsAscii[1].str();

  numberAsAscii[2] << depth;
  argStrings[2] = "d " + numberAsAscii[2].str();

  numberAsAscii[3] << minelement;
  argStrings[3] = "e " + numberAsAscii[3].str();

  const char* argv[4] = {argStrings[0].c_str(),
                         argStrings[1].c_str(),
                         argStrings[2].c_str(),
                         argStrings[3].c_str()};

  int errCode = UG_NS<dim>::LBCommand(4, argv);

  if (errCode)
    DUNE_THROW(GridError, "UG" << dim << "d::LBCommand returned error code " << errCode);

  // Renumber everything
  setIndices();

}

#ifdef ModelP
namespace Dune {

  template <class T, template <class> class P, int GridDim>
  class UGDataCollector {
  public:

    static int gather(DDD_OBJ obj, void* data)
    {
      int codim=0;

      P<T>* p = (P<T>*)data;

      int index = 0;
      switch (codim) {
      case 0 :
        index = UG_NS<GridDim>::index((typename TargetType<0,GridDim>::T*)obj);
        break;
      case GridDim :
        index = UG_NS<GridDim>::index((typename TargetType<GridDim,GridDim>::T*)obj);
        break;
      default :
        DUNE_THROW(GridError, "UGGrid::communicate only implemented for this codim");
      }

      p->gather(*dataArray, index);

      return 0;
    }

    static int scatter(DDD_OBJ obj, void* data)
    {
      int codim=0;

      P<T>* p = (P<T>*)data;

      int index = 0;
      switch (codim) {
      case 0 :
        index = UG_NS<GridDim>::index((typename TargetType<0,GridDim>::T*)obj);
        break;
      case GridDim :
        index = UG_NS<GridDim>::index((typename TargetType<GridDim,GridDim>::T*)obj);
        break;
      default :
        DUNE_THROW(GridError, "UGGrid::communicate only implemented for codim 0 and dim");
      }

      p->scatter(*dataArray, index);

      return 0;
    }

    static T* dataArray;

  };

}   // end namespace Dune

template <class T, template<class> class P, int GridDim>
T* Dune::UGDataCollector<T,P,GridDim>::dataArray = 0;
#endif

/** \todo How do I incorporate the level argument? */
template < int dim, int dimworld >
template<class T, template<class> class P, int codim>
void Dune::UGGrid < dim, dimworld >::communicate (T& t, InterfaceType iftype, CommunicationDirection dir, int level)
{
#ifdef ModelP

  // Currently only elementwise communication is supported
  if (codim != 0)
    DUNE_THROW(GridError, "Currently UG supports only element-wise communication!");

  UGDataCollector<T,P,dim>::dataArray = &t;

  // Translate the communication direction from Dune-Speak to UG-Speak
  DDD_IF_DIR UGIfDir = (dir==ForwardCommunication) ? IF_FORWARD : IF_BACKWARD;

  // Trigger communication
  DDD_IFOneway(UG::ElementVHIF,
               UGIfDir,
               sizeof(P<T>),
               &UGDataCollector<T,P,dim>::gather,
               &UGDataCollector<T,P,dim>::scatter);

#endif
}


template < int dim, int dimworld >
void Dune::UGGrid < dim, dimworld >::createbegin()
{}


template < int dim, int dimworld >
void Dune::UGGrid < dim, dimworld >::createend()
{
  // set the subdomainIDs
  typename TargetType<0,dim>::T* theElement;
  for (theElement=multigrid_->grids[0]->elements[0]; theElement!=NULL; theElement=theElement->ge.succ)
    UG_NS<dim>::SetSubdomain(theElement, 1);

  // Complete the subdomain structure
  // From namespace UG?d
  SetEdgeAndNodeSubdomainFromElements(multigrid_->grids[0]);

  // Complete the UG-internal grid data structure
  if (CreateAlgebra(multigrid_) != UG_NS<dim>::GM_OK)
    DUNE_THROW(IOError, "Call of 'UG::CreateAlgebra' failed!");

  /* here all temp memory since CreateMultiGrid is released */
  Release(multigrid_->theHeap, UG::FROM_TOP, multigrid_->MarkKey);
  multigrid_->MarkKey = 0;

  // Set the local indices
  setIndices();

  // Clear refinement flags
  typename Traits::template Codim<0>::LevelIterator eIt    = lbegin<0>(0);
  typename Traits::template Codim<0>::LevelIterator eEndIt = lend<0>(0);

  for (; eIt!=eEndIt; ++eIt)
    UG_NS<dim>::WriteCW(getRealEntity<0>(*eIt).target_, UG_NS<dim>::NEWEL_CE, 0);

}

template <int dim, int dimworld>
void Dune::UGGrid<dim, dimworld>::insertElement(GeometryType type,
                                                const std::vector<unsigned int>& vertices)
{
  int vertices_C_style[vertices.size()];
  for (size_t i=0; i<vertices.size(); i++)
    vertices_C_style[i] = vertices[i];

  if (dim==2) {
    switch (type) {
    case simplex :
      // Everything alright
      if (vertices.size() != 3)
        DUNE_THROW(GridError, "You have requested to enter a triangle, but you"
                   << " have provided " << vertices.size() << " vertices!");
      break;

    case cube :
      if (vertices.size() != 4)
        DUNE_THROW(GridError, "You have requested to enter a quadrilateral, but you"
                   << " have provided " << vertices.size() << " vertices!");

      // DUNE and UG numberings differ --> reorder the vertices
      vertices_C_style[2] = vertices[3];
      vertices_C_style[3] = vertices[2];
      break;
    default :
      DUNE_THROW(GridError, "You cannot insert a " << type << " into a UGGrid<2,2>!");
    }
  } else {
    switch (type) {
    case simplex :
      if (vertices.size() != 4)
        DUNE_THROW(GridError, "You have requested to enter a tetrahedron, but you"
                   << " have provided " << vertices.size() << " vertices!");
      break;
    case pyramid :
      if (vertices.size() != 5)
        DUNE_THROW(GridError, "You have requested to enter a pyramid, but you"
                   << " have provided " << vertices.size() << " vertices!");
      break;
    case prism :
      if (vertices.size() != 6)
        DUNE_THROW(GridError, "You have requested to enter a prism, but you"
                   << " have provided " << vertices.size() << " vertices!");
      break;

    case cube :
      if (vertices.size() != 8)
        DUNE_THROW(GridError, "You have requested to enter a hexahedron, but you"
                   << " have provided " << vertices.size() << " vertices!");

      // DUNE and UG numberings differ --> reorder the vertices
      vertices_C_style[2] = vertices[3];
      vertices_C_style[3] = vertices[2];
      vertices_C_style[6] = vertices[7];
      vertices_C_style[7] = vertices[6];
      break;
    default :
      DUNE_THROW(GridError, "You cannot insert a " << type << " into a UGGrid<3,3>!");

    }
  }

  if (InsertElementFromIDs(multigrid_->grids[0], vertices.size(), vertices_C_style, NULL)==NULL)
    DUNE_THROW(GridError, "Inserting " << type << " into UGGrid failed!");

}

template < int dim, int dimworld >
void Dune::UGGrid < dim, dimworld >::setIndices()
{
  levelIndexSets_.resize(maxlevel()+1);

  for (int i=0; i<=maxlevel(); i++)
    levelIndexSets_[i].update(*this, i);

  leafIndexSet_.update();

  localIdSet_.update();

  globalIdSet_.update();

}

// /////////////////////////////////////////////////////////////////////////////////
//   Explicit instantiation of the dimensions that are actually supported by UG.
//   g++-4.0 wants them to be _after_ the method implementations.
// /////////////////////////////////////////////////////////////////////////////////

template class Dune::UGGrid<2,2>;
template class Dune::UGGrid<3,3>;
