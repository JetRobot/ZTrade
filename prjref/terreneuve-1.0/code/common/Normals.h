#pragma once
#ifndef NORMALS_H
#define NORMALS_H

#include <valarray>
#include "./types.h"
#include "./utils.h"
using namespace std;

Real NormalDensity(Real x);

Real CumulativeNormal(Real x);

Real InverseCumulativeNormal(Real x);

Real Average(valarray<Real> Ptr,LongNatural dim);

Real Maximize(valarray<Real> Ptr,LongNatural dim);

/** Bivariate normal distribution - from Hull's book
@author Yann*/ 
Real CumulativeBivariateNormal(Real a,Real b,Real rho);
Real SubFunctionForBivariateNormal(Real X,Real y,Real ap,Real bp,Real rho);

#endif
