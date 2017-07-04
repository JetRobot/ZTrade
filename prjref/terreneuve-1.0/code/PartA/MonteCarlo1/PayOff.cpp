#include "./payoff.h"
#include "../../common/Normals.h"
#include<minmax.h>

PayOff::PayOff(Real Strike_) : Strike(Strike_)
{
}

PayOff::PayOff(void) : Strike(0)
{
}

void PayOff::SetStrike(Real Strike_)
{
	Strike=Strike_;
}


Real PayOff::operator () (Real Spot) const
{
    return max(Spot-Strike,0.0);
}

Real PayOff::Call(Real Fwd)
{
	return max(Fwd-Strike,0.0);
}

Real PayOff::Put(Real Fwd)
{
	return max(Strike-Fwd,0.0);
}

Real PayOff::AsianCall(valarray<Real> Path,LongNatural nDates)
{
	return max(Average(Path,nDates)-Strike,0.0);
}

Real PayOff::AsianPut(valarray<Real> Path,LongNatural nDates)
{
	return max(Strike-Average(Path,nDates),0.0);
}

Real PayOff::RevLookbackCall(valarray<Real> Path,LongNatural nDates)
{
	return max(Maximize(Path,nDates)-Strike,0.0);
}

Real PayOff::RevLookbackPut(valarray<Real> Path,LongNatural nDates)
{
	return max(Strike-Maximize(Path,nDates),0.0);
}

Real PayOff::BarrierCall(valarray<Real> Path,LongNatural nDates)
{
	for (unsigned int i=0;i<nDates;i++) {
		if (Path[i]>=Strike) return 1.;
	}
	return 0.;
}

Real PayOff::BarrierPut(valarray<Real> Path,LongNatural nDates)
{
	for (unsigned int i=0;i<nDates;i++) {
		if (Path[i]<=Strike) return 1.;
	}
	return 0.;
}

Real PayOff::FlooredCliquet (Real Spot, Real Fwd)
{
	if (Fwd>=Strike)
		return Fwd-Spot;
	else return Strike-Spot;
}

Real PayOff::CappedCliquet (Real Spot, Real Fwd)
{
	if (Fwd<=Strike)
		return Fwd-Spot;
	else return Strike-Spot;
}

Real PayOff::Rainbow2SpreadOptionMax(Real Fwd1,Real Fwd2,Real W1,Real W2,Real Mult)
{
	return max(Mult*(W1*Fwd1-W2*Fwd2-Strike),0);
}

Real PayOff::Rainbow2AssetsBasketMax(Real Fwd1,Real Fwd2,Real W1,Real W2,Real Mult)
{
	return max(Mult*(W1*Fwd1+W2*Fwd2-Strike),0);
}

Real PayOff::RainbowBestOf2AssetsCash(Real Fwd1,Real Fwd2,Real W1,Real W2)
{
	return max(max(W1*Fwd1,W2*Fwd2),Strike);
}

Real PayOff::RainbowWorstOf2AssetsCash(Real Fwd1,Real Fwd2,Real W1,Real W2)
{
	return min(min(W1*Fwd1,W2*Fwd2),Strike);
}

Real PayOff::RainbowMax2AssetsCall(Real Fwd1,Real Fwd2,Real W1,Real W2,Real Mult)
{
	return max(Mult* (max(W1*Fwd1 , W2*Fwd2)-Strike), 0);
}

Real PayOff::RainbowMin2AssetsCall(Real Fwd1,Real Fwd2,Real W1,Real W2,Real Mult)
{
	return max(Mult* (min(W1*Fwd1 , W2*Fwd2)-Strike), 0);;
}

Real PayOff::RainbowMax2AssetsPut(Real Fwd1,Real Fwd2,Real W1,Real W2,Real Mult)
{
	return max(Strike-Mult* (max(W1*Fwd1 , W2*Fwd2)), 0);
}

Real PayOff::RainbowMin2AssetsPut(Real Fwd1,Real Fwd2,Real W1,Real W2,Real Mult)
{
	return max(Strike-Mult* (min(W1*Fwd1 , W2*Fwd2)), 0);;
}

Real PayOff::Convertible(Real Fwd, Real ConversionRatio, Real BondValue, Real CallPrice, Real PutPrice)
{
	return max(
		max(ConversionRatio * Fwd, min(BondValue, CallPrice)),
		PutPrice);
}

