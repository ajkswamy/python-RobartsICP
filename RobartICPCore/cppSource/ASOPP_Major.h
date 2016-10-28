/*
 * FILE: ASOPP_Major.h
 *
 * Elvis C.S. Chen
 * chene@robarts.ca
 *
 * January, 17, 2014
 *
 * This function implements the PRESCALING ANISOTROPIC Orthogonal Procrustes
 * Analysis algorithm based on the Majorization Principal algorithm presented
 * by Mohammed Bennani Dosse and Jos Ten Berge:
 *
 *
 *  @Article{springerlink:10.1007/s00357-010-9046-8,
 *    Author         = {Bennani Dosse, Mohammed and Ten Berge, Jos},
 *    Title          = {Anisotropic Orthogonal Procrustes Analysis},
 *    Journal        = {Journal of Classification},
 *    Volume         = {27},
 *    Pages          = {111-128},
 *    Note           = {10.1007/s00357-010-9046-8},
 *    affiliation    = {University of Rennes 2, Place du Recteur Henri Le
 *                     Moal, CS 24307 35043 Rennes Cedex, France},
 *    issn           = {0176-4268},
 *    issue          = {1},
 *    keyword        = {Computer Science},
 *    publisher      = {Springer New York},
 *    url            = {http://dx.doi.org/10.1007/s00357-010-9046-8},
 *    year           = 2010
 *  }
 *
 * We use FRE as the stopping criteria
 *
 *
 *
 *
 * The BR algorithm basically is a loop that solves for Rotation
 * and scaling independently in each iteration.  We use FRE as the
 * stopping criteria.
 *
 *
 * X and Y are array of 3D points where
 *
 *     size(X,2) = size(Y,2), and
 *     size(X,1) = size(Y,1) = 3
 *
 *
 * optional inputs:
 *     threshold -- the stopping criteria based on FRE
 *                  defaults to 1e-9
 *
 *
 * OUTPUTS:
 *     R, the orthonormal (rotation), 3x3
 *     A, the anisotropic scaling , 3x3
 *     t, the translation, 3x1
 *     FRE, Fiducial Registration Error, the residual
 *
 * We attempt to solve for the PRESCALING (scaling before rotation) case:
 *
 *     Y = R * A * X + t
 *
 * Based on my original Matlab code written on Oct. 22nd, 2010
 *
 */
#ifndef __ASOPP_MAJOR_H__
#define __ASOPP_MAJOR_H__

#include "matrix.h"

void ASMajor_point_register( Matrix<double> &x, Matrix<double> &y,
			     Matrix<double> &Q,
			     Matrix<double> &A,
			     Vec<double> &t,
			     double &FRE, double threshold,
                 Vec<double> &FREMag );



// with the default threshold set to 1e-9
void ASMajor_point_register( Matrix<double> &x, Matrix<double> &y,
                             Matrix<double> &Q,
                             Matrix<double> &A,
                             Vec<double> &t,
                             double &FRE,
                             Vec<double> &FREMag );


#endif // of __ASOPP_MAJOR_H__
