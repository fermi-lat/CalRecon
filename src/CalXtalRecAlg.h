#ifndef __CALXTALRECALG_H
#define __CALXTALRECALG_H 1

#include "GaudiKernel/Algorithm.h"
#include "Event/Digi/CalDigi.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "CalXtalResponse/IXtalEneTool.h"
#include "CalXtalResponse/IXtalPosTool.h"
#include "idents/VolumeIdentifier.h"
#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"

/** @class CalXtalRecAlg
    @brief  Calorimeter crystal reconstruction algorithm

    This algorithm reconstructs energy and position in each calorimeter crystal.
    See CalXtalResponse/XtalEneTool & CalXtalResponse/XtalPosTool for details

    @author           A.Chekhtman
    @author           Zach Fetwrell

    $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalXtalRecAlg.h,v 1.8 2005/03/02 07:20:53 fewtrell Exp $
*/
class CalXtalRecAlg : public Algorithm
{
 public:
  CalXtalRecAlg(const std::string& name, ISvcLocator* pSvcLocator);
  virtual ~CalXtalRecAlg() {}
    
  /// initialize internal data members.
  StatusCode initialize();
  ///  Reconstruct ene & pos for all xtal hits in event
  StatusCode execute();
  /// required by Gaudi Algorithm class
  StatusCode finalize() {return StatusCode::SUCCESS;}

 private:
  ///  function for setting pointers to the input and output data in Gaudi TDS
  StatusCode retrieve();
    
  /** @brief method to calculate energy deposited in a crystal
  
  Creates 1 CalXtalRangeRec object which represents the best estimate
   
  @param recData pointer to CalXtalRecData object to store reconstructed 
  energy
  @param digi pointer to CalDigi object with input data
  @param below_thresh set to FALSE when either LEX8 adc val is below it's 
  respective LAC threshold
  */
  StatusCode computeEnergy(Event::CalXtalRecData &recData,
                           const Event::CalDigi &digi,
                           bool &below_thresh);
    
  /** @brief method to calculate longitudinal position in a crystal
   
  @param recData pointer to CalXtalRecData object to store reconstructed 
  position
  @param digi pointer to CalDigi object with input data
   
  @pre computePosition() assumes that the input digi data was good enough 
  for computeEnergy() to make a valid estimate.
  */
  StatusCode computePosition(Event::CalXtalRecData &recData, 
                             const Event::CalDigi &digi);
    
 private:
  /// pointer to input data collection in TDS
  Event::CalDigiCol* m_calDigiCol;

  /// pointer to the output data collection in TDS
  Event::CalXtalRecCol* m_calXtalRecCol;

  //-- XML GEOMETRY CONSTANTS --//
  
  int m_xNum;    ///< number of x towers
  int m_yNum;    ///< number of y towers

  /// the value of fTowerObject field, defining calorimeter module 
  int m_eTowerCAL;
  int m_eLATTowers; ///< the value of fLATObjects field, defining LAT towers 
  int m_eXtal;      ///< the value of fCellCmp field defining CsI crystal
  int m_nCsISeg;    ///< number of geometric segments per Xtal

  double m_CsILength; ///< length of CsI xtal in mm

  IGlastDetSvc* m_detSvc; ///< pointer to the Glast Detector Service
    
  /// pointer to CalResponse tool for converting xtal digi info -> energy 
  IXtalEneTool *m_xtalEneTool;
  /// pointer to CalResponse tool for converting xtal digi info -> pos
  IXtalPosTool *m_xtalPosTool;

  /// name of IXtalEneTool instantiation
  StringProperty m_eneToolName;
  /// name of IXtalPosTool instantiation
  StringProperty m_posToolName;
};

#endif
