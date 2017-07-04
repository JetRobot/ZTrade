#include ".\rainbowoption.h"

RainbowOption::RainbowOption(void)
{
	Date today;
	today.setDateToToday();
	_startDate=today;
	_expiryInYears=RO_DEFAULT_MATURITY;
	_Strike=RO_DEFAULT_STRIKE;
	_Multiplier=1;
	_NumberOfAssets=RO_DEFAULT_NB_ASSETS;
	_weights=valarray<Real>(1/(Real)_NumberOfAssets ,_NumberOfAssets);  // equaly weighted
	_CorrelationMatrix=IdentityMatrix(_NumberOfAssets); // we need the correlation to compute the paths
	_volatilities=valarray<Real>(RO_DEFAULT_VOL,_NumberOfAssets);
	_volatilitiesSurfaces=valarray<volsurface>(volsurface(RO_DEFAULT_VOL),_NumberOfAssets);
	_yc=yieldCurve(RO_DEFAULT_RATE); // no quanto so risk free rate is unique.
	_spots=valarray<Real>(RO_DEFAULT_STRIKE,RO_DEFAULT_NB_ASSETS);

	_DFTomaturity=_yc.discountFactor(_expiryInYears);
	haveClosedFormVariablesBeenComputed=false;
	_outputMsgs=false;
	_type=BestOf2AssetsCash; // the most famous one

	_seed=(LongNatural)RO_SEED;
	MersenneTwister* pRandGen = new MersenneTwister(_seed);
	_pRandom=new Random(pRandGen);
}

RainbowOption::RainbowOption(rainbowType type,Date startDate,Real expiry,Real Strike,yieldCurve yc,valarray<volsurface> vols,valarray <Real> spots,Real Multiplier,Matrix Correl,valarray<Real> weights,bool outputMsgs)
							 :	_type(type),
								_startDate(startDate),
								_expiryInYears(expiry),
								_Strike(Strike),
								_spots(spots),
								_yc(yc),
								_volatilitiesSurfaces(vols),
								_Multiplier(Multiplier),
								_CorrelationMatrix(Correl),
								_weights(weights),
								_outputMsgs(outputMsgs)
{
	_NumberOfAssets=_CorrelationMatrix.GetRows();
	_thePayOff.SetStrike(_Strike);
	_DFTomaturity=_yc.discountFactor(_expiryInYears);
	reassignVolsAtThestrike();
	haveClosedFormVariablesBeenComputed=false;
	_seed=(LongNatural)RO_SEED;
	MersenneTwister* pRandGen = new MersenneTwister(_seed);
	_pRandom=new Random(pRandGen);
}

RainbowOption::RainbowOption(rainbowType type,Date start,Real exp,Real Strike,yieldCurve yc,valarray<volsurface> vols,Real Spot1,Real Spot2,Real Mult,Real Correl12,Real weight1,Real weight2,bool outputMsgs)
							 :	_type(type),
								_startDate(start),
								_expiryInYears(exp),
								_Strike(Strike),
								_yc(yc),
								_volatilitiesSurfaces(vols),
								_Multiplier(Mult),
								_outputMsgs(outputMsgs)
{
	_NumberOfAssets =2;
	_CorrelationMatrix=IdentityMatrix(_NumberOfAssets);
	_CorrelationMatrix.SetValue(1,0,Correl12);
	_CorrelationMatrix.SetValue(0,1,Correl12);

	_weights=valarray<Real>(_NumberOfAssets);
	_weights[0]=weight1;
	_weights[1]=weight2;

	_spots=valarray<Real>(_NumberOfAssets);
	_spots[0]=Spot1 ;
	_spots[1]=Spot2 ;
	
	_DFTomaturity=_yc.discountFactor(_expiryInYears);
	_volatilities.resize(_NumberOfAssets);

	reassignVolsAtThestrike();
	haveClosedFormVariablesBeenComputed=false;	
	_seed=(LongNatural)RO_SEED;
	MersenneTwister* pRandGen = new MersenneTwister(_seed);
	_pRandom=new Random(pRandGen);
}

