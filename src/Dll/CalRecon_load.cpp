/** 
* @file CalRecon_load.cpp
* @brief This is needed for forcing the linker to load all components
* of the library.
*
*/

#include "GaudiKernel/DeclareFactoryEntries.h"

DECLARE_FACTORY_ENTRIES( CalRecon ) 
{
    DECLARE_SERVICE(   CalReconSvc                );
    DECLARE_AUDITOR(   CalReconAuditor            );
    DECLARE_ALGORITHM( CalClustersAlg             );
    DECLARE_ALGORITHM( CalClassifyAlg             );
    DECLARE_ALGORITHM( CalMipFinderAlg            );
    DECLARE_ALGORITHM( CalEventEnergyAlg          );
    DECLARE_ALGORITHM( CalDisplay                 );
    DECLARE_ALGORITHM( PropertiesCheckAlg         );
    DECLARE_TOOL(      CalMSTClusteringTool       );
    DECLARE_TOOL(      CalSingleClusteringTool    );
    DECLARE_TOOL(      CalSimpleClusteringTool    );
    DECLARE_TOOL(      StdMipFindingTool          );
    DECLARE_TOOL(      CalRawEnergyTool           );
    DECLARE_TOOL(      CalTkrLikelihoodTool       );
    DECLARE_TOOL(      CalLastLayerLikelihoodTool );
    DECLARE_TOOL(      CalLikelihoodManagerTool   );
    DECLARE_TOOL(      NewCalFullProfileTool      );
    DECLARE_TOOL(      CalFullProfileTool         );
    DECLARE_TOOL(      CalProfileTool             );
    DECLARE_TOOL(      CalValsCorrTool            );
    DECLARE_TOOL(      CalTransvOffsetTool        );
    DECLARE_TOOL(      CalClusterNBClassifyTool   );
    DECLARE_TOOL(      CalEnergyClassificationTool);
    DECLARE_TOOL(      CalRawEnergySelectionTool  );
} 




