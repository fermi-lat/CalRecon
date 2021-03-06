/**
   @file CalMomentsData.h

   @class CalMomentsData
   
   @brief Utility data object for the moments analysis which attempts to make
   the class independent of the actual Cal data objects used.

   In the old implementation the class used to encapsulate essentially two pieces
   of information: the xtal position and energy. Now there's much more information,
   including whether the longitudinal position is on the xtal end, whether the
   xtal is saturated, and, if available, the corrected longitudinal position provided
   by the fit---which is using the transverse information only.
   
   @author Tracy Usher, Luca Baldini (luca.baldini@pi.infn.it)
   
   $Revision: 1.6 $
   $Date: 2012/11/27 14:28:25 $
   $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Clustering/CalMomentsData.h,v 1.6 2012/11/27 14:28:25 lbaldini Exp $
 */


/// Maximum distance to declare the hit position on the xtal edge.
#define DIST_LONG_POS_INVALID  0.01
/// Maximum distance to declare the fit position near the xtal edge. 
#define DIST_FIT_POS_NEAR_EDGE 25.0

#include "geometry/Ray.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "Event/Recon/CalRecon/CalFitParams.h"
#include "CalRecon/ICalReconSvc.h"

#include <vector>

class CalMomentsData
{
 public:

  enum StatusBits {
    /// All empty!
    ZERO               = 0x0,
    /// Longitudinal position right on the xtal edge (i.e. it was outside the xtal).
    LONG_POS_INVALID   = 0x00000001,
    /// Indicates that the signal in the positive face is saturated.
    POS_FACE_SATURATED = 0x00000010,
    /// Indicates that the signal in the negative face is saturated. 
    NEG_FACE_SATURATED = 0x00000020,
    /// Longitudinal position from the fit available.
    FIT_POS_AVAILABLE  = 0x00000100,
    /// The fit axis is passing close to one of the xtal edges.
    FIT_POS_NEAR_EDGE  = 0x00000200,
    /// Longitudinal position from the fit out of the xtal
    /// (set right on one of the xtal edges when applying the correction).
    FIT_POS_INVALID    = 0x00000400,
    /// Use the corrected longitudinal position (i.e. the value from the fit).
    USE_FIT_POS        = 0x00000800,
    /// Do not use it in the moments analysis (which is not really used, yet).
    MASKED             = 0x00001000,
  };

  /// Constructor from all the parameters.
  //CalMomentsData(const Point& position, const double weight, int tower, int layer, int column);

  /// Convenience constructor from a Event::CalXtalRecData* recData object.
  CalMomentsData(Event::CalXtalRecData* recData);
    
  /// Destructor.
  ~CalMomentsData() {}
    
  /// Provides access to data.
  inline unsigned int getStatusBits()        const { return m_statusBits; }
  inline const Point& getBasePosition()      const { return m_basePosition; }
  inline const Point& getCorrPosition()      const { return m_corrPosition; }
  inline double getWeight()                  const { return m_weight; }
  inline int getTower()                      const { return m_tower; }
  inline int getLayer()                      const { return m_layer; }
  inline int getColumn()                     const { return m_column; }
  inline double getDistToAxis()              const { return m_distToAxis; }
  inline double getCoordAlongAxis()          const { return m_coordAlongAxis; }
  inline double getLongitudinalPosition()    const { return m_longitudinalPosition; }

  /// Provides set functions.
  inline void setBasePosition(const Point& pos)    { m_basePosition = pos; }
  inline void setCorrPosition(const Point& pos)    { m_corrPosition = pos; }
  inline void setWeight(double weight)             { m_weight = weight; }
  inline void setTower(int tower)                  { m_tower = tower; }
  inline void setLayer(int layer)                  { m_layer = layer; }
  inline void setColumn(int column)                { m_column = column; }
  inline void setDistToAxis(double dist)           { m_distToAxis = dist; }
  inline void setCoordAlongAxis(double coord)      { m_coordAlongAxis = coord; }
  inline void setLongitudinalPosition(double pos)  { m_longitudinalPosition = pos; }

  /// Methods to check wheter the underlying xtals lies along the x or y axis.
  bool isx()                                 const { return (m_layer % 2 == 0); }
  bool isy()                                 const { return (m_layer % 2 != 0); }

