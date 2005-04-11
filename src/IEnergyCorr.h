
#ifndef __IEnergyCorr_H
#define __IEnergyCorr_H 1

#include "GaudiKernel/IAlgTool.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "geometry/Vector.h"
#include "CalReconKernel.h"

/**   
* @class IEnergyCorr
*
* base class for energy leakage corrections 
*
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/IEnergyCorr.h,v 1.4 2005/03/21 10:09:19 chamont Exp $
*/

static const InterfaceID IID_IEnergyCorr("IEnergyCorr", 1 , 0) ;

class IEnergyCorr : virtual public IAlgTool {

public:

    // retrieve interface ID
    static const InterfaceID & interfaceID() { return IID_IEnergyCorr ; }
    
    //! constructor
	IEnergyCorr() {}
    //! destructor
    virtual ~IEnergyCorr() {}
    
    //! Worker function for calculating corrections
	/*! Performs the reconstruction, creates one CalEnergyCorr object and stores
     *  there the following results: 
     *  - Energy per layer is computed and stored in CalEnergyCorr in MeV
     *  - Barycenter per layer is also computed and stored in CalEnergyCorr
     */        
    virtual StatusCode doEnergyCorr( Event::CalCluster * ) =0 ;
    
    // unused ! virtual double getEnergyCorr() =0 ;

} ;

#endif



