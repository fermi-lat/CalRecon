
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
* Base class for clustering tools, containing member data
*
*
* $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Cluster.h,v 1.6 2003/05/11 19:59:12 burnett Exp $
*/


class Cluster : public ICluster,  public AlgTool {
	
public:
    
    //! constructor
	
    Cluster(const std::string& type, 
        const std::string& name, 
        const IInterface* parent);
    //! destructor
    virtual ~Cluster() {}; 
    
    virtual StatusCode initialize()  {
		return StatusCode::SUCCESS;
	};
	
	
    // workder function for finding clusters
    virtual StatusCode findClusters(Event::CalXtalRecCol* calXtalRecCol) 
	{return StatusCode::SUCCESS;};
	
    virtual StatusCode execute() {return StatusCode::SUCCESS;};
	
    virtual StatusCode finalize() {return StatusCode::SUCCESS;};
    
    virtual Event::CalXtalRecCol* getRecCol() {return m_calXtalRecCol;};

    virtual Event::CalClusterCol* getClusterCol() {return m_calClusterCol;};
	
    virtual void setClusterCol(Event::CalClusterCol* calClusterCol)
			{m_calClusterCol = calClusterCol;};

protected:
    
    // calculate the direction of the shower from the hits
    virtual Vector Fit_Direction(std::vector<Vector> pos,
                                     std::vector<Vector> sigma2,
                                     int nlayers) {
        return Vector(0.,0.,0.);
    }
	
private:
	
	
	//! reconstructed data for crystals, the input of the reconstruction
	Event::CalXtalRecCol* m_calXtalRecCol;
		
	Event::CalClusterCol* m_calClusterCol;
	};

inline Cluster::Cluster(const std::string& type, 
                        const std::string& name, 
                        const IInterface* parent)
                        : AlgTool( type, name, parent ) { 
                            declareInterface<ICluster>(this);
                        }



#endif
	
	
	