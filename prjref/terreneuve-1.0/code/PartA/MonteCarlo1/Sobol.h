#pragma once
#ifndef SOBOL_H
#define SOBOL_H

#include "./RandomGenerator.h"
#include "../../common/types.h"
#include <valarray>
using namespace std;

const Natural MAXBIT=30,MAXDIM=6;

class Sobol :
	public RandomGenerator
{
public:
	/** Default constructor */
	Sobol(LongNatural Seed_=0);
	~Sobol();

	void sobseq(Integer *n, Real x[]);
	
	/** Create one random integer */
    LongNatural GetOneRandomInteger();

	/** Creates one uniform number on (0.0,1.0) */
	Real getUniform();

	/** Set seed for generator */
    void SetSeed(LongNatural Seed);

	/** Return maximum number of random numbers */
    VeryLongNatural Max();
	/** Return minimum of numbers generated */
    LongNatural Min();

private:
    LongNatural Seed;
	Integer n_;
	Real x_[MAXDIM+1];
};

#endif
