// File and version Information:
//   $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalXtalRecAlg.cxx,v 1.20.2.1 2004/08/06 17:24:04 berrie Exp $
//
// Description:
//    CalXtalRecAlg is an algorithm to reconstruct calorimeter
//    information in each individual crystal
//
// Author: A.Chekhtman



#include "CalXtalRecAlg.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
//#include "GaudiKernel/IDetDataSvc.h"
#include "idents/VolumeIdentifier.h"
#include "CLHEP/Geometry/Transform3D.h"
#include "geometry/Point.h"
#include "Event/Digi/CalDigi.h"
#include "Event/TopLevel/EventModel.h"
#include <map>


static const AlgFactory<CalXtalRecAlg>  Factory;
const IAlgFactory& CalXtalRecAlgFactory = Factory;

using namespace Event;

// constructor
CalXtalRecAlg::CalXtalRecAlg(const std::string& name, ISvcLocator* pSvcLocator):
    Algorithm(name, pSvcLocator)

{ 
}


StatusCode CalXtalRecAlg::initialize()

// Purpose and method:
//           This function sets values to private data members,
//           representing the calorimeter geometry and digitization
//           constants. Information  from xml files is obtained using 
//           GlastdetSvc::getNumericConstByName() function.
//           To make this constant extraction in a loop, the pairs
//           'constant pointer, constant name' are stored in
//           std::map container. 
//           Double and integer constants are extracted separatly,
//           because constants of both types are returned
//           by getNumericConstByName() as double.
//      
        
        
{
  MsgStream log(msgSvc(), name());
  StatusCode sc = StatusCode::SUCCESS;

        
  // extracting int constants
  double value;  // intermediate variable for reading constants from
                   // xml file as doubles and converting them to interger 
  typedef std::map<int*,std::string> PARAMAP;
  PARAMAP param; // map containing pointers to integer constants to be read
                   // with their symbolic names from xml file used as a key 

  //   filling the map with information on constants to be read 
  param[&m_xNum]=        std::string("xNum");
  param[&m_yNum]=        std::string("yNum");
  param[&m_eTowerCAL]=   std::string("eTowerCAL");
  param[&m_eLATTowers]=  std::string("eLATTowers");
  param[&m_nCsISeg]= std::string("nCsISeg");
  param[&m_eXtal]=       std::string("eXtal");
  param[&m_thresh]=std::string("cal.zeroSuppressEnergy");
    
  //    IGlastDetSvc* m_detSvc;
  sc = service("GlastDetSvc", m_detSvc);
    
  // loop over all constants information contained in the map
  for(PARAMAP::iterator it=param.begin(); it!=param.end();it++){

    //  attempt to get teh constant value via the method of GlastDetSvc
    if(!m_detSvc->getNumericConstByName((*it).second, &value)) {

      // if not successful - give the error message and return
      log << MSG::ERROR << " constant " <<(*it).second
          <<" not defined" << endreq;
      return StatusCode::FAILURE;

      //  if successful - fill the constant using the pointer from the map
    } else *((*it).first)= int(value);
  }
        
    
  // get cal energy tool
  sc = toolSvc()->retrieveTool("TestEnergyTool", m_pTestEnergyTool);
    if (sc.isFailure() ) {
    log << MSG::ERROR << "  Unable to create " << "TestEnergyTool" << endreq;
    return sc;
  }

  // get cal pos tool
  sc = toolSvc()->retrieveTool("TestPosTool", m_pTestPosTool);
  if (sc.isFailure() ) {
    log << MSG::ERROR << "  Unable to create " << "TestPosTool" << endreq;
    return sc;
  }

  // Get properties from the JobOptionsSvc
  sc = setProperties();
  if ( !sc.isSuccess() ) {
      log << MSG::ERROR << "Could not set jobOptions properties" << endreq;
      return sc;
  }

  return sc;
}


StatusCode CalXtalRecAlg::execute()

//  Purpose and method:
//    This function is called to do reconstruction
//    for all hitted crystals in one event.
//    It calls retrive() method to get access to input and output TDS data.
//    It iterates over all elements in CalDigiCol and calls
//    computeEnergy() and computePosition() methods doing real reconstruction.
//
//   TDS input: 
//         CalDigiCol* m_CalDigiCol - private class member containing
//         a pointer to the calorimeter digi callection 
//
//   TDS output: 
//         CalXtalRecCol* m_CalXtalRecCol - private class member containing
//         a pointer to the calorimeter crystal reconstructed data collection 
//
//     

