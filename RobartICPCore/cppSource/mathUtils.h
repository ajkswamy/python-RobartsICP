#ifndef __MATHUTILS_H__
#define __MATHUTILS_H__

#include "matrix.h"


// 
// determinant of a 3x3 matrix
//
double m3x3_det( Matrix<double> &m );


// convert a quaternion to 3x3 rotation matrix
//
// a quaternion is defined as:
//
// q[0] = v1*sin(phi/2)
// q[1] = v2*sin(phi/2)
// q[2] = v3*sin(phi/2)
// q[3] =    cos(phi/2)
void q2m3x3( Vec<double> &qin, Matrix<double> &m );

//
// generate 40 rotations that includes the tetrahedral and
// the octahedral/hexahedral group as per the original ICP paper
// by Besl and McKay (page 247)
//
//
// q is a 4x40 quaterion matrix where each column of q is an unit quaternion
void FourtyRotations( Matrix<double> &q );
void SixtyRotations( Matrix<double> &q );

//
// find the closest points using Mahalanobis distance
//
// X, Y are point clouds using column vectors
// S is the covariance matrix, and in this particular case, diagonal
// out is the closest points of X in Y, hence out has the same dimension as X
void closestPoint_with_MahalanobisDistance( Matrix<double> &X,
                                            Matrix<double> &Y,
                                            Matrix<double> &S,
                                            Matrix<double> &out );
void closestPoint_with_EuclideanDistance( Matrix<double> &X,
                                          Matrix<double> &Y,
                                          Matrix<double> &out );

void calFREMag( Matrix<double> &X,
                Matrix<double> &Y,
                Vec<double> &FREMag );

//
// ordering of 3 numbers
//
// using the box trick
//
void ordering3Numbers( double a, double b, double c,
                       double &min, double &mid, double &max );

void estimateScalesFromPoints( Matrix<double> &p, Matrix<double> &m,
			       double &initScale,
			       Matrix<double> &initR );
#endif // of __MATHUTILS_H__