
#ifndef IClusterFiller_h
#define IClusterFiller_h

#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "ICalClusteringTool.h"

/**   
* @class IClusterFiller
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/IClusterFiller.h,v 0.0 2005/05/23 17:55:41 chamont Exp $
*/


class IClusterFiller 
{
public:
    //* Defines the method for filling cluster info into CalCluster TDS objects
    virtual Event::CalCluster* fillClusterInfo(const XtalDataVec* xtalVec) = 0;
};

#endif
	
	
	
