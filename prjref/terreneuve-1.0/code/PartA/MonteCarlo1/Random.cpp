#include ".\random.h"

Random::Random(LongNatural Dimensionality_,RandomGenerator* rndGen)
:	Dimensionality(Dimensionality_)
{
	InnerGenerator=rndGen;
//	Reciprocal = 1/(1.0+rndGen->Max());
}

Random::Random(RandomGenerator* rndGen){
	InnerGenerator=rndGen;
	Dimensionality=1;
}

Random* Random::clone() const
{
    return new Random(*this);   
}

void Random::GetUniforms(valarray<Real>& variates)
{
    for (LongNatural j=0; j < GetDimensionality(); j++)
        variates[j] = InnerGenerator->getUniform();
}

void Random::GetUniform(Real& variate)
{
    variate = InnerGenerator->getUniform();
}

void Random::Skip(LongNatural numberOfPaths)
{
    valarray<Real> tmp(GetDimensionality());
    for (LongNatural j=0; j < numberOfPaths; j++)
        GetUniforms(tmp);
}

void Random::SetSeed(LongNatural Seed)
{
    InitialSeed = Seed;
    InnerGenerator->SetSeed(Seed);
}

void Random::Reset()
{
    InnerGenerator->SetSeed(InitialSeed);
}


void Random::ResetDimensionality(LongNatural NewDimensionality)
{
    Dimensionality = NewDimensionality;
    InnerGenerator->SetSeed(InitialSeed);
}
