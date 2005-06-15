#ifndef ICalClusteringTool_h
#define ICalClusteringTool_h 

#include "GaudiKernel/IAlgTool.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include <vector>

/**   
* @class ICalClusteringTool
*
* Base class for clustering tools
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Clustering/ICalClusteringTool.h,v 1.1 2005/05/26 21:23:23 usher Exp $
*/

static const InterfaceID IID_ICalClusteringTool("ICalClusteringTool",1,0) ;
    
//! Typedefs to define associated Xtal's in cluster analysis (not the best place for this?)
typedef  std::vector<Event::CalXtalRecData *> XtalDataVec ;
typedef  std::vector<XtalDataVec *>           XtalDataVecVec ;

class ICalClusteringTool : virtual public IAlgTool {

  public:

    // retrieve Gaudi interface ID
    static const InterfaceID& interfaceID() {return IID_ICalClusteringTool;}

    //! main method
    virtual StatusCode findClusters(Event::CalClusterCol* calClusterCol) = 0;
 } ;

#endif