Real RainbowOption::getPrice(priceType priceMethod,LongNatural nPaths){		
	Real price;
	switch (_type) {
		case SpreadOptionMax://no closed form
			if((priceMethod!=MonteCarlo)&&(_outputMsgs))
				cout<<"In pricing the rainbow spread, you specified Closed form and there is no closed form\nPrice will be MC"<<endl;
			price=PriceByMc_2SpreadOptionMax(nPaths);
			break;
		case AssetsBasketMax://no closed form
			if((priceMethod!=MonteCarlo)&&(_outputMsgs))
				cout<<"In pricing the rainbow basket, you specified Closed form and there is no closed form\nPrice will be MC"<<endl;
			price=PriceByMc_2AssetsBasketMax(nPaths);
			break;
		case BestOf2AssetsCash:
			switch(priceMethod){
				case MonteCarlo:
					price=PriceByMc_BestOf2AssetsCash(nPaths);
					break;
				case ClosedForm:
					price=PriceByClosedForm_BestOf2_plusCash();
					break;
			}
			break;
		case WorstOf2AssetsCash://no closed form
			if((priceMethod!=MonteCarlo)&&(_outputMsgs))
				cout<<"In pricing the rainbow Worst of + cash, you specified Closed form and there is no closed form\nPrice will be MC"<<endl;
			price=PriceByMc_WorstOf2AssetsCash(nPaths);
			break;
		case BetterOf2Assets:
			switch(priceMethod){
				case MonteCarlo:
					price=PriceByMc_BetterOf2Assets(nPaths);
					break;
				case ClosedForm:
					price=PriceByClosedForm_BetterOf2();
					break;
			}
			break;
		case WorseOf2Assets:
			switch(priceMethod){
				case MonteCarlo:
					price=PriceByMc_WorseOf2Assets(nPaths);
					break;
				case ClosedForm:
					price=PriceByClosedForm_WorseOf2();
					break;
			}
			break;
		case Max2AssetsCall:
			switch(priceMethod){
				case MonteCarlo:
					price=PriceByMc_Max2AssetsCall(nPaths);
					break;
				case ClosedForm:
					price=PriceByClosedForm_MaxOf2_call();
					break;
			}
			break;
		case Min2AssetsCall:
			switch(priceMethod){
				case MonteCarlo:
					price=PriceByMc_Min2AssetsCall(nPaths);
					break;
				case ClosedForm:
					price=PriceByClosedForm_MinOf2_call();
					break;
			}
			break;
		case Max2AssetsPut:
			switch(priceMethod){
				case MonteCarlo:
					price=PriceByMc_Max2AssetsPut(nPaths);
					break;
				case ClosedForm:
					price=PriceByClosedForm_MaxOf2_put();
					break;
			}
			break;
		case Min2AssetsPut:
			switch(priceMethod){
				case MonteCarlo:
					price=PriceByMc_Min2AssetsPut(nPaths);
					break;
				case ClosedForm:
					price=PriceByClosedForm_MinOf2_put();
					break;
			}
			break;
	}
	return price;
}

Real RainbowOption::getPartialDelta(Natural security,priceType priceMethod){
	bool outputmsgstate=_outputMsgs ;
	_outputMsgs=false; // in case we are in the case where user wants to see things
	Natural secIndexInSpotArray = (((security > 0)&&(security <=_NumberOfAssets) )? security -1: 0); // in case he enters 0..
	Real priceup,pricedown;

	Real oldSpot=_spots[secIndexInSpotArray]; // save it
	Real spotUp=oldSpot*(1.0+(Real)GREEKAPPROX),spotDown=oldSpot*(1.0-(Real)GREEKAPPROX);
	_spots[secIndexInSpotArray]=spotUp;
	haveClosedFormVariablesBeenComputed=false;
	priceup=getPrice(priceMethod);// even if we do not have the closed form, the switch should get the MC
	_spots[secIndexInSpotArray]=spotDown;
	haveClosedFormVariablesBeenComputed=false;
	pricedown=getPrice(priceMethod);// even if we do not have the 	closed form, the switch should get the MC
	_spots[secIndexInSpotArray]=oldSpot; // put it back
	haveClosedFormVariablesBeenComputed=false;

	_outputMsgs=outputmsgstate; // where it was
	return (priceup-pricedown)/(2.0*(Real)GREEKAPPROX);
}

