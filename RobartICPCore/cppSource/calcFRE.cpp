
// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <cfloat>

// local includes
#include "matrix.h"


// ANN includes
#include <ANN/ANN.h>


void calFREMag( Matrix<double> &X,
               Matrix<double> &Y,
               Vec<double> &FREMag )
  {
  int Xx = X.num_rows();
  int Xy = X.num_cols();
  int Yx = Y.num_rows();
  int Yy = Y.num_cols();

  assert( Xx == Yx ); // make sure we are using column vectors and in 3D
  assert( Xx == 3 );
  assert( Xy == Yy ); // make sure we have homologous points

  if ( FREMag.size() != Xy )
    {
    FREMag.newsize( Xy );
    }

  double d;
  for ( int i = 0; i < Xy; i++ )
    {
    d = 0.0;
    for ( int j = 0; j < Xx; j++ )
      {
      d += ( X[j][i] - Y[j][i] ) * ( X[j][i] - Y[j][i] );
      } // j
    FREMag[i] = sqrt(d);
    }//i

  }

int main( int argc, char *argv[] )
  {
  // space allocation
  int nPoints, nModels;
  std::ifstream model_file;
  std::ifstream point_file;

  point_file.open( argv[1] );
  model_file.open( argv[2] );

  if ( !model_file.is_open() )
    {
    std::cerr << argv[1] << " not opened" << std::endl;
    exit(1);
    }

  if ( !point_file.is_open() )
    {
    std::cerr << argv[2] << " not opened" << std::endl;
    exit(1);
    }

  /*
  * Parse the input point clouds
  */
  model_file >> nModels;
  point_file >> nPoints;


  Matrix<double> models( 3, nModels ), points( 3, nPoints );

  for ( int i = 0; i < nModels; i++ )
    {
    model_file >> models[0][i]
    >> models[1][i]
    >> models[2][i];
    }
  for ( int i = 0; i < nPoints; i++ )
    {
    point_file >> points[0][i]
    >> points[1][i]
    >> points[2][i];

    }

  // close the files are they are not needed anymore
  model_file.close();
  point_file.close();

  std::cerr << "Finished reading files" << std::endl;

  // apply transform
  

  std::ifstream file;
  file.open( argv[3] );
  if ( !file.is_open() )
    {
    exit(1);
    }
  double m[16];
  for ( int i = 0; i < 16; i++ )
    {
    file >> m[i];
    }
  file.close();
  Matrix<double> Transform(4,4);
  int count = 0;
  for ( int i = 0; i < 4; i++ )
    {
    for ( int j = 0; j < 4; j++ )
      {
      Transform[i][j] = m[count++];
      }
    }
  std::cerr << "Transform: " << Transform;

  Matrix<double> pointsHomogeneous( 4, nPoints ), pointsTransformed;
  for ( int i = 0; i < nPoints; i++ )
    {
    for ( int j = 0; j < 3; j++ )
      {
      pointsHomogeneous[j][i] = points[j][i];
      } // j
    pointsHomogeneous[3][i] = 1.0;
    } // i
  
  std::cerr << "Done Transformation1" << std::endl;
  pointsTransformed = Transform * pointsHomogeneous;
  for ( int i = 0; i < nPoints; i++ )
    {
    for ( int j = 0; j < 3; j++ )
      {
      points[j][i] = pointsTransformed[j][i];
      }
    }
  std::cerr << "Done Transformation" << std::endl;

  
  // build a KD-tree of the "model"
  ANNpointArray data_pts;   // data (model) points
  ANNpoint query_pt;        // query point
  ANNidxArray nn_idx;       // near neighbour indices
  ANNdistArray dists;       // near neighbour distances
  ANNkd_tree *model_tree;   // search structure (KD-tree)
  int ANNk = 1;             // find 1 near neighbor
  int ANNdim = 3;           // dimension
  double ANNeps = 0.0;      // exact near neighbor

  query_pt = annAllocPt( ANNdim );
  data_pts = annAllocPts( nModels, ANNdim );
  nn_idx = new ANNidx[ ANNk ];
  dists = new ANNdist[ ANNk ];



  //
  // copy all the model points into ANN structure
  //
  for ( int i = 0; i < nModels; i++ )
    {
    for ( int j = 0; j < 3; j++ ) 
      {
      data_pts[i][j] = models[j][i];
      }
    }
  

  //
  // initialize the KD tree
  //
  model_tree = new ANNkd_tree( data_pts, nModels, ANNdim );

  Matrix<double> Y(3,nPoints);


  //
  // initialize the KD tree
  //
  model_tree = new ANNkd_tree( data_pts, nModels, ANNdim );

  // find the nearest neighbour of each point in 'l'
  for ( int i = 0; i < nPoints; i++ )
    {
    for ( int j = 0; j < 3; j++ )
      {
      query_pt[j] = points[j][i];
      } // j

    model_tree->annkSearch( query_pt, ANNk, nn_idx, dists, ANNeps );
    for ( int j = 0; j < 3; j++ )
      {
      Y[j][i] = data_pts[ nn_idx[0] ][j];
      }
    } // i


  Vec<double> FREMag;
  calFREMag( points,
    Y,
    FREMag );

  std::ofstream myfile;
  myfile.open( "test.txt" );
  std::cerr << FREMag.size() << std::endl;
  for ( int i = 0; i < FREMag.size(); i++ )
    myfile << FREMag[i] << " ";
  myfile << std::endl;
  myfile.close();
  return(0);
  }