
#ifndef __IClusteringTool_H
#define __IClusteringTool_H 1

#include "GaudiKernel/IAlgTool.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "geometry/Vector.h"

/**   
* @class IClusteringTool
*
* Base class for clustering tools
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/IClusteringTool.h,v 1.4 2003/05/11 19:59:12 burnett Exp $
*/

static const InterfaceID IID_IClusteringTool("IClusteringTool",1,0) ;

class IClusteringTool : virtual public IAlgTool {

  public:

    // retrieve Gaudi interface ID
    static const InterfaceID& interfaceID()
     { return IID_IClusteringTool; }

    IClusteringTool() {}
    virtual ~IClusteringTool() {}

    //! main method
    virtual StatusCode findClusters(
        Event::CalXtalRecCol *,
        Event::CalClusterCol *
      ) =0 ;

 } ;

#endif