Real RainbowOption::getPartialGamma(Natural security,priceType priceMethod){
	bool outputmsgstate=_outputMsgs ;
	_outputMsgs=false; // in case we are in the case where user wants to see things
	Natural secIndexInSpotArray = (((security > 0)&&(security <=_NumberOfAssets) )? security -1: 0); // in case he enters 0..
	Real deltaup,deltadown;

	Real oldSpot=_spots[secIndexInSpotArray]; // save it
	Real spotUp=oldSpot*(1.0+(Real)GREEKAPPROX),spotDown=oldSpot*(1.0-(Real)GREEKAPPROX);
	_spots[secIndexInSpotArray]=spotUp;
	haveClosedFormVariablesBeenComputed=false;
	deltaup=getPartialDelta(security,priceMethod);// even if we do not have the closed form, the switch should get the MC
	_spots[secIndexInSpotArray]=spotDown;
	haveClosedFormVariablesBeenComputed=false;
	deltadown=getPartialDelta(security,priceMethod);// even if we do not have the closed form, the switch should get the MC
	_spots[secIndexInSpotArray]=oldSpot; // put it back
	haveClosedFormVariablesBeenComputed=false;

	_outputMsgs=outputmsgstate; // where it was
	return (deltaup-deltadown)/(2.0*(Real)GREEKAPPROX);
}

Real RainbowOption::getPartialVega(Natural security,priceType priceMethod){
	bool outputmsgstate=_outputMsgs ;
	_outputMsgs=false; // in case we are in the case where user wants to see things
	Natural secIndexInSpotArray = (((security > 0)&&(security <=_NumberOfAssets) )? security -1: 0); // in case he enters 0..
	Real priceup,pricedown;

	Real oldVol=_volatilities [secIndexInSpotArray]; // save it
	Real volUp=oldVol*(1.0+(Real)GREEKAPPROX),volDown=oldVol*(1.0-(Real)GREEKAPPROX);
	_volatilities[secIndexInSpotArray]=volUp;
	haveClosedFormVariablesBeenComputed=false;
	priceup=getPrice(priceMethod);// even if we do not have the closed form, the switch should get the MC
	_volatilities[secIndexInSpotArray]=volDown;
	haveClosedFormVariablesBeenComputed=false;
	pricedown=getPrice(priceMethod);// even if we do not have the closed form, the switch should get the MC
	_volatilities[secIndexInSpotArray]=oldVol; // put it back
	haveClosedFormVariablesBeenComputed=false;

	_outputMsgs=outputmsgstate; // where it was
	return (priceup-pricedown)/(2.0*(Real)GREEKAPPROX);
}

Real RainbowOption::getDelta(priceType priceMethod){
	Matrix partialDeltas=Matrix(0.0,_NumberOfAssets,1);
	for(Natural i=0;i<_NumberOfAssets;i++){
		partialDeltas.SetValue(i,0,getPartialDelta(i,priceMethod));
	}
	return partialDeltas.SumColumn(0);
}

Real RainbowOption::getGamma(priceType priceMethod){
	Matrix partialGammas=Matrix(0.0,_NumberOfAssets,1);
	for(Natural i=0;i<_NumberOfAssets;i++){
		partialGammas.SetValue(i,0,getPartialGamma(i,priceMethod));
	}
	return partialGammas.SumColumn(0);
}

