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
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/CalRecon/ICalClusteringTool.h,v 1.1 2005/06/02 12:02:54 chamont Exp $
*/

static const InterfaceID IID_ICalClusteringTool("ICalClusteringTool",1,0) ;
    
//! Typedefs to define associated Xtal's in cluster analysis
typedef  std::list<Event::CalXtalRecData *> XtalDataList ;
typedef  std::vector<XtalDataList *>        XtalDataListVec ;

class ICalClusteringTool : virtual public IAlgTool {

  public:

    // retrieve Gaudi interface ID
    static const InterfaceID& interfaceID() {return IID_ICalClusteringTool;}

    //! main method
    virtual StatusCode findClusters(Event::CalClusterCol* calClusterCol) = 0;
 } ;

#endif



