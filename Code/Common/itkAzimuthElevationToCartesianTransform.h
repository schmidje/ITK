/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkAzimuthElevationToCartesianTransform.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#ifndef __itkAzimuthElevationToCartesianTransform_h
#define __itkAzimuthElevationToCartesianTransform_h

#include "itkAffineTransform.h"
#include "vnl/vnl_math.h"



/**
 * Transforms from an azimuth, elevation, radius coordinate system to 
 * a cartesian coordinate system, or vice versa.
 *
 * The three coordinate axis are azimuth, elevation, and range.
 *
 * The azimuth elevation coordinate system is defined similarly to spherical 
 * coordinates but is slightly different in that the azimuth and elevation are
 * measured in degrees between the r-axis (i.e z axis) and the projection on 
 * the x-z and y-z planes, respectively.  Range, or r, is the distance from 
 * the origin.
 * 
 * The equations form performing the conversion from azimuth-elevation 
 * coordinates to cartesian coordinates are as follows:
 * z = sqrt((r^2*(cos(azimuth))^2)/(1 + (cos(azimuth))^2 * (tan(elevation))^2);
 * x = z * tan(azimuth)
 * y = z * tan(elevation)
 *
 * The reversed transforms are:
 * azimuth = arctan(x/y)
 * elevation = arctan(y/z)
 * r = sqrt(x^2 + y^2 + z^2)
 *
 * In this class, we can also set what a "forward" transform means.  If we call
 * SetForwardAzimuthElevationToCartesian(), a forward transform will return
 * cartesian coordinates when passed azimuth,elevation,r coordinates.  Calling
 * SetForwardCartesianToAzimuthElevation() will cause the forward transform
 * to return azimuth,elevation,r coordinates from cartesian coordinates.
 *
 * Setting the FirstSampleDistance to a non-zero value means that a r value
 * of 12 is actually (12 + FirstSampleDistance) distance from the origin.
 *
 * There are two template parameters for this class:
 *
 * ScalarT       The type to be used for scalar numeric values.  Either
 *               float or double.
 *
 * NDimensions   The number of dimensions of the vector space (must be >=3).
 *
 * \ingroup Transforms
 *
 * \todo Is there any real value in allowing the user to template
 * over the scalar type?  Perhaps it should always be double, unless
 * there's a compatibility problem with the Point class.
 *
 *
 * \todo  Derive this class from a yet undefined TransformBase class.  
 *        Currently, this class derives from AffineTransform, although
 *        it is not an affine transform.
 *
 *
 *
 **/
 
