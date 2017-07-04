#pragma once
#ifndef bond_h
#define bond_h

#include "../common/types.h"
#include "../common/date.h"
#include <valarray>
#include "../PartB/yieldCurve.h"
#include "../PartF/creditCurve.h"


using namespace std;

class cashflow{
private:
	valarray<Date> _dates;
	valarray<Real> _cashflows;

public:
	cashflow(valarray<Date> dates, valarray<Real> cashflows);
	~cashflow(){};

	valarray<Date> getDates();
	valarray<Real> getCashflows();


};

class bond{

protected:
	Date _issue;
	Date _maturity;
	Date _firstcoupondate;
	Real _coupon;
	Frequency _freq;
	Real _faceamount;
	yieldCurve _yc;
protected:
	DayCountConvention _daycount;
public:
	/** Constructor
	@param issue: date of issue of the bond
	@param maturity: maturity of the bond
	@param firstcoupondate: date of the first coupon
	@param coupon: coupon of the bond, express as a percentqge of the faceamount
	@param freq: frequency of the coupon
	@param faceamount: par value
	@param daycount: daycount convention
	@param yc: yieldcurve
	*/
	bond(Date issue, Date maturity, Date firstcoupondate, Real coupon, Frequency freq, Real faceamount,DayCountConvention daycount, yieldCurve yc);
	
	 /** Destructor */
	~bond(void){};

	cashflow getCashflow();
	
	virtual Real quotedPrice(Date today);
	virtual	Real fairvalue(Date today);
	virtual Real yieldToMaturity(Date today);
	virtual Real duration(Date today);
	virtual Real convexity(Date today);
	
	virtual Real quotedPrice(){return quotedPrice(_issue);};
	virtual	Real fairvalue(){return fairvalue(_issue);};
	virtual Real yieldToMaturity(){return yieldToMaturity(_issue);};
	virtual Real duration(){return duration(_issue);};
	virtual Real convexity(){return convexity(_issue);};

	virtual Date getMaturity(){return _maturity;};
	virtual Date getIssue(){return _issue;};
	virtual Real getMaturityInYears(){return getMaturityInYears(_issue);};
	virtual Real getMaturityInYears(Date today){return today.dayCount(_maturity);};
	virtual Real getFaceAmount() const {return _faceamount;};
};

class treasurybond : public bond{
private :
	
public :
	/** Constructor
	@param issue: date of issue of the bond
	@param maturity: maturity of the bond
	@param firstcoupondate: date of the first coupon
	@param coupon: coupon of the bond, express as a percentqge of the faceamount
	@param freq: frequency of the coupon
	@param faceamount: par value
	@param daycount: daycount convention
	@param yc: yieldcurve
	*/
	treasurybond(Date issue, Date maturity, Date firstcoupondate, Real coupon, Frequency freq, Real faceamount, DayCountConvention daycount, yieldCurve yc);
	
	/** Constructor : bond whose freq is semiannual, faceamount=100 and daycount is actual/360 */
	treasurybond(Date issue, Date maturity, Date firstcoupondate, Real coupon, yieldCurve yc);
	
	/** Constructor : ZCbond */
	treasurybond(Date issue, Date maturity, Real faceamount, DayCountConvention daycount, yieldCurve yc);
	
	/** Destructor */
	~treasurybond(void){};

	/** bond with same parameters and a shifted yield curve */
	treasurybond shiftedbond(Real shift);

	/** return the derivative of the bond price with respect to interest rates*/
	Real rho(Date today);
	Real rho(){return rho(_issue);};
};

class riskybond : public bond{

private :
	creditCurve _cc;

public :
	/** Constructor
	@param issue: date of issue of the bond
	@param maturity: maturity of the bond
	@param firstcoupondate: date of the first coupon
	@param coupon: coupon of the bond, express as a percentqge of the faceamount
	@param freq: frequency of the coupon
	@param faceamount: par value
	@param daycount: daycount convention
	@param yc: yieldcurve
	@param cc: creditcurve
	*/
	riskybond(Date issue, Date maturity, Date firstcoupondate, Real coupon, Frequency freq, Real faceamount,DayCountConvention daycount, yieldCurve yc,  creditCurve cc);

	/** Constructor : ZCbond */
	riskybond(Date issue, Date maturity, Real faceamount, DayCountConvention daycount, yieldCurve yc,  creditCurve cc);


	/** Destructor */
	~riskybond(void){};

	virtual Real quotedPrice(Date today);
	
	/** bond with same parameters and a shifted yieldCurve */
	virtual riskybond shiftedbond(Real shift);

	/** return the derivative of the bond price with respect to interest rates*/
	virtual Real rho(Date today);
	virtual Real rho(){return rho(_issue);};
};

#endif

