#pragma once
#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include "../../Common/types.h"

class RandomGenerator
{
public:

	/** Default constructor: initialize variable */
	RandomGenerator(LongNatural Seed_=0);
	virtual ~RandomGenerator(void);

	/** Create one random integer */
    virtual LongNatural GetOneRandomInteger()=0;

	/** Creates one uniform number on (0.0,1.0) */
	virtual Real getUniform()=0;

	/** Set seed for generator */
    virtual void SetSeed(LongNatural Seed);

	/** Return maximum number of random numbers */
    virtual VeryLongNatural Max()=0;
	/** Return minimum of numbers generated */
    virtual LongNatural Min()=0;

private:
    LongNatural Seed;
};


#endif
