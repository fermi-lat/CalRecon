
#ifndef __IEnergyCorr_H
#define __IEnergyCorr_H 1

#include "GaudiKernel/IAlgTool.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "geometry/Vector.h"

/**   
* @class IEnergyCorr
*
* base class for energy leakage corrections 
*
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/IEnergyCorr.h,v 1.1 2003/02/13 23:43:42 richard Exp $
*/

static const InterfaceID IID_IEnergyCorr("IEnergyCorr", 1 , 0);

class IEnergyCorr : virtual public IAlgTool {

public:
	IEnergyCorr() {;};
    // retrieve interface ID
    static const InterfaceID& interfaceID() { return IID_IEnergyCorr; }
    //! constructor
    //! destructor
    virtual ~IEnergyCorr() {}; 
    
    virtual StatusCode initialize()=0;

        
    // worker function for calculating corrections
    virtual StatusCode doEnergyCorr(double eTotal, Event::CalCluster* cluster)=0;
    virtual StatusCode execute()=0;

    virtual StatusCode finalize()=0; 

};

#endif



