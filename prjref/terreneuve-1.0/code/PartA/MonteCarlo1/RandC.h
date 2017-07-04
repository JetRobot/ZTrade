#pragma once
#include "./RandomGenerator.h"

class RandC :
	public RandomGenerator
{
public:

	/** Default constructor: initialize variable */
	RandC(LongNatural Seed_=0);
	~RandC(void);
	
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
};