Real RainbowOption::getVega(priceType priceMethod){
	Matrix partialVegas=Matrix(0.0,_NumberOfAssets,1);
	for(Natural i=0;i<_NumberOfAssets;i++){
		partialVegas.SetValue(i,0,getPartialVega(i,priceMethod));
	}
	return partialVegas.SumColumn(0);
}

Real RainbowOption::getCorrelRisk(priceType priceMethod){
	bool outputmsgstate=_outputMsgs ;
	_outputMsgs=false; // in case we are in the case where user wants to see things
	Real priceup,pricedown;

	Matrix oldCorrel=_CorrelationMatrix; // save it
	Natural size=(Natural)_CorrelationMatrix.GetRows();
	Matrix corrup=_CorrelationMatrix,corrdown=_CorrelationMatrix;
	Real multup=(1.0+(Real)GREEKAPPROX);
	Real multdown=(1.0-(Real)GREEKAPPROX);
	Real tempmult;//if corr =1 or -1, we cannot go beyond...
	for(Natural i=0;i<size;i++){
		for(Natural j=0;j<size;j++){
			if(i!=j){
				if(_CorrelationMatrix(i,j)==1){
					tempmult=multup;
					multup=1;
				}
				else if(_CorrelationMatrix(i,j)==-1){
					tempmult=multdown ;
					multdown =1;
				}
				corrup.SetValue(i,j,multup*_CorrelationMatrix(i,j));
				corrdown.SetValue(i,j,multdown*_CorrelationMatrix (i,j));

				if(_CorrelationMatrix(i,j)==1)
					multup=tempmult;
				else if(_CorrelationMatrix(i,j)==-1)
					multdown =tempmult;
			}
		}
	}
	
	_CorrelationMatrix =corrup;
	haveClosedFormVariablesBeenComputed=false;
	priceup=getPrice(priceMethod);// even if we do not have the closed form, the switch should get the MC
	_CorrelationMatrix =corrdown;
	haveClosedFormVariablesBeenComputed=false;
	pricedown=getPrice(priceMethod);// even if we do not have the closed form, the switch should get the MC
	_CorrelationMatrix =oldCorrel; // put it back
	haveClosedFormVariablesBeenComputed=false;

	_outputMsgs=outputmsgstate; // where it was
	return (priceup-pricedown)/(2.0*(Real)GREEKAPPROX);
}

Real RainbowOption::getRho(priceType priceMethod){
	bool outputmsgstate=_outputMsgs ;
	_outputMsgs=false; // in case we are in the case where user wants to see things
	Real priceup,pricedown;

	yieldCurve oldYc=_yc; // save it
	yieldCurve ycup=_yc.shiftZCBRateCurve(0.001),ycdown=_yc.shiftZCBRateCurve(-0.001);

	_yc =ycup;
	haveClosedFormVariablesBeenComputed=false;
	priceup=getPrice(priceMethod);// even if we do not have the closed form, the switch should get the MC
	_yc =ycdown;
	haveClosedFormVariablesBeenComputed=false;
	pricedown=getPrice(priceMethod);// even if we do not have the closed form, the switch should get the MC
	_yc =oldYc; // put it back
	haveClosedFormVariablesBeenComputed=false;

	_outputMsgs=outputmsgstate; // where it was
	return (priceup-pricedown)/(2.0*(Real)GREEKAPPROX);
}

Real RainbowOption::getTheta(priceType priceMethod){
	Real timeStep=1/365.0;
	bool outputmsgstate=_outputMsgs ;
	_outputMsgs=false; // in case we are in the case where user wants to see things
	Real priceup,pricedown;

	Real oldMat=_expiryInYears; // save it
	Real up=_expiryInYears+timeStep,down=_expiryInYears-timeStep;

	_expiryInYears =up;
	haveClosedFormVariablesBeenComputed=false;
	priceup=getPrice(priceMethod);// even if we do not have the closed form, the switch should get the MC
	_expiryInYears =down;
	haveClosedFormVariablesBeenComputed=false;
	pricedown=getPrice(priceMethod);// even if we do not have the closed form, the switch should get the MC
	_expiryInYears =oldMat; // put it back
	haveClosedFormVariablesBeenComputed=false;

	_outputMsgs=outputmsgstate; // where it was
	return (priceup-pricedown)/(2.0*timeStep);	
}


