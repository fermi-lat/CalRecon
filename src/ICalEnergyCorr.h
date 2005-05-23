
#ifndef __ICalEnergyCorr_H
#define __ICalEnergyCorr_H 1

#include "GaudiKernel/IAlgTool.h"

/**   
* @class ICalEnergyCorr
*
* base class for energy leakage corrections 
*
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/ICalEnergyCorr.h,v 1.1 2005/04/20 16:41:20 chamont Exp $
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
    
    //! Calculate corrections on all cal clusters
    virtual StatusCode doEnergyCorr() =0 ;
    
} ;

#endif



