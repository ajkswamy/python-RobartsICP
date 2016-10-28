// C++ includes
#include <cfloat>
#include <cmath>
#include <iostream>
#include <assert.h>


// matrix include
#include "matrix.h"
#include "svd.h"

// local include
#include "OPP.h"
#include "mathUtils.h"


//
// solves for the Orthogonal Procrustes Analysis using
// the SVD solution
void point_register( Matrix<double> &l, Matrix<double> &r,
			         Matrix<double> &R,
			         Vec<double> &t,
			         double &FRE, double threshold,
                     Vec<double> &FREMag )
  {
  
  int lx = l.num_rows();
  int ly = l.num_cols();
  
  int rx = r.num_rows();
  int ry = r.num_cols();

  assert( lx == 3 );
  assert( rx == 3 ); // make sure we are using column vectors
  assert( ly == ry ); // make sure we have equal number of points

  // find the centroids
  Vec<double> cl( 3, 0.0 ), cr( 3, 0.0 );
  for ( int i = 0; i < ly; i++ )
    {
    for ( int j = 0; j < 3; j++ )
      {
      cl[j] += l[j][i];
      cr[j] += r[j][i];
      } // j
    } // i

  for ( int i = 0; i < 3; i++ )
    {
    cl[i] /= (double)ly;
    cr[i] /= (double)ry;
    }


  // translate the input points by their weighted centroids
  //
  Matrix<double> lprime( 3, ly ), rprime( 3, ry );
  for ( int i = 0; i < ly; i++ ) 
    {
    for ( int j = 0; j < 3; j++ ) 
      {
      lprime[j][i] = l[j][i] - cl[j];
      rprime[j][i] = r[j][i] - cr[j];
      } // j
    } // i

  //
  // calculate the cross covariance matrix
  //
  Matrix<double> H = lprime * transpose( rprime );

  //
  // compute the SVD
  //
  Matrix<double> U, V;
  Vec<double> S;
  svdcmp( H, S, U, V );
  
  Matrix<double> dd = eye(3,1.0), VU = V * U;
  dd[2][2] = m3x3_det( VU );

  //
  // and the rotation is:
  //
  R = V * dd * transpose( U );
  
  //
  // and the translation is:
  //
  t = cr - R*cl;
  
  Matrix<double> FREvect;
  FREvect = rprime - R * lprime;
  FRE = 0.0;
  
  if ( FREMag.size() != FREvect.num_cols() )
    { 
    FREMag.newsize( FREvect.num_cols() );
    }

  for ( int i = 0; i < FREvect.num_cols(); i++ )
    {
    FREMag[i] = sqrt( FREvect[0][i] * FREvect[0][i] +
                      FREvect[1][i] * FREvect[1][i] +
                      FREvect[2][i] * FREvect[2][i] );
    FRE += FREMag[i];
    } // i

  FRE = ( FRE/(double)FREvect.num_cols() );
  }