{
	StatusCode sc = StatusCode::SUCCESS;
  MsgStream log(msgSvc(), name());

  sc = retrieve(); //get access to TDS data collections

  // loop over all calorimeter digis in CalDigiCol
	for (Event::CalDigiCol::const_iterator it = m_CalDigiCol->begin(); 
           it != m_CalDigiCol->end(); it++) {
           idents::CalXtalId xtalId = (*it)->getPackedId();
		
           // create new object to store crystal reconstructed data     
           Event::CalXtalRecData* recData = 
                   new Event::CalXtalRecData((*it)->getMode(),xtalId);
	   
           
           // calculate energy in the crystal
           int adcP, adcM;
           if(computeEnergy(recData, adcP, adcM, *it)){      

              // calculate position in the crystal
	            computePosition(recData, adcP, adcM);   

              // add new reconstructed data to the collection
              m_CalXtalRecCol->push_back(recData);
           }else{
                delete recData;
           }
	}

	return sc;
}


StatusCode CalXtalRecAlg::finalize()
// empty function: required by base class (Algorithm)
{
	StatusCode sc = StatusCode::SUCCESS;
	return sc;
}



StatusCode CalXtalRecAlg::retrieve()

// Purpose and method: 
//            This function provides access to the TDS input and output data
//            by setting the private data members m_CalDigiCol            
//            and m_CalXtalRecCol
//
//   TDS input: CalDigiCol
//   TDS output: CalXtalrecCol
//        
//        
        
{
	
    MsgStream log(msgSvc(), name());
    StatusCode sc = StatusCode::SUCCESS;

    m_CalXtalRecCol = 0;

    //  create output data collection
    m_CalXtalRecCol = new CalXtalRecCol;

    DataObject* pnode=0;

    // search for CalRecon section of Event directory in TDS
    sc = eventSvc()->retrieveObject( EventModel::CalRecon::Event, pnode );
    
    // if the required directory doesn't exist - create it
    if( sc.isFailure() ) {
        sc = eventSvc()->registerObject( EventModel::CalRecon::Event,
                                         new DataObject);
        if( sc.isFailure() ) {
       
             // if cannot create the directory - write an error message
            log << MSG::ERROR << "Could not create CalRecon directory"
                << endreq;
            return sc;
        }
    }

    
    // get a pointer to the input TDS data collection
    m_CalDigiCol = SmartDataPtr<Event::CalDigiCol>(eventSvc(),
                                EventModel::Digi::CalDigiCol); 

    //register output data collection as a TDS object
    sc = eventSvc()->registerObject(EventModel::CalRecon::CalXtalRecCol,
                                                       m_CalXtalRecCol);
    return sc;
}




bool CalXtalRecAlg::computeEnergy( CalXtalRecData* recData, 
                                   int &adcP, int &adcM,
                                   const Event::CalDigi* digi )

//   Purpose and method:
//           This function calculates the energy for one crystal.
//           It makes a loop over all readout ranges (1 or 4, depending
//           on readout mode), and converts adc values for both crystal
//           faces into energy, using m_pTestEnergyTool
//
//  
//  Input: CalDigi* digi - pointer to digitized calorimeter data for
//                         one crystal
//                            
//  Output: CalXtalRecData* recData - pointer to reconstructed data for
//                                    this crystal

