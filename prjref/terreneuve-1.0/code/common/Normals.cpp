#include ".\normals.h"
/*
code to implement the basic distribution functions necessary in mathematical finance
via rational approximations
  */
 
#include <cmath>

// the basic math functions should be in namespace std but aren't in VCPP6
#if !defined(_MSC_VER)
using namespace std;
#endif

const Real OneOverRootTwoPi = 0.398942280401433; 

// probability density for a standard Gaussian distribution
Real NormalDensity(Real x)
{
    return OneOverRootTwoPi*exp(-x*x/2);
}

// the InverseCumulativeNormal function via the Beasley-Springer/Moro approximation
Real InverseCumulativeNormal(Real u)
{


    static Real a[4]={  2.50662823884,
                        -18.61500062529,
                         41.39119773534,
                        -25.44106049637};

    static Real b[4]={-8.47351093090,
                        23.08336743743,
                       -21.06224101826,
                         3.13082909833};

    static Real c[9]={0.3374754822726147,
                        0.9761690190917186,
                        0.1607979714918209,
                        0.0276438810333863,
                        0.0038405729373609,
                        0.0003951896511919,
                        0.0000321767881768,
                        0.0000002888167364,
                        0.0000003960315187};

    
    Real x=u-0.5;
    Real r;

    if (fabs(x)<0.42) // Beasley-Springer
    {
        Real y=x*x;
        
        r=x*(((a[3]*y+a[2])*y+a[1])*y+a[0])/
                ((((b[3]*y+b[2])*y+b[1])*y+b[0])*y+1.0);
               
    }
    else // Moro
    {
    
        r=u;
    
        if (x>0.0) 
            r=1.0-u;
        
        r=log(-log(r));
        
        r=c[0]+r*(c[1]+r*(c[2]+r*(c[3]+r*(c[4]+r*(c[5]+r*(c[6]+
                r*(c[7]+r*c[8])))))));
        
        if (x<0.0) 
            r=-r;
    
    }

    return r;
}


// standard normal cumulative distribution function
Real CumulativeNormal(Real x)
{
    static Real a[5] = { 0.319381530,
                          -0.356563782,
                           1.781477937,
                          -1.821255978,
                           1.330274429};

    Real result;
    
    if (x<-7.0)
        result = NormalDensity(x)/sqrt(1.+x*x);
    
    else 
    {
        if (x>7.0)
            result = 1.0 - CumulativeNormal(-x);
        else
        {
            Real tmp = 1.0/(1.0+0.2316419*fabs(x));

            result=1-NormalDensity(x)*
                     (tmp*(a[0]+tmp*(a[1]+tmp*(a[2]+tmp*(a[3]+tmp*a[4])))));

            if (x<=0.0) 
                result=1.0-result;

        }
    }

    return result;
}


Real Average(valarray<Real> Ptr,LongNatural dim)
{
	LongNatural i;
	Real result=0;

	for (i=0;i<dim;i++)
	{
		result+=Ptr[i];
	}

	result/=Real(dim);
	return result;
}

Real Maximize(valarray<Real> Ptr,LongNatural dim)
{
	LongNatural i;
	Real result=0;

	for (i=0;i<dim;i++)
	{
		if (Ptr[i]>result)
			result=Ptr[i];
	}

	return result;
}

Real CumulativeBivariateNormal(Real a,Real b,Real rho)
{
    static Real aarray[5]= {0.24840615, 0.39233107, 0.21141819, 0.03324666, 0.00082485334};
	static Real barray[5]={0.10024215, 0.48281397, 1.0609498, 1.7797294, 2.6697604};
 //   static Real aarray[4]= {0.325303, 0.4211071, 0.1334425, 0.006374323};
	//static Real barray[4]={0.1337764, 0.6243247, 1.3425378, 2.2626645};
    Real rho1 , rho2 , delta ;
    Real ap , bp ;
    Real Sum, Pi;
	Real sgnA,sgnB;
    Natural i , j ;
	Real BivN, denum;

	sgnA=(Real)sign(a);
	sgnB=(Real)sign(b);
    
	Pi = 3.14159265358979;
    if(pow(rho,2)==1)
		rho=(Real)sign(rho)*0.99999;
    ap = a / sqrt(2 * (1 - pow(rho,2)));
    bp = b / sqrt(2 * (1 - pow(rho,2)));
    
	if( (a <= 0) && (b <= 0) && (rho <= 0) ){
        Sum = 0;
		for (i = 0 ;i< 5;i++)
            for (j = 0 ;j< 5;j++)
                Sum +=  + aarray[i] * aarray[j] * SubFunctionForBivariateNormal(barray[i], barray[j], ap, bp, rho);
        BivN = Sum * sqrt(1 - pow(rho,2)) / Pi;
	}  
	else if ((a <= 0 )&& (b >= 0) && (rho >= 0)) 
        BivN = CumulativeNormal(a) - CumulativeBivariateNormal(a, -b, -rho); 
    else if ((a >= 0) && (b <= 0) && (rho >= 0)) 
        BivN = CumulativeNormal(b) - CumulativeBivariateNormal(-a, b, -rho);
    else if ((a >= 0) && (b >= 0) && (rho <= 0))
        BivN = CumulativeNormal(a) + CumulativeNormal(b) - 1 + CumulativeBivariateNormal(-a, -b, rho);
	else if (a * b * rho >= 0) {
        denum = sqrt(pow(a,2) - 2 * rho * a * b + pow(b,2));
        rho1 = (rho * a - b) * sgnA / denum;
        rho2 = (rho * b - a) * sgnB / denum;
        delta = (1 - sgnA*sgnB) / 4;
        BivN = CumulativeBivariateNormal(a, 0, rho1) + CumulativeBivariateNormal(b, 0, rho2) - delta;
	}	
	return BivN;
}

Real SubFunctionForBivariateNormal(Real X,Real y,Real ap,Real bp,Real rho){
	Real r;
	r = ap * (2 * X - ap) + bp * (2 * y - bp) + 2 * rho * (X - ap) * (y - bp);
	return exp(r);
}
