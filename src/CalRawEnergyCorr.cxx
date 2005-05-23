
/** @file CalRawEnergyCorr.cxx
@brief Implementation of the class CalRawEnergyCorr
*/

#include "CalEnergyCorr.h"
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/GaudiException.h" 

#include "Event/TopLevel/EventModel.h"
#include "Event/Recon/CalRecon/CalCluster.h"
// TO BE USED SOON WITH NEW TDS
//#include "Event/Recon/CalRecon/CalEventEnergy.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Matrix/Matrix.h"

/**   
* @class CalRawEnergyCorr
* @author CalRecon Rewrite Group
*
* This sets the "raw" energy for an event (after clustering)
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalRawEnergyCorr.h,v 0.1 2005/04/11 13:28:50 chamont Exp $
*/

class CalRawEnergyCorr : public AlgTool, virtual public ICalEnergyCorr
{
public:

    //! Constructor
    CalRawEnergyCorr( const std::string& type, const std::string& name, const IInterface* parent);
    //! destructor
    virtual ~CalRawEnergyCorr() {}; 

    StatusCode initialize();

    // worker function to get the corrected energy      
    StatusCode doEnergyCorr() ;

private:

    /// Pointer to the Gaudi data provider service
    IDataProviderSvc* m_dataSvc;
};

#include "GaudiKernel/DeclareFactoryEntries.h"
DECLARE_TOOL_FACTORY(CalRawEnergyCorr) ;

CalRawEnergyCorr::CalRawEnergyCorr( const std::string & type, 
                                    const std::string & name, 
                                    const IInterface * parent )
                                    : AlgTool(type,name,parent)
{ 
    declareInterface<ICalEnergyCorr>(this) ; 
}

StatusCode CalRawEnergyCorr::initialize()
{
    // This function does following initialization actions:
    //    - Sets up a pointer to the data service as a convenience for the tool
    MsgStream log(msgSvc(), name());
    StatusCode sc = StatusCode::SUCCESS;
    log << MSG::INFO << "Initializing CalRawEnergyCorr" <<endreq;

    //Locate and store a pointer to the geometry service
    IService*   iService = 0;

    //Locate and store a pointer to the data service which allows access to the TDS
    if ((sc = serviceLocator()->getService("EventDataSvc", iService)).isFailure())
    {
        throw GaudiException("Service [EventDataSvc] not found", name(), sc);
    }
    m_dataSvc = dynamic_cast<IDataProviderSvc*>(iService);

    return sc;
}


StatusCode CalRawEnergyCorr::doEnergyCorr()
{
    //Purpose and method:
    //
    //   This function determines the overal "raw" energy for an event
    //   Note: This sums the energy contribution from all clusters and
    //         does a weighted average for centroid position and axis
    // 
    // TDS input:  CalClusters
    // TDS output: CalEventEnergy
    
    MsgStream lm(msgSvc(), name());

    StatusCode sc = StatusCode::SUCCESS;

    // Recover the collection of all clusters 
    SmartDataPtr<Event::CalClusterCol> calClusters(m_dataSvc, EventModel::CalRecon::CalClusterCol);

    // Set up to loop over all clusters to get total raw energy
    double     rawEnergy   = 0.;
    double     rawEneError = 0.;
    HepVector  posSum(3);
    HepMatrix  posWghtSum(3,3,0.);
    HepVector  axisSum(3);
    HepMatrix  axisWghtSum(3,3,0.);

// TO BE USED SOON WITH NEW TDS
//    // Do the loop and accumulate information
//    for(Event::CalClusterCol::iterator clusIter = calClusters->begin(); clusIter != calClusters->end(); clusIter++)
//    {
//        Event::CalCluster*       cluster = *clusIter;
//        const Event::CalParams&  params  = cluster->getCalParams();
//
//        HepVector centroid(3);
//        centroid[0] = params.getCentroid().x();
//        centroid[1] = params.getCentroid().y();
//        centroid[2] = params.getCentroid().z();
//
//        HepVector axis(3);
//        axis[0] = params.getAxis().x();
//        axis[1] = params.getAxis().y();
//        axis[2] = params.getAxis().z();
//
//        rawEnergy   += params.getEnergy();
//        rawEneError += params.getEnergyErr() * params.getEnergyErr();
//
//        HepMatrix posCovInv = params.getCentroidErrs();
//        int       matInvErr = 0;
//        posCovInv.invert(matInvErr);
//        posWghtSum += posCovInv;
//        posSum     += posCovInv * centroid;
//
//
//        HepMatrix axisCovInv = params.getAxisErrs();
//        axisCovInv.invert(matInvErr);
//        axisWghtSum += axisCovInv;
//        axisSum     += axisCovInv * axis;
//    }
//
//    // Get new errors and weighted average centroid 
//    int matInvErr = 0;
//    posWghtSum.invert(matInvErr);
//    posSum = posWghtSum * posSum;
//
//    // Get new errors and weighted average axis
//    axisWghtSum.invert(matInvErr);
//    axisSum = axisWghtSum * axisSum;
//
//    // New estimate of energy error
//    rawEneError = sqrt(rawEneError);
//
//    // Create a CalParams object to contain the results
//    Point  centroid(posSum[0], posSum[1], posSum[2]);
//    Vector axis(axisSum[0], axisSum[1], axisSum[3]);
//    Event::CalParams params(rawEnergy, rawEneError, centroid, posWghtSum, axis, axisWghtSum);
//
//    // Create a CalCorToolResult object to hold the information
//    Event::CalCorToolResult* corResult = new Event::CalCorToolResult();
//
//    corResult->setStatusBit(Event::CalCorToolResult::RAWENERGY);
//    corResult->setParams(params);
//    corResult->setChiSquare(1.);
//
//    // Note that this tool **cannot** be called without an CalEventEnergy object already
//    // existing in the TDS. No checking done to insure this though...
//    SmartDataPtr<Event::CalEventEnergy> eventEnergy(m_dataSvc,EventModel::CalRecon::CalEventEnergy);
//
//    // Add the corrected result to the CalEventEnergy object
//    eventEnergy->push_back(corResult);

    return sc;
}

