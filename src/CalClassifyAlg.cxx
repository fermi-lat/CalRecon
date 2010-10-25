
#include <CalRecon/ICalClassifyTool.h>
#include <CalRecon/ICalReconSvc.h>

// for implementation
#include "src/Utilities/CalException.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/DeclareFactoryEntries.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"

#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"

#include "Event/Recon/CalRecon/CalCluster.h"
#include "Event/TopLevel/EventModel.h"

/**   
* @class CalClassifyAlg
*
* @brief Algorithm for the classification of calorimeter clusters
*
* The idea is to use all the information available from the first pass
* of CalRecon, so containing CAL only information, in order to classify
* the clusters to tell what's their probability to be gamma/hadron/mip like,
* and potentially pass the most gamma-like cluster to the tracker reconstruction.
*
*        Implemented from CalClustersAlg, Johan, October 2010
*/


class CalClassifyAlg : public Algorithm
{
public:
    
    //! constructor
    CalClassifyAlg( const std::string & name, ISvcLocator * pSvcLocator ) ; 
    //! destructor
    virtual ~CalClassifyAlg() {}; 
    
    virtual StatusCode initialize() ;

        
    /*!Classifies the clusters and stores
    * there the following results: 
    * - probability to be a gamma
    * - probability to be a hadron
    * - probability to be a mip
    * - TO BE UPDATED   -- TBD      
    */        
    StatusCode execute();

    StatusCode finalize() ;
        
private:
    
    /// name of Tool for finding clusters
    StringProperty      m_classifierToolName;

    /// pointer to actual tool for finding clusters
    ICalClassifyTool* m_classifierTool;

    //! package service
    ICalReconSvc *      m_calReconSvc ;
} ;


//==============================================
// IMPLEMENTATION
//==============================================


DECLARE_ALGORITHM_FACTORY(CalClassifyAlg) ;

CalClassifyAlg::CalClassifyAlg(const std::string & name, ISvcLocator * pSvcLocator )
                             : Algorithm(name,pSvcLocator), m_calReconSvc(0)
{   
    declareProperty ("classifierToolName", m_classifierToolName="CalClusterNBClassifyTool") ;
}

StatusCode CalClassifyAlg::initialize()
{
    MsgStream log(msgSvc(), name()) ;
    StatusCode sc = StatusCode::SUCCESS ;

    setProperties();
    
    if (service("CalReconSvc",m_calReconSvc,true).isFailure())
    {
        log<<MSG::ERROR<<"Could not find CalReconSvc"<<endreq ;
        return StatusCode::FAILURE ;
    }

    sc = toolSvc()->retrieveTool(m_classifierToolName,  m_classifierTool);
    if (sc.isFailure() ) 
    {
        log << MSG::ERROR << "  Unable to create " << m_classifierToolName << endreq;
        return sc;
    }
        
    return sc;
}

StatusCode CalClassifyAlg::execute()
{
    MsgStream log(msgSvc(), name());
    StatusCode sc = StatusCode::SUCCESS;
    
    log<<MSG::DEBUG<<"Begin execute()"<<endreq ;
    
    // non fatal errors 
    // if there's no Cluster, we should not classify them
    // check code below to get the cluster collectin through the SmartDataPtr -- TBD
    if (!m_calReconSvc->getClusters())  return StatusCode::SUCCESS ;

    // call the classifier tool
    try 
    {
        // Insure CalRecon/Event directory in TDS
        DataObject * pnode = 0 ;
        if ((eventSvc()->retrieveObject(EventModel::CalRecon::Event,pnode)).isFailure()
            && (eventSvc()->registerObject(EventModel::CalRecon::Event,new DataObject)).isFailure()) 
        { 
            throw CalException("cannot register Event/CalRecon") ;
        } 

        // Look up the CalClusterCol
	// TBD -- why do we call this SmartDataPtr stuff and not getClusters()
        Event::CalClusterCol* calClusterCol = 
            SmartDataPtr<Event::CalClusterCol>(eventSvc(),EventModel::CalRecon::CalClusterCol) ;

        // Call the tool to classify the clusters
        if (m_classifierTool->classifyClusters(calClusterCol).isFailure()) 
        {
            sc = m_calReconSvc->handleError(name(),"classifier tool failure") ;
        }
        
	// Shall we sort the cluster now that they are classified ? -- TBD
	
        // display cluster energies, and classification for DEBUG -- TBD
        Event::CalClusterCol::const_iterator cluster ;
        for ( cluster = calClusterCol->begin() ; 	 
              cluster != calClusterCol->end() ; 	 
              cluster++) { 	 
            log<<MSG::DEBUG<<"CalCluster Energy: "
              <<(*cluster)->getCalParams().getEnergy()
              <<endreq ;
        }
        
    // Catch any exceptions here
    } catch( CalException & e ) {
        sc = m_calReconSvc->handleError(name()+" CalException",e.what()) ;
    } catch( std::exception & e) {
        sc = m_calReconSvc->handleError(name()+" std::exception",e.what()) ;
    } catch(...) {
        sc = m_calReconSvc->handleError(name(),"unknown exception") ;
    }
    
    log<<MSG::DEBUG<<"End execute()"<<endreq ;
    return sc;
}

StatusCode CalClassifyAlg::finalize()
{ 
    return StatusCode::SUCCESS ; 
}




