// $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalRecoAlg.cxx,v 1.3 2000/12/07 18:46:41 burnett Exp $

// Include files
#include "CalRecon/CalRecoAlg.h"

#include "Gaudi/MessageSvc/MsgStream.h"
#include "Gaudi/Kernel/AlgFactory.h"
#include "Gaudi/Interfaces/IDataProviderSvc.h"
#include "Gaudi/DataSvc/SmartDataPtr.h"

#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"


//#include "reconstruction/ReconData.h"
#include "reconstruction/GlastTuple.h"
#include "reconstruction/PrintReconData.h"
#include "GlastEvent/Raw/TdCsIData.h"

static const AlgFactory<CalRecoAlg>  Factory;
const IAlgFactory& CalRecoAlgFactory = Factory;

//------------------------------------------------------------------------------
/// Algorithm parameters which can be set at run time must be declared.
/// This should be done in the constructor.
CalRecoAlg::CalRecoAlg(const std::string& name, ISvcLocator* pSvcLocator) :
Algorithm(name, pSvcLocator), m_detSvc(0) {
}


//------------------------------------------------------------------------------
/*! The "functional" part of the class: For the EmptyAlgorithm example they do
nothing apart from print out info messages.
NB in the initialize method: you must explicitly initialize the base class
before using any services (message service, event data service etc.) otherwise 
the behaviour will be unpredictable at best.
*/
StatusCode CalRecoAlg::initialize() {
    
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "initialize" << endreq;
    
    // Use the Job options service to set the Algorithm's parameters
    setProperties();
    
    // now try to find the GlastDevSvc service
    IGlastDetSvc* detSvc = 0;
#if 1    
    const IID&  IID_IGlastDetSvc  =  401; // wired it for now!
    StatusCode sc = serviceLocator()->getService ("GlastDetSvc",
        IID_IGlastDetSvc, reinterpret_cast<IInterface*&>( detSvc ));
    
    m_detSvc = detSvc;

#else //this is the new way in v6, 
    StatusCode sc = service("GlastDetSvc", m_detSvc);
#endif    

    if (!sc.isSuccess ()){
        log << MSG::ERROR << "Couldn't find the GlastDetSvc!" << endreq;
        return StatusCode::FAILURE;
    }
    return StatusCode::SUCCESS;
}


//------------------------------------------------------------------------------
StatusCode CalRecoAlg::execute() {
    
    StatusCode  sc = StatusCode::SUCCESS;
    MsgStream   log( msgSvc(), name() );
    log << MSG::INFO << "execute" << endreq;
    

    // create a GlastData object
    //GlastData data;

    // fill it from the IRF (FAILS)
    //m_detSvc->accept(data);
    
    /*! Goin to try the Dynamic cast and see what happens
    */ 
    DataObject* pObject;

    /*! Currently the retrieval from the TDS is no operational in
        the CalRecon package.
    */
    sc = eventSvc()->retrieveObject("/Event/Raw/TdCsIDatas", pObject);

    if( sc.isFailure() ) {
        log << MSG::INFO << "TdCsIData not retrieved from the the TDS" << endreq;
    
        return sc;
    }
    
    log << MSG::INFO << "Successfully retrieved TdCsIData object from the TDS!!!" << endreq;

    TdCsIData* newData;
    try {
        newData  = dynamic_cast<TdCsIData*>(pObject);
    } catch(...) {
        log << MSG::INFO << "Failed to convert object to MCACDHitVector" << endreq;
        return StatusCode::FAILURE;
    }
    

    
    // see what is there
    //data.printOn(std::cout);

    // create the recon object from the reconstrution package and pass data to it.

    CalRecon recon;
    recon.reconstruct(newData);

    // print out the  tuple
    recon.accept(PrintReconData(std::cout));
    return sc;
}


//------------------------------------------------------------------------------
StatusCode CalRecoAlg::finalize() {
    
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "finalize" << endreq;
    
    return StatusCode::SUCCESS;
}