void RainbowOption::instanciateMCVariables()
{
	Natural i;
	_Drifts.resize(_NumberOfAssets);
	for (i=0;i<_NumberOfAssets;i++){
		_Drifts[i]=Drift(_startDate,_expiryInYears,_yc.spotRate(_expiryInYears),_volatilities[i]);
	}
	_thePayOff.SetStrike(_Strike);
	_pRandom->SetSeed(RO_SEED);
	_gaussianSample=0.;
	_TerminalPoints.resize(_NumberOfAssets);
	_pHazardRateProcesses.resize(_NumberOfAssets);
	for(i=0;i<_NumberOfAssets;i++)
	{
		_pHazardRateProcesses[i]=GaussianProcess(_Drifts[i].GetvDates(),1,_spots[i],_Drifts[i].GetvDrift(),0.0,_volatilities[i]);
	}
}

void RainbowOption::reassignVolsAtThestrike(){
	for (Natural i=0;i<_NumberOfAssets;i++)
		_volatilities[i]=_volatilitiesSurfaces[i].volatility(_Strike,_startDate.plusDays((Natural)(365*_expiryInYears)));

}

void RainbowOption::reassignVolsAtThemoney(){
	for (Natural i=0;i<_NumberOfAssets;i++)// ATM is the more probable approximation
		_volatilities[i]=_volatilitiesSurfaces[i].volatility(_spots[i],_startDate.plusDays((Natural)(365*_expiryInYears)));
}

Real RainbowOption::PriceByMc_2SpreadOptionMax(LongNatural nPaths)
{
	// vol is to be adjusted atm, as W1*S1-W2*S2-K, the K has nothing to do with strike in the vol sense
	//reassignVolsAtThemoney();
	instanciateMCVariables();
	
	_MCEngine=MCEngine(nPaths,_DFTomaturity);
	_MCEngine.RunEngineRainbow2SpreadOptionMax(_pRandom,_pHazardRateProcesses,_thePayOff,_gaussianSample,_TerminalPoints,_weights,_CorrelationMatrix,_Multiplier);
	//reassignVolsAtThestrike();// put them back
	return _MCEngine.MCResult();
}

Real RainbowOption::PriceByMc_2AssetsBasketMax(LongNatural nPaths)
{
	instanciateMCVariables();
	
	_MCEngine=MCEngine(nPaths,_DFTomaturity);
	_MCEngine.RunEngineRainbow2AssetsBasketMax(_pRandom,_pHazardRateProcesses,_thePayOff,_gaussianSample,_TerminalPoints,_weights,_CorrelationMatrix,_Multiplier);
	return _MCEngine.MCResult();
}

Real RainbowOption::PriceByMc_BestOf2AssetsCash(LongNatural nPaths)
{
	instanciateMCVariables();
	
	_MCEngine=MCEngine(nPaths,_DFTomaturity);
	_MCEngine.RunEngineRainbowBestOf2AssetsCash(_pRandom,_pHazardRateProcesses,_thePayOff,_gaussianSample,_TerminalPoints,_weights,_CorrelationMatrix);
	return _MCEngine.MCResult();
}

Real RainbowOption::PriceByMc_WorstOf2AssetsCash(LongNatural nPaths)
{
	instanciateMCVariables();
	
	_MCEngine=MCEngine(nPaths,_DFTomaturity);
	_MCEngine.RunEngineRainbowWorstOf2AssetsCash(_pRandom,_pHazardRateProcesses,_thePayOff,_gaussianSample,_TerminalPoints,_weights,_CorrelationMatrix);
	return _MCEngine.MCResult();
}

