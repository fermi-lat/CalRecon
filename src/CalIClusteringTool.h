
#ifndef __CalIClusteringTool_H
#define __CalIClusteringTool_H 1

#include "GaudiKernel/IAlgTool.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "geometry/Vector.h"

/**   
* @class CalIClusteringTool
*
* Base class for clustering tools
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalIClusteringTool.h,v 1.1 2005/01/25 07:51:08 chamont Exp $
*/

static const InterfaceID IID_CalIClusteringTool("CalIClusteringTool",1,0) ;

class CalIClusteringTool : virtual public IAlgTool {

  public:

    // retrieve Gaudi interface ID
    static const InterfaceID& interfaceID()
     { return IID_CalIClusteringTool; }

    CalIClusteringTool() {}
    virtual ~CalIClusteringTool() {}

    //! main method
    virtual StatusCode findClusters(
        const Event::CalXtalRecCol *,
        Event::CalClusterCol *
      ) =0 ;

 } ;

#endif



