#include ".\mcengine.h"

MCEngine::MCEngine(LongNatural nPaths,LongNatural nDates,valarray<Real> DiscFactors)
					:	m_nDates(nDates),
						m_nPaths(nPaths)
{
	m_price=0.;
	m_DiscFactor=DiscFactors[nDates-1];
}

MCEngine::MCEngine(LongNatural nPaths,Real DFToMaturity)
					:	m_nPaths(nPaths),
						m_DiscFactor(DFToMaturity)
{
	m_nDates=1;
	m_price=0.;

}

MCEngine::~MCEngine()
{
}

MCEngine::MCEngine(void)
{
}

void MCEngine::RunEngineRainbow2SpreadOptionMax(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation,Real Mult)
{
	Natural size=Correlation.GetRows();
	Matrix independantShocks=Matrix(0.0,size,1);
	Matrix Cholesky=Correlation.CholeskyDecomposition();
	Matrix correlatedShocks=Matrix(0.0,size,1);
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		// generating N non correlated samples
		for(Natural j=0;j<size;j++){
			pRandom->GetGaussian(gaussianSample);
			independantShocks.SetValue(j,0,gaussianSample);
		}
		// recorrelated them
		correlatedShocks=Cholesky*independantShocks;
		for(Natural j=0;j<size;j++){
			TerminalPoints[j]=pHazardRateProcesses[j].BuildTerminalPoint((Real)correlatedShocks[j][0]);
		}
		
		m_price+=thePayOff.Rainbow2SpreadOptionMax(TerminalPoints[0],TerminalPoints[1],weights[0],weights[1],Mult)*m_DiscFactor;
	}
}

void MCEngine::RunEngineRainbow2AssetsBasketMax(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation,Real Mult)
{
	Natural size=Correlation.GetRows();
	Matrix independantShocks=Matrix(0.0,size,1);
	Matrix Cholesky=Correlation.CholeskyDecomposition();
	Matrix correlatedShocks=Matrix(0.0,size,1);
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		// generating N non correlated samples
		for(Natural j=0;j<size;j++){
			pRandom->GetGaussian(gaussianSample);
			independantShocks.SetValue(j,0,gaussianSample);
		}
		// recorrelated them
		correlatedShocks=Cholesky*independantShocks;
		for(Natural j=0;j<size;j++){
			TerminalPoints[j]=pHazardRateProcesses[j].BuildTerminalPoint((Real)correlatedShocks[j][0]);
		}
		
		m_price+=thePayOff.Rainbow2AssetsBasketMax(TerminalPoints[0],TerminalPoints[1],weights[0],weights[1],Mult)*m_DiscFactor;
	}
}


void MCEngine::RunEngineRainbowBestOf2AssetsCash(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation)
{
	Natural size=Correlation.GetRows();
	Matrix independantShocks=Matrix(0.0,size,1);
	Matrix Cholesky=Correlation.CholeskyDecomposition();
	Matrix correlatedShocks=Matrix(0.0,size,1);
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		// generating N non correlated samples
		for(Natural j=0;j<size;j++){
			pRandom->GetGaussian(gaussianSample);
			independantShocks.SetValue(j,0,gaussianSample);
		}
		// recorrelated them
		correlatedShocks=Cholesky*independantShocks;
		for(Natural j=0;j<size;j++){
			TerminalPoints[j]=pHazardRateProcesses[j].BuildTerminalPoint((Real)correlatedShocks[j][0]);
		}
		
		m_price+=thePayOff.RainbowBestOf2AssetsCash(TerminalPoints[0],TerminalPoints[1],weights[0],weights[1])*m_DiscFactor;
	}
}

void MCEngine::RunEngineRainbowWorstOf2AssetsCash(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation)
{
	Natural size=Correlation.GetRows();
	Matrix independantShocks=Matrix(0.0,size,1);
	Matrix Cholesky=Correlation.CholeskyDecomposition();
	Matrix correlatedShocks=Matrix(0.0,size,1);
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		// generating N non correlated samples
		for(Natural j=0;j<size;j++){
			pRandom->GetGaussian(gaussianSample);
			independantShocks.SetValue(j,0,gaussianSample);
		}
		// recorrelated them
		correlatedShocks=Cholesky*independantShocks;
		for(Natural j=0;j<size;j++){
			TerminalPoints[j]=pHazardRateProcesses[j].BuildTerminalPoint((Real)correlatedShocks[j][0]);
		}
		
		m_price+=thePayOff.RainbowWorstOf2AssetsCash(TerminalPoints[0],TerminalPoints[1],weights[0],weights[1])*m_DiscFactor;
	}
}

