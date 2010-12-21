/**
 *
 * File and Version Information:
 *      $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/src/Clustering/CalMomentsAnalysis.cxx,v 1.9 2010/04/23 10:45:45 lbaldini Exp $
 */

#include "src/Clustering/CalMomentsAnalysis.h"

#include <algorithm>


void CalMomentsAnalysis::clear()
{
  m_weightSum        = 0.;
  m_centroid         = Point(0.,0.,0.); 
  m_moment           = Vector(0.,0.,0.);
  m_axis[0]          = Vector(0.,0.,0.);
  m_axis[1]          = Vector(0.,0.,1.);
  m_axis[2]          = Vector(0.,0.,0.);
  m_longRms          = 0.;
  m_transRms         = 0.;
  m_longRmsAsym      = 0.;
  m_longSkewness     = 0.;
  m_numIterations    = 0;
  m_numDroppedPoints = 0;
}

double CalMomentsAnalysis::doMomentsAnalysis(CalMomentsDataVec& dataVec, const Point& iniCentroid)
{
    // The Moments Analysis done here
    // This version lifted directly from code supplied to Bill Atwood by Toby Burnett
    // TU 5/24/2005
    m_weightSum = 0.;
    m_longSkewness = -9999.;
    // Check that we have enough points to proceed - need at least three
    double chisq = -1.;
    if (dataVec.size() < 2) return chisq;
        
    double Ixx       = 0.,  
           Iyy       = 0.,  
           Izz       = 0.,
           Ixy       = 0.,  
           Ixz       = 0.,  
           Iyz       = 0.;
    double weightSum = 0.;
    double skewness  = -9999.;
    Point  centroid(0.,0.,0.);
 
    // Loop through the data points
    for(CalMomentsDataVec::iterator vecIter = dataVec.begin(); vecIter != dataVec.end(); vecIter++)
    {
        // Construct elements of (symmetric) "Inertia" Tensor:
        // See Goldstein, 1965, Chapter 5 (especially, eqs. 5-6,7,22,26).
        // Analysis easy when translated to energy centroid.
        // get pointer to the reconstructed data for given crystal
        const CalMomentsData& dataPoint = *vecIter;

        double  weight = dataPoint.getWeight();
        Vector  hit    = dataPoint.getPoint() - iniCentroid;

        double Rsq  = hit.mag2();
        double xprm = hit.x();
        double yprm = hit.y();
        double zprm = hit.z();

        Ixx += (Rsq - xprm*xprm) * weight;
        Iyy += (Rsq - yprm*yprm) * weight;
        Izz += (Rsq - zprm*zprm) * weight;
        Ixy -= xprm*yprm * weight;
        Ixz -= xprm*zprm * weight;
        Iyz -= yprm*zprm * weight;
	
        weightSum += weight;
        centroid  += weight * dataPoint.getPoint();
    }

    // Render determinant of Inertia Tensor into cubic form.
    double p = - (Ixx + Iyy + Izz);
    double q =   Iyy*Izz + Iyy*Ixx + Izz*Ixx - (Ixy*Ixy + Iyz*Iyz + Ixz*Ixz);
    double r = - Ixx*Iyy*Izz + Ixx*Iyz*Iyz + Iyy*Ixz*Ixz +
                 Izz*Ixy*Ixy - 2.*Ixy*Iyz*Ixz;

    // See CRC's Standard Mathematical Tables (19th edition), pp 103-105.
    // The substitution, y = x - p/3 converts  y^3 + p*y^2 + q*y + r = 0
    // to the form  x^3 + a*x + b = 0 .  Then, if b^2/4 + a^3/27 < 0 ,
    // there will be three real roots -- guaranteed since the Inertia Tensor
    // is symmetric.  A second substitution, x = m*cos(psi) , yields the roots.
    double a = (3.*q - p*p)/3.;
    double b = (2.*p*p*p - 9.*p*q + 27.*r)/27.;

    double rad_test = b*b/4. + a*a*a/27.;

    if ((rad_test < 0.) && (Ixy != 0.) && (Ixz != 0.) && (Iyz != 0.))
    {
        // Update the weight and centroid
        m_weightSum  = weightSum;
        m_centroid   = centroid;
        m_centroid  /= weightSum;

        // Construct the roots, which are the principal moments.
        double m   = 2. * sqrt(-a/3.);
        double psi = acos( 3.*b/(a*m) ) / 3.;

        m_moment[0] = m * cos(psi) - p/3.;
        m_moment[1] = m * cos(psi + 2.*M_PI/3.) - p/3.;
        m_moment[2] = m * cos(psi + 4.*M_PI/3.) - p/3.;

        // Construct direction cosines; dircos for middle root is parallel to
        // longest principal axis.
        for(int iroot=0; iroot < 3; iroot++) 
        {
            double A = Iyz * (Ixx - m_moment[iroot]) - Ixy*Ixz;
            double B = Ixz * (Iyy - m_moment[iroot]) - Ixy*Iyz;
            double C = Ixy * (Izz - m_moment[iroot]) - Ixz*Iyz;

            double D = sqrt( 1. / ( 1./(A*A) + 1./(B*B) + 1./(C*C) ) ) / C;

            m_axis[iroot] = Vector(D*C/A, D*C/B, D);

            // Set axis to "point up"
            if (m_axis[iroot].z() < 0.) m_axis[iroot] = -m_axis[iroot];
        }

        // "Chi-squared" = sum of residuals about principal axis, through centroid, using input weight
        chisq = 0.;
	// Skewness = third moment along the main axis w.r.t. the energy centroid.
	skewness = 0.;
        for(CalMomentsDataVec::iterator vecIter = dataVec.begin(); vecIter != dataVec.end(); vecIter++)
        {
            CalMomentsData& dataPoint = *vecIter;

            double distToAxis = dataPoint.calcDistToAxis(m_centroid,m_axis[1]);
	    double coordAlongAxis = dataPoint.calcCoordAlongAxis(m_centroid,m_axis[1]);

            chisq += dataPoint.getWeight() * distToAxis * distToAxis;
	    skewness += dataPoint.getWeight() * coordAlongAxis * coordAlongAxis * coordAlongAxis;
        }

        // Scale by number of data points
        chisq /= weightSum * dataVec.size();

        // Final calculations to return moment of principal axis and average of other two
        double longMag1 = fabs(m_moment[0]);
	double longMag2 = fabs(m_moment[2]); 
	
        m_longRms     = (longMag1 + longMag2) / 2.;
	m_transRms    =  fabs(m_moment[1]);
	m_longRmsAsym = (longMag1 - longMag2)/(longMag1 + longMag2);
	m_longSkewness = skewness;
    }
    else chisq = -1.;

    return chisq;
}

