#include ".\yieldCurve.h"

/*------------------------------------------------------*/
/*                 Yield Point Implementation           */
/*------------------------------------------------------*/

yieldPoint::yieldPoint(void) {
   _rate     = 0.0;
   _maturity = 0.0;
   _type     = Cash;
   _dayCount = ACT_360;
}

yieldPoint::yieldPoint(Real r,Real T,TypeOfRate type,DayCountConvention dayCount)
{
   _rate     = r;
   _maturity = T;
   _type     = type;
   _dayCount = dayCount;                              
}


char *yieldPoint::TypeAsString(TypeOfRate t)
{
	switch (t) {
		case Cash:
			return "Cash";
		case Swap:
			return "Swap";
		default:
			return "No rate type has been defined";
	}
}

yieldPoint::~yieldPoint(void)
{
};

/*------------------------------------------------------*/
/*                 Yield Curve Implementation           */
/*------------------------------------------------------*/
yieldCurve::yieldCurve(void)
:_marketRates(YC_MAX_NUMBER_POINTS),
 _zcbRates(YC_MAX_NUMBER_POINTS)
{
      sprintf(_name, "unamed");
      /** @TODO clean ? */
      assignFlatRate();
      computeZCBRatesBootstrap();
}


void yieldCurve::assignFlatRate(Real r)
{
     Natural size = _marketRates.size();
     for(Natural i=0;i<size;i++){
         _marketRates[i].setRate(r);       
		 _marketRates[i].setDayCount(ACT_360);   
		 _marketRates[i].setType(Cash);   
		 _marketRates[i].setMaturity(i);   
     }    
}

yieldCurve::yieldCurve(Real flatRate)
:_marketRates(YC_MAX_NUMBER_POINTS),
 _zcbRates(YC_MAX_NUMBER_POINTS)
{
      sprintf(_name, "unamed");
      /** @TODO clean ?*/
      assignFlatRate(flatRate);  
      computeZCBRatesBootstrap();                         
}

yieldCurve::yieldCurve(valarray<yieldPoint> yieldPoints,char *name)
:_marketRates(yieldPoints),
 _zcbRates(yieldPoints) //default they are equal
{
      strcpy(_name,name);
	  sortMarketRatesByMaturity();
	  sortCashSwap();
      /** @TODO clean ?*/
      computeZCBRatesBootstrap();
}

yieldCurve::~yieldCurve(void)
{
}

yieldPoint yieldCurve::getPointAtMaturity(Real maturity)
{
	Natural size =_marketRates.size(),i=0,indexFound=0;
	yieldPoint y;
	bool hasFound=false;

	while (i<size && !hasFound){
		if(_marketRates[i].getMaturity()==maturity){
			hasFound=true;
			indexFound =i;
		}
		i++;
	}
	if(hasFound){
		y=_marketRates[indexFound];
	}
	else{
		y=yieldPoint();
	}
	return y;
}


void yieldCurve::sortMarketRatesByMaturity()
{
	Natural size = _marketRates.size();
	valarray<yieldPoint> tempstorage(size);
	Real toSort[YC_MAX_NUMBER_POINTS];
	for (Natural i=0;i<size;i++) 
	{
		toSort[i]=_marketRates[i].getMaturity();
	}
	sort(toSort,toSort+size);
	// toSort now is sorted by ascending maturity
	for (i=0;i<size;i++) 
	{
		tempstorage[i]=getPointAtMaturity(toSort[i]);
		// we just have to assign the yieldPoints corresponding to the maturity sorted
		// no need to handle not found maturities here as we know that the maturities in toSort
		// are bijectively taken from _marketRates
	}
	_marketRates = tempstorage;
}


void yieldCurve::sortCashSwap()
{
	valarray<yieldPoint> tempstorage(_marketRates.size());
	Natural count=0;
	for (Natural i=0;i<_marketRates.size();i++)
	{
		if(_marketRates[i].getType()==Cash){
			tempstorage[count]=_marketRates[i];
			count++;
		}
	}
	for (Natural i=0;i<_marketRates.size();i++)
	{
		if(_marketRates[i].getType()==Swap){
			tempstorage[count]=_marketRates[i];
			count++;
		}
	}
	_marketRates=tempstorage;
}

