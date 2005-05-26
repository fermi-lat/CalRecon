#ifndef __ICalEnergyCorr_H
#define __ICalEnergyCorr_H 1

#include "GaudiKernel/IAlgTool.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "Event/Recon/CalRecon/CalCorToolResult.h"
#include "Event/Recon/TkrRecon/TkrVertex.h"

/**   
* @class ICalEnergyCorr
*
* base class for energy leakage corrections 
*
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/ICalEnergyCorr.h,v 0.1 2005/04/11 13:28:50 chamont Exp $
*/

static const InterfaceID IID_ICalEnergyCorr("ICalEnergyCorr", 1 , 0) ;

class ICalEnergyCorr : virtual public IAlgTool {

public:

    // retrieve interface ID
    static const InterfaceID & interfaceID() { return IID_ICalEnergyCorr ; }
    
    //! constructor
    ICalEnergyCorr() {}
    //! destructor
    virtual ~ICalEnergyCorr() {}
    
    //! Worker function for calculating corrections
    /*! Performs the reconstruction, creates one CalEnergyCorr object and stores
     *  there the following results: 
     *  - Energy per layer is computed and stored in CalEnergyCorr in MeV
     *  - Barycenter per layer is also computed and stored in CalEnergyCorr
     */        
    //virtual StatusCode doEnergyCorr(Event::CalCluster*, Event::TkrVertex* ) =0 ;
    virtual Event::CalCorToolResult* doEnergyCorr(Event::CalCluster*, Event::TkrVertex* ) = 0;

} ;

#endif



