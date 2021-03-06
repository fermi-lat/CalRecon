
#include "StdClusterInfo.h"
 
/// This makes CalClusters out of associated CalXtalRecData pointers
Event::CalCluster* StdClusterInfo::fillClusterInfo(const XtalDataList* xTalVec)
{
    int   calnLayers = m_calReconSvc->getCalNLayers() ;
    const Point p0(0.,0.,0.);  

    //Initialize variables
    double ene = 0;                                 // Total energy in this cluster
    Vector pCluster(0.,0.,0.);                      // Cluster position
    std::vector<double> eneLayer(calnLayers,0.);     // Energy by layer
    std::vector<Vector> pLayer(calnLayers);       // Position by layer
    std::vector<Vector> rmsLayer(calnLayers);     // rms by layer

    // Compute barycenter and various moments
    
    // loop over all crystals in the current cluster
    XtalDataList::const_iterator xTalIter;
    for(xTalIter = xTalVec->begin(); xTalIter != xTalVec->end(); xTalIter++)
    {
        // get pointer to the reconstructed data for given crystal
                Event::CalXtalRecData* recData = *xTalIter;
        
        // get reconstructed values
        double eneXtal = recData->getEnergy();                // crystal energy
        Vector pXtal   = recData->getPosition() - p0;         // Vector of crystal position
        int    layer   = (recData->getPackedId()).getLayer(); // layer number

        // update energy of corresponding layer
        eneLayer[layer] += eneXtal;
        
        // update average position of corresponding layer
        Vector ptmp = eneXtal*pXtal;
        pLayer[layer] += ptmp;
        
        // Vector containing squared coordinates, weighted by crystal energy 
        Vector ptmp_sqr(ptmp.x()*pXtal.x(), ptmp.y()*pXtal.y(), ptmp.z()*pXtal.z());
 
        // update quadratic spread, which is proportional to eneXtal;
        // this means, that position error in one crystal
        // is assumed to be 1/sqrt(eneXtal) 
        rmsLayer[layer] += ptmp_sqr;
        
        // update energy sum
        ene  += eneXtal;

        // update cluster position
        pCluster += ptmp;
    }

    // construct the cluster
    Event::CalCluster* cl = new Event::CalCluster(0);
    cl->setProducerName("StdClusterInfo") ;
    cl->clear();

    // Now take the means

    // if energy sum is not zero - normalize cluster position
    if(ene > 0.) {
        pCluster /= ene; 
        cl->setStatusBit(Event::CalCluster::CENTROID) ;
    }
         // if energy is zero - set cluster position to non-physical value
    else pCluster = Vector(-1000., -1000., -1000.);
    
    // loop over calorimeter layers
    int i ;
    for( i = 0; i < calnLayers; i++)
    {
        // if energy in the layer is not zero - finalize calculations
        if(eneLayer[i]>0)
        {
            // normalize position in the layer
            pLayer[i] *= (1./eneLayer[i]); 
            
            // normalize quadratic spread in the laye
            rmsLayer[i] *= (1./eneLayer[i]);
            
            // Vector containing the squared average position in each component
            Vector sqrLayer(pLayer[i].x()*pLayer[i].x(),
                            pLayer[i].y()*pLayer[i].y(),
                            pLayer[i].z()*pLayer[i].z());
            
            // the precision of transverse coordinate measurement
            // if there is no fluctuations: 1/sqrt(12) of crystal width
            Vector d;
            double csIWidth = m_calReconSvc->getCalCsIWidth() ;
            if(i%2 == 1) d = Vector(csIWidth*csIWidth/12.,0.,0.);
            else d = Vector(0.,csIWidth*csIWidth/12.,0.);
            
            // subtracting the  squared average position and adding
            // the square of crystal width, divided by 12
            rmsLayer[i] += d-sqrLayer;
        }
            
        
        // if energy in the layer is zero - reset position and spread Vectors
        else 
        {
            pLayer[i]=p0;
            rmsLayer[i]=p0;
        }
    }
    
    // Now sum the different rms to have one transverse and one longitudinal rms
    double rms_trans = 0;
    double rms_long = 0;
    std::vector<Vector> posrel(calnLayers);
    
    for(int ilayer = 0; ilayer < calnLayers ; ilayer++)
    {
        posrel[ilayer]=pLayer[ilayer]-pCluster;
       
        // Sum alternatively the rms
        if(ilayer%2)
        {
            rms_trans += rmsLayer[ilayer].y();
            rms_long += rmsLayer[ilayer].x();
        }
        else
        {
            rms_trans += rmsLayer[ilayer].x();
            rms_long += rmsLayer[ilayer].y();
        }
    }
 
    // Compute direction using the positions and rms per layer
    Vector caldir = fitDirection(pLayer, rmsLayer);
    
    Event::CalXtalsParams xtalsParams;

    Event::CalMomParams momParams(ene, 10*ene, pCluster.x(), pCluster.y(), pCluster.z(),
                                  1.,0.,0.,1.,0.,1.,
                                  caldir.x(),   caldir.y(),   caldir.z(),   1.,0.,0.,1.,0.,1.);
    // Initial fit parameters
    Event::CalFitParams fitParams(4, 0., pCluster.x(), pCluster.y(), pCluster.z(),
                                  1.,0.,0.,1.,0.,1.,
                                  caldir.x(),   caldir.y(),   caldir.z(),   1.,0.,0.,1.,0.,1.);

    // initialize empty CalMSTreeParams container - CalMSTreePar
    Event::CalMSTreeParams treeParams(0.,0.,0,0.,0.,0.,0.,0.,0.);

    // Initialize an empty CalClassParamsContainer.
    Event::CalClassParams classParams;

    // Fill CalCluster data
    cl->initialize(xtalsParams, treeParams, fitParams, momParams, classParams);

    for( i = 0; i < calnLayers; i++)
    {
        Point layerPos(pLayer[i].x(), pLayer[i].y(), pLayer[i].z());

        // Set the data for the vector
        Event::CalClusterLayerData layerData(eneLayer[i], layerPos, rmsLayer[i]);

        cl->push_back(layerData);
    }

    // Fill CalCluster data
    //Event::CalCluster* cl = new Event::CalCluster(ene, pCluster + p0);

    //cl->initialize(ene, eneLayer, pLayer, rmsLayer, rms_long, rms_trans, caldir, 0.);

  return cl;
 }

