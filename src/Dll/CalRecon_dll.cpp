//====================================================================
//  GlastSvc_dll.cpp
//--------------------------------------------------------------------
//
//  Package    : GlastSvc
//
//  Description: Implementation of DllMain routine.
//               The DLL initialisation must be done seperately for 
//               each DLL. 
//
//  Author     : H. Gillespie
//  Created    : 1 Aug 00
//  Changes    : 
//               Version copied from Glast, and addopted for GlastSvc
//
//====================================================================

// DllMain entry point
#include "GaudiKernel/DllMain.icpp"
#include <iostream>
void GaudiDll::initialize(void*) 
{
}

void GaudiDll::finalize(void*) 
{
}
extern void CalRecon_load();
#include "GaudiKernel/FactoryTable.h"

extern "C" FactoryTable::EntryList* getFactoryEntries() {
  static bool first = true;
  if ( first ) {  // Don't call for UNIX
    CalRecon_load();
    first = false;
  }
  return FactoryTable::instance()->getEntries();
} 

void FATAL(const char * msg) {
    std::cerr << "Stupid error from Calrecon DLL: " << msg << std::endl;
}

