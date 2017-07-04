#ifndef _utils_h_
#define _utils_h_

#include "types.h"
#include "matrix.h"
#include <valarray>
#include <iostream>
#include <string>
using namespace std;

#define DEFAULT_PRECISION 0.00001

/**
sign of a Real
@author Yann 
*/
short int sign(Real x);

/**
Transforms a 1D array into a column vector - easier to handle our structures and do operations on them
*/
Matrix transform1DvalarrayToColumnMatrix(valarray<Real> array);

/**
Transforms a 2D array into a matrix 
*/
Matrix transform2DvalarrayToMatrix(valarray<valarray<Real> > array);

/**
Transforms a column matrix to a 1D array
*/
valarray<Real> transformColumnMatrixTo1Dvalarray(Matrix M);

/**
Transforms a normal matrix to a 2D array
*/
valarray<valarray<Real> > transformMatrixTo2Dvalarray(Matrix M);

/**
@author Aloke
*/

valarray<Real> mergeunique(valarray<Real> a1, valarray<Real> a2);
void ErrorMsg(string str, bool mustexit = false);
int getint(istream& istr);

bool 
realsEqual(Real realOne, Real realTwo, Real precision = 0.00001);

string valarrayRealToString(const valarray<Real> &theArray);
#endif