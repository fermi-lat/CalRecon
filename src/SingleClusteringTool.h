
#ifndef __SingleClusteringTool_H
#define __SingleClusteringTool_H 1

#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"
#include "ClusteringTool.h"

/**   
* @class SingleClusteringTool
*
* Find single cluster from all CAL hits
*
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/SingleClusteringTool.h,v 1.4 2003/05/11 19:59:12 burnett Exp $
*/


class SingleClusteringTool : public ClusteringTool
 {
  public:
    
    SingleClusteringTool
     ( const std::string & type, 
       const std::string & name, 
       const IInterface * parent ) ;
    ~SingleClusteringTool() ;
        
  protected:

    // use a single global cluster
    xTalDataVec nextXtalsSet( xTalDataVec & xTalVec ) ;

 } ;

#endif



