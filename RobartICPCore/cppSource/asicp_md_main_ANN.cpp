// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <cfloat>

// local includes
#include "matrix.h"
#include "mathUtils.h"
#include "asicp_md_ann.h"

int main( int argc, char *argv[] )
  {
  
  std::cerr << std::endl
    << "The Iterative Closest Point with Anisotropic Scaling (ASICP)" << std::endl
    << "using Mahalanobis Distance" << std::endl << std::endl;

  if ( argc < 2 )
    {
    std::cerr << "USAGE: " << argv[0]
              << " <points.txt> <model.txt> <initT -- optional> " << std::endl
              << std::endl << std::endl
              << "file format is given as: " << std::endl << std::endl
              << "n" << std::endl
              << "x1 y1 z1" << std::endl
              << "x1 y2 z2" << std::endl
              << "..." << std::endl
              << "xn yn zn" << std::endl;

    exit(1);
    }
  
  double m[16];
  Matrix<double> transform(4,4);;
  bool hasInitTransform = false;
  if ( argc > 3 )
    {
    std::cerr << "reading init" << std::endl;
    std::ifstream tfile;
    tfile.open( argv[3] );

    if ( !tfile.is_open() )
      {
      std::cerr << argv[1] << " not opened" << std::endl;
      exit(1);
      }
    for ( int i = 0; i < 16; i++ )
      {
      tfile >> m[i];
      }
    
    int count=0;
    for ( int i = 0; i < 4; i++ )
      for ( int j = 0; j < 4; j++ )
        transform[i][j] = m[count++];
    
    tfile.close();
    hasInitTransform = true;

    }
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

  // initial rotation
  double FRE = 0.0, tau = 1e-9;
  Matrix<double> A( 3, 3, 0.0 ); // scaling
  Matrix<double> R( 3, 3 );      // rotation
  Vec<double> t(3);              // translation

  Matrix<double> initialQuaternions; // rotation group
  FourtyRotations( initialQuaternions ); // fourty uniformally sampled rotations

  // loop through all the rotation group
  Vec<double> quat(4), minT(3);
  double minRMS = 0.0;
  Matrix<double> minA(3,3), minR(3,3);
  Vec<double> FREMag, minFREMag;
  Matrix<double> initQ(3,3), minInitQ(3,3);

  if ( hasInitTransform )
    { // run only 1 iteration of ICP based on the initial transform
    for ( int i = 0; i < 3; i++ )
      {
      for ( int j = 0; j < 3; j++ )
        {
        R[i][j] = transform[i][j];
        } // j
      t[i] = transform[i][3];
      } // i
    std::cerr << "using initial transform" << std::endl;
    asicp_md( points, models, R, A, t, FRE, tau, FREMag );
    
        minRMS = FRE;
        minR = R;
        minA = A;
        minT = t;
        minFREMag = FREMag;
    }
  else
    {
    // go through the rotation group
    for ( int i = 0; i < initialQuaternions.num_cols(); i++ )
      {
      // go through all the rotations
      quat[0] = initialQuaternions[0][i];
      quat[1] = initialQuaternions[1][i];
      quat[2] = initialQuaternions[2][i];
      quat[3] = initialQuaternions[3][i];
      t[0] = t[1] = t[2] = 0.0;
      q2m3x3( quat, R ); // initial guess on rotation
      initQ = R;
      A = eye( 3, 1.0 ); // initial guess on scaling
      // translation does not matter much


      asicp_md( points, models, R, A, t, FRE, tau, FREMag );


      std::cerr << i << " FRE: " << minRMS << std::endl
        << R << A << t << std::endl;
      if ( i == 0 )
        { 
        minRMS = FRE;
        minR = R;
        minA = A;
        minT = t;
        minFREMag = FREMag;
        minInitQ = initQ;
        }

      if ( FRE < minRMS )
        {
        minRMS = FRE;
        minR = R;
        minA = A;
        minT = t;
        minFREMag = FREMag;
        minInitQ = initQ;
        }
      }

    }
  


  std::cerr << "Final answer: " << minInitQ << minR << minA << minT << minFREMag[minFREMag.size() - 1]<< std::endl;

  
//  std::ofstream myfile;
//  myfile.open( "test.txt" );
//  std::cerr << minFREMag.size() << std::endl;
//  for ( int i = 0; i < minFREMag.size(); i++ )
//    myfile << minFREMag[i] << " ";
//  myfile << std::endl;
//  myfile.close();
  return(0);
  }
