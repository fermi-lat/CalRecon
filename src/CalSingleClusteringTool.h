
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
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalSingleClusteringTool.h,v 1.1 2005/01/25 07:51:08 chamont Exp $
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

    //! Distinguish sets of related xtals
    virtual void makeSets( const XtalDataVec & xtals, XtalDataVecVec & clusters ) ;

 } ;

#endif