void MCEngine::RunEngineRainbowMax2AssetsCall(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation,Real Mult)
{
	Natural size=Correlation.GetRows();
	Matrix independantShocks=Matrix(0.0,size,1);
	Matrix Cholesky=Correlation.CholeskyDecomposition();
	Matrix correlatedShocks=Matrix(0.0,size,1);
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		// generating N non correlated samples
		for(Natural j=0;j<size;j++){
			pRandom->GetGaussian(gaussianSample);
			independantShocks.SetValue(j,0,gaussianSample);
		}
		// recorrelated them
		correlatedShocks=Cholesky*independantShocks;
		for(Natural j=0;j<size;j++){
			TerminalPoints[j]=pHazardRateProcesses[j].BuildTerminalPoint((Real)correlatedShocks[j][0]);
		}
		
		m_price+=thePayOff.RainbowMax2AssetsCall(TerminalPoints[0],TerminalPoints[1],weights[0],weights[1],Mult)*m_DiscFactor;
	}
}

void MCEngine::RunEngineRainbowMin2AssetsCall(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation,Real Mult)
{
	Natural size=Correlation.GetRows();
	Matrix independantShocks=Matrix(0.0,size,1);
	Matrix Cholesky=Correlation.CholeskyDecomposition();
	Matrix correlatedShocks=Matrix(0.0,size,1);
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		// generating N non correlated samples
		for(Natural j=0;j<size;j++){
			pRandom->GetGaussian(gaussianSample);
			independantShocks.SetValue(j,0,gaussianSample);
		}
		// recorrelated them
		correlatedShocks=Cholesky*independantShocks;
		for(Natural j=0;j<size;j++){
			TerminalPoints[j]=pHazardRateProcesses[j].BuildTerminalPoint((Real)correlatedShocks[j][0]);
		}
		
		m_price+=thePayOff.RainbowMin2AssetsCall(TerminalPoints[0],TerminalPoints[1],weights[0],weights[1],Mult)*m_DiscFactor;
	}
}

void MCEngine::RunEngineRainbowMax2AssetsPut(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation,Real Mult)
{
	Natural size=Correlation.GetRows();
	Matrix independantShocks=Matrix(0.0,size,1);
	Matrix Cholesky=Correlation.CholeskyDecomposition();
	Matrix correlatedShocks=Matrix(0.0,size,1);
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		// generating N non correlated samples
		for(Natural j=0;j<size;j++){
			pRandom->GetGaussian(gaussianSample);
			independantShocks.SetValue(j,0,gaussianSample);
		}
		// recorrelated them
		correlatedShocks=Cholesky*independantShocks;
		for(Natural j=0;j<size;j++){
			TerminalPoints[j]=pHazardRateProcesses[j].BuildTerminalPoint((Real)correlatedShocks[j][0]);
		}
		
		m_price+=thePayOff.RainbowMax2AssetsPut(TerminalPoints[0],TerminalPoints[1],weights[0],weights[1],Mult)*m_DiscFactor;
	}
}

void MCEngine::RunEngineRainbowMin2AssetsPut(Random* pRandom,valarray<GaussianProcess> pHazardRateProcesses,PayOff thePayOff,Real gaussianSample,valarray<Real> TerminalPoints,valarray<Real> weights,Matrix Correlation,Real Mult)
{
	Natural size=Correlation.GetRows();
	Matrix independantShocks=Matrix(0.0,size,1);
	Matrix Cholesky=Correlation.CholeskyDecomposition();
	Matrix correlatedShocks=Matrix(0.0,size,1);
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		// generating N non correlated samples
		for(Natural j=0;j<size;j++){
			pRandom->GetGaussian(gaussianSample);
			independantShocks.SetValue(j,0,gaussianSample);
		}
		// recorrelated them
		correlatedShocks=Cholesky*independantShocks;
		for(Natural j=0;j<size;j++){
			TerminalPoints[j]=pHazardRateProcesses[j].BuildTerminalPoint((Real)correlatedShocks[j][0]);
		}
		
		m_price+=thePayOff.RainbowMin2AssetsPut(TerminalPoints[0],TerminalPoints[1],weights[0],weights[1],Mult)*m_DiscFactor;
	}
}

void MCEngine::RunEngineAsianCall(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath)
{
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		pRandom->GetGaussians(gaussianSample);
		vHazardRatePath=pHazardRateProcess->BuildPath(gaussianSample);

		m_price+=thePayOff.AsianCall(vHazardRatePath,m_nDates)*m_DiscFactor;
	}
}

void MCEngine::RunEngineAsianPut(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath)
{
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		pRandom->GetGaussians(gaussianSample);
		vHazardRatePath=pHazardRateProcess->BuildPath(gaussianSample);

		m_price+=thePayOff.AsianPut(vHazardRatePath,m_nDates)*m_DiscFactor;
	}
}

