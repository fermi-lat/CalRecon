
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/SmartDataPtr.h"

#include "Event/Recon/CalRecon/CalCluster.h"
#include "Event/TopLevel/EventModel.h"

#include "src/MipFinding/IMipFindingTool.h"
#include "src/Utilities/CalException.h"

#include <CalRecon/ICalReconSvc.h>

/**   
* @class CalMipFinderAlg
*
* @brief An algorithm for controlling and applying the various energy correction tools
*        used to determine the final event energy for GLAST
* 
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalMipFinderAlg.cxx,v 1.3 2005/06/22 21:30:16 usher Exp $
*/


class CalMipFinderAlg : public Algorithm
{
public:
    //! constructor
    CalMipFinderAlg( const std::string & name, ISvcLocator * pSvcLocator ); 
    
    //! destructor
    virtual ~CalMipFinderAlg() {};
    
    virtual StatusCode initialize();

    StatusCode execute();

    StatusCode finalize() ;

private:

    //! correction tool names
    std::string      m_mipFinderName ;
    
    //! correction tools
    IMipFindingTool* m_mipFinder ;
    
    //! package service
    ICalReconSvc *      m_calReconSvc ;
} ;

#include "GaudiKernel/DeclareFactoryEntries.h"
DECLARE_ALGORITHM_FACTORY(CalMipFinderAlg) ;


CalMipFinderAlg::CalMipFinderAlg( const std::string & name, ISvcLocator * pSvcLocator )
 : Algorithm(name,pSvcLocator)
{   
    // Declare the properties with these defaults
    declareProperty("MipFinderName", m_mipFinderName = "StdMipFindingTool");
}



StatusCode CalMipFinderAlg::initialize()
{
    // Purpose and Method: Initialize the algorithm:
    //                     - Initializes the vector of pointers to the various
    //                       energy correction tools to all for this particular
    //                       iteration of reconstruction

    MsgStream log(msgSvc(), name());
    StatusCode sc = StatusCode::SUCCESS;

    // Basic initialization first
    log << MSG::INFO << "CalMipFinderAlg Initialization";
    if( (sc = setProperties()).isFailure()) 
    {
        log << " didn't work!" << endreq;
        return sc;
    }
    log << endreq;
        
    if (service("CalReconSvc",m_calReconSvc,true).isFailure())
    {
        log<<MSG::ERROR<<"Could not find CalReconSvc"<<endreq ;
        return StatusCode::FAILURE ;
    }

    if ((sc = toolSvc()->retrieveTool(m_mipFinderName, m_mipFinder)).isFailure())
    {
        log << MSG::ERROR << " Unable to create " << m_mipFinderName << endreq ;
        return sc ;
    }

    return sc;
}


StatusCode CalMipFinderAlg::execute()
{
    //Purpose and method: Primary driver for running the various energy correction tools
    //                    Also creates and registers the output TDS object for the Event Energy
    //                    and retrieves the "best" energy as the one to use this event.
    // 
    // TDS input:  CalClusterCol
    // TDS output: CalEventEnergy (with a vector of CalCorToolResult objects for each tool run)
    //
    MsgStream log(msgSvc(), name());
    StatusCode sc = StatusCode::SUCCESS;
    
    log<<MSG::DEBUG<<"Begin"<<endreq ;

    // Retrieve our TDS objects, we use Clusters to output corrected energy in CalEventEnergy
    //Event::CalClusterCol*  calClusters = SmartDataPtr<Event::CalClusterCol>(eventSvc(),EventModel::CalRecon::CalClusterCol);
    //Event::CalEventEnergy* calEnergy   = SmartDataPtr<Event::CalEventEnergy>(eventSvc(),EventModel::CalRecon::CalEventEnergy);
    //Event::TkrVertexCol*   tkrVertices = SmartDataPtr<Event::TkrVertexCol>(eventSvc(),EventModel::TkrRecon::TkrVertexCol);

    // find mips
    try {
        if ((m_mipFinder->findMIPCandidates()).isFailure()) {
            sc = m_calReconSvc->handleError(name(),"mip finding tool failure") ;
        }        
    } catch( CalException & e ) {
        sc = m_calReconSvc->handleError(name()+" CalException",e.what()) ;
    } catch( std::exception & e) {
        sc = m_calReconSvc->handleError(name()+" std::exception",e.what()) ;
    } catch(...) {
        sc = m_calReconSvc->handleError(name(),"unknown exception") ;
    }

    log<<MSG::DEBUG<<"End"<<endreq ;
    return sc;
}

StatusCode CalMipFinderAlg::finalize()
{ 
    return StatusCode::SUCCESS ; 
}




