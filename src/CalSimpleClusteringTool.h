/**
 * @class CalSimpleClusteringTool
 *
 * @brief Implements a Gaudi Tool for performing very simple clustering in the Cal 
 *
 * @author Tracy Usher
 *
 * File and Version Information:
 *      $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalSimpleClusteringTool.h,v 1.1 2005/01/25 07:51:08 chamont Exp $
 */

#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "CalClusteringTool.h"


class CalSimpleClusteringTool : public CalClusteringTool
 {
  public :
  
    /// Standard Gaudi Tool interface constructor
    CalSimpleClusteringTool(
      const std::string& type,
      const std::string& name,
      const IInterface* parent ) ;
    virtual ~CalSimpleClusteringTool() ;
    
  protected:

    //! Distinguish sets of related xtals
    virtual void makeSets( const XtalDataVec & xtals, XtalDataVecVec & clusters ) ;

  private:
  
    XtalDataVec            getXtalsInLayer(XtalDataVec& xTalVec, Event::CalXtalRecData* xTal);
    Event::CalXtalRecData* getNearestXtalInSameLayer(XtalDataVec& xTalVec, XtalDataVec& NNvec, Event::CalXtalRecData* xTal);
    Event::CalXtalRecData* getNearestXtalInDiffLayer(XtalDataVec& xTalVec, Event::CalXtalRecData* xTal, int layer);

 } ;

