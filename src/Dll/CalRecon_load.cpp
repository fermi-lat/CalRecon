// $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Dll/CalRecon_load.cpp,v 1.2 2001/02/28 22:26:37 burnett Exp $
//====================================================================
//  GlastSvc_load.cpp
//--------------------------------------------------------------------
//
//  Package    : Gaudi/System
//
//  Description: Implementation of <Package>_load routine.
//               This routine is needed for forcing the linker
//               to load all the components of the library. 
//
//====================================================================

#include "GaudiKernel/ICnvFactory.h"
#include "GaudiKernel/ISvcFactory.h"
#include "GaudiKernel/IAlgFactory.h"


#define DLL_DECL_SERVICE(x)    extern const ISvcFactory& x##Factory; x##Factory.addRef();
#define DLL_DECL_CONVERTER(x)  extern const ICnvFactory& x##Factory; x##Factory.addRef();
#define DLL_DECL_ALGORITHM(x)  extern const IAlgFactory& x##Factory; x##Factory.addRef();
#define DLL_DECL_OBJECT(x)     extern const IFactory& x##Factory; x##Factory.addRef();

//! Load all  services: 
void CalRecon_load() {

	DLL_DECL_SERVICE( CalGeometrySvc );

    DLL_DECL_ALGORITHM( CalRecLogsAlg );
	DLL_DECL_ALGORITHM( CalIRFAlg );
	DLL_DECL_ALGORITHM( CalClustersAlg );
} 

extern "C" void CalRecon_loadRef()    {
  CalRecon_load();
}

