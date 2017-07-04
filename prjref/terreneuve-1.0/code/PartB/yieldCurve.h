#pragma once
#ifndef yieldCurve_h
#define yieldCurve_h

#include "../common/types.h"
#include "../common/date.h"
#include "../common/interpolator.h" 
#include "../common/matrix.h" 
#include "..\common\utils.h"
#include <string>
#include <math.h>
#include <algorithm>
#include<time.h>

using namespace std;

#define YC_NAME_STRLEN 128
#define YC_DEFAULT_NUMER_POINTS 15
#define YC_MAX_NUMBER_POINTS 50
/**
@author Yann
*/

enum interestComposition {
		Discrete,		// 
		Continuous		//
};
	
enum TypeOfRate {
	// Zero Coupon rate
     Cash,
	 // rate quoted on a sawp rate basis : for maturity T = fixed rate so that the annualy compounded swap is 0
     Swap 
};


class yieldPoint
{
public:

	/** Default constructor */
	yieldPoint(void);

	/**Constructor 
	@param r: Real rate associated to the Point - on an annual basis
	@param T: Real maturity of the rate
	@param Type: Type of the market Point 
	Type : either cash or swap. Default will be the case where the short term is always cash, long is swap.
	*/
	yieldPoint(Real r,Real T,TypeOfRate type=Cash,DayCountConvention dayCount=ACT_360);

	/** Standard Destructor	*/
	~yieldPoint(void);

	/** Returns the rate associated to the Point */
	Real    getRate()     { return _rate;}

	/** Returns the maturity associated to the Point */
	Real    getMaturity() { return _maturity;}

	/** Returns the type associated to the Point */
	TypeOfRate     getType()     { return _type;}

	/** Returns the dayCount associated to the Point */
	DayCountConvention getDayCount() { return _dayCount;}

	/** Set the rate associated to the Point */
	void     setRate(Real r)     { _rate = r;}

	/** Set the maturity associated to the Point */
	void     setMaturity(Real m) { _maturity = m;}

	/** Set the type associated to the Point */
	void     setType(TypeOfRate t)     { _type = t;}

	/** Set the dayCount associated to the Point */
	void     setDayCount(DayCountConvention d) { _dayCount = d;}

	static char *TypeAsString(TypeOfRate t);


	private:
		Real    _rate;
		Real    _maturity;
		TypeOfRate     _type;
		DayCountConvention _dayCount;
};


static const Real defaultshiftfactorForShortRate = 0.0001 ;//1 bp

class yieldCurve
{
/** display maturities and spotrates in the curve. 
	@param os - the output stream to direct output to
	@param c - the curve to display
	@returns output stream as is standard for operator<<
	*/
	friend ostream& operator << (ostream &os, const yieldCurve& c);
	friend ostream& operator << (ostream &os, const yieldCurve* c) {
		return os << *c;
	}

	public:
		/** Default void constructor*/
		yieldCurve(void);
	    
		/** FlatCurve 
		@param flatRate : Real is the flat rate - by default, it will simulate a flat ZCB curve with 15 Points by default
		*/
		yieldCurve(Real flatRate);
	
		/** For flat rate curves, set the flat rate	value */
		void assignFlatRate(Real r=0.0);

		/** sets an EXISTING rate at a certain level */
		void assignZCBrateAtIndex(Real rate,Natural i);

		/** for risk mngmt purposes, shifts the yc*/ 
		yieldCurve shiftZCBRateCurve(Real shift=defaultshiftfactorForShortRate);

		/** for risk mngmt purposes, rotates the yc with a ref to how you move the shortest rate, around which rate*/ 
		yieldCurve rotateZCBRateCurve(Real moveInShortestRate=defaultshiftfactorForShortRate,Real maturityOfRotation=7);
	    
		/** Constructor 
		@param yieldPoints the array of yieldPoints
		@param name the name we give it
		*/
		yieldCurve(valarray<yieldPoint> yieldPoints,char *name = "unnamed");

		~yieldCurve(void);

		/**
		Calculates the spot ZCB rate
		@param maturity : if it is exact it just gives the result from a Point, else an interpolated one based on interpolator
		*/
		virtual Real spotRate(Real maturity) const;

