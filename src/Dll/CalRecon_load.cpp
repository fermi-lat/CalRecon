/** 
* @file CalRecon_load.cpp
* @brief This is needed for forcing the linker to load all components
* of the library.
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Dll/CalRecon_load.cpp,v 1.21 2005/04/11 13:28:51 chamont Exp $
*/

#include "GaudiKernel/DeclareFactoryEntries.h"
#include "GaudiKernel/IToolFactory.h"

DECLARE_FACTORY_ENTRIES( CalRecon ) {
    DECLARE_ALGORITHM( CalXtalRecAlg ) ;
    DECLARE_ALGORITHM( CalClustersAlg ) ;
    DECLARE_ALGORITHM( CalDisplay ) ;
    DECLARE_ALGORITHM( PropertiesCheckAlg ) ;
    DECLARE_TOOL( CalReconKernel ) ;
    DECLARE_TOOL( CalSingleClustering ) ;
    DECLARE_TOOL( CalSimpleClustering ) ;
    DECLARE_TOOL( CalLastLayerCorr );
    DECLARE_TOOL( CalTkrLikelihoodCorr );
    DECLARE_TOOL( CalProfileCorr );
    DECLARE_TOOL( CalValsCorr );
    DECLARE_TOOL( CalTransvOffsetCorr );
} 