  /// Manipulate status bits.
  inline void setStatusBit(StatusBits bit)         { m_statusBits |=  bit; }
  //inline void setXtalStatusBit(StatusBits bit)         { m_xtalRecData->setStatusBits(bit); } // really useful?
  inline void clearStatusBit(StatusBits bit)       { m_statusBits &= ~bit; }
  inline bool checkStatusBit(StatusBits bit) const { return ( (m_statusBits & bit) != ZERO ); }
  inline bool posFaceSaturated()             const { return checkStatusBit(POS_FACE_SATURATED); }
  inline bool negFaceSaturated()             const { return checkStatusBit(NEG_FACE_SATURATED); }
  inline bool bothFacesSaturated()           const { return (posFaceSaturated() & negFaceSaturated()); }
  inline bool saturated()                    const { return (posFaceSaturated() | negFaceSaturated()); }
  inline void setPosFaceSaturated()                { setStatusBit(POS_FACE_SATURATED); }
  inline void setNegFaceSaturated()                { setStatusBit(NEG_FACE_SATURATED); }
  inline void setBothFacesSaturated()              { setPosFaceSaturated(); setNegFaceSaturated(); }
  inline void mask()                               { setStatusBit(MASKED); }
  inline void unmask()                             { clearStatusBit(MASKED); }
  inline bool masked()                       const { return checkStatusBit(MASKED); }
 
  /// Apply the fit longitudinal correction, i.e. set (but do not use, yet) all the
  /// data members related to the correction of the the longitudinal position of the crystal
  /// based on the fit of the transverse positions.
  void applyFitCorrection(const Event::CalFitParams fitParams, const ICalReconSvc* calReconSvc);

  /// If the fit correction is available and the corresponding position falls within
  /// the xtal, set the USE_FIT_POS status bit in order to use it.
  void enableFitCorrection();

  /// Set the USE_FIT_POS status bit in order to use it, if the corrected position is
  /// available, regardless of any other circumstance.
  void forceFitCorrection();

  /// Return the "best position" (i.e. use the fit longitudinal position if the
  /// USE_FIT_POS status bit is set).
  const Point& getPosition() const;

  /// Get the value of the fit longitudinal correction (in mm), i.e. the
  /// difference between the fit position and the xtal position along the xtal.
  double getFitCorrAmount() const; 

  /// Determine distance to given axis.
  /// Note that this sets a class member, as well.
  double calcDistToAxis(const Point& centroid, const Vector& axis);

  /// Determine the coordinate along a given axis.
  /// Note that this sets a class member, as well.
  double calcCoordAlongAxis(const Point& centroid, const Vector& axis);

  /// Set the reference centroid/axis for the CAL hit.
  /// For historical reasons the previous two functions calls (calcDistToAxis() and
  /// calcCoordAlongAxis()) are used throughout the code, though it would be probably
  /// neater to use this method, followed by the proper get methods.
  void setReferenceAxis(const Point& centroid, const Vector& axis);

  /// function to get the longitudial position error of this xtal
  /// based on empirical tests with MC allGamma
  /// centroid and axis inputs allow for an optional correction based on xtal distance to axis.
  double getLongitudinalPositionErr() const; 
  double getLongitudinalPositionErr(const Point& centroid, const Vector& axis) const; 

  /// function to get the weight error of this xtal
  /// based on empirical tests with MC allGamma
  double getWeightErr() const;


  /// Define how to sort.
  const bool operator<(const CalMomentsData& right)  const
  {return m_distToAxis < right.getDistToAxis();}

  /// Std output facility.
  std::ostream& fillStream(std::ostream& s) const;
  friend std::ostream& operator<< (std::ostream& s, const CalMomentsData& obj)
  {
    return obj.fillStream(s);
  }

  
 private:
  /// The status bits.
  unsigned int m_statusBits;
  /// The position of this data point.
  Point m_basePosition;
  /// The position after the fit correction of the coordinate along the xtal.
  Point m_corrPosition;
  /// A weight to assign to the point in the moments calculation.
  double m_weight;
  /// The CAL tower this hit belongs to.
  int m_tower;
  /// The CAL layer this hit belongs to.
  int m_layer;
  /// The CAL column this hit belongs to.
  int m_column;
  /// The distance from the "axis" of this point.
  double m_distToAxis;
  /// The position along the "axis" of this point (with sign, used to calculate the skewness).
  double m_coordAlongAxis;
  /// Longitudinal position along xtal, in xtal reference frame (0 at the center)
  double m_longitudinalPosition;

  
  Event::CalXtalRecData *m_xtalRecData;
};


typedef std::vector<CalMomentsData> CalMomentsDataVec;

