#ifndef BLACKSCHOLES_H
#define BLACKSCHOLES_H

#include "../../common/types.h"
#include "../../common/Normals.h"
#include <cmath>

enum TypeOptionBS {
		Call,		//Call type, payoff=(St-K)+
		Put			//Put type, payoff=(K-St)+
};

class OptionStrategy;

class BlackScholes
{
public:
	/** Default constructor
	@param spot: Spot price of the asset
	@param volOrPrice: Parameter given : either vol or price, the other will be computed
	@param isVol: Bool allowing the constructor to know if the col or the price has been given
	@param r: Spot Rate until maturity of option
	@param K: Strike of the option
	@param T: Maturity of the option
	@param typeOption: Type of the option (Call or Put)
	*/
	BlackScholes(Real spot, Real volOrPrice, bool isVol, Real r, Real K, Real T,TypeOptionBS typeOption);
	BlackScholes();
	virtual ~BlackScholes();
	/** Return price of the option */
	inline Real getPrice();
	/** Return the Delta value for the option */
	Real getDelta();
	/** Return the Gamma value for the option */
	Real getGamma();
	/** Return the Vega value for the option */
	Real getVega();
	/** Return the Theta value for the option */
	Real getTheta();
	/** Return the Rho value for the option */
	Real getRho();
	/** Return the Volatility for the option */
	Real getVolatility() const;
	/** Return the strike of the option */
	Real getStrike() const;
	/** Return the risk free rate at maturity of the option */
	Real getRate() const;
	/** Return the spot of the option */
	Real getSpot() const;
	/** Return the maturity of the option */
	Real getMaturity() const;
	/** Return the type of the option */
	bool isCall() const;

protected:
	/**Allow to change rate for testing sensibility*/
	friend OptionStrategy;
    void changeRate(Real newRate);
	void changeVol(Real newVol);
	void changeMaturity(Real newMat);
	void changeSpot(Real newSpot);
	void changeStrike(Real newVol);

private:
	void recalcInformation();
	Real _spot,_vol,_r,_K,_T;
	Real d1,d2;
	Real _price;
	TypeOptionBS _type;
};

inline Real BlackScholes::getPrice()
{
	switch (_type) {
			case Call:
			default:
				_price= _spot*CumulativeNormal(d1)-_K*exp(-_r*_T)*CumulativeNormal(d2);
				break;
			case Put:
				_price= _K*exp(-_r*_T)*CumulativeNormal(-d2)-_spot*CumulativeNormal(-d1);
				break;
	}
	return _price;
}

#endif
