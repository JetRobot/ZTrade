#include "utils.h"

/** helper function to merge two valarrays of reals sorted in ascending order
	and remove duplicates.

	@params a1 - first array to merge
	@params a2 - second array to merge

	@returns the merged array
	*/
valarray<Real> 
mergeunique(valarray<Real> a1, valarray<Real> a2) {
	valarray<Real> temp(a1.size() + a2.size());
	Real tempmin = -1 * TN_INFINITY;

	Natural a1idx = 0;
	Natural a2idx = 0;
	Natural tempidx = 0;
	bool a1hasmore = (a1idx < a1.size());
	bool a2hasmore = (a2idx < a2.size());

	while (a1hasmore || a2hasmore) {
		if (a1hasmore && !a2hasmore) {
			if (a1[a1idx] > tempmin) {
				tempmin = temp[tempidx++] = a1[a1idx];
			}
			a1idx++;
		} else if (!a1hasmore && a2hasmore) {
			if (a2[a2idx] > tempmin) {
				tempmin = temp[tempidx++] = a2[a2idx];
			}
			a2idx++;
		} else if (a1[a1idx] < a2[a2idx]) {
			// a1 is less than a2, take from a1
			if (a1[a1idx] > tempmin) {
				tempmin = temp[tempidx++] = a1[a1idx];
			}
			a1idx++;
		} else if (a1[a1idx] > a2[a2idx]) {
			// a2 is less than a1, take from a2
			if (a2[a2idx] > tempmin) {
				tempmin = temp[tempidx++] = a2[a2idx];
			}
			a2idx++;
		} else {
			// a1 = a2, take from a1 and advance both pointers
			if (a1[a1idx] > tempmin) {
				tempmin = temp[tempidx++] = a1[a1idx];
			}
			a1idx++;
			a2idx++;
		}
		a1hasmore = (a1idx < a1.size());
		a2hasmore = (a2idx < a2.size());
	}

	temp.resize(tempidx);
	return temp;
}


short int sign(Real x)
{
	if(x>0)
		return 1;
	else if(x<0)
		return -1;
	else
		return 0;
}


//display error message, exit program if necessary
void ErrorMsg(string str, bool mustexit)
{
	cout << "Error: " << str << '\n';

	if(mustexit) {
		cout << "Exiting program\n";
		exit(1);
	}
	else {
		char ch = 'Q';

		do {
			cout << "Continue program execution? (Y/N): ";
			cin >> ch;
			ch = toupper(ch);
		} while((ch != 'Y') && (ch != 'N'));

		if(ch == 'N') exit(1);
		if(ch == 'Y') return;        //yes, superfluous, but makes it more undertandable
	}
}

Matrix transform1DvalarrayToColumnMatrix(valarray<Real> array)
{
	Natural size=(Natural)array.size();
	Matrix res=Matrix(0.0,size,1);
	for(Natural i=0;i<size;i++)
		res.SetValue(i,0,array[i]);
	return res;
}

// each valarray is here supposed to be of the same size
// array[i] = the line (in the prupose of arrays of random numbers, [i] is the asset, and the line is the set of numbers
Matrix transform2DvalarrayToMatrix(valarray<valarray<Real> > array)
{
	Natural col=(Natural)array.size();
	Natural row=(Natural)array[0].size();
	Matrix res=Matrix(0.0,row,col);
	for(Natural i=0;i<row;i++)
		for(Natural j=0;j<col;j++)
			res.SetValue(i,j,array[i][j]);
	return res;
}


valarray<Real> transformColumnMatrixTo1Dvalarray(Matrix M)
{
	Natural size=M.GetRows();
	valarray<Real> res(size);
	for(Natural i=0;i<size;i++)
		res[i]=M(i,0);
	return res;
}

valarray<valarray<Real> > transformMatrixTo2Dvalarray(Matrix M)
{
	Natural col=M.GetColumns();
	Natural row=M.GetRows();
	valarray<valarray<Real> > res(row);
	for(Natural i=0;i<row;i++){
		res[i].resize(col);
		for(Natural j=0;j<col;j++)
			res[i][j]=M(i,j);
	}
	return res;
}


//get safe int input
int getint(istream& istr)
{
	int temp = 0;
	string str;

	while(!(istr >> temp)) {
		istr.clear();
		istr >> str;
	}

	return temp;
}

bool 
realsEqual(Real realOne, Real realTwo, Real precision) {
	return (fabs(realOne - realTwo) < precision);
}

string
valarrayRealToString(const valarray<Real> &theArray) {
	Natural size = theArray.size();
	char realString[10];
	string result = "[ ";
	for (Natural i = 0; i < size; i++) {
		sprintf(realString, "%5.4f", theArray[i]);
		result = result + realString + " ";
	}
	sprintf(realString, "%d", size);
	result = result + "] size: " + realString;
	return result;
}