
#ifndef __CalSingleClusteringTool_H
#define __CalSingleClusteringTool_H 1

#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"
#include "CalClusteringTool.h"

/**   
* @class CalSingleClusteringTool
*
* Find single cluster from all CAL hits
*
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalSingleClusteringTool.h,v 1.1 2005/01/21 15:02:05 chamont Exp $
*/


class CalSingleClusteringTool : public CalClusteringTool
 {
  public:
    
    CalSingleClusteringTool
     ( const std::string & type, 
       const std::string & name, 
       const IInterface * parent ) ;
    ~CalSingleClusteringTool() ;
        
  protected:

    // use a single global cluster
    xTalDataVec nextXtalsSet( xTalDataVec & xTalVec ) ;

 } ;

#endif



