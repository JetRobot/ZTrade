#include "..\common\date.h"
#include "..\common\interpolator.h"
#include<valarray>

bool maininterpolator(void){
	valarray<Real> x1(4);
    valarray<Real> x2(4);
    valarray<Real> temp(4);
    valarray<valarray<Real> > y(4);
    Integer i,j;
    for(i=0;i<4;i++)
    {
            x1[i]=i/10.0;
            x2[i]=i/10.0;
    }
    
    for(i=0;i<4;i++)
    {
            for(j=0;j<4;j++)
                  temp[j]=sin(x1[i]+x2[j]*x2[j]);

            y[i].resize(4);
            y[i]=temp;
    }

    interpolator terneuve(x1, x2, y);
	Integer n1 = x1.size();
	Integer n2 = x2.size();

//  Real tab[] = {0.15,0.25,0.35,0.45};
//  valarray<Real> temp(tab, 4);


  Real res;

  Real x_i = 0.251;//x1[1];
  Real x_j = 0.129;//x2[2];
  Real x_k = 0.1;
  res = terneuve.interpolate(x_i,x_j);
//  res = terneuve.interpolate(x_k,x_j);

  cout<<" f(x1,x2)=sin(x1+x2*x2)"<<endl;
  cout<<"we give in input f(x1,x2) for (x1,x2) in {0,0.1,0.2,0.3}"<<endl;
  cout<<"we want to interpolate f for x1=0.251 and x2=0.129"<<endl;
  cout<<"the interpolation gives  "<<res<<endl;
  cout<<"and the true value is 0.26445472"<<endl;
  return false;
}