Vector StdClusterInfo::fitDirection(std::vector<Vector> pos, std::vector<Vector> sigma2)
//
// Purpose and Method:
//       find the particle direction from average positions in each
//       layer and their quadratic errors. The fit is made independently
//       in XZ and YZ planes for odd and even layers, respectively.
//       Then the 3-vector of particle direction is calculated from these 
//       2 projections.
//       Only position information based on the transversal crystal position
//       is used. The position along the crystal, calculated from signal
//       asymmetry is not used.
//
// Inputs:
//         pos      - average position for each calorimeter layer
//         sigma2   - quadratic error of position measurement for each layer
//                    in each direction  
//         nlayers  - number of calorimeter layers
//
// Returned value:    3-Vector of reconstructred particle direction
//
{
    // sigma2.z() is useless here no matter its value.
    double cov_xz = 0;  // covariance x,z
    double cov_yz = 0;  // covariance y,z
    double mx=0;        // mean x
    double my=0;        // mean y
    double mz1=0;       // mean z for x pos
    double mz2=0;       // mean z for y pos
    double norm1=0;     // sum of weights for odd layers
    double norm2=0;     // sum of weights for even layers
    double var_z1=0;    // variance of z for odd layers        
    double var_z2=0;    // variance of z for even layers
    
    // number of layers with non-zero energy deposition
    // in X and Y direction, respectively
    int nlx=0,nly=0;
    
    // "non-physical vector of direction, which is returned
    // if fit is imposible due to insufficient number of hitted layers
    Vector nodir(-1000.,-1000.,-1000.);
    
    // loop over calorimeter layers
    for(int il = 0; il < m_calReconSvc->getCalNLayers(); il++)
    {                
        // For the moment forget about longitudinal position
        
        // odd layers used for XZ fit
        if(il % 2 == 1)
        {
            
            // only if the spread in X direction is not zero,
            // which is the case if there is non-zero energy
            // deposition in this layer
            if (sigma2[il].x()>0.)
            {
                nlx++; // counting layers used for the fit in XZ plane 
                
                // calculate weighting coefficient for this layer
                double err = 1/sigma2[il].x(); 
                
                // calculate sums for least square linear fit in XZ plane
                cov_xz += pos[il].x()*pos[il].z()*err;
                var_z1 += pos[il].z()*pos[il].z()*err;
                mx += pos[il].x()*err;
                mz1 += pos[il].z()*err;
                norm1 += err;
            }
        }
        // even layers used for YZ fit
        else
        {
            // only if the spread in Y direction is not zero,
            // which is the case if there is non-zero energy
            // deposition in this layer
            if(sigma2[il].y()>0.)
            {
                
                nly++; // counting layers used for the fit in YZ plane 
                
                // calculate weighting coefficient for this layer
                double err = 1/sigma2[il].y();
                
                
                // calculate sums for least square linear fit in YZ plane
                cov_yz += pos[il].y()*pos[il].z()*err;
                var_z2 += pos[il].z()*pos[il].z()*err;
                my += pos[il].y()*err;
                mz2 += pos[il].z()*err;
                norm2 += err;
            }
        }
    }                
    
    // linear fit requires at least 2 hitted layers in both XZ and YZ planes
    // otherwise non-physical direction is returned
    // which means that direction hasn't been found
    if(nlx <2 || nly < 2 )return nodir;
    
    mx /= norm1;
    mz1 /= norm1;
    cov_xz /= norm1;
    cov_xz -= mx*mz1;
    var_z1 /= norm1;
    var_z1 -= mz1*mz1;
    
    // protection against dividing by 0 in the next statment
    if(var_z1 == 0) return nodir;
    
    // Now we have cov(x,z) and var(z) we can
    // deduce slope in XZ plane
    double tgthx = cov_xz/var_z1;
    
    
    my /= norm2;
    mz2 /= norm2;
    cov_yz /= norm2;
    cov_yz -= my*mz2;
    var_z2 /= norm2;
    var_z2 -= mz2*mz2;
    
    // protection against dividing by 0 in the next statment
    if(var_z2 == 0) return nodir;
    
    // Now we have cov(y,z) and var(z) we can
    // deduce slope in YZ plane
    double tgthy = cov_yz/var_z2;
    
    // combining slope in XZ and YZ planes to get normalized 3-vector
    // of particle direction
    double tgtheta_sqr = tgthx*tgthx+tgthy*tgthy;
    double costheta = 1/sqrt(1+tgtheta_sqr);
    Vector dir(costheta*tgthx,costheta*tgthy,costheta);
    return dir;
}
        
