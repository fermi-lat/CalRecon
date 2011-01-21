
#ifndef MomentsClusterInfo_h
#define MomentsClusterInfo_h

#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "Event/Recon/CalRecon/CalMomParams.h"
#include "Event/Recon/CalRecon/CalNBCClassParams.h"
#include "CalRecon/ICalClusterFiller.h"
#include "CalRecon/ICalReconSvc.h"
#include "CalMomentsAnalysis.h"

#include "TMath.h"
#include "TMinuit.h"

/**
   @file MomentsClusterInfo.h
   
   @class MomentsClusterInfo

   @brief Base class for clustering tools, containing member data and default code for
   the global algorithm, the preparation of a cluster from a set of crystals, and the
   computing of its direction.

   The only pure virtual method which needs to be implemented in a derived class is
   nextXtalsSet(), which is selecting the crystals to be grouped together.

   @author Tracy Usher, Philippe Bruel, Luca Baldini (luca.baldini@pi.infn.it)

   $Revision: 1.14 $
   $Date: 2011/01/14 17:37:46 $
   $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Clustering/MomentsClusterInfo.h,v 1.14 2011/01/14 17:37:46 lbaldini Exp $
*/


class MomentsClusterInfo : virtual public ICalClusterFiller
{
 public:
  
  /// Constructor.
  MomentsClusterInfo(ICalReconSvc* calReconSvc);

  /// Destructor.
  virtual ~MomentsClusterInfo() {};
    
  Event::CalCluster* fillClusterInfo(const XtalDataList* xtalVec);

  /// Return true if the xtal in a given position is saturated.
  bool xtalSaturated(int tower, int layer, int column) const;
  /// And a convenience overload to pass a CalMomentsData directly.
  bool xtalSaturated(const CalMomentsData& momData) const;

private:
  
  /// Use this to fill the layer data.
  void fillLayerData(const XtalDataList* xtalVec, Event::CalCluster* cluster);
  
  /// Use this to fill the moments information.
  void fillMomentsData(const XtalDataList* xtalVec, Event::CalCluster* cluster);

  /// calculate the centroid of the shower from the hits using only the transverse
  /// position information (Philippe Bruel : needed when dealing with saturated crystals)
  int getCentroidTransverseInfoOnly(const XtalDataList* xtalVec);
  
  /// calculate the direction and centroid of the shower from the hits using only the
  /// transverse position information (Philippe Bruel : using minuit)
  int fitDirectionCentroid(const XtalDataList* xtalVec) ;
  
  /// Correct the longitudinal position using output of fitDirectionCentroid
  /// (Philippe Bruel : used for saturated crystals)
  Point GetCorrectedPosition(Point pcrystal, int itower, int ilayer, int icolumn);
  
  /// Look for saturated crystals : fill m_saturated[][][] (Philippe Bruel)
  int DetectSaturation();
  
  /// package service
  ICalReconSvc* m_calReconSvc;
  
  Point               m_p0;
  int                 m_calnLayers;
  Point               m_p1;         // centroid using only the transverse position information
    
  /// in order to handle saturation
  float m_saturationadc;
  int   m_Nsaturated;
  bool  m_saturated[16][8][12];
  
  TMinuit* m_minuit;
  int m_fit_nlayers;
  static Point m_fit_p_layer[8];
  static Vector m_fit_v_layer[8];
  static double m_fit_e_layer[8];
  static double m_fit_errp_layer[8];
  static double m_fit_chisq;
  static double m_fit_xcentroid;
  static double m_fit_ycentroid;
  static double m_fit_zcentroid;
  static double m_fit_xdirection;
  static double m_fit_ydirection;
  static double m_fit_zdirection;
  static int m_fit_option;
  
  /// function passed to Minuit to minimize
  static void fcncal(int & , double *, double &f, double *par, int );
  static double compute_chi2_cal(double *par);
  static double getsqdistancebetweenlines(Point p0, Vector v0, Point p1, Vector v1);
};

#endif
        
        
        
