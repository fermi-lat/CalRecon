
#ifndef __StdClusterInfo_H
#define __StdClusterInfo_H 1

#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include <CalRecon/ICalClusterFiller.h>
#include <CalRecon/ICalReconSvc.h>

/**   
* @class StdClusterInfo
*
* Base class for clustering tools, containing member data and
* default code for the global algorithm, the preparation of
* a cluster from a set of crystals, and the computing of its
* direction.
* The only pure virtual method which needs to be implemented
* in a derived class is nextXtalsSet(), which is selecting the
* crystals to be grouped together.
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Clustering/StdClusterInfo.h,v 1.5 2010/01/25 17:57:39 usher Exp $
*/


class StdClusterInfo : virtual public ICalClusterFiller
{
public:
    StdClusterInfo(const ICalReconSvc* calReconSvc) : m_calReconSvc(calReconSvc) {};
   virtual ~StdClusterInfo() {};
    
   Event::CalCluster* fillClusterInfo(const XtalDataList* xtalVec);
private:

    // calculate the direction of the shower from the hits
    Vector fitDirection( std::vector<Vector> pos, std::vector<Vector> sigma2) ;

    //! package service
    const ICalReconSvc* m_calReconSvc;
};

#endif
        
        
        