valarray<yieldPoint> yieldCurve::getSwapRates()
{
	valarray<yieldPoint> tempstorage(YC_MAX_NUMBER_POINTS);
	tempstorage[0]=yieldPoint(spotRate(1),1,Swap); //1Y swap rate = 1y cash rate as (1+s1Y)*DF1=1
	Natural i=0,count=1;
	while(i<_marketRates.size() && _marketRates[i].getType()==Cash) {i++;} //get to the first swap rate of the array
	while(i<_marketRates.size())
	{
		tempstorage[count]=_marketRates[i];
		count++;
		i++;
	}

	tempstorage.resize(count);
	return tempstorage;
}

valarray<yieldPoint> yieldCurve::getSequentSwapRates()
{
	/** Swaps are quoted per year sequent then gaps : we need to fill in the gaps with a linear interpolation*/
	valarray<yieldPoint> tempstorage(getSwapRates());
	Natural size=tempstorage.size();
	valarray<yieldPoint> res((Natural)tempstorage[size-1].getMaturity());
	Natural i=1,diff=0,j=0,nextMat=0,prevMat=0,start=0;
	/** we know that the first one is in 1Y swap rate (assigned at 1Y cash rate), as for the rest we ahve to 
	Check for 1y sequent swap rates first*/
	while(i<size && tempstorage[i-1].getMaturity()==(tempstorage[i].getMaturity()-1)) {
		res[j]=tempstorage[i-1];
		i++;
		j++;
	}
	start=j;
	// set natural year maturities from 1 up to the last swap maturity 
//and tests if maturities are sequent or not
	while (j<res.size() && i<size){	
		if(tempstorage[i].getMaturity()==(Real)(j+1)){
			res[j]=tempstorage[i];
			i++;
		}
		else{
			res[j].setMaturity((Real)(j+1)); 
			res[j].setType(Swap);
			nextMat=(Natural)tempstorage[i].getMaturity();
			prevMat=(Natural)tempstorage[i-1].getMaturity();
			diff= nextMat-prevMat;
			res[j].setRate( (tempstorage[i-1].getRate()*(nextMat-j-1)+  tempstorage[i].getRate()*(j+1-prevMat )) /diff);
		}	
		j++;
	}
	return res;	
}

Real yieldCurve::spotRate(Real maturity) const
{
      Natural i =0;
      while(i<_zcbRates.size() &&  _zcbRates[i].getMaturity()<maturity ) {i++;}
      if (i==_zcbRates.size())
			//longer term than we know : best approximation is the last Point
            return _zcbRates[i-1].getRate(); 
      else if(i==0)
			// we are looking for a rate of a shorter term : best approximation is the 1st rate on the curve
            return _zcbRates[i].getRate();
      else{
			// linear Naturalepolation
            Real rmin=_zcbRates[i-1].getRate();
            Real rmax=_zcbRates[i].getRate();
            Real tmin=_zcbRates[i-1].getMaturity();
            Real tmax=_zcbRates[i].getMaturity();
            
            //in the zcb calculation we get sure that there is no redundant rate
            return ( (tmax-maturity)*rmin + (maturity-tmin)*rmax )/(tmax-tmin);
      }
}

Real yieldCurve::spotRate(Date maturityDate) const {
	Date today=Date();
	today.setDateToToday();
	return  spotRate(today.dayCount(maturityDate,Day30_360));
}

Real yieldCurve::discountFactor(Real maturity,interestComposition composition) // continuously compound for now - see dayCOunt later
{
	switch (composition) {
		  case Discrete:
			  return (Real)pow(1/(1+spotRate(maturity)),maturity);
		  case Continuous:
			  return (Real)exp(-maturity*spotRate(maturity));
		  default:
			  return (Real)exp(-maturity*spotRate(maturity));
	}
}

Real yieldCurve::discountFactor(Date maturityDate,interestComposition composition){
	Date today=Date();
	today.setDateToToday();
	return discountFactor(today.dayCount(maturityDate,Day30_360),composition);
}

