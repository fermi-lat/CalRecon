/** 
* @file CalRecon_load.cpp
* @brief This is needed for forcing the linker to load all components
* of the library.
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Dll/CalRecon_load.cpp,v 1.20 2005/03/25 09:14:57 chamont Exp $
*/

#include "GaudiKernel/DeclareFactoryEntries.h"
#include "GaudiKernel/IToolFactory.h"

DECLARE_FACTORY_ENTRIES( CalRecon ) {
    DECLARE_ALGORITHM( CalXtalRecAlg ) ;
    DECLARE_ALGORITHM( CalClustersAlg ) ;
    DECLARE_ALGORITHM( CalDisplay ) ;
    DECLARE_ALGORITHM( PropertiesCheckAlg ) ;
    DECLARE_TOOL( CalReconKernel ) ;
    DECLARE_TOOL( CalSingleClusteringTool ) ;
    DECLARE_TOOL( CalSimpleClusteringTool ) ;
    DECLARE_TOOL( LastLayerCorrTool );
    DECLARE_TOOL( CalTkrLikelihoodTool );
    DECLARE_TOOL( ProfileTool );
    DECLARE_TOOL( CalValsCorrTool );
    DECLARE_TOOL( CalTransvOffsetTool );
} 




