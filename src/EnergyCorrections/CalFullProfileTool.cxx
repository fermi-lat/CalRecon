/**   
*
* THIS CODE IS STILL NOT FULLY DOCUMENTED !!!!!!!!!!!!!!!!!!!!
* DESCRIPTION COMMENTS WILL BE ADDED SOON.
* Philippe Bruel
*
*/

#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/GaudiException.h" 

#include "Event/TopLevel/EventModel.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "Event/Recon/CalRecon/CalEventEnergy.h"
#include "Event/Recon/TkrRecon/TkrVertex.h"

#include "TkrUtil/ITkrGeometrySvc.h"

#include <CalRecon/ICalEnergyCorr.h>
#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"

// to access an XML containing Profile Bias parameters file
#include "xmlBase/IFile.h"

#include "FullShowerDevelopmentDescriptionManager.h"
#include "FullShowerProfileParamsManager.h"

//Gamma function and Minuit
#include "TMath.h"
#include "TMinuit.h"
#include "TNtuple.h"
#include "TFile.h"

/**   
* @class CalFullProfileTool
*
* Algorithm for calculating energy by fitting the longitudinal
* shower profile using a full (= longitudinal AND radial) description of the shower development in the calorimeter.
*
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/EnergyCorrections/CalFullProfileTool.cxx,v 1.3 2005/07/20 12:22:34 bruel Exp $
*/


class CalFullProfileTool  : public AlgTool, virtual public ICalEnergyCorr 
{
public:
  //! destructor
  CalFullProfileTool( const std::string& type, const std::string& name, const IInterface* parent);
  ~CalFullProfileTool() {}; 
  
  StatusCode initialize();
  
  //! Longitudinal profile fitting method
  /*! It performs a longitudinal profile fitting using :
   * the FullShowerDevelopmentDescriptionManager to determine the energy deposition in the layers 
   * and the FullShowerProfileParamsManager to constrain the fit parameters during the fitting procedure.
   *
   * \b Revision:
   * - 07/06/05       Philippe Bruel    first implementation
   */     
  
  Event::CalCorToolResult* doEnergyCorr(Event::CalClusterCol*, Event::TkrVertex* );

  double GetRadiationLengthInTracker(Event::TkrVertex*);
  
  StatusCode finalize();
    
private:
  /// Pointer to the Gaudi data provider service
  IDataProviderSvc* m_dataSvc;
  
  /// Detector Service
  IGlastDetSvc *    m_detSvc; 
  
  /// TkrGeometrySvc used for access to tracker geometry info
  ITkrGeometrySvc*  m_tkrGeom;

  TMinuit* m_minuit;
  static FullShowerProfileParamsManager *m_fsppm;
  static FullShowerDevelopmentDescriptionManager *m_fsddm;
  static double m_elayer_dat[8];
  static double m_elayer_fit[8];
  static double m_eelayer_fit[8];
  
  // function passed to Minuit to minimize
  static void fcn(int & , double *, double &f, double *par, int );
  static double compute_chi2(double *par);
  static double compute_deposited_energy(double *par, double z0, double z1);
  
  static double m_chisq;
  static double m_params_contribution;
  static double m_params_contribution_factor;
  static double m_totchisq;

private:
  double BIAS0;
  double BIAS1;
};

#include "GaudiKernel/DeclareFactoryEntries.h"
DECLARE_TOOL_FACTORY(CalFullProfileTool) ;

FullShowerDevelopmentDescriptionManager * CalFullProfileTool::m_fsddm = 0;
FullShowerProfileParamsManager * CalFullProfileTool::m_fsppm = 0;
double CalFullProfileTool::m_elayer_dat[8];
double CalFullProfileTool::m_elayer_fit[8];
double CalFullProfileTool::m_eelayer_fit[8];
double CalFullProfileTool::m_chisq = 0;
double CalFullProfileTool::m_params_contribution = 0;
double CalFullProfileTool::m_params_contribution_factor = 0;
double CalFullProfileTool::m_totchisq = 0;

