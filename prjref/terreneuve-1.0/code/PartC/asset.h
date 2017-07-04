#pragma once
#ifndef asset_h
#define asset_h

#include "../common/types.h"
#include "../common/date.h"
#include "../PartB/yieldCurve.h"

#define ASSET_DEFAULT_VOL 0.20

using namespace std;

/**
@author Yann
Should have include vol surface / yc, etc to just pass assets and not all parameters into the product classes
but this one was not the most important
*/

class flowSchedule
{
private:
	Date _dateOfFlowPayment;
	Real _FlowAmountInPercent;
	BusinessDayConvention _businessDayConventionOnPaymentDate;	
	
public:
	/** Default Constructor*/
	flowSchedule(void);

	~flowSchedule(void);

	/** Constructor*/
	flowSchedule(Date date,Real percent,BusinessDayConvention bd=Unadjusted);
	
	/** Sets a date in the schedule */
	void setDate(Date date) {_dateOfFlowPayment =date;}
	
	/** Sets an amount in the schedule */
	void setAmount(Real percent) {_FlowAmountInPercent =percent;}

	/** Sets a bus day  convention in the schedule */
	void setBusDayConv(BusinessDayConvention bd) {_businessDayConventionOnPaymentDate =bd;}

	/** @returns the date member of the object */
	Date getDate() {return _dateOfFlowPayment ;}

	/** @returns the amount member of the object */
	Real getAmount() {return _FlowAmountInPercent;}

	/** @returns the bus day convention member of the object */
	BusinessDayConvention getBusDayConv() {return _businessDayConventionOnPaymentDate;}
};

class asset
{
private:
	Real _currentPrice;
	yieldCurve _yc;
	bool _areDividendsAsGrowingRate;
	Real _dividendGrowingRate;
	valarray<flowSchedule> _announcedDividendFlows;
	Currency _denomCur;
	Real _volatility;
public:
	/** default contructor */
	asset(void);

	/** contructor 
	@param price : current price of the asset
	@param yc : mkt yc
	@param areDividendsAsRate : tells u if divs are a future schedule (usually useless) or a fixed rate
	@param divrate: so called fixed future div rate
	@param flowsc : anticipated future div flows
	@param CCY : currency denomination
	@param volatility : stock's volatility (should be a volsurface but this is a simplification)
	*/
	asset(Real price,yieldCurve yc,bool areDividendsAsRate=true,Real divRate=0.0,valarray<flowSchedule> flowsc=valarray<flowSchedule>(),Currency ccy=USD,Real volatility = ASSET_DEFAULT_VOL);

	asset(Real price, Real volatility = ASSET_DEFAULT_VOL);

	/** a stock is delta 1!*/
	Real getDelta() {return 1.0;} 

	/** Rho vs fwd*/
	Real getRho(Real T) ;

	/** sets the spot price */
	void setPrice(Real p) {_currentPrice =p;}

	/** sets the yc */
	void setYieldCurve(yieldCurve yc) {_yc =yc;}

	/** sets the div rate */
	void setDivAsRate(Real rate=0.0);

	/** sets the flow schedule */
	void setDivFlows(flowSchedule fc) {_announcedDividendFlows=fc;}

	/** sets the currency */
	void setCcy(Currency ccy) {_denomCur=ccy;}

	/** sets the volatility */
	void setVolatility(Real volatility) {_volatility = volatility;}

	/** gets the spot price */
	Real getPrice() const {return _currentPrice ;}

	/** gets the spot price */
	yieldCurve getYieldCurve() {return _yc ;}

	/** gets the yc */
	bool areDivAsRate() {return _areDividendsAsGrowingRate;}

	/** gets the div rate*/
	Real getRate() {return _dividendGrowingRate;}

	/** gets the div future flow schedule */
	valarray<flowSchedule> getFlowSchedule() {return _announcedDividendFlows;}

	/** gets the currency */
	Currency GetCurrencyFormat() {return _denomCur;}

	/** gets the volatility */
	Real GetVolatility() const {return _volatility;}

	/** If we have an asset model we can inherit from this class and change the method */
	virtual Real Price(); 
	
	/** get the forward price */
	Real forwardPrice(Date maturityDate);

	/** get the forward price */
	Real forwardPrice(Real T);
	~asset(void);
};
#endif