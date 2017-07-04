#pragma once
#ifndef interpolator_h
#define interpolator_h

#include <iostream>
#include <valarray>
#include <fstream>
#include <istream>
#include <ostream>
#include "./types.h"



using namespace std;









class interpolator{
private :
	valarray<Real> _x;
	valarray<Real> _y;


	valarray<Real> _x1;
	valarray<Real> _x2;
	valarray<valarray<Real> > _ymat;


public :
	/** Default Constructor */
	interpolator();
	
	/** Constructor for 1 dimension
	@param x: array of points
	@param y: array, f(x)
	*/
	interpolator(valarray<Real> x, valarray<Real> y);

	/** Constructor for 2 dimensions
	@param x1: array of first components of points
	@param x2: array of second components of points
	@param y: matrix, f(x1, x2)
	*/
	interpolator(valarray<Real> x1, valarray<Real> x2, valarray<valarray<Real> > ymat);

	/** Interpolate for point x in dimension 1 */
	Real interpolate(Real x);
	
	/** Interpolate for point (x1, x2) in dimension 2 */
	Real interpolate(Real x1, Real x2);

	/** Interpolate for all points x in vec in dimension 1 */
	valarray<Real> interpolate(valarray<Real> vec);
	
	/** Interpolate for all points (x1, x2) in (vec1, vec2) in dimension 2 */
	valarray<valarray<Real> > interpolate(valarray<Real> vec1, valarray<Real> vec2);

	Real getInterpolation(valarray<Real> xa,valarray<Real> ya, Real x);

	Integer getPlace(Real x);
	Integer getPlaceOnXi(Real x, Integer i);
};


#endif