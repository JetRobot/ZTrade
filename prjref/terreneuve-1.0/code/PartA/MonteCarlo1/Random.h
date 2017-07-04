#pragma once
#ifndef RANDOM_H
#define RANDOM_H

#include "../../common/normals.h"
#include "../../common/types.h"
#include "RandomGenerator.h"

#include<valarray>
using namespace std;

class Random
{
public:

	/** Default constructor: set dimension */
    Random(LongNatural Dimensionality,RandomGenerator* rndGen);

	/** @author Yann if dimension 1 - ne need to specifty dimensionality*/
    Random(RandomGenerator* rndGen);

	/** Return dimension */
    inline LongNatural GetDimensionality() const;

	/** Clone function */
    Random* clone() const;
	/** Get uniforms Real */
    inline void GetUniforms(valarray<Real>& variates);

	/** @author Yann - Get uniform in a Real to avoid arrays */
    void GetUniform(Real& variate);

    void Skip(LongNatural numberOfPaths);
    void SetSeed(LongNatural Seed);
    void Reset();

	/** Get gaussian random numbers */
    inline void GetGaussians(valarray<Real>& variates);
	/** @author Yann - Get gaussian random number in a Real to avoid array */
    inline void GetGaussian(Real& variate);
    void ResetDimensionality(LongNatural NewDimensionality);

private:
    RandomGenerator* InnerGenerator;
    LongNatural Dimensionality;
    LongNatural InitialSeed;
    Real Reciprocal;
};

LongNatural Random::GetDimensionality() const
{
    return Dimensionality;
}

inline void Random::GetGaussians(valarray<Real>& variates)
{
    GetUniforms(variates);

    for (LongNatural i=0; i < Dimensionality; i++)
    {
        Real x=variates[i];
        variates[i] = InverseCumulativeNormal(x);    
    }
}

inline void Random::GetGaussian(Real& variate)
{
    GetUniform(variate);
    Real x=variate;
    variate = InverseCumulativeNormal(x);    
}

#endif
