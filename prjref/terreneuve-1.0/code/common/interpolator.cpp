#include "./interpolator.h"
       
       
       
       
interpolator::interpolator(){}

interpolator::interpolator(valarray<Real> x, valarray<Real> y)
    :  _x(x),
      _y(y)
	{
	}


interpolator::interpolator(valarray<Real> x1, valarray<Real> x2, valarray<valarray<Real> > ymat)
    : _x1(x1),
      _x2(x2),
      _ymat(ymat)
  {
  }

Real interpolator::getInterpolation(valarray<Real> xa,valarray<Real> ya, Real x){
  Integer j=0;

  Real P1 = ya[j];
  Real P2 = ya[j+1];
  Real P3 = ya[j+2];

  Real P12 = ((x - xa[j+1])*P1 + (xa[j] - x)*P2)/(xa[j]-xa[j+1]);
  Real P23 = ((x - xa[j+2])*P2 + (xa[j+1] - x)*P3)/(xa[j+1]-xa[j+2]);
  Real P123 = ((x - xa[j+2])*P12 + (xa[j] - x)*P23)/(xa[j]-xa[j+2]);

  return P123;
}
valarray<Real> interpolator::interpolate(valarray<Real> vec){
  Integer n = vec.size();
  valarray<Real> result(n);
  Integer i;

  for(i=0;i<n;i++)
    result[i] = interpolate(vec[i]);

  return result;
}

Integer interpolator::getPlace(Real x){
  Integer n = _x.size();
  Integer i = 0;
  while((x>_x[i]) && (i<n))
    i++;

  return i;
}

Integer interpolator::getPlaceOnXi(Real x, Integer i)
{
  Integer n = _x1.size();
  Integer k = 0;
  if (i==1)
    {
    while((x>_x1[k]) && (k<n))
      k++;
    }
  else
    {
    while((x>_x2[k]) && (k<n))
      k++;
    }

  return k;
}



Real interpolator::interpolate(Real x)
{
  Integer i = getPlace( x);
  Integer n = _x.size();

  if(i==0)
	  return _x[0]; //out of the boundary we return the value of the nearest point
  else if (i==n)
	  return _x[n-1];
  else{
	Integer j,k;
	valarray<Real> xa(3);
	valarray<Real> ya(3);

	if (i=n-1) j=n-3;
	else j=i-1;

	for(k=0;k<3;k++)
		{
		xa[k]=_x[j+k];
		ya[k]=_y[j+k];
		}

	return getInterpolation(xa, ya, x);
  }
}


Real interpolator::interpolate(Real x1, Real x2)
{

	Integer i1 = getPlaceOnXi(x1, 1);
	Integer i2 = getPlaceOnXi(x2, 2);
	Integer N = _x1.size();
	Integer M = _x2.size();


	valarray<Real> xa1(3);
	valarray<Real> xa2(3);
	valarray<Real> temp(3);
	valarray<valarray<Real> >yx(3);
	valarray<Real> interpol1(3);
	valarray<Real> interpol2(N);
	interpolator *tempInt;
  
    Integer j,k;

	if (i1==0)
	{
		tempInt = new interpolator(_x2, _ymat[0]);
		return tempInt->interpolate(x2);
	} 
	else if (i1==N)
	{
		  tempInt = new interpolator(_x2, _ymat[N-1]);
		  return tempInt->interpolate(x2);
	}
	else if (i2==0)
	{
		for(j=0;j<N;j++)
			interpol2[j] = _ymat[j][0];
		tempInt = new interpolator(_x1, interpol2);
		return tempInt->interpolate(x1);
	}
	else if (i2==M)
	{
		for(j=0;j<N;j++)
			interpol2[j] = _ymat[j][M-1];
		tempInt = new interpolator(_x1, interpol2);
		return tempInt->interpolate(x1);
	}
	else
	{
		if (i1==N-1) i1= N-3;
		else i1--;

		if (i2==M-1) i2=M-3;
		else i2--;

		for(j=0;j<3;j++)
		{
			for(k=0;k<3;k++)
				temp[k] = _ymat[j+i1][k+i2];

		yx[j].resize(3);
		yx[j]=temp;	
		}

		for(j=0;j<3;j++)
		{
			xa1[j] = _x1[j+i1];
			xa2[j] = _x2[j+i2];
		}

		for(j=0;j<3;j++)
		{
			interpol1[j] = getInterpolation(xa2, yx[j], x2);
		}

		return getInterpolation(xa1, interpol1, x1);
	}
}


valarray<valarray<Real> > interpolator::interpolate(valarray<Real> vec1, valarray<Real> vec2)
{
  Integer n1 = vec1.size();
  Integer n2 = vec2.size();
  valarray<valarray<Real> > res(n1);
  valarray<Real> temp(n2);

  Integer i,j;
  for(i=0;i<n1;i++)
    {
    for(j=0;j<n2;j++)
	temp[j] = interpolate(vec1[i], vec2[j]);

    res[i].resize(n2);
    res[i] = temp;
    }
  return res;
}

Integer interpolatormain()
{

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

  Real x_i = x1[1];
  Real x_j = x2[2];
  Real x_k = 0.1;
//  res = terneuve.interpolate(x_i,x_j);
  res = terneuve.interpolate(x_k,x_j);

  cout<<res;
  
  return 0;


}


