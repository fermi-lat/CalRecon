
#ifndef ICalClusterFiller_h
#define ICalClusterFiller_h

#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include <CalRecon/ICalClusteringTool.h>

/**   
* @class ICalClusterFiller
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/CalRecon/ICalClusterFiller.h,v 1.3 2010/01/25 17:57:38 usher Exp $
*/


class ICalClusterFiller 
{
public:
    //* Defines the method for filling cluster info into CalCluster TDS objects
    virtual Event::CalCluster* fillClusterInfo(const XtalDataList* xtalVec) = 0;
};

#endif
        
        
        