		/**
		Return the maturities present in the market curve, both from the Cash and Swap Pointsvalarray<Real> 
		*/
		virtual valarray<Real> getMaturitiesInTheMarketCurve() const;

		/**
		Return the maturities present in the market curve, both from the Cash and Swap Pointsvalarray<Real> 
		*/
		virtual valarray<Real> getMaturitiesInTheZCBCurve() const;

		/**
		Calculates the spot ZCB rate
		@param maturityDate : maturityDate of the ZCB
		*/
		virtual Real spotRate(Date maturityDate) const;

		/**
		Calculates the discountFactor
		@param maturity : just after ZCBrates are computed, it it very easy [done at trhe constructor level]
		*/     
		virtual Real discountFactor(Real maturity,interestComposition composition=Continuous);
	    
		/**
		Calculates the discountFactor
		@param maturity : just after ZCBrates are computed, it it very easy [done at trhe constructor level]
		*/     
		virtual Real discountFactor(Date maturityDate,interestComposition composition=Continuous);


		/**
		Calculates the discountFactor
		@param maturity : just after ZCBrates are computed, it it very easy [done at trhe constructor level]
		*/     
		virtual Real forwardDiscountFactor(Real forwardstart,Real lengthofcontractafterstart,interestComposition composition=Continuous);
	    

		/**
		Calculates the fwd rate
		@param forwardStart start of the rate
		@param maturityAfterForward maturity after the start
		*/          
		virtual Real forwardRate(Real forwardStart,Real effectiveLengthOfTheContractAfterStart,interestComposition composition=Continuous);

		/**
		Calculates the fwd rate
		@param forwardStart start of the rate
		@param maturityAfterForward maturity after the start
		*/          
		virtual Real forwardRate(Date forwardStart,Date forwardEnd,interestComposition composition=Continuous);

		/** Forward curve*/
		virtual yieldCurve forwardZCBCurve(Real forwardStart);
	    
		char *getName() {return &_name[0];}
		
		virtual bool operator==(const yieldCurve& yours);

		virtual bool operator!=(const yieldCurve& yours);

	private :
		valarray<yieldPoint> _marketRates;     
		valarray<yieldPoint> _zcbRates; 
		char _name[YC_NAME_STRLEN];   

		/** for know maturities, we can return the market Point as it is
		@param maturity : Be careful with this method as if it does not know the maturity it will give a blank Point*/
		yieldPoint getPointAtMaturity(Real maturity);

		/** Sorting rates by maturity, just in case ...*/
		void sortMarketRatesByMaturity();

		/** Routine to make sure the short term rates (cash) are before the mid/long term (swap)*/
		void sortCashSwap();

		/** needed in the bootstrap method to be able to back the ZC */
		valarray<yieldPoint> getSwapRates();


		/** 1Y difference in swaps -> ZC's easily backed up 
		The method assumes that rates are sorted by ascending maturity 
		The mkt curve is 1Y by 1Y - we should fill in the gaps by linear interpolation
		*/
		valarray<yieldPoint> getSequentSwapRates();

		/**
		Go from the swap rates to the ZC - matrix inversion to be done
		Swap rates can be annual or semi annual in their most common quotes on the market.
		Having semi annual swaps with annual maturities makes life hard as to bootstrap the ZC's as we would not be
		able to back out the semi annual ZC's to move to the next step without market bond prices, etc: it would really be cumbersome
		On the quote screen provided by Prof Laud, swaps are quoted annualy from 2 to 10 -> easy calculation [cf latex Code]
		as for the 15 - 20 and 30 with frequency annual, we need to find a way to find mid Points 11-14, 16-19 21-29.
		*/
		void computeZCBRatesBootstrap();

		/** Discount Factors corresponding to annual swaps quoted on sequent yrs : X-1 * (1...1)'
		where X is as follows:
		- diag = +1 swap (i)
		- inferior triang (i,j) = swap (i)
		- superior diag (i,j) =0
		*/
		valarray<Real> SequentDiscountFactorsByInvertSwapMatrix();

};


#endif


