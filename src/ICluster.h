
#ifndef __ICluster_H
#define __ICluster_H 1

#include "GaudiKernel/IAlgTool.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "geometry/Vector.h"

/**   
* @class ICluster
*
* Base class for clustering tools
*
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/ICluster.h,v 1.2 2003/02/16 19:08:16 richard Exp $
*/

static const InterfaceID IID_ICluster("ICluster", 1 , 0);

class ICluster : virtual public IAlgTool {

public:
	ICluster() {;};
    // retrieve interface ID
    static const InterfaceID& interfaceID() { return IID_ICluster; }
    //! constructor
    //! destructor
    virtual ~ICluster() {}; 
    
    virtual StatusCode initialize()=0;

        
    // worker function for finding clusters
    virtual StatusCode findClusters(Event::CalXtalRecCol* calXtalRecCol)=0;
    
	virtual StatusCode execute()=0;

    virtual StatusCode finalize()=0;
    
protected:
    virtual Vector Fit_Direction(std::vector<Vector> pos,
                                     std::vector<Vector> sigma2,
                                     int nlayers)=0;
    
    
    
};

#endif



