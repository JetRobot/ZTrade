#pragma once

#ifndef VARIANCESWAP_H
#define VARIANCESWAP_H

#include "../PartA/BlackScholes/OptionStrategy.h"

/**
@author Simon
*/

class VarianceSwap
{
public:
	/**Default Constructor 
	@param options: pointer to a basket of options to price the swap
	@param maturity: maturity of the swap
	@param forwardprice: forward value of the underlying = S0*exp(r*T) 
	*/
	VarianceSwap(OptionStrategy* options,Real maturity, Real forwardprice);
	~VarianceSwap(void);
	/** Return price */
	Real getPrice();
	/** Return sensitivity to a move in rates */
	Real getRho(Real shiftCurve=defaultshiftRate);
	/** Return sensitivity to a move in volatility */
	Real getVega(Real shiftVol=defaultshiftVol);
	/** Return sensitivity to a move in time */
	Real getTheta(Real shiftMat=defaultshiftMat);

private:
	OptionStrategy* _options;
	Real _maturity,_forward;
};

#endif;
