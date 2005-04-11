
#ifndef __CalClusteringTool_H
#define __CalClusteringTool_H 1

#include "CalIClusteringTool.h"
#include "CalReconActor.h"
#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/ISvcLocator.h"

/**   
* @class CalClusteringTool
*
* Base class for clustering tools, containing member data and
* default code for the global algorithm, the preparation of
* a cluster from a set of crystals, and the computing of its
* direction.
* The only pure virtual method which needs to be implemented
* in a derived class is nextXtalsSet(), which is selecting the
* crystals to be grouped together.
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalClusteringTool.h,v 1.2 2005/01/25 08:53:26 chamont Exp $
*/


class CalClusteringTool : public CalIClusteringTool,  public AlgTool, protected CalReconActor {
	
  public:
    
    CalClusteringTool
     ( const std::string & type, 
       const std::string & name,
       const IInterface * parent ) ;
    virtual ~CalClusteringTool() ;
    
	/// @brief Intialization of the tool
    virtual StatusCode initialize() ;

    /// @brief Default cluster finding framework
    virtual StatusCode findClusters() ;

  protected:
    
    //! useful types
    typedef  std::vector<Event::CalXtalRecData *> XtalDataVec ;
    typedef  std::vector<XtalDataVec *> XtalDataVecVec ;

    //! Collect CalXtalRecData pointers
    virtual void getXtals( XtalDataVec & xtals ) ;

    //! THE METHOD TO IMPLEMENT: aggregate the sets of related xtals
    virtual void makeSets( const XtalDataVec & xtals, XtalDataVecVec & clusters ) =0 ;

    //! Construct CalClusters from sets of related xtals
    virtual void setClusters( const XtalDataVecVec & ) ;

    // calculate the direction of the shower from the hits
    virtual Vector fitDirection
     ( std::vector<Vector> pos,
       std::vector<Vector> sigma2 ) ;
	
 } ;

#endif
	
	
	
