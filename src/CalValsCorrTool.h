
#ifndef __CalValsCorrTool_H
#define __CalValsCorrTool_H 1

#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"
#include "EnergyCorr.h"

class IValsTool;

/**   
* @class CalValsCorrTool
*
* Tool to get corrected energy from CalValsTool, which corrects for cracks
* and leakage.
*
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalValsCorrTool.h,v 1.2 2003/02/24 20:08:16 burnett Exp $
*/


class CalValsCorrTool : public AlgTool, public EnergyCorr {

public:
    
    //! destructor
    CalValsCorrTool( const std::string& type, const std::string& name, const IInterface* parent);
     ~CalValsCorrTool() {}; 
    
     StatusCode initialize();

        
     // worker function to get the corrected energy      
     StatusCode doEnergyCorr(double eTotal, Event::CalCluster* cluster);

     StatusCode finalize();
    
     StatusCode execute();

    
private:

    /// name of Tool for finding last layer energy leakage
    std::string m_calValsToolName;

    /// pointer to actual tool for last layer energy correlation
    IValsTool* m_calValsTool;



};

#endif