void MCEngine::RunEngineCall(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray<Real> gaussianSample,valarray<Real> vHazardRatePath)
{
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		if (i % 2 == 0)
			pRandom->GetGaussians(gaussianSample);
		else {
			for (LongNatural j=0;j<gaussianSample.size();j++) {
				gaussianSample[j]=-gaussianSample[j];
			}
		}
		vHazardRatePath=pHazardRateProcess->BuildPath(gaussianSample);

		m_price+=thePayOff.Call(vHazardRatePath[0])*m_DiscFactor;
	}
}

void MCEngine::RunEnginePut(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath)
{
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		pRandom->GetGaussians(gaussianSample);
		vHazardRatePath=pHazardRateProcess->BuildPath(gaussianSample);

		m_price+=thePayOff.Put(vHazardRatePath[0])*m_DiscFactor;
	}
}

void MCEngine::RunEngineRevLookbackCall(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath)
{
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		pRandom->GetGaussians(gaussianSample);
		vHazardRatePath=pHazardRateProcess->BuildPath(gaussianSample);

		m_price+=thePayOff.RevLookbackCall(vHazardRatePath,m_nDates)*m_DiscFactor;
	}
}

void MCEngine::RunEngineRevLookbackPut(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath)
{
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		pRandom->GetGaussians(gaussianSample);
		vHazardRatePath=pHazardRateProcess->BuildPath(gaussianSample);

		m_price+=thePayOff.RevLookbackPut(vHazardRatePath,m_nDates)*m_DiscFactor;
	}
}

void MCEngine::RunEngineBarrierCall(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath)
{
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		pRandom->GetGaussians(gaussianSample);
		vHazardRatePath=pHazardRateProcess->BuildPath(gaussianSample);

		m_price+=thePayOff.BarrierCall(vHazardRatePath,m_nDates)*m_DiscFactor;
	}
}

void MCEngine::RunEngineBarrierPut(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath)
{
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		pRandom->GetGaussians(gaussianSample);
		vHazardRatePath=pHazardRateProcess->BuildPath(gaussianSample);

		m_price+=thePayOff.BarrierPut(vHazardRatePath,m_nDates)*m_DiscFactor;
	}
}

void MCEngine::RunEngineFlooredCliquet(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath)
{
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		pRandom->GetGaussians(gaussianSample);
		vHazardRatePath=pHazardRateProcess->BuildPath(gaussianSample);

		m_price+=thePayOff.FlooredCliquet(vHazardRatePath[0],vHazardRatePath[m_nDates])*m_DiscFactor;
	}
}

void MCEngine::RunEngineCappedCliquet(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray <Real> gaussianSample,valarray<Real> vHazardRatePath)
{
	for(LongNatural i=0;i<m_nPaths;++i)
	{
		pRandom->GetGaussians(gaussianSample);
		vHazardRatePath=pHazardRateProcess->BuildPath(gaussianSample);

		m_price+=thePayOff.CappedCliquet(vHazardRatePath[0],vHazardRatePath[m_nDates])*m_DiscFactor;
	}
}

void MCEngine::RunEngineGeneral(Random* pRandom,GaussianProcess* pHazardRateProcess,PayOff thePayOff,valarray<Real> gaussianSample,valarray<Real> vHazardRatePath,Natural Product)
{
	if (Product==1)
		this->RunEngineCall(pRandom,pHazardRateProcess,thePayOff,gaussianSample,vHazardRatePath);
	if (Product==2)
		this->RunEnginePut(pRandom,pHazardRateProcess,thePayOff,gaussianSample,vHazardRatePath);
	if (Product==3)
		this->RunEngineAsianCall(pRandom,pHazardRateProcess,thePayOff,gaussianSample,vHazardRatePath);
	if (Product==4)
		this->RunEngineAsianPut(pRandom,pHazardRateProcess,thePayOff,gaussianSample,vHazardRatePath);
	if (Product==5)
		this->RunEngineRevLookbackCall(pRandom,pHazardRateProcess,thePayOff,gaussianSample,vHazardRatePath);
	if (Product==6)
		this->RunEngineRevLookbackPut(pRandom,pHazardRateProcess,thePayOff,gaussianSample,vHazardRatePath);
	if (Product==7)
		this->RunEngineBarrierCall(pRandom,pHazardRateProcess,thePayOff,gaussianSample,vHazardRatePath);
	if (Product==8)
		this->RunEngineBarrierPut(pRandom,pHazardRateProcess,thePayOff,gaussianSample,vHazardRatePath);
	if (Product==9)
		this->RunEngineFlooredCliquet(pRandom,pHazardRateProcess,thePayOff,gaussianSample,vHazardRatePath);
	if (Product==10)
		this->RunEngineCappedCliquet(pRandom,pHazardRateProcess,thePayOff,gaussianSample,vHazardRatePath);
}

Real MCEngine::MCResult()
{
	return m_price/m_nPaths;
}