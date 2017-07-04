#include ".\randomgenerator.h"

RandomGenerator::RandomGenerator(LongNatural Seed_) : Seed(Seed_)
{
	SetSeed(Seed_);
}

RandomGenerator::~RandomGenerator(void)
{
}

void RandomGenerator::SetSeed(LongNatural Seed_)
{
  Seed=Seed_;
  if (Seed ==0)
        Seed=1;
}

Real RandomGenerator::getUniform()
{
	return Real(GetOneRandomInteger()-0.5)/Max();
}