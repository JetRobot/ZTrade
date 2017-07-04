#include "BlackScholes.h"

Real absolute(Real x){
	if (x>=0)
		return x;
	else
		return -x;
}

BlackScholes::BlackScholes(Real spot, Real volOrPrice, bool isVol, Real r, Real K, Real T,TypeOptionBS typeOption):
							_spot(spot),
							_r(r),
							_K(K),
							_T(T),
							_type(typeOption)
{
	// If paramter given is volatility, we compute the price
	if (isVol) {
		_vol=volOrPrice;
		d1= (log(_spot/_K)+(_r+_vol*_vol/2)*_T)/(_vol*sqrt(_T));
		d2= d1-_vol*sqrt(_T);
	} 
	// If the parameter given is the price, we compute the implied volatility
	else {
		_price=volOrPrice;
		Real vol;
		Real p,vega;
		vol= sqrt(2*absolute(log(_spot/_K)+_r*_T)/_T);
		for (int i=0;i<100;i++){
			BlackScholes bs=BlackScholes(_spot,vol,true,_r,_K,_T,_type);
			p=bs.getPrice();
			vega=bs.getVega();
			vol+=(_price-p)/vega;
		}
		if (!(volOrPrice>0))
			volOrPrice=0;
		_vol=vol;
		d1= (log(_spot/_K)+(_r+_vol*_vol/2)*_T)/(_vol*sqrt(_T));
		d2= d1-_vol*sqrt(_T);
	}
}

BlackScholes::BlackScholes() {
}

BlackScholes::~BlackScholes()
{
}

Real BlackScholes::getDelta() {
	switch (_type) {
			case Call:
			default:
				return CumulativeNormal(d1);
			case Put:
				return CumulativeNormal(d1)-1;
			}
}

/*Real BlackScholes::getDelta() {
	Real delta,p_d;
	BlackScholes BS_d(_spot*1.01,_vol,_r,_K,_T);
	p_d=BS_d.getPrice();
	delta=(p_d-_price)/(0.01*_spot);
	return delta;
}*/

Real BlackScholes::getGamma() {
	return (NormalDensity(d1)/(_spot*_vol*sqrt(_T)));
}

Real BlackScholes::getVega() {
	return (_spot*sqrt(_T)*NormalDensity(d1));
}

/*Real BlackScholes::getVega() {
	Real vega,p_v;
	BlackScholes BS_v(_spot,_vol*1.01,_r,_K,_T);
	p_v=BS_v.getPrice();
	vega=(p_v-_price)/(0.01*_vol);
	return vega;
}*/

Real BlackScholes::getTheta() {
	switch (_type) {
			case Call:
			default:
				return ((-_spot*_vol*NormalDensity(d1))/(2*sqrt(_T))-_r*_K*exp(-_r*_T)*CumulativeNormal(d2));
			case Put:
				return ((-_spot*_vol*NormalDensity(d1))/(2*sqrt(_T))+_r*_K*exp(-_r*_T)*CumulativeNormal(-d2));
	}
}

/*Real BlackScholes::getTheta() {
	Real theta,p_t;
	BlackScholes BS_t(_spot,_vol,_r,_K,_T*1.01);
	p_t=BS_t.getPrice();
	theta=(p_t-_price)/(0.01*_T);
	return theta;
}*/

Real BlackScholes::getRho() {
	switch (_type) {
			case Call:
			default:
				return (_K*_T*exp(-_r*_T)*CumulativeNormal(d2));
			case Put:
				return (-_K*_T*exp(-_r*_T)*CumulativeNormal(-d2));
	}
}

/*Real BlackScholes::getRho() {
	Real rho,p_r;
	BlackScholes BS_r(_spot,_vol,_r*1.01,_K,_T);
	p_r=BS_r.getPrice();
	rho=(p_r-_price)/(0.01*_r);
	return rho;
}*/

Real BlackScholes::getVolatility() const{
	return _vol;
}

Real BlackScholes::getStrike() const{
	return _K;
}

Real BlackScholes::getRate() const{
	return _r;
}

Real BlackScholes::getSpot() const{
	return _spot;
}

Real BlackScholes::getMaturity() const{
	return _T;
}

bool BlackScholes::isCall() const{
	return (_type==Call);
}

void BlackScholes::changeRate(Real newRate) {
	_r=newRate;
	recalcInformation();
}

void BlackScholes::changeVol(Real newVol) {
	_vol=newVol;
	recalcInformation();
}

void BlackScholes::changeMaturity(Real newMat) {
	_T=newMat;
	recalcInformation();
}

void BlackScholes::changeSpot(Real newSpot) {
	_spot=newSpot;
	recalcInformation();
}

void BlackScholes::changeStrike(Real newStrike) {
	_K=newStrike;
	recalcInformation();
}

void BlackScholes::recalcInformation() {
	d1= (log(_spot/_K)+(_r+_vol*_vol/2)*_T)/(_vol*sqrt(_T));
	d2= d1-_vol*sqrt(_T);
}