double CalMomentsAnalysis::doIterativeMomentsAnalysis(CalMomentsDataVec dataVec, const Point& inputCentroid, double scaleFactor)
{
    double chiSq   = -1.;
    bool   iterate = true;

    m_numIterations    = 0;
    m_numDroppedPoints = 0;

    // Set the data centroid
    Point centroid = inputCentroid;

    // Iterate until either failure (chiSq < 0) or all remaining data points 
    // are within "range" of axis
    while(iterate)
    {
        // Do the standard moments analysis
        double localChiSq = doMomentsAnalysis(dataVec, centroid);

        // Make sure it didn't fail on this iteration
        if (localChiSq < 0.) 
        {
            break;
        }

        // Update global chi-square to pick up this iteration's value
        chiSq = localChiSq;

        // Update the centroid for subsequent passes
        centroid = getCentroid();

        // Get the transverse moment
        double transRms = getTransRms();

        // Convert to distance by scaling with the sum of the weights 
        // and taking the square root
        transRms = sqrt(transRms / m_weightSum);

        // Sort the data points by their distance from the principal axis
        std::sort(dataVec.begin(), dataVec.end());

        // Assume all data within range
        iterate = false;

        m_numIterations++;

        // Check the last element in the now sorted list of data points
        // and see if it is out of range
        while(!dataVec.empty())
        {
            CalMomentsData& momentsData = dataVec.back();

            // If out of range drop this point and loop back to check again
            if (momentsData.getDistToAxis() > scaleFactor * transRms)
            {
                dataVec.pop_back();
                iterate = true;
                m_numDroppedPoints++;
            }
            else break;
        }

        // Make it harder to drop points on each iteration by boosting the scale factor
        scaleFactor *= 2.;

        // Finally, make sure have enough points remaining to proceed!
        if (dataVec.size() < 3) break;
    }

    return chiSq;
}

double CalMomentsData::calcDistToAxis(const Point& centroid, const Vector& axis)
{
    Vector diffVec   = centroid - m_point;
    Vector crossProd = axis.cross(diffVec);

    return m_distToAxis = crossProd.mag();
}

double CalMomentsData::calcCoordAlongAxis(const Point& centroid, const Vector& axis)
{
  // We explicitly assume that the axis is normalized, here.
  return m_coordAlongAxis = (m_point - centroid).dot(axis);
}
