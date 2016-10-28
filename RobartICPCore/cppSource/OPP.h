#ifndef __OPP_H__
#define __OPP_H__

#include "matrix.h"

void point_register( Matrix<double> &x, Matrix<double> &y,
			         Matrix<double> &Q,
			         Vec<double> &t,
			         double &FRE, double threshold,
                     Vec<double> &FREMag );

#endif // of__OPP_H__