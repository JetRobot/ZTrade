#pragma once
#ifndef PARKMILLER_H
#define PARKMILLER_H

#include "../../common/types.h"
#include "RandomGenerator.h"

const LongInteger a = 16807;
const LongInteger m = 2147483647;
const LongInteger q = 127773;
const LongInteger r = 2836;

class ParkMiller : public RandomGenerator
{
public:
    
	/** Default constructor: initialize variable */
    ParkMiller(LongNatural Seed_=0);
	~ParkMiller();

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

#endif