double CalFullProfileTool::compute_deposited_energy(double *par, double z0, double z1)
{
  double beta = (par[0]-1)/par[1];
  return par[2]*(TMath::Gamma(par[0],beta*z1)-TMath::Gamma(par[0],beta*z0));
}

double CalFullProfileTool::compute_chi2(double *par)
{
  if(par[2]<=0.) return 1e10;
  m_params_contribution = m_fsppm->GetChi2Contribution(par);
  if(m_params_contribution<0) return 1e10;
  
  m_fsddm->FillCurrentFSDD(par[1]);

  int i,j;
  
  // fill TMeF
  for(i=0;i<8;++i) m_elayer_fit[i] = 0;
  double e_i;
  for(i=0;i<m_fsddm->CurrentFSDD->NStep;++i)
    {
      e_i = compute_deposited_energy(par,m_fsddm->CurrentFSDD->X0[i],m_fsddm->CurrentFSDD->X0[i+1]);
      for(j=0;j<8;++j) m_elayer_fit[j] += e_i*m_fsddm->CurrentFSDD->layerfraction[j][i];
    }

  double maxlayerenergy = -99999.;
  for(i=0;i<8;++i) if(m_elayer_fit[i]>maxlayerenergy) maxlayerenergy = m_elayer_fit[i];

  double fit_error = 0.1;
  if(maxlayerenergy>0) fit_error = maxlayerenergy*m_fsppm->relerr;

  // calculate chisquare
  m_chisq = 0;
  double delta;
  for(i=0;i<8;++i)
    {
      delta  = (m_elayer_dat[i]-m_elayer_fit[i])/fit_error;
      m_chisq += delta*delta;
    }

  m_params_contribution_factor = 3;
  if(par[1]/m_fsddm->CurrentFSDD->lastx0>0.9)
    {
      m_params_contribution_factor = 3+(par[1]/m_fsddm->CurrentFSDD->lastx0 - 0.9);
      if(m_params_contribution_factor>10) m_params_contribution_factor = 10.;
    }

  m_totchisq = m_chisq + m_params_contribution_factor * m_params_contribution;

  return m_totchisq;
}

void CalFullProfileTool::fcn(int & , double *, double &f, double *par, int )
{
  f = compute_chi2(par);
}

CalFullProfileTool::CalFullProfileTool( const std::string& type, 
				      const std::string& name, 
				      const IInterface* parent)
  : AlgTool(type,name,parent)
{
  // declare base interface for all consecutive concrete classes
  declareInterface<ICalEnergyCorr>(this);
  // Declare the properties that may be set in the job options file
}

StatusCode CalFullProfileTool::initialize()
  // This function does following initialization actions:
  //    - extracts geometry constants from xml file using GlastDetSvc
{
  MsgStream log(msgSvc(), name());
  StatusCode sc = StatusCode::SUCCESS;
  log << MSG::DEBUG << "Initializing CalFullProfileTool" <<endreq;
  
  //Locate and store a pointer to the data service which allows access to the TDS
  if ((sc = service("EventDataSvc", m_dataSvc)).isFailure())
    {
      throw GaudiException("Service [EventDataSvc] not found", name(), sc);
    }
  
  if ((sc = service("GlastDetSvc", m_detSvc, true)).isFailure())
    { 
      throw GaudiException("Service [GlastDetSvc] not found", name(), sc);
    }
  
  // find TkrGeometrySvc service
  if ((sc = service("TkrGeometrySvc", m_tkrGeom, true)).isFailure())
    {
      throw GaudiException("Service [TkrGeometrySvc] not found", name(), sc);
    }

  BIAS0 = 2.655362;
  BIAS1 = 0.007235;

  m_fsppm = new FullShowerProfileParamsManager();
  m_fsddm = new FullShowerDevelopmentDescriptionManager(m_detSvc,14,2.,1.,1.85,0.80,0.1);
  
  // Minuit object
  m_minuit = new TMinuit(5);
  
  //Sets the function to be minimized
  m_minuit->SetFCN(fcn);

  return sc;
}