Real RainbowOption::PriceByMc_BetterOf2Assets(LongNatural nPaths){
	//a bettor of is a max Call with stike 0
	//reassignVolsAtThemoney();
	Real tempStrike=_Strike;
	_Strike=EPSILON;
	instanciateMCVariables();
	
	Real result=PriceByMc_Max2AssetsCall(nPaths);
	_Strike=tempStrike; // put it back
	//reassignVolsAtThestrike();
	return result;

}
Real RainbowOption::PriceByMc_WorseOf2Assets(LongNatural nPaths){
	//a worse of is a min Call with stike 0
	//reassignVolsAtThemoney();
	Real tempStrike=_Strike;
	_Strike=EPSILON;
	instanciateMCVariables();
	Real result=PriceByMc_Min2AssetsCall(nPaths);
	_Strike=tempStrike; // put it back
	//reassignVolsAtThestrike();
	return result;
}

Real RainbowOption::PriceByMc_Max2AssetsCall(LongNatural nPaths)
{
	instanciateMCVariables();

	_MCEngine=MCEngine(nPaths,_DFTomaturity);
	_MCEngine.RunEngineRainbowMax2AssetsCall(_pRandom,_pHazardRateProcesses,_thePayOff,_gaussianSample,_TerminalPoints,_weights,_CorrelationMatrix,_Multiplier);
	return _MCEngine.MCResult();
}

Real RainbowOption::PriceByMc_Min2AssetsCall(LongNatural nPaths)
{
	instanciateMCVariables();

	_MCEngine=MCEngine(nPaths,_DFTomaturity);
	_MCEngine.RunEngineRainbowMin2AssetsCall(_pRandom,_pHazardRateProcesses,_thePayOff,_gaussianSample,_TerminalPoints,_weights,_CorrelationMatrix,_Multiplier);
	return _MCEngine.MCResult();
}

Real RainbowOption::PriceByMc_Max2AssetsPut(LongNatural nPaths)
{
	instanciateMCVariables();
	
	_MCEngine=MCEngine(nPaths,_DFTomaturity);
	_MCEngine.RunEngineRainbowMax2AssetsPut(_pRandom,_pHazardRateProcesses,_thePayOff,_gaussianSample,_TerminalPoints,_weights,_CorrelationMatrix,_Multiplier);
	return _MCEngine.MCResult();
}

Real RainbowOption::PriceByMc_Min2AssetsPut(LongNatural nPaths)
{
	instanciateMCVariables();
	
	_MCEngine=MCEngine(nPaths,_DFTomaturity);
	_MCEngine.RunEngineRainbowMin2AssetsPut(_pRandom,_pHazardRateProcesses,_thePayOff,_gaussianSample,_TerminalPoints,_weights,_CorrelationMatrix,_Multiplier);
	return _MCEngine.MCResult();
}

void RainbowOption::compute_sigmaA(){
	Real vol1,vol2,rho;
	vol1=_volatilities[0];
	vol2=_volatilities[1];
	rho=_CorrelationMatrix(0,1);
	sigmaA=sqrt(pow(vol1,2)+pow(vol2,2)-2*rho*vol1*vol2);
}

void RainbowOption::compute_rho1(){
	Real vol1,vol2,rho;
	vol1=_volatilities[0];
	vol2=_volatilities[1];
	rho=_CorrelationMatrix(0,1);
	rho1=(rho*vol2-vol1)/sigmaA;
}

void RainbowOption::compute_rho2(){
	Real vol1,vol2,rho;
	vol1=_volatilities[0];
	vol2=_volatilities[1];
	rho=_CorrelationMatrix(0,1);
	rho2=(rho*vol1-vol2)/sigmaA;
}

void RainbowOption::compute_d1(){
	Real S1,vol1,rate;
	S1=_spots[0];
	vol1=_volatilities[0];
	rate=_yc.spotRate(_expiryInYears);
	//if K = 0 like in some BO asset (no cash, the d1 should go to -infinity)
	d1=(log(S1/_Strike)+(rate+0.5*pow(vol1,2))*_expiryInYears)/(vol1*sqrt(_expiryInYears));
}

