#include "./MersenneTwister.h"
#include "time.h"

// Period parameters
const LongNatural N = 624;
const LongNatural M = 397;
// constant vector a
const LongNatural MATRIX_A = 0x9908b0dfUL;
// most significant w-r bits
const LongNatural UPPER_MASK=0x80000000UL;
// least significant r bits
const LongNatural LOWER_MASK=0x7fffffffUL;

MersenneTwister::MersenneTwister(LongNatural seed)
: mt(N)
{
	SetSeed(seed);
}

MersenneTwister::~MersenneTwister(void)
{
}

VeryLongNatural MersenneTwister::Max() {
	return 4294967296; //return 2^32;
}

LongNatural MersenneTwister::Min()
{
    return 1;
}

void MersenneTwister::SetSeed(LongNatural seed) {
	LongNatural s = (seed != 0 ? seed : clock());
	mt[0]= s & 0xffffffffUL;
	for (mti=1; mti<N; mti++) {
		mt[mti] = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
		mt[mti] &= 0xffffffffUL;
	}
}

MersenneTwister::MersenneTwister(const valarray<LongNatural>& seeds)
: mt(N) {
	SetSeed(19650218UL);
	LongNatural i=1, j=0, k = (N>seeds.size() ? N : seeds.size());
	for (; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
			+ seeds[j] + j; /* non linear */
		mt[i] &= 0xffffffffUL;
		i++; j++;
		if (i>=N) { mt[0] = mt[N-1]; i=1; }
		if (j>=seeds.size()) j=0;
	}
	for (k=N-1; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
			- i;
		mt[i] &= 0xffffffffUL;
		i++;
		if (i>=N) { mt[0] = mt[N-1]; i=1; }
	}

	mt[0] = 0x80000000UL;
}

LongNatural MersenneTwister::GetOneRandomInteger() {

	LongNatural y;
	static LongNatural mag01[2]={0x0UL, MATRIX_A};

	if (mti >= N) { /* generate N words at one time */
		LongNatural kk;

		for (kk=0;kk<N-M;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		for (;kk<N-1;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
		mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		mti = 0;
	}

	y = mt[mti++];

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}
