
#ifndef __CALCLUSTERSALG_H
#define __CALCLUSTERSALG_H 1

#include "GaudiKernel/Algorithm.h"
#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"
#include "geometry/Vector.h"
#include "GlastEvent/Recon/CalRecon/CalXtalRecData.h"
#include "GlastEvent/Recon/CalRecon/CalCluster.h"
class Midnight;

//----------------------------------------------
//
//   CalClustersAlg
//
//   Algorithm Data constructor of CsIClusterList
//----------------------------------------------
//             J.A Hernando, Santa Cruz 02/29/00
//----------------------------------------------


//! Performs high level energy corrections

/*! The reconstruction here uses CalXtalRecCol to produce a CsIClusterList.
*  It evaluates the barycenter for each layer using the coordinates stored in
*  the CalXtalRecCol, and tries to correct for energy leakage using two different
*  methods:
*  -  Profile performs profile fitting
*  -  Leak performs a correction using the last layer correlation
* 
* See the description of Profile() and Leak() for a description
* For most of the test beam energies Leak() should give better results.
* there can be a strong bias though because of miscalibration
*
* For a comparison one can see on the following plots the results of this
* method on R138 data and a MC run of 20 GeV positrons
* 
* \image html figurearticle2.gif
* \image latex figurearticle2.eps width=10cm
* \image html figuresim2.gif
* \image latex figuresim2.eps width=10cm
* \author Regis Terrier
* \author Alexandre Chehtman
* 
* \bug These energy correction methods are implemented on axis.
* \warning May not give sensible results when there is a large uncertainty in 
* the gains especially for the Leak method if there is a problem in the last 
* layer.
* \warning High energy corrections are intended for high energy 
* \todo Add low energy corrections 
*/


//##########################################################
class CalClustersAlg : public Algorithm
//##########################################################
{
public:
    
    //! constructor
    CalClustersAlg(const std::string& name, ISvcLocator* pSvcLocator); 
    //! destructor
    virtual ~CalClustersAlg() {}; 
    
    // operations
    StatusCode initialize();
    StatusCode execute();
    StatusCode finalize();
    
    //! Leakage corrections with last layer
    double Leak(double sum,double elast);
    //! Leakage corrections with profile fitting
    void Profile(double sum, GlastEvent::CalCluster* cl);
    //! Direction reconstruction
    Vector Fit_Direction(std::vector<Vector> pos,std::vector<Vector> sigma2,int nlayers);
    
protected:
    
    StatusCode retrieve();
    
private:
    
    //! reconstructed data for crystals, the input of the reconstruction
	GlastEvent::CalXtalRecCol* m_calXtalRecCol;
    //! the clusters list, the output of the reconstruction
    GlastEvent::CalClusterCol* m_calClusterCol;
    //! the minimizer for Profile()
    Midnight* minuit;
    int m_callNumber;

	double m_CsIWidth;
    double m_CsIHeight;

	int m_CalnLayers;

	IGlastDetSvc* detSvc;

};

#endif



