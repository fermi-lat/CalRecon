/** 
* @file CalRecon_load.cpp
* @brief This is needed for forcing the linker to load all components
* of the library.
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Dll/CalRecon_load.cpp,v 1.8 2002/05/20 19:26:59 chehtman Exp $
*/

#include "GaudiKernel/DeclareFactoryEntries.h"

DECLARE_FACTORY_ENTRIES(CalRecon) {
    DECLARE_ALGORITHM( CalXtalRecAlg);
    DECLARE_ALGORITHM( CalClustersAlg);
    DECLARE_ALGORITHM( CalDisplay);
} 




