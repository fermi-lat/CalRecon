/**
 * @class SimpleClusteringTool
 *
 * @brief Implements a Gaudi Tool for performing very simple clustering in the Cal 
 *
 * @author Tracy Usher
 *
 * File and Version Information:
 *      $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/SimpleClusteringTool.cxx,v 1.2 2005/01/20 15:52:20 chamont Exp $
 */

#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "ClusteringTool.h"


class SimpleClusteringTool : public ClusteringTool
 {
  public :
  
    /// Standard Gaudi Tool interface constructor
    SimpleClusteringTool(
      const std::string& type,
      const std::string& name,
      const IInterface* parent ) ;
    virtual ~SimpleClusteringTool() ;
    
  protected:

    /// This finds the next highest energy cluster in a vector of CalXtalRecData pointers
    xTalDataVec            nextXtalsSet(xTalDataVec& xTalVec);

  private:
  
    xTalDataVec            getXtalsInLayer(xTalDataVec& xTalVec, Event::CalXtalRecData* xTal);
    Event::CalXtalRecData* getNearestXtalInSameLayer(xTalDataVec& xTalVec, xTalDataVec& NNvec, Event::CalXtalRecData* xTal);
    Event::CalXtalRecData* getNearestXtalInDiffLayer(xTalDataVec& xTalVec, Event::CalXtalRecData* xTal, int layer);

 } ;

