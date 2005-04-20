
#ifndef __CalSingleClustering_H
#define __CalSingleClustering_H 1

#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"
#include "CalClustering.h"

/**   
* @class CalSingleClustering
*
* Find single cluster from all CAL hits
*
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalSingleClustering.h,v 1.2 2005/01/25 08:53:26 chamont Exp $
*/


class CalSingleClustering : public CalClustering
 {
  public:
    
    CalSingleClustering
     ( const std::string & type, 
       const std::string & name, 
       const IInterface * parent ) ;
    ~CalSingleClustering() ;
        
  protected:

    //! Distinguish sets of related xtals
    virtual void makeSets( const XtalDataVec & xtals, XtalDataVecVec & clusters ) ;

 } ;

#endif