void RainbowOption::compute_d2(){
	Real S2,vol2,rate;
	S2=_spots[1];
	vol2=_volatilities[1];
	rate=_yc.spotRate(_expiryInYears);
	//if K = 0 like in some BO asset (no cash, the d1 should go to -infinity)
	d2=(log(S2/_Strike)+(rate+0.5*pow(vol2,2))*_expiryInYears)/(vol2*sqrt(_expiryInYears));
}

void RainbowOption::compute_d3(){
	Real S1,S2;
	S1=_spots[0];
	S2=_spots[1];
	//if K = 0 like in some BO asset (no cash, the d1 should go to -infinity)
	d3=(log(S1/S2)+(0.5*pow(sigmaA,2))*_expiryInYears)/(sigmaA*sqrt(_expiryInYears));
}

void RainbowOption::compute_d4(){
	Real S1,S2;
	S1=_spots[0];
	S2=_spots[1];
	//if K = 0 like in some BO asset (no cash, the d1 should go to -infinity)
	d4=(log(S2/S1)+(0.5*pow(sigmaA,2))*_expiryInYears)/(sigmaA*sqrt(_expiryInYears));
}

void RainbowOption::compute_A(){
	Real S1=_spots[0];
	Real cum3=CumulativeNormal(d3);
	A=S1*(cum3-CumulativeBivariateNormal(-d1,d3,rho1));
}

void RainbowOption::compute_B(){
	Real S2=_spots[1];
	Real cum4=CumulativeNormal(d4);
	B=S2*(cum4-CumulativeBivariateNormal(-d2,d4,rho2));
}

void RainbowOption::compute_C(){
	Real vol1,vol2,rate,a,b,rho;
	vol1=_volatilities[0];
	vol2=_volatilities[1];
	rate=_yc.spotRate(_expiryInYears);
	a=-d1+vol1*sqrt(_expiryInYears);
	b=-d2+vol2*sqrt(_expiryInYears);
	rho=_CorrelationMatrix(0,1);
	C=_Strike*exp(-rate*_expiryInYears)*CumulativeBivariateNormal(a,b,rho);
}

void RainbowOption::compute_ClosedFormsParameters(){
	compute_sigmaA();
	compute_rho1();
	compute_rho2();
	compute_d1();
	compute_d2();
	compute_d3();
	compute_d4();
	compute_A();
	compute_B();
	compute_C();
}

Real RainbowOption::PriceByClosedForm_BestOf2_plusCash(){
	if(!haveClosedFormVariablesBeenComputed){
		compute_ClosedFormsParameters();
		haveClosedFormVariablesBeenComputed=true;
	}
	if(_outputMsgs)
		cout<<" Note to the user:\n This closed form solution of BestOf2+Cash reflects weight1=weight2=1 - MAX(S1,S2,Cash)"<<endl;
	return A + B + C;
}

Real RainbowOption::PriceByClosedForm_BetterOf2(){
	if(_outputMsgs)
		cout<<" Note to the user:\n This closed form solution of BetterOf2 reflects weight1=weight2=1 and K=0 - MAX(S1,S2,0)"<<endl;
	
	//reassignVolsAtThemoney();
	Real temp=_Strike; //store it
	_Strike=EPSILON;
	compute_ClosedFormsParameters();// need to recompute in this case
	haveClosedFormVariablesBeenComputed=false;//so that if we price another one we can reuse the real values by recomputing.
	_Strike=temp;//memorize it back as key variables have been computed.
	//reassignVolsAtThestrike();// and the vols for future use
	return A + B + C; // with strike = 0 (that puts d1=d2=1)
}