{
  MsgStream log(msgSvc(), name());


  idents::CalXtalId xtalId = digi->getPackedId();

  const Event::CalDigi::CalXtalReadoutCol& readoutCol = digi->getReadoutCol();
	bool below_thresh = false;	
  //
  // loop over readout ranges
  for ( Event::CalDigi::CalXtalReadoutCol::const_iterator it = 
                            readoutCol.begin(); it !=readoutCol.end(); it++){

    // get readout range number for both crystal faces
    idents::CalXtalId::AdcRange rangeP = 
      (idents::CalXtalId::AdcRange) it->getRange(idents::CalXtalId::POS); 
    idents::CalXtalId::AdcRange rangeM = 
      (idents::CalXtalId::AdcRange) it->getRange(idents::CalXtalId::NEG); 

    // get adc values 
    adcP = it->getAdc(idents::CalXtalId::POS);	
    adcM = it->getAdc(idents::CalXtalId::NEG);	

    // convert adc values into energy
    float eneP;
    float eneM;
    m_pTestEnergyTool->calculate( 
              xtalId, idents::CalXtalId::POS, rangeP, adcP,
              0., eneP ); // position (=0.) arg is not used by TestEnergyTool
    m_pTestEnergyTool->calculate( 
              xtalId, idents::CalXtalId::NEG, rangeM, adcM,
              0., eneM ); // position (=0.) arg is not used by TestEnergyTool

    below_thresh = below_thresh || ((rangeP == 0) && (eneP < m_thresh)) 
                                || ((rangeM == 0) && (eneM < m_thresh));
    // create output object
    CalXtalRecData::CalRangeRecData* rangeRec =
        new CalXtalRecData::CalRangeRecData(rangeP,eneP,rangeM,eneM);

    // add output object to output collection
    recData->addRangeRecData(*rangeRec);
    delete rangeRec;
  }		
	return !below_thresh;
}


void CalXtalRecAlg::computePosition( CalXtalRecData* recData, 
                                     int adcP, int adcM )

// Purpose and method:
//              This function calculates the longitudinal position
//              for each crystal from light asymmetry.
//              
//  Input: CalXtalRecData* recData - pointer to the reconstructed crystal data
//  Output: the same object, the calculated position is stored using
//             public function SetPosition()  
{
	MsgStream msg(msgSvc(), name());

  // get crystal identification 
  idents::CalXtalId xtalId = recData->getPackedId();	
	
  // unpack crystal identification into tower, layer and column number
	int layer = xtalId.getLayer();
	int tower = xtalId.getTower();
	int col   = xtalId.getColumn();

  // create Volume Identifier for segment 0 of this crystal
	idents::VolumeIdentifier segm0Id;
	segm0Id.append(m_eLATTowers);
	segm0Id.append(tower/m_xNum);
	segm0Id.append(tower%m_xNum);
	segm0Id.append(m_eTowerCAL);
	segm0Id.append(layer);
	segm0Id.append(layer%2); 
  segm0Id.append(col);
	segm0Id.append(m_eXtal);
	segm0Id.append(0);

  HepTransform3D transf;

  //get 3D transformation for segment 0 of this crystal
	m_detSvc->getTransform3DByID(segm0Id,&transf);

  //get position of the center of the segment 0
  Vector vect0 = transf.getTranslation();

  // create Volume Identifier for the last segment of this crystal
  idents::VolumeIdentifier segm11Id;

  // copy all fields from segm0Id, except segment number
	for(int ifield = 0; ifield<fSegment; ifield++)
    segm11Id.append(segm0Id[ifield]);
	segm11Id.append(m_nCsISeg-1); // set segment number for the last segment

	//get 3D transformation for the last segment of this crystal
  m_detSvc->getTransform3DByID(segm11Id,&transf);

  //get position of the center of the last segment
	Vector vect11 = transf.getTranslation();

	Point p0(0.,0.,0.);		

  // position of the crystal center
	Point pCenter = p0+(vect0+vect11)*0.5; 

  //normalized vector of the crystal direction 
  Vector dirXtal = 0.5*(vect11-vect0)*m_nCsISeg/(m_nCsISeg-1);	

  // calculate average position of energy deposit in this crystal
  idents::CalXtalId::AdcRange rangeP = 
    (idents::CalXtalId::AdcRange) recData->getRange(
                          0, idents::CalXtalId::POS ); 
  idents::CalXtalId::AdcRange rangeM = 
    (idents::CalXtalId::AdcRange) recData->getRange(
                          0, idents::CalXtalId::NEG ); 

  float relpos;
  m_pTestPosTool->calculate( xtalId, rangeP, rangeM, adcP, adcM, relpos );
	Point pXtal = pCenter+dirXtal*relpos;
	
  // store calculated position in the reconstructed data
  // for the best readout range
  (recData->getRangeRecData(0))->setPosition(pXtal);
}
