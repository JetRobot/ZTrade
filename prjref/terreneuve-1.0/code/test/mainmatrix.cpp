#include "..\common\matrix.h"
#include "..\common\types.h"
#include "..\common\utils.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include<valarray>
using namespace std;


/**
@author Yann
*/


bool mainmatrix(void){
	cout << "Test cholesky." << endl;
	
	Matrix M2chol,M3cholTrans,M4recombChol;
	Matrix M1testChol= Matrix (0.0,3,3);

	M1testChol.SetValue(0,0,5);
	M1testChol.SetValue(0,1,4);
	M1testChol.SetValue(0,2,1);
	M1testChol.SetValue(1,0,4);
	M1testChol.SetValue(1,1,10);
	M1testChol.SetValue(1,2,7);
	M1testChol.SetValue(2,0,1);
	M1testChol.SetValue(2,1,7);
	M1testChol.SetValue(2,2,25);
	cout << M1testChol << endl;

	M2chol=M1testChol.CholeskyDecomposition();
	cout << M2chol << endl;

	M3cholTrans=M2chol.GetTransposed();
	cout << M3cholTrans << endl;

	M4recombChol=M2chol*M3cholTrans;
	cout << M4recombChol << endl;

	//////

	cout << "\nTest utils"<<endl;

	Matrix M1= M1testChol;
	Matrix M2;
	Matrix M3= Matrix(0.0,3,1);
	Matrix M4;

	cout << M1 << endl;

	valarray<valarray<Real> > array=transformMatrixTo2Dvalarray(M1);
	
	Natural col=(Natural)array.size();
	Natural row=(Natural)array[0].size();
	for(Natural i=0;i<row;i++){
		for(Natural j=0;j<col;j++)
			cout<<array[i][j]<< endl;
		cout<<"\n"<<endl;
	}

	M2=transform2DvalarrayToMatrix(array);
	cout << M2 << endl;

	M3.SetValue(0,0,5);
	M3.SetValue(1,0,4);
	M3.SetValue(2,0,1);
	cout << M3 << endl;

	valarray<Real> array2=transformColumnMatrixTo1Dvalarray(M3);
	Natural size=(Natural)array.size();
	for(Natural i=0;i<size;i++)
			cout<<array2[i] <<endl;

	M4=transform1DvalarrayToColumnMatrix (array2);
	cout << M4 << endl;
	
	return false;
}
