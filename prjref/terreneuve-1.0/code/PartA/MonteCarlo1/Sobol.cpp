#include ".\sobol.h"
#include <minmax.h>

Sobol::Sobol(LongNatural Seed_)
{
	Seed=Seed_;
	n_=-1;
	sobseq(&n_,x_);
	n_=1;
}

Sobol::~Sobol()
{
}

void Sobol::SetSeed(LongNatural Seed_) {
	Seed=Seed_;
	n_=-1;
	sobseq(&n_,x_);
	n_=1;
}

void Sobol::sobseq(Integer *n, Real x[])
{
	Natural j,k,l;
	LongInteger i,im,ipp;
	static LongNatural mdeg[MAXDIM+1]={0,1,2,3,3,4,4};
	static LongNatural in;
	static LongNatural ix[MAXDIM+1];
	static LongNatural *iu[MAXBIT+1];
	static LongNatural ip[MAXDIM+1]={0,0,1,1,2,1,4};
	static LongNatural iv[MAXDIM*MAXBIT+1]=
		{0,1,1,1,1,1,1,3,1,3,3,1,1,5,7,7,3,3,5,15,11,5,15,13,9};
	static Real fac;

	if (*n < 0) {
		for (k=1;k<MAXDIM;k++) ix[k]=0;
		in=0;
		if (iv[1] != 1) return;
		fac=1.0/(1L << MAXBIT);
		for (j=1,k=0;j<=MAXBIT;j++,k+=MAXDIM) iu[j] = &iv[k];
		for (k=1;k<=MAXDIM;k++) {
			for (j=1;j<=mdeg[k];j++) iu[j][k] <<= (MAXBIT-j);
			for (j=mdeg[k]+1;j<=MAXBIT;j++) {
				ipp=ip[k];
				i=iu[j-mdeg[k]][k];
				i ^= (i >> mdeg[k]);
				for (l=mdeg[k]-1;l>=1;l--) {
					if (ipp & 1) i ^= iu[j-l][k];
					ipp >>= 1;
				}
				iu[j][k]=i;
			}
		}
	} else {
		im=in++;
		for (j=1;j<=MAXBIT;j++) {
			if (!(im & 1)) break;
			im >>= 1;
		}
		if (j > MAXBIT)
			j=MAXBIT;
		im=(j-1)*MAXDIM;
		for (k=1;k<=min(*n,MAXDIM);k++) {
			ix[k] ^= iv[im+k];
			x[k]=ix[k]*fac;
		}
	}
}

VeryLongNatural Sobol::Max() {
    return 2;
}

LongNatural Sobol::Min() {
    return 1;
}

LongNatural Sobol::GetOneRandomInteger()
{
	return 1;
}

Real Sobol::getUniform()
{
	sobseq(&n_,x_);
	return x_[1];
}