Real yieldCurve::forwardDiscountFactor(Real forwardstart,Real lengthofcontractafterstart,interestComposition composition){
	switch (composition) {
		  case Discrete:
			  return (Real)pow(1/(1+forwardRate(forwardstart,lengthofcontractafterstart,composition)),lengthofcontractafterstart);
		  case Continuous:
			  return (Real)exp(-lengthofcontractafterstart*forwardRate(forwardstart,lengthofcontractafterstart,composition));
		  default:
			  return (Real)exp(-lengthofcontractafterstart*forwardRate(forwardstart,lengthofcontractafterstart,composition));
	}	
}

Real yieldCurve::forwardRate(Real forwardStart,Real effectiveLengthOfTheContractAfterStart,interestComposition composition)
{
	switch (composition) {
	case Discrete:
		return (Real)(pow(discountFactor(forwardStart,composition)/discountFactor(forwardStart+effectiveLengthOfTheContractAfterStart,composition),1/effectiveLengthOfTheContractAfterStart)-1) ;
	case Continuous:
		return (Real) (spotRate(forwardStart+effectiveLengthOfTheContractAfterStart)*(forwardStart+effectiveLengthOfTheContractAfterStart)
			-spotRate(forwardStart)*forwardStart)/effectiveLengthOfTheContractAfterStart;
	default:
		return (Real) (spotRate(forwardStart+effectiveLengthOfTheContractAfterStart)*(forwardStart+effectiveLengthOfTheContractAfterStart)
			-spotRate(forwardStart)*forwardStart)/effectiveLengthOfTheContractAfterStart;
	}

}

yieldCurve yieldCurve::forwardZCBCurve(Real forwardStart){
	valarray<yieldPoint> fwdPts(_zcbRates);
	Natural i=0,size=fwdPts.size(),start;
	while(i<size && forwardStart>=_zcbRates[i].getRate())
		i++;
	if(i==size) //fwd term too high
		return yieldCurve();
	else{
		start=i;
		fwdPts.resize(size-start);
		for (Natural i=start;i<fwdPts.size();i++){
			fwdPts[i-start].setRate(forwardRate(forwardStart,_zcbRates[i].getMaturity()-forwardStart));
			fwdPts[i-start].setMaturity(_zcbRates[i].getMaturity()-forwardStart);
		}
		return yieldCurve(fwdPts,"forwardCurve");
	}

}

Real yieldCurve::forwardRate(Date forwardStart,Date forwardEnd,interestComposition composition){
	Date today=Date();
	today.setDateToToday();	
	return forwardRate(today.dayCount(forwardStart,Day30_360),forwardStart.dayCount(forwardEnd,Day30_360),composition);
}


valarray<Real> yieldCurve::SequentDiscountFactorsByInvertSwapMatrix()
{
	valarray<yieldPoint> tempstorage(getSequentSwapRates());

	Natural size = tempstorage.size();
	Matrix *M= new Matrix (0.0,size,size);
	for (Natural i=0; i < size; i++)  
	{
		for (Natural j=0; j<size; j++)
		{
			if(i==j) 
				M->SetValue(i,j,1+tempstorage[i].getRate()); // diagonals are 1 + s(i)
			else if (i>j)
				M->SetValue(i,j,tempstorage[i].getRate()); // inferior triangle are s(i)
			else
				M->SetValue(i,j,0); // superior triangle are 0
		}
	}
	M->Invert();//M now contains the invert of the swap formatted matrix
	valarray<Real> res(size);
	for(i=0;i<size;i++){
		res[i]=M->SumRow (i);
	}
	delete M;

	return res;
}

valarray<Real> yieldCurve::getMaturitiesInTheMarketCurve() const
{
	Natural size=_marketRates.size();
	valarray<Real> maturities(size);
	for (Natural i=0;i<size;i++){
		maturities[i]=_marketRates[i].getMaturity();
	}
	return maturities;
}

valarray<Real> yieldCurve::getMaturitiesInTheZCBCurve() const
{
	Natural size= _zcbRates.size();
	valarray<Real> maturities(size);
	for (Natural i=0;i<size;i++){
		maturities[i]=_zcbRates[i].getMaturity();
	}
	return maturities;
}