Real RainbowOption::PriceByClosedForm_WorseOf2(){
	if(_outputMsgs)
		cout<<" Note to the user:\n This closed form solution of WorstOf2 reflects weight1=weight2=1 and K=0 - MAX(MIN(S1,S2),0)"<<endl;

	Real S1,S2,vol1,vol2,rate,bscall1,bscall2,bo;
	S1=_spots[0];
	S2=_spots[1];
	vol1=_volatilities[0];
	vol2=_volatilities[1];
	rate=_yc.spotRate(_expiryInYears);
	BlackScholes bs1= BlackScholes(S1,vol1,true,rate,EPSILON,_expiryInYears,Call);
	bscall1=bs1.getPrice();
	BlackScholes bs2= BlackScholes(S2,vol2,true,rate,EPSILON,_expiryInYears,Call);
	bscall2=bs2.getPrice();
	bo=PriceByClosedForm_BetterOf2();

	return bscall1+bscall2-bo;
}

Real RainbowOption::PriceByClosedForm_MaxOf2_call(){
	if(!haveClosedFormVariablesBeenComputed){
		compute_ClosedFormsParameters();
		haveClosedFormVariablesBeenComputed=true;
	}
	if(_outputMsgs)
		cout<<" Note to the user:\n This closed form solution of MaxOf2_call reflects multiplier=weight1=weight2=1 - MAX(MAX(S1,S2)-K,0)"<<endl;

	Real rate= _yc.spotRate(_expiryInYears);
	return A + B + C - _Strike*exp(-rate*_expiryInYears);
}

Real RainbowOption::PriceByClosedForm_MinOf2_call(){
	if(_outputMsgs)
		cout<<" Note to the user:\n This closed form solution of MinOf2_call reflects multiplier=weight1=weight2=1 - MAX(MIN(S1,S2)-K,0)"<<endl;

	Real S1,S2,vol1,vol2,rate,bscall1,bscall2,maxcall;
	S1=_spots[0];
	S2=_spots[1];
	vol1=_volatilities[0];
	vol2=_volatilities[1];
	rate=_yc.spotRate(_expiryInYears);
	BlackScholes bs1= BlackScholes(S1,vol1,true,rate,_Strike,_expiryInYears,Call);
	bscall1=bs1.getPrice();
	BlackScholes bs2= BlackScholes(S2,vol2,true,rate,_Strike,_expiryInYears,Call);
	bscall2=bs2.getPrice();
	maxcall=PriceByClosedForm_MaxOf2_call();

	return bscall1+bscall2-maxcall;
}

Real RainbowOption::PriceByClosedForm_MaxOf2_put(){
	if(!haveClosedFormVariablesBeenComputed){
		compute_ClosedFormsParameters();
		haveClosedFormVariablesBeenComputed=true;
	}
	if(_outputMsgs)
		cout<<" Note to the user:\n This closed form solution of MaxOf2_put reflects multiplier=weight1=weight2=1 - MAX(K-MAX(S1,S2),0)"<<endl;
	Real rate= _yc.spotRate(_expiryInYears);
	return PriceByClosedForm_MaxOf2_call()-PriceByClosedForm_BetterOf2()+ _Strike*exp(-rate*_expiryInYears);
}

Real RainbowOption::PriceByClosedForm_MinOf2_put(){
	if(_outputMsgs)
		cout<<" Note to the user:\n This closed form solution of MinOf2_put reflects multiplier=weight1=weight2=1 - MAX(K-MIN(S1,S2),0)"<<endl;
	Real S1,S2,vol1,vol2,rate,bsput1,bsput2;
	S1=_spots[0];
	S2=_spots[1];
	vol1=_volatilities[0];
	vol2=_volatilities[1];
	rate=_yc.spotRate(_expiryInYears);
	BlackScholes bs1= BlackScholes(S1,vol1,true,rate,_Strike,_expiryInYears,Put);
	bsput1=bs1.getPrice();
	BlackScholes bs2= BlackScholes(S2,vol2,true,rate,_Strike,_expiryInYears,Put);
	bsput2=bs2.getPrice();

	return bsput1+bsput2-PriceByClosedForm_MaxOf2_put();
}

RainbowOption::~RainbowOption(void)
{
	//Should be deleted but if I do, I cannot price...
	//delete _pRandom; 
	//delete _MCEngine;
}