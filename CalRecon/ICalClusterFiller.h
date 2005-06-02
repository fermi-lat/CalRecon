
#ifndef ICalClusterFiller_h
#define ICalClusterFiller_h

#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include <CalRecon/ICalClusteringTool.h>

/**   
* @class ICalClusterFiller
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Clustering/ICalClusterFiller.h,v 1.1 2005/05/26 21:23:23 usher Exp $
*/


class ICalClusterFiller 
{
public:
    //* Defines the method for filling cluster info into CalCluster TDS objects
    virtual Event::CalCluster* fillClusterInfo(const XtalDataVec* xtalVec) = 0;
};

#endif
	
	
	