namespace itk
{


template < 
  class TScalarType=double,         // Data type for scalars (e.g. float or double)
  unsigned int NDimensions=3,       // Number of dimensions in the input space
  class TParameters = Point< double, NDimensions*(NDimensions+1) >,
  class TJacobianType = Matrix<double,NDimensions,NDimensions*(NDimensions+1) >
  > 
class AzimuthElevationToCartesianTransform : public AffineTransform< TScalarType,
                                              NDimensions,
                                              TParameters,
                                              TJacobianType >
{

public:

    /**
     * Standard Self Typedef
     */
    typedef AzimuthElevationToCartesianTransform  Self;

    /// Dimension of the domain space
    enum { SpaceDimension = NDimensions,
           ParametersDimension = NDimensions * (NDimensions+1) };


    /**
     * Standard "Superclass" typedef.
     */
    typedef AffineTransform<  TScalarType, NDimensions,
                              TParameters, TJacobianType >  Superclass;


    /** 
     * Smart pointer typedef support 
     */
    typedef SmartPointer<Self>        Pointer;
    typedef SmartPointer<const Self>  ConstPointer;


    /** 
     * Run-time type information (and related methods).
     */
    itkTypeMacro( AzimuthElevationToCartesianTransform, AffineTransform);


    /** 
     * New macro for creation of through a Smart Pointer
     */
    itkNewMacro( Self );



    /**
     * Parameters Type
     */
    typedef typename Superclass::ParametersType  ParametersType;


    /**
     * Jacobian Type
     */
    typedef typename Superclass::JacobianType  JacobianType;


    /// Standard scalar type for this class
    typedef typename Superclass::ScalarType ScalarType;


    /**
     * Standard coordinate point type for this class
     */
    typedef  typename Superclass::InputPointType     InputPointType;
    typedef  typename Superclass::OutputPointType    OutputPointType;

    
    /**
     * Standard matrix type for this class
     */
    typedef Matrix<TScalarType, SpaceDimension, SpaceDimension> MatrixType;


    /**
     * Set the transformation parameters
     *
     **/
    void SetAzimuthElevationToCartesianParameters( const double sampleSize, 
                            const double blanking,
                            const long maxAzimuth, 
                            const long maxElevation, 
                            const double azimuthAngleSeparation,
                            const double elevationAngleSeparation);

    void SetAzimuthElevationToCartesianParameters( const double sampleSize, 
                            const double blanking,
                            const long maxAzimuth, 
                            const long maxElevation);


    /**
     * Transform from azimuth-elevation to cartesian
     *
     **/
    OutputPointType     TransformPoint (const InputPointType  &point ) const;

    /**
     * Back transform from cartesian to azimuth-elevation
     *
     **/
    inline InputPointType      BackTransform(const OutputPointType  &point ) const;
    InputPointType  BackTransformPoint(const OutputPointType  &point) const;

    /**
     * Print contents of an AzimuthElevationTransform
     **/
    void PrintSelf(std::ostream &s, Indent indent) const;
    
    /**
     * Defines that the forward transform goes from azimuth,elevation to cartesian
     **/
    void SetForwardAzimuthElevationToCartesian();

    /**
     * Defines that the forward transform goes from cartesian to azimuth,elevation
     **/
    void SetForwardCartesianToAzimuthElevation();

    /**
     * Perform conversion from Azimuth Elevation coordinates to Cartesian Coordinates
     **/
    OutputPointType TransformAzElToCartesian(const InputPointType &point) const; 

    /**
     * Perform conversion from Cartesian Coordinates to Azimuth Elevation coordinates 
     **/
    OutputPointType TransformCartesianToAzEl(const OutputPointType &point) const;

    /**
     *  Set the maximum azimuth
     *  The maximum azimuth and elevation can be set so that the resulting 
     *  cartesian space is symmetric about the z axis.  Therefore, the line 
     *  defined by azimuth/2,elevation/2 = z-axis.  
     **/
    itkSetMacro(MaxAzimuth, long);

    /**
     *  Set the maximum elevation
     *  The maximum azimuth and elevation can be set so that the resulting 
     *  cartesian space is symmetric about the z axis.  Therefore, the line 
     *  defined by azimuth/2,elevation/2 = z-axis.  
     **/
    itkSetMacro(MaxElevation, long);

    /**
     *  Set the number of cartesian units between each unit along the R
     **/
    itkSetMacro(RadiusSampleSize, double);

    /**
     *  Set the number of degrees between each azimuth unit
     **/
    itkSetMacro(AzimuthAngularSeparation, double);

    /**
     *  Set the number of degrees between each elevation unit
     **/
    itkSetMacro(ElevationAngularSeparation, double);

    itkSetMacro(FirstSampleDistance, double);
protected:

    /**
     * Create an AzimuthElevationToCartesianTransform object
     **/
    AzimuthElevationToCartesianTransform();      

    /**
     * Destroy an AzimuthElevationToCartesianTransform object
     **/
    virtual ~AzimuthElevationToCartesianTransform();


private:

    long    m_MaxAzimuth;
    long    m_MaxElevation;
    double  m_RadiusSampleSize;
    double  m_AzimuthAngularSeparation;
    double  m_ElevationAngularSeparation;
    double  m_FirstSampleDistance;
    bool    m_ForwardAzimuthElevationToPhysical;
}; //class AzimuthElevationToCartesianTransform


}  // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAzimuthElevationToCartesianTransform.txx"
#endif

#endif /* __itkAzimuthElevationToCartesianTransform_h */


