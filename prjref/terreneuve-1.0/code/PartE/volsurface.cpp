

#include ".\volsurface.h"



volsurface::volsurface(Real stockPrice, Date today, valarray<Real> strikes,  valarray<Date> maturities, yieldCurve yCurve, valarray<valarray<Real> > callputprices, valarray<valarray<bool> > iscallputprices)
: _stockPrice(stockPrice),
  _today(today),
  _strikes(strikes),
  _maturities(maturities),
  _yieldCurve(yCurve),
  _callputprices(callputprices),
  _volsurfconst(false),
  _iscallputprices(iscallputprices)
  {
  }

volsurface::volsurface(Real stockPrice, Date today, yieldCurve yCurve, volsurfaceparams &params)
: _stockPrice(stockPrice),
  _today(today),
  _strikes(params.strikes),
  _maturities(params.maturities),
  _yieldCurve(yCurve),
  _callputprices(params.callputprices),
  _volsurfconst(false),
  _iscallputprices(params.iscallputprices)
  {
  }


volsurface::volsurface(Real constantvol)
	:  _volsurfconst(true),
	    _constantvol(constantvol)
{
}

volsurface::volsurface(void)
	:  _volsurfconst(true),
	    _constantvol(0)
{
}

volsurface::volsurface(valarray<valarray<Real> > volsurf)
	: _impliedvolsurface(volsurf)
{
}


volsurface::~volsurface(void)
{
}


Real volsurface::invertBSformula(Real r, Real maturity, Real stockPrice, Real strike, Real callputPrice, bool isacall)
{
	if (isacall){
		BlackScholes bs = BlackScholes(stockPrice, callputPrice, false, r, strike, maturity, Call);
		return bs.getVolatility();
	}
	else{
		BlackScholes bs = BlackScholes(stockPrice, callputPrice, false, r, strike, maturity, Put);
		return bs.getVolatility();
	}

}

void volsurface::setvolsurface()
{	
	if(_volsurfconst){
		int N=10;
		_impliedvolsurface.resize(N);
		valarray<Real> temp(N);
		int i;
		for(i=0;i<10;i++)
			temp[i]=_constantvol;
		for(i=0;i<10;i++){
			_impliedvolsurface[i].resize(10);
			_impliedvolsurface[i]=temp;
		}

	}
	else{
		int i=_callputprices.size();
		int j=_callputprices[1].size();

		valarray<Real> temp(j);
		_impliedvolsurface.resize(i);

// lines are maturities and column strikes

		int k,l;
		Date maturity;
		Real timetomaturity;
		Real r;

		for(k=0;k<i;k++){
			maturity=_maturities[k];
			timetomaturity=_today.dayCount(maturity);
			r=_yieldCurve.spotRate(timetomaturity);

			for(l=0;l<j;l++){
				temp[l]=invertBSformula(r, timetomaturity, _stockPrice, _strikes[l], _callputprices[k][l], _iscallputprices[k][l]);
			}

			_impliedvolsurface[k].resize(j);
			_impliedvolsurface[k]=temp;
		}
		valarray<Real> vecmaturity(i);
		for(k=0;k<i;k++){
			vecmaturity[k]=_today.dayCount(_maturities[k]);
		}

		_interpolvolsurf=interpolator(vecmaturity, _strikes, _impliedvolsurface);
	}
}

valarray<valarray<Real> > volsurface::getvolsurface(){
	return _impliedvolsurface;
}

Real volsurface::volatility(Real K, Date T){
	if(_volsurfconst)
		return _constantvol;
	else{
		Real timetomaturity=_today.dayCount(T);
		return _interpolvolsurf.interpolate(timetomaturity, K);
	}

}
Real volsurface::variance(Real K, Date T){
	if(_volsurfconst)
		return _constantvol*_constantvol;
	else{
		Real vol=volatility(K,T);
		return vol*vol;
	}
}
Real volsurface::forwardVolatility(Real K, Date t, Date T){
	Real v1=variance(K,t);
	Real v2=variance(K,T);
	Real timetoT=_today.dayCount(T);
	Real timetot=_today.dayCount(t);
	Real v12=(timetoT*v2 - timetot*v1)/(timetoT - timetot);
	return sqrt(v12);
}

volsurface volsurface::forwardvolsurface(Date t){

	Integer i=0;
	Integer N =_maturities.size();

	while((i<N)&&(_maturities[i]<t)){
		i++;
	}
	valarray<valarray<Real> > fwdvs;
	if (i<N){
		fwdvs.resize(N-i);
		Integer j,k;
		Integer M = _strikes.size();
		valarray<Real> temp(M);
		valarray<Date> maturities(M);
		for(j=0;j<N-i;j++){
			for(k=0;k<M;k++)
				temp[k]=forwardVolatility(_strikes[k], t, _maturities[j+i]);
			fwdvs[j].resize(M);
			fwdvs[j] = temp;
			maturities[j] = _maturities[j+i];
		}
		
	volsurface forwardvd(fwdvs);
	return forwardvd;
	}
	else{
		fwdvs.resize(1);
		Integer M = _strikes.size();
		fwdvs[1].resize(M);
		fwdvs[1] = _impliedvolsurface[N-1];
		volsurface forwardvd(fwdvs);
		return forwardvd;
	}

}

volsurface volsurface::shiftedYCvolsurface(Real shift){

	yieldCurve shiftedyc = _yieldCurve.shiftZCBRateCurve(shift);
	volsurface shiftedvs(_stockPrice, _today, _strikes,  _maturities, shiftedyc, _callputprices, _iscallputprices);
	shiftedvs.setvolsurface();

	return shiftedvs;
}

volsurface volsurface::shiftedvolsurface(Real shift){

	Integer M = _strikes.size();
	Integer N = _maturities.size();
	Integer j,k;

	valarray<valarray<Real> > shiftedvs;
	valarray<Real> temp(M);
	
	for(j=0;j<N;j++){
		for(k=0;k<M;k++)
			temp[k] = _impliedvolsurface[j][k] + shift;
		shiftedvs[j].resize(M);
		shiftedvs[j] = temp;
	}
	volsurface shiftedvolsurf(shiftedvs);

	return shiftedvolsurf;
}