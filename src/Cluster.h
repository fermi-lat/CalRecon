
#ifndef __Cluster_H
#define __Cluster_H 1

#include "ICluster.h"
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/ISvcLocator.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "Event/Recon/CalRecon/CalCluster.h"

/**   
* @class Cluster
*
* Algorithm for reconstruction of energy and direction of incident particle
*
*
* Performs high level energy corrections
*
* The reconstruction here uses CalXtalRecCol to produce a CalClusterCol.
*  It evaluates the barycenter for each layer using the coordinates stored in
*  the CalXtalRecCol,
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Cluster.h,v 1.3 2003/02/16 19:08:16 richard Exp $
*/


class Cluster : virtual public ICluster {
	
public:
    
    //! constructor
	
    Cluster() {};
    //! destructor
    virtual ~Cluster() {}; 
    
    virtual StatusCode initialize()  {
		return StatusCode::SUCCESS;
	};
	
	
	/*!Performs the reconstruction, creates one CalCluster object and stores
	* there the following results: 
	* - Energy per layer is computed and stored in CalCluster in MeV
	* - Barycenter per layer is also computed and stored in CalCluster
	*/        
    virtual StatusCode findClusters(Event::CalXtalRecCol* calXtalRecCol) 
	{return StatusCode::SUCCESS;};
	
    virtual StatusCode execute() {return StatusCode::SUCCESS;};
	
    virtual StatusCode finalize() {return StatusCode::SUCCESS;};
    
    virtual Event::CalXtalRecCol* getRecCol() {return m_calXtalRecCol;};

    virtual Event::CalClusterCol* getClusterCol() {return m_calClusterCol;};
	
    virtual void setClusterCol(Event::CalClusterCol* calClusterCol)
			{m_calClusterCol = calClusterCol;};

protected:
    
    virtual Vector Fit_Direction(std::vector<Vector> pos,
                                     std::vector<Vector> sigma2,
                                     int nlayers) {
        return (Vector)(0.,0.,0.);
    }
	
private:
	
	
	//! reconstructed data for crystals, the input of the reconstruction
	Event::CalXtalRecCol* m_calXtalRecCol;
		
	Event::CalClusterCol* m_calClusterCol;
	};


#endif
	
	
	
