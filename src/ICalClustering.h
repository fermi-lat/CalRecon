
#ifndef __ICalClustering_H
#define __ICalClustering_H 1

#include "ICalReconSvc.h"
#include "geometry/Vector.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "GaudiKernel/IAlgTool.h"

/**   
* @class ICalClustering
*
* Base class for clustering tools
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/ICalClustering.h,v 1.1 2005/04/20 16:41:20 chamont Exp $
*/

static const InterfaceID IID_ICalClustering("ICalClustering",1,0) ;

class ICalClustering : virtual public IAlgTool {

  public:

    // retrieve Gaudi interface ID
    static const InterfaceID& interfaceID()
     { return IID_ICalClustering; }

    ICalClustering() {}
    virtual ~ICalClustering() {}

    //! main method
    virtual StatusCode findClusters() =0 ;

 } ;

#endif



