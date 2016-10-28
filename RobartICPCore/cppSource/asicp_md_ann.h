#ifndef __ASICP_MD_H__
#define __ASICP_MD_H__

#include "matrix.h"

void asicp_md( Matrix<double> &points,             
              Matrix<double> &model,
              Matrix<double> &R,
              Matrix<double> &A,
              Vec<double> &t,
              double &FRE, double threshold,
              Vec<double> &FREMag );

#endif // of __ASICP_MD_H__