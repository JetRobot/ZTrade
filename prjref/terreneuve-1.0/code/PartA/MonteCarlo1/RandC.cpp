#include "./randc.h"
#include "../../common/types.h"
#include <stdlib.h>
#include <time.h>

const Natural Maxim=32767;

RandC::RandC(LongNatural Seed_)
{
	Seed=Seed_;
	srand(time(NULL));
}

RandC::~RandC(void)
{
}

VeryLongNatural RandC::Max() {
    return Maxim;
}

LongNatural RandC::Min() {
    return 1;
}

LongNatural RandC::GetOneRandomInteger() {
    return rand()%Maxim+1;
}

Real RandC::getUniform() {
	return (GetOneRandomInteger()-0.5)/Real(Max());
}

void RandC::SetSeed(LongNatural Seed_) {
  Seed=Seed_;
}