double CalFullProfileTool::GetRadiationLengthInTracker(Event::TkrVertex* vertex)
{
  if(!vertex) return 0;
  
  Vector vv = (vertex->getDirection()).unit();
  double costheta = fabs(vv.z());
  
  // copied from CalValsCorrTool
  
  // Get the First Track - from vertex - THIS IS BAD - NEED A BETTER WAY HERE
  SmartRefVector<Event::TkrTrack>::const_iterator pTrack1 = vertex->getTrackIterBegin(); 
  const Event::TkrTrack* track_1 = *pTrack1;
  
  double tkr_RLn = track_1->getTkrCalRadlen();
  
  // Patch for error in KalFitTrack: 1/2 of first radiator left out
  int plane = m_tkrGeom->getPlane(track_1->front()->getTkrId());
  int layer = m_tkrGeom->getLayer(plane);
  if (m_tkrGeom->isTopPlaneInLayer(plane)) {
    tkr_RLn += 0.5*m_tkrGeom->getRadLenConv(layer)/costheta;
  }
  
  // add up the rad lens; this could be a local array if you're bothered by the overhead
  //   but hey, compared to the propagator...
  double tkr_radLen_nom = 0.; 
  int layerCount = layer;
  for(; layerCount>=0; --layerCount) {
    tkr_radLen_nom += m_tkrGeom->getRadLenConv(layerCount) 
      + m_tkrGeom->getRadLenRest(layerCount);
  }
  if(costheta!=0)
    {
      tkr_radLen_nom /= costheta;
      if(tkr_RLn > tkr_radLen_nom * 1.5)     {tkr_RLn = tkr_radLen_nom * 1.5;}
      else if(tkr_RLn < tkr_radLen_nom * .5) {tkr_RLn  = tkr_radLen_nom * .5;}
    }
  
  return tkr_RLn;
}

