
#ifndef ICalClusterFiller_h
#define ICalClusterFiller_h

#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include <CalRecon/ICalClusteringTool.h>

/**   
* @class ICalClusterFiller
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/CalRecon/ICalClusterFiller.h,v 1.1 2005/06/02 12:02:54 chamont Exp $
*/


class ICalClusterFiller 
{
public:
    //* Defines the method for filling cluster info into CalCluster TDS objects
    virtual Event::CalCluster* fillClusterInfo(const XtalDataList* xtalVec) = 0;
};

#endif
	
	
	
