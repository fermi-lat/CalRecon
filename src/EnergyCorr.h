
#ifndef __EnergyCorr_H
#define __EnergyCorr_H 1

#include "IEnergyCorr.h"
#include "CalReconActor.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/ISvcLocator.h"

/**   
* @class EnergyCorr
*
* Base class for energy corrections
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/EnergyCorr.h,v 1.6 2005/03/21 10:09:19 chamont Exp $
*/

class EnergyCorr :  public IEnergyCorr, public AlgTool, protected CalReconActor {
	
public:
    
    //! constructor
    EnergyCorr( const std::string & type, 
                const std::string & name, 
                const IInterface * parent)
     : AlgTool(type,name,parent)
     {}
     
    virtual StatusCode initialize() ;

    //! destructor
    virtual ~EnergyCorr()
     {} 
        
} ;

#endif
	
	
	