Event::CalCorToolResult* CalFullProfileTool::doEnergyCorr(Event::CalClusterCol * clusters, Event::TkrVertex* vertex)
  //               This function fits the parameters of shower profile using
  //               the Minuit minimization package and stores the fitted
  //               parameters in the CalCluster object
  //
  //  Inputs:    eTotal - total energy deposition in the calorimeter
  //             cl - pointer to the CalCluster object to store the fitted
  //                  parameters 
  //
  //  Output:   parameters fit_energy,ki2,fit_start,fit_alpha,fit_lambda,
  //            stored in CalCluster object using initProfile() method
{

  Event::CalCorToolResult* corResult = 0;  
  MsgStream lm(msgSvc(), name());
  
    if (clusters->empty())
    {
        lm << MSG::DEBUG << "Ending doEnergyCorr: No Cluster" 
            << endreq;
        return corResult;
    }
    Event::CalCluster * cluster = clusters->front() ;

  double pp[3];
  double vv[3];
  double tkr_RLn = 0;

  int i;
  
  double eTotal = cluster->getCalParams().getEnergy()/1000.;
  if( eTotal<1.)
    {
      lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : eTotal<1GeV -> no energy computation" <<endreq;
      return corResult;
    }
  
  if (vertex == 0)
    {
      pp[0] = (cluster->getPosition()).x();
      pp[1] = (cluster->getPosition()).y();
      pp[2] = (cluster->getPosition()).z();
      if((cluster->getDirection()).z()>0)
	{
	  vv[0] = -(cluster->getDirection()).x();
	  vv[1] = -(cluster->getDirection()).y();
	  vv[2] = -(cluster->getDirection()).z();
	}
      else
	{
	  vv[0] = (cluster->getDirection()).x();
	  vv[1] = (cluster->getDirection()).y();
	  vv[2] = (cluster->getDirection()).z();
	}
      tkr_RLn = 0;
    }
  else
    {
      pp[0] = (vertex->getPosition()).x();
      pp[1] = (vertex->getPosition()).y();
      pp[2] = (vertex->getPosition()).z();
      vv[0] = (vertex->getDirection()).x();
      vv[1] = (vertex->getDirection()).y();
      vv[2] = (vertex->getDirection()).z();
      tkr_RLn = GetRadiationLengthInTracker(vertex);
    }

  lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : pp = " <<  pp[0] << " " << pp[1] << " " << pp[2] << " " <<endreq;
  lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : vv = " <<  vv[0] << " " << vv[1] << " " << vv[2] << " " <<endreq;
  lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : tkr_RLn = " << tkr_RLn <<endreq;
  

  double wideningfactor = 1.;
  double mytheta;
  double tkrrln1; // when wideningfactor = 1
  if(vertex != 0 && fabs(vv[2])>0)
    {
      mytheta = atan(-sqrt(vv[0]*vv[0]+vv[1]*vv[1])/vv[2])*180./TMath::Pi();
      if(mytheta>40)
	{
	  tkrrln1 = 2-0.05*(mytheta-40.);
	  if(tkr_RLn>tkrrln1)
	    {
	      wideningfactor = 1.+(tkr_RLn-tkrrln1)/1.5;
	      if(wideningfactor>2) wideningfactor = 2.;
	    }
	}
    }

  m_fsddm->SetWideningFactor(wideningfactor);

  if(!m_fsddm->Compute(pp,vv,tkr_RLn))
    {
      lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : Problem during m_fsddm->Compute. Returning empty corResult." <<endreq;
      return corResult;
    }

  if(m_fsddm->mintotx0cal<0.5) // Make sure that lastx0>0 during the fitting procedure
    {
      lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr :  m_fsddm->mintotx0cal<0.5. No energy computation. Returning empty corResult." <<endreq;
      return corResult;
    }
  
  for(i=0;i<=m_fsddm->NDevelopment;++i)
    {
      lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : m_fsddm->FSDDX0[" << i <<"] :"
	 << " startx0 = " << m_fsddm->FSDDX0[i]->startx0
	 << " totx0cal = " << m_fsddm->FSDDX0[i]->totx0cal
	 << " lastx0 = " << m_fsddm->FSDDX0[i]->lastx0
	 <<endreq;
    }

  for(i=0;i<8;++i)
    {
      lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : m_fsddm->meantotx0lay[" << i <<"] :"
	 << m_fsddm->meantotx0lay[i]
	 << " meanposx0lay = " << m_fsddm->meanposx0lay[i]
	 <<endreq;
    }

  // defines global variable to be used for fcn
  for (i=0;i<8;++i)
    {
      // We are working in GeV
      m_elayer_dat[i] = (*cluster)[i].getEnergy()/1000.;
    }
  
  double arglist[10];
  int ierflg = 0;
  double amin,edm,errdef;
  int nvpar,nparx,icstat;
  
  // Set no output because of tuple output
  arglist[0] = -1;
  m_minuit->mnexcm("SET PRI", arglist ,1,ierflg);
  
  // idem with warnings ( minuit prints warnings
  // when the Hessian matrix is not positive )
  m_minuit->mnexcm("SET NOW", arglist ,1,ierflg);
  
  arglist[0] = 1;
  m_minuit->mnexcm("SET ERR", arglist ,1,ierflg);
  
  // defines the strategy used by minuit
  // 1 is standard
  arglist[0] = 2;
  m_minuit->mnexcm("SET STR", arglist ,1,ierflg);        
  
  double vstart[3];
  double vstep[3];
  
  double par0,par1,par2;
  double epar0,epar1,epar2;
  
  m_fsppm->Fill(eTotal);
  
  vstart[0] = m_fsppm->alpha;
  vstart[1] = m_fsppm->tmax;
  vstart[2] = eTotal;
  
  vstep[0] = 0.05;
  vstep[1] = 0.05;
  vstep[2] = eTotal*0.1;
  
  m_minuit->mnparm(0, "a1", vstart[0], vstep[0], 1.1,20,ierflg);
  m_minuit->mnparm(1, "a2", vstart[1], vstep[1], 0.1,20,ierflg);
  m_minuit->mnparm(2, "a3", vstart[2], vstep[2], 0.1,10000,ierflg);
  
  lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : Initial parameters : " << vstart[0] << " " << vstart[1] << " " << vstart[2] <<endreq;

  // Calls Migrad with 500 iterations maximum
  arglist[0] = 500;
  arglist[1] = 1.;
  m_minuit->mnexcm("MIGRAD", arglist ,2,ierflg);

  double m_ierflg = (double)ierflg;
    
  lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : ierflg = " << ierflg <<endreq;
  lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : totchisq = " << m_totchisq << " = " << m_chisq << " + " << m_params_contribution_factor << " * " << m_params_contribution <<endreq;
  
  m_minuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  
  m_minuit->GetParameter(0,par0,epar0);
  m_minuit->GetParameter(1,par1,epar1);
  m_minuit->GetParameter(2,par2,epar2);
      
  double m_lastx0 = m_fsddm->CurrentFSDD->lastx0;
  double m_totx0cal = m_fsddm->CurrentFSDD->totx0cal;
    
  lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : results : alpha = " << par0 
     << " tmax = " << par1 
     << " energy = " << par2 
     << " fit flag " << ierflg 
     << " totchisq = " << amin <<endreq;
  
  // Clear minuit
  arglist[0] = 1;
  m_minuit->mnexcm("CLEAR", arglist ,1,ierflg);
    
  // Quick Bias Correction
  double bias = 1.;
  if(par2>0.1)
    bias = log(BIAS1*log(par2)+BIAS0);
  if(bias<0.97) bias = 0.97;
  if(bias>0.99) bias = 0.99;
  par2 /= bias;
  epar2 /= bias;
  
  // Ok, fill in the corrected information and exit
  Event::CalParams params = cluster->getCalParams();

  params.setEnergy(1000.*par2);
  params.setEnergyErr(1000.*epar2);
  
  corResult = new Event::CalCorToolResult();
  corResult->setStatusBit(Event::CalCorToolResult::VALIDPARAMS);
  corResult->setCorrectionName(type());
  corResult->setParams(params);
  corResult->setChiSquare(amin);
  corResult->insert(Event::CalCorEneValuePair("fit_energy",1000.*par2)); // energy (in MeV)
  corResult->insert(Event::CalCorEneValuePair("energy_err",1000.*epar2)); // energy error (in MeV)
  corResult->insert(Event::CalCorEneValuePair("fitflag", m_ierflg)); // flag from Minuit
  corResult->insert(Event::CalCorEneValuePair("alpha", par0)); // alpha (profile fit parameter 0)
  corResult->insert(Event::CalCorEneValuePair("tmax", par1)); // tmax (profile fit parameter 1)
  corResult->insert(Event::CalCorEneValuePair("tkr_RLn",tkr_RLn )); // radiation length in tracker (0 for cal only events)
  corResult->insert(Event::CalCorEneValuePair("lastx0",m_lastx0 )); // total radiation length seen by the shower
  corResult->insert(Event::CalCorEneValuePair("cal_eff_RLn",m_totx0cal)); // effective radiation length in cal (i.e in CsI)
  corResult->insert(Event::CalCorEneValuePair("totchisq",m_totchisq)); // total chisquare = usual chisquare + weight * parameters constraint
  corResult->insert(Event::CalCorEneValuePair("chisq",m_chisq)); // usual chisquare (sum_layers ( (e-efit)/de )^2
  corResult->insert(Event::CalCorEneValuePair("parcf",m_params_contribution_factor)); // weighting factor of the parameters constraint
  corResult->insert(Event::CalCorEneValuePair("parc",m_params_contribution)); // parameters constraint contribution to the chisquare
  corResult->insert(Event::CalCorEneValuePair("recp0",pp[0])); // Point and direction information - kept for the moment for debugging purpose
  corResult->insert(Event::CalCorEneValuePair("recp1",pp[1]));
  corResult->insert(Event::CalCorEneValuePair("recp2",pp[2]));
  corResult->insert(Event::CalCorEneValuePair("recv0",vv[0]));
  corResult->insert(Event::CalCorEneValuePair("recv1",vv[1]));
  corResult->insert(Event::CalCorEneValuePair("recv2",vv[2]));
  corResult->insert(Event::CalCorEneValuePair("widening",wideningfactor));

  
//   m_fsddm->SetWideningFactor(1.5);
//   if(!m_fsddm->Compute(pp,vv,tkr_RLn))
//     {
//       lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : Problem during m_fsddm->Compute. Returning empty corResult." <<endreq;
//     }
//   else
//     {
//       arglist[0] = -1;
//       m_minuit->mnexcm("SET PRI", arglist ,1,ierflg);
//       m_minuit->mnexcm("SET NOW", arglist ,1,ierflg);
//       arglist[0] = 1;
//       m_minuit->mnexcm("SET ERR", arglist ,1,ierflg);
//       arglist[0] = 2;
//       m_minuit->mnexcm("SET STR", arglist ,1,ierflg);        
//       m_fsppm->Fill(eTotal);  
//       vstart[0] = m_fsppm->alpha;
//       vstart[1] = m_fsppm->tmax;
//       vstart[2] = eTotal;
//       vstep[0] = 0.05;
//       vstep[1] = 0.05;
//       vstep[2] = eTotal*0.1;
//       m_minuit->mnparm(0, "a1", vstart[0], vstep[0], 1.1,20,ierflg);
//       m_minuit->mnparm(1, "a2", vstart[1], vstep[1], 0.1,20,ierflg);
//       m_minuit->mnparm(2, "a3", vstart[2], vstep[2], 0.1,10000,ierflg);
//       arglist[0] = 500;
//       arglist[1] = 1.;
//       m_minuit->mnexcm("MIGRAD", arglist ,2,ierflg);
//       m_minuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);  
//       m_minuit->GetParameter(0,par0,epar0);
//       m_minuit->GetParameter(1,par1,epar1);
//       m_minuit->GetParameter(2,par2,epar2);
//       m_lastx0 = m_fsddm->CurrentFSDD->lastx0;
//       m_totx0cal = m_fsddm->CurrentFSDD->totx0cal;
      
//       lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : results widening = 1.5:"
// 	 << " alpha = " << par0 
// 	 << " tmax = " << par1 
// 	 << " energy = " << par2 
// 	 << " fit flag " << ierflg 
// 	 << " totchisq = " << amin <<endreq;
//       arglist[0] = 1;
//       m_minuit->mnexcm("CLEAR", arglist ,1,ierflg);
//       bias = 1.;
//       if(par2>0.1)
// 	bias = log(BIAS1*log(par2)+BIAS0);
//       if(bias<0.97) bias = 0.97;
//       if(bias>0.99) bias = 0.99;
//       par2 /= bias;
//       epar2 /= bias;
//       corResult->insert(Event::CalCorEneValuePair("wf1.5_fit_energy",1000.*par2)); // energy (in MeV)
//       corResult->insert(Event::CalCorEneValuePair("wf1.5_alpha", par0)); // alpha (profile fit parameter 0)
//       corResult->insert(Event::CalCorEneValuePair("wf1.5_tmax", par1)); // tmax (profile fit parameter 1)
//       corResult->insert(Event::CalCorEneValuePair("wf1.5_lastx0",m_lastx0 )); // total radiation length seen by the shower
//       corResult->insert(Event::CalCorEneValuePair("wf1.5_cal_eff_RLn",m_totx0cal)); // effective radiation length in cal (i.e in CsI)
//       corResult->insert(Event::CalCorEneValuePair("wf1.5_totchisq",m_totchisq)); // total chisquare = usual chisquare + weight * parameters constraint
//       corResult->insert(Event::CalCorEneValuePair("wf1.5_chisq",m_chisq)); // usual chisquare (sum_layers ( (e-efit)/de )^2
//       corResult->insert(Event::CalCorEneValuePair("wf1.5_parcf",m_params_contribution_factor)); // weighting factor of the parameters constraint
//       corResult->insert(Event::CalCorEneValuePair("wf1.5_parc",m_params_contribution)); // parameters constraint contribution to the chisquare
//     }

//   m_fsddm->SetWideningFactor(2.);
//   if(!m_fsddm->Compute(pp,vv,tkr_RLn))
//     {
//       lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : Problem during m_fsddm->Compute. Returning empty corResult." <<endreq;
//     }
//   else
//     {
//       arglist[0] = -1;
//       m_minuit->mnexcm("SET PRI", arglist ,1,ierflg);
//       m_minuit->mnexcm("SET NOW", arglist ,1,ierflg);
//       arglist[0] = 1;
//       m_minuit->mnexcm("SET ERR", arglist ,1,ierflg);
//       arglist[0] = 2;
//       m_minuit->mnexcm("SET STR", arglist ,1,ierflg);        
//       m_fsppm->Fill(eTotal);  
//       vstart[0] = m_fsppm->alpha;
//       vstart[1] = m_fsppm->tmax;
//       vstart[2] = eTotal;
//       vstep[0] = 0.05;
//       vstep[1] = 0.05;
//       vstep[2] = eTotal*0.1;
//       m_minuit->mnparm(0, "a1", vstart[0], vstep[0], 1.1,20,ierflg);
//       m_minuit->mnparm(1, "a2", vstart[1], vstep[1], 0.1,20,ierflg);
//       m_minuit->mnparm(2, "a3", vstart[2], vstep[2], 0.1,10000,ierflg);
//       arglist[0] = 500;
//       arglist[1] = 1.;
//       m_minuit->mnexcm("MIGRAD", arglist ,2,ierflg);
//       m_minuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);  
//       m_minuit->GetParameter(0,par0,epar0);
//       m_minuit->GetParameter(1,par1,epar1);
//       m_minuit->GetParameter(2,par2,epar2);
//       m_lastx0 = m_fsddm->CurrentFSDD->lastx0;
//       m_totx0cal = m_fsddm->CurrentFSDD->totx0cal;
      
//       lm << MSG::DEBUG << "CalFullProfileTool::doEnergyCorr : results widening = 2.:"
// 	 << " alpha = " << par0 
// 	 << " tmax = " << par1 
// 	 << " energy = " << par2 
// 	 << " fit flag " << ierflg 
// 	 << " totchisq = " << amin <<endreq;
//       arglist[0] = 1;
//       m_minuit->mnexcm("CLEAR", arglist ,1,ierflg);
//       bias = 1.;
//       if(par2>0.1)
// 	bias = log(BIAS1*log(par2)+BIAS0);
//       if(bias<0.97) bias = 0.97;
//       if(bias>0.99) bias = 0.99;
//       par2 /= bias;
//       epar2 /= bias;
//       corResult->insert(Event::CalCorEneValuePair("wf2_fit_energy",1000.*par2)); // energy (in MeV)
//       corResult->insert(Event::CalCorEneValuePair("wf2_alpha", par0)); // alpha (profile fit parameter 0)
//       corResult->insert(Event::CalCorEneValuePair("wf2_tmax", par1)); // tmax (profile fit parameter 1)
//       corResult->insert(Event::CalCorEneValuePair("wf2_lastx0",m_lastx0 )); // total radiation length seen by the shower
//       corResult->insert(Event::CalCorEneValuePair("wf2_cal_eff_RLn",m_totx0cal)); // effective radiation length in cal (i.e in CsI)
//       corResult->insert(Event::CalCorEneValuePair("wf2_totchisq",m_totchisq)); // total chisquare = usual chisquare + weight * parameters constraint
//       corResult->insert(Event::CalCorEneValuePair("wf2_chisq",m_chisq)); // usual chisquare (sum_layers ( (e-efit)/de )^2
//       corResult->insert(Event::CalCorEneValuePair("wf2_parcf",m_params_contribution_factor)); // weighting factor of the parameters constraint
//       corResult->insert(Event::CalCorEneValuePair("wf2_parc",m_params_contribution)); // parameters constraint contribution to the chisquare
//     }
  
  return corResult;
}


  StatusCode CalFullProfileTool::finalize()
{
  StatusCode sc = StatusCode::SUCCESS;
  
  // delete Minuit object
  delete m_minuit;
  
  delete m_fsddm;
  delete m_fsppm; 
  
  return sc;
}

