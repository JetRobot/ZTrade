#include ".\parkmiller.h"
#include "time.h"

ParkMiller::ParkMiller(LongNatural Seed_ ) : Seed(Seed_)
{
    Seed != 0 ? Seed : clock();
}

ParkMiller::~ParkMiller()
{
}
/*
void ParkMiller::SetSeed(LongInteger Seed_)
{
  Seed=Seed_;
  if (Seed ==0)
        Seed=1;
}
*/
VeryLongNatural ParkMiller::Max() {
    return m-1;
}

LongNatural ParkMiller::Min() {
    return 1;
}

LongNatural ParkMiller::GetOneRandomInteger() {
    LongInteger k;
    k=Seed/q;

	LongInteger Seed2;
    Seed2=a*(Seed-k*q)-r*k; 

    if (Seed2 < 0) 
        Seed2 += m;

	Seed=Seed2;
    return Seed;
}

Real ParkMiller::getUniform() {
	return (GetOneRandomInteger()-0.5)/Real(m-1);
}

void ParkMiller::SetSeed(LongNatural Seed_) {
  Seed=Seed_;
  if (Seed ==0)
        Seed=1;
}