void yieldCurve::computeZCBRatesBootstrap()
{
    _zcbRates=_marketRates;
	
	if(_marketRates[_marketRates.size()-1].getType()!=Cash){
		// if the last rate (as it is sorted) of the mkt is a cash rate, then it means that all the curve is already ZCB'd
		valarray<Real> DFs(SequentDiscountFactorsByInvertSwapMatrix());
		Natural start=0,size = DFs.size();

		while (_zcbRates[start].getType()==Cash) {start ++;}
		start --; // as 1Y ZC = 1 Y swap
		_zcbRates.resize(size+start);
		for (Natural i=0;i<size;i++){
			
		
			//they are 1Y sequent swap rates
			_zcbRates[start+i].setRate((Real)pow(1/DFs[i],1/((Real)(i+1)))-1);// Rm = (1/DFm)^(1/m)
			_zcbRates[start+i].setMaturity((Real)i+1);
			_zcbRates[start+i].setType(Cash);
		}
	}
}

void yieldCurve::assignZCBrateAtIndex(Real rate,Natural i){
	if (i<_zcbRates.size())
		_zcbRates[i].setRate(rate); //else do nothing
}

yieldCurve yieldCurve::shiftZCBRateCurve(Real shift)
{
	char * name=new char[50];
	sprintf (name,"%f",shift);
	strcat (name," shift on the current ZCB curve");
	
	yieldCurve resultShifted=yieldCurve(_zcbRates,name);//new yc to output, already ZCB is good
	for (Natural i=0;i<_zcbRates.size();i++)
		resultShifted.assignZCBrateAtIndex(_zcbRates[i].getRate()+shift,i);
	return resultShifted;
}

/** to have the opposite move on the last rate vs short rate -- cf graph
i know it is a simplification as usually curves rotate around the 7y rate, but good starting point
 shift = a * mat(min) + b
 - shift = a * mat(max) + b
*/
yieldCurve yieldCurve::rotateZCBRateCurve(Real moveInShortestRate,Real maturityOfRotation){
	char * name=new char[64];
	sprintf (name,"%f",moveInShortestRate);
	strcat (name," move on short rate and rotation around 7Y");
	
	yieldCurve resultRotated=yieldCurve(_zcbRates,name);//new yc to output, already ZCB is good
	Real move=0.;
	Real difmat=_zcbRates[0].getMaturity()-min(2*maturityOfRotation,_zcbRates[_zcbRates.size()-1].getMaturity()); // <0
	Real summat=_zcbRates[0].getMaturity()+min(2*maturityOfRotation,_zcbRates[_zcbRates.size()-1].getMaturity());
	for (Natural i=0;i<_zcbRates.size();i++){
		move= (moveInShortestRate/difmat)*(2*_zcbRates[i].getMaturity()-summat);
		resultRotated.assignZCBrateAtIndex(_zcbRates[i].getRate()+move,i);
	}
	return resultRotated;	
}

/** compares two y curves.  Two y curves are equal if they give 
	identical spotrates for all ZCB maturities in each curve

	@param param - the creditcurve to compare to

	@returns true if all spotrates match, otherwise false
	*/
bool
yieldCurve::operator==(const yieldCurve& yours) {
	bool result = true;

	valarray<Real> myMaturities =
		getMaturitiesInTheZCBCurve();
	valarray<Real> yourMaturities =
		yours.getMaturitiesInTheZCBCurve();
	valarray<Real> termValues = 
		mergeunique(myMaturities, yourMaturities);
	Natural numTermValues = termValues.size();

	Real m = 0;
	Real y = 0;
	for (Natural i = 0; (i < numTermValues) && result; i++) {
		m = spotRate(termValues[i]);
		y = yours.spotRate(termValues[i]);

		// if difference is > .1 basis Points we say they are not equal
		// (a function of precision in the input file)
		if (fabs(m - y) > 0.00001) {
			cout << "mismatch m: " << m << " y: " << y << endl;
			result &= false;
		}
	}

	return result;
}

bool
yieldCurve::operator!=(const yieldCurve& yours) {
	return !operator==(yours);
}

ostream& 
operator << (ostream &os, const yieldCurve &c) {
	valarray<Real> myMaturities = 
		c.getMaturitiesInTheZCBCurve();
	Natural numTermValues = myMaturities.size();

	for (Natural i = 0; i < numTermValues; i++) {
		os << myMaturities[i] << "," << c.spotRate(myMaturities[i]) << endl;
	}

	return os;
}
  

