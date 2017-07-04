#pragma once
#ifndef testObjects_h
#define testObjects_h

#include <string>
#include "../PartB/yieldCurve.h"
#include "../PartE/volsurface.h"
using namespace std;

/** test the date class 
    @returns true if pass, otherwise fail
	*/
bool
maindate(void);

/** test the montecarlo option pricer functionality
    @returns true if pass, otherwise fail
	*/
bool 
mainmontecarlo(void);

/** main routine which runs the montecarlo pricer */
double 
mainmc(double Expiry, double Strike, double Spot, volsurface* Vol,
	   yieldCurve* r,unsigned long nPaths,unsigned long nDates,int PrdName);

/** test the BS option pricing functionality
    @returns true if pass, otherwise fail
	*/
bool
mainoption(void);

/** test the option strategy functionality
    @returns true if pass, otherwise fail
	*/
bool
mainoptionstrategy(void);

/** test the matrix functions
    @returns true if pass, otherwise fail
	*/
bool
mainmatrix(void);

/** test the 2d/3d interpolator functionality
    @returns true if pass, otherwise fail
	*/
bool
maininterpolator(void);

/** test the yield curve functionality
    @returns true if pass, otherwise fail
	*/
bool
mainyieldcurve(void);

/** test the asset functionality
    @returns true if pass, otherwise fail
	*/
bool
mainasset(void);

/** test the file reader functionality
    @returns true if pass, otherwise fail
	*/
bool 
mainfilereader(void);

/** file reader basic test
	@returns true if pass, otherwise fail
	*/
bool
fr_basic(void);

/** test the volsurface functionality
	@returns true if pass, otherwise fail
	*/
bool
mainvolsurface(void);

/** test of the credit curve object
	@returns true if pass, otherwise fail
	*/
bool
maincreditcurve(void);

/** test of the credit curve object
	@returns true if pass, otherwise fail
	*/
bool
mainrainbowoptions(void);

/** test the bond functionality
	@returns true if pass, otherwise fail
	*/
bool
mainbond(void);

/** test of the variance swaps object
	@returns true if pass, otherwise fail
	*/
bool
mainvarianceswap(void);

/** test of the binomial tree object
	@returns true if pass, otherwise fail
	*/
bool
mainbinomialtree(void);

/** test of the convertible bond object
    @returns true if pass, otherwise fail
	*/
bool
mainconvertiblebond(void);

/**
	Test of the Vanilla Swap - Yann
    @returns true if pass, otherwise fail
*/
bool mainIRVanillaSwap(void);

#endif