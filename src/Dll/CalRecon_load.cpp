/** 
* @file CalRecon_load.cpp
* @brief This is needed for forcing the linker to load all components
* of the library.
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Dll/CalRecon_load.cpp,v 1.15.4.1 2004/12/09 23:00:01 chamont Exp $
*/

#include "GaudiKernel/DeclareFactoryEntries.h"
#include "GaudiKernel/IToolFactory.h"

DECLARE_FACTORY_ENTRIES(CalRecon) {
    DECLARE_ALGORITHM( CalXtalRecAlg);
    DECLARE_ALGORITHM( CalClustersAlg);
    DECLARE_ALGORITHM( CalDisplay);
    DECLARE_TOOL( SingleClusterTool );
    DECLARE_TOOL( FuzzyClusterTool );
    DECLARE_TOOL( SimpleClusterTool );
    DECLARE_TOOL( LastLayerCorrTool );
    DECLARE_TOOL( ProfileTool );
    DECLARE_TOOL( CalValsCorrTool );
} 




