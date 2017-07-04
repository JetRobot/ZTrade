#pragma once
#ifndef volsurface_h
#define volsurface_h

#include "../common/types.h"
#include "../common/date.h"
#include "../common/interpolator.h" 
#include <valarray>
#include <cmath> // c library of math functions
#include "../PartB/yieldCurve.h"
#include "../PartA/BlackScholes/BlackScholes.h"

#ifndef PI
#define PI 3.141592653589793238462643
#endif

using namespace std;

class volsurfaceparams {
public:
	valarray<Real> strikes;
	valarray<Date> maturities;
	valarray<valarray<Real> > callputprices;
	valarray<valarray<bool> > iscallputprices;
};

class volsurface{
      
private :
  Real _stockPrice;
  Date _today;
  valarray<Real> _strikes;
  valarray<Date> _maturities;
  yieldCurve _yieldCurve;
  valarray<valarray<Real> > _callputprices;
  valarray<valarray<bool> > _iscallputprices;
  valarray<valarray<Real> > _impliedvolsurface;
  interpolator _interpolvolsurf;
  bool _volsurfconst;
  Real _constantvol;
  
public : 

	/** Constructor
	@param stockPrice: Current stock price
	@param today: date of today
	@param strikes: vectror of strikes
	@param maturities: vector of maturities
	@param yCurve: the yield curve
	@param callputprices: matrix of option prices
	@param iscallputprices: matrix indicating if we have call or put prices
	*/
  volsurface(Real stockPrice, Date today, valarray<Real> strikes,  valarray<Date> maturities, yieldCurve yCurve, valarray<valarray<Real> > callputprices, valarray<valarray<bool> > iscallputprices);
  
	/** Constructor
	@param stockPrice: Current stock price
	@param today: date of today
	@param yCurve: the yield curve
	@param params: strikes, maturities, prices, etc.
	*/
  volsurface(Real stockPrice, Date today, yieldCurve yCurve, volsurfaceparams &params);
  
  	/** Constructor
	@param volsurf: volsurface
	*/
  volsurface(valarray<valarray<Real> > volsurf);
  
  
  /** Default Constructor */
  volsurface(Real constantvol);

  /** Default Constructor */
  volsurface(void);

  /** Destructor */
  ~volsurface(void);
  
  Real invertBSformula(Real r, Real maturity, Real stockPrice, Real strike, Real callputPrice, bool isacall);
  Real volatility(Real K, Date T);
  Real variance(Real K, Date T);
  Real forwardVolatility(Real K, Date t, Date T);
  void setvolsurface();
  valarray<valarray<Real> > getvolsurface();


  /** Implied volatility surface seen at time t */
  volsurface forwardvolsurface(Date t);

  /** Implied volatility surface when the yield curve is shifted */
  volsurface shiftedYCvolsurface(Real shift);

  /** Shift the implied volatility surface*/
  volsurface shiftedvolsurface(Real shift);
};
#endif

