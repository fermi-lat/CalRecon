//$Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/CalDisplay.cxx,v 1.2 2001/06/11 18:59:17 chehtman Exp $

/// Gaudi specific include files
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"

// gui, display includes
#include "GuiSvc/IGuiSvc.h"
#include "gui/DisplayControl.h"
#include "gui/GuiMgr.h"
#include "CalRecon/CsIClusters.h"
#include "CalRecon/CalRecLogs.h"
#include "CalRecon/CalGeometrySvc.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**
    Display the Cal reconstructed information
  */
class CalDisplay : public Algorithm
{
public:
    //! Constructor of this form must be provided
    CalDisplay(const std::string& name, ISvcLocator* pSvcLocator); 
    virtual ~CalDisplay() {}
    //! mandatory
    StatusCode initialize();
    //! mandatory
    StatusCode execute();
    //! mandatory
    StatusCode finalize(){ return StatusCode::SUCCESS;}
private:
    
	CalRecLogs* m_crl;
	CsIClusterList* m_cls;
	CalGeometrySvc* m_CalGeo; 

};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static const AlgFactory<CalDisplay>  Factory;
const IAlgFactory& CalDisplayFactory = Factory;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CalDisplay::CalDisplay(const std::string& name, ISvcLocator* pSvcLocator) :
Algorithm(name, pSvcLocator) 
{
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class CalRep : public gui::DisplayRep {
private:
	CalRecLogs** m_pp_crl;
	CsIClusterList** m_pp_cls;
	float m_logheight;

public:
    CalRep(CalRecLogs** pp_crl, CsIClusterList** pp_cls, float logheight)
		:m_pp_crl(pp_crl),m_pp_cls(pp_cls),m_logheight(logheight){}
    void update(){

		const Point p0(0.,0.,0.);
		CalRecLogs* crl = *m_pp_crl;
		if(crl){

// drawing red box for each log with a size proportional to energy deposition
			
			setColor("red");

			int nLogs = crl->num();
			double emax = 0.;
			for (int jlog = 0; jlog < nLogs ; jlog++) {
				CalRecLog* recLog = crl->Log(jlog);
				double eneLog = recLog->energy();
				if(eneLog>emax)emax=eneLog;
			}
			if(emax>0){
				for (jlog = 0; jlog < nLogs ; jlog++) {
					CalRecLog* recLog = crl->Log(jlog);
					double eneLog = recLog->energy();
					if(eneLog>0.01*emax){
						Vector pLog = recLog->position() - p0;
						double x = pLog.x();
						double y = pLog.y();
						double z = pLog.z();
						double s = 0.45*m_logheight*eneLog/emax;
						moveTo(Point(x-s, y-s, z-s));
						lineTo(Point(x+s, y-s, z-s));
						lineTo(Point(x+s, y-s, z+s));
						lineTo(Point(x-s, y-s, z+s));
						lineTo(Point(x-s, y-s, z-s));
						moveTo(Point(x-s, y+s, z-s));
						lineTo(Point(x+s, y+s, z-s));
						lineTo(Point(x+s, y+s, z+s));
						lineTo(Point(x-s, y+s, z+s));
						lineTo(Point(x-s, y+s, z-s));
						moveTo(Point(x-s, y-s, z-s));
						lineTo(Point(x-s, y+s, z-s));
						moveTo(Point(x+s, y-s, z-s));
						lineTo(Point(x+s, y+s, z-s));
						moveTo(Point(x-s, y-s, z+s));
						lineTo(Point(x-s, y+s, z+s));
						moveTo(Point(x+s, y-s, z+s));
						lineTo(Point(x+s, y+s, z+s));
					}
				}
			}
		}

//		drawing the cross in the average position for each layer 
		CsIClusterList* cls = *m_pp_cls;
		if(cls){
			double s=0.1*m_logheight;
			setColor("blue");
			CsICluster* cl = cls->Cluster(0);
			double energy_sum = cl->energySum();
			const std::vector<double>& eneLayer = cl->getEneLayer();
			const std::vector<Vector>& posLayer = cl->getPosLayer();
			for( int l=0;l<8;l++){
				double x=(posLayer[l]).x();
				double y=(posLayer[l]).y();
				double z=(posLayer[l]).z();
				moveTo(Point(x-s, y, z));
				lineTo(Point(x+s, y, z));
				moveTo(Point(x, y-s, z));
				lineTo(Point(x, y+s, z));
				moveTo(Point(x, y, z-s));
				lineTo(Point(x, y, z+s));
			}


//		drawing the center of the cluster		
			setColor("green");
			double x = (cl->position()).x();
			double y = (cl->position()).y();
			double z = (cl->position()).z();
			moveTo(Point(x-s, y, z));
			lineTo(Point(x, y, z+s));
			lineTo(Point(x+s, y, z));
			lineTo(Point(x, y, z-s));
			lineTo(Point(x-s, y, z));
			lineTo(Point(x, y+s, z));
			lineTo(Point(x+s, y, z));
			lineTo(Point(x, y-s, z));
			lineTo(Point(x-s, y, z));
			moveTo(Point(x, y-s, z));
			lineTo(Point(x, y, z+s));
			lineTo(Point(x, y+s, z));
			lineTo(Point(x, y, z-s));
			lineTo(Point(x, y-s, z));		
		}
	}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
StatusCode CalDisplay::initialize()
{
    //Look for the gui service
    IGuiSvc* guiSvc = 0;
    StatusCode sc = service("GuiSvc", guiSvc);

    m_crl = 0;
    m_cls = 0;
	
     sc = service("CalGeometrySvc", m_CalGeo);
	 float logheight = m_CalGeo->logHeight();

    
    //Ok, see if we can set up the display
    if (sc.isSuccess())  {
	//Set up the display rep for Clusters
	guiSvc->guiMgr()->display().add(new CalRep(&m_crl,&m_cls,logheight), "Cal recon");
    }
    
    return sc;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
StatusCode CalDisplay::execute()
{
    StatusCode sc = StatusCode::SUCCESS;
    MsgStream log(msgSvc(), name());


	m_crl = SmartDataPtr<CalRecLogs>(eventSvc(),"/Event/CalRecon/CalRecLogs"); 
	if (m_crl == 0){ sc = StatusCode::FAILURE;
	
	        log << MSG::ERROR << "CalDisplay failed to access CalRecLogs" << endreq;
			return sc;
	}
    
	
	m_cls  = SmartDataPtr<CsIClusterList>(eventSvc(),"/Event/CalRecon/CsIClusterList");

	if (m_cls == 0){ sc = StatusCode::FAILURE;
	
	        log << MSG::ERROR << "CalDisplay failed to access CsIClusterList" << endreq;
			return sc;
	}
    
    return sc;
}

