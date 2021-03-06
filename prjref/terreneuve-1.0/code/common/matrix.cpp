//------------------------------------
//   Implementation of Matrix Class
//------------------------------------

#include <iostream>
#include <fstream>
#include <cmath>
#include "matrix.h"
#include "utils.h"
using namespace std;

//appends an identity matrix to the right of the calling object
//used in finding the inverse
//the user probably doesn't need access to this
Matrix& Matrix::RightAppendIdentity()
{
	Matrix temp;
	temp = Matrix(0.0, m_nRows, (2 * m_nCols));

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			temp.m_pData[i][j] = m_pData[i][j];
		}
	}
	for(int q=0; q<m_nRows; ++q) {
		temp.m_pData[q][m_nCols+q] = 1;
	}

	*this = temp;

	return *this;
}

//removes an identity matrix from the left of the calling object
//used in finding the inverse
//the user probably doesn't need access to this
Matrix& Matrix::LeftRemoveIdentity()
{
	Matrix temp;
	temp = Matrix(0.0, m_nRows, (m_nCols / 2));

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols/2; ++j) {
			temp.m_pData[i][j] = m_pData[i][m_nCols/2+j];
		}
	}

	*this = temp;

	return *this;
}

//standard constructor
//Data = Null; rows = columns = 0
Matrix::Matrix()
{
	m_pData = NULL;
	m_nCols = m_nRows = 0;
}

//constructor with explicit values
//of the matrix, rows = 'Rows', columns = 'Cols'
//all spots of matrix = InitVal
Matrix::Matrix(double InitVal, int Rows, int Cols)
{
	m_nRows = Rows;
	m_nCols = Cols;

	m_pData = new double*[m_nRows];
	for(int i=0; i<m_nRows; ++i) {
		m_pData[i] = new double[m_nCols];
		for(int j=0; j<m_nCols; ++j) {
			m_pData[i][j] = InitVal;
		}
	}
}

//constructor with explicit values
//of the matrix, rows = 'Rows', columns = 'Cols'
//all spots of matrix are the spots of 'Data', going row-by-row
Matrix::Matrix(double* Data, int Rows, int Cols)
{
	m_nRows = Rows;
	m_nCols = Cols;

	m_pData = new double*[m_nRows];
	for(int i=0; i<m_nRows; ++i) {
		m_pData[i] = new double[m_nCols];
		for(int j=0; j<m_nCols; ++j) {
			m_pData[i][j] = Data[i*m_nRows+j];
		}
	}
}

//constructor with explicit values
//of the matrix, rows = 'Rows', columns = 'Cols'
//spots in matrix correspond to the spots in the double Pointer-Pointer
Matrix::Matrix(double** Data, int Rows, int Cols)
{
	m_nRows = Rows;
	m_nCols = Cols;

	m_pData = new double*[m_nRows];
	for(int i=0; i<m_nRows; ++i) {
		m_pData[i] = new double[m_nCols];
		for(int j=0; j<m_nCols; ++j) {
			m_pData[i][j] = Data[i][j];
		}
	}
}

//copy constructor
Matrix::Matrix(const Matrix& obj)
{
	m_nRows = obj.m_nRows;
	m_nCols = obj.m_nCols;

	m_pData = new double*[m_nRows];
	for(int i=0; i<m_nRows; ++i) {
		m_pData[i] = new double[m_nCols];
		for(int j=0; j<m_nCols; ++j) {
			m_pData[i][j] = obj.m_pData[i][j];
		}
	}
}

//destructor
Matrix::~Matrix()
{
	for(int i=0; i<m_nRows; ++i) {
		delete[] m_pData[i];
		m_pData[i] = NULL;
	}
	delete[] m_pData;
	m_pData = NULL;
}

//add two matrices
Matrix& Matrix::operator +(const Matrix& obj) const
{
	if((m_nRows != obj.m_nRows) || (m_nCols != obj.m_nCols))
		ErrorMsg("mismatched matrices in addition", true);

	static Matrix temp;
	temp = Matrix(0.0, m_nRows, m_nCols);

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			temp.m_pData[i][j] = m_pData[i][j] + obj.m_pData[i][j];
		}
	}

	return temp;
}

//subtract two matrices
Matrix& Matrix::operator -(const Matrix& obj) const
{
	if((m_nRows != obj.m_nRows) || (m_nCols != obj.m_nCols))
		ErrorMsg("mismatched matrices in addition", true);

	static Matrix temp;
	temp = Matrix(0.0, m_nRows, m_nCols);

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			temp.m_pData[i][j] = m_pData[i][j] - obj.m_pData[i][j];
		}
	}

	return temp;
}

//multiply two matrices
Matrix& Matrix::operator *(const Matrix& obj) const
{
	if(m_nCols != obj.m_nRows)
		ErrorMsg("mismatched matrices in multiplication", true);

	double sum = 0;
	double prod = 1;

	static Matrix temp;
	temp = Matrix(0.0, m_nRows, obj.m_nCols);

	for(int i=0; i<temp.m_nRows; ++i) {
		for(int j=0; j<temp.m_nCols; ++j) {
			sum = 0;
			for(int q=0; q<m_nCols; ++q) {
				prod = m_pData[i][q] * obj.m_pData[q][j];
				sum += prod;
			}
			temp.m_pData[i][j] = sum;
		}
	}

	return temp;
}

//multiply a matrix by a double scalar
Matrix& Matrix::operator *(const double _d) const
{
	static Matrix temp;
	temp = Matrix(m_pData, m_nCols, m_nRows);

	for(int i=0; i<temp.m_nRows; ++i) {
		for(int j=0; j<temp.m_nCols; ++j) {
			temp.m_pData[i][j] *= _d;
		}
	}

	return temp;
}

//multiply a matrix by an int scalar
Matrix& Matrix::operator *(const int _i) const
{
	static Matrix temp;
	temp = Matrix(m_pData, m_nCols, m_nRows);

	for(int i=0; i<temp.m_nRows; ++i) {
		for(int j=0; j<temp.m_nCols; ++j) {
			temp.m_pData[i][j] *= _i;
		}
	}

	return temp;
}

//divide a matrix by another matrix
Matrix& Matrix::operator /(const Matrix& obj) const
{
	return(*this * obj.GetInverse());
}

//divide a matrix by a double scalar
Matrix& Matrix::operator /(const double _d) const
{
	if(_d == 0) ErrorMsg("divide by zero in double division", true);

	static Matrix temp;
	temp = Matrix(m_pData, m_nCols, m_nRows);

	for(int i=0; i<temp.m_nRows; ++i) {
		for(int j=0; j<temp.m_nCols; ++j) {
			temp.m_pData[i][j] /= _d;
		}
	}

	return temp;
}

//divide a matrix by an int scalar
Matrix& Matrix::operator /(const int _i) const
{
	if(_i == 0) ErrorMsg("divide by zero in integer division", true);

	static Matrix temp;
	temp = Matrix(m_pData, m_nCols, m_nRows);

	for(int i=0; i<temp.m_nRows; ++i) {
		for(int j=0; j<temp.m_nCols; ++j) {
			temp.m_pData[i][j] /= _i;
		}
	}

	return temp;
}

//add two matrices and assign
Matrix& Matrix::operator +=(const Matrix& obj)
{
	return(*this = *this + obj);
}

//subtract two matrices and assign
Matrix& Matrix::operator -=(const Matrix& obj)
{
	return(*this = *this - obj);
}

//multiply two matrices and assign
Matrix& Matrix::operator *=(const Matrix& obj)
{
	return(*this = *this * obj);
}

//multiply a matrix by a double scalar and assign
Matrix& Matrix::operator *=(const double _d)
{
	return(*this = *this * _d);
}

//multiply a matrix by an int scalar and assign
Matrix& Matrix::operator *=(const int _i)
{
	return(*this = *this * _i);
}

//divide a matrix by a matrix and assign
Matrix& Matrix::operator /=(const Matrix& obj)
{
	return(*this = *this / obj);
}

//divide a matrix by a double scalar and assign
Matrix& Matrix::operator /=(const double _d)
{
	return(*this = *this / _d);
}

//divide a matrix by an int scalar and assign
Matrix& Matrix::operator /=(const int _i)
{
	return(*this = *this / _i);
}

//assignment operator
Matrix& Matrix::operator =(const Matrix& obj)
{
	if(&obj == this) return *this;

	if(m_nRows | m_nCols) {
		for(int i=0; i<m_nRows; ++i) {
			delete[] m_pData[i];
		}
		delete[] m_pData;
	}

	m_nRows = obj.m_nRows;
	m_nCols = obj.m_nCols;

	m_pData = new double*[m_nRows];
	for(int i=0; i<m_nRows; ++i) {
		m_pData[i] = new double[m_nCols];
		for(int j=0; j<m_nCols; ++j) {
			m_pData[i][j] = obj.m_pData[i][j];
		}
	}

	return *this;
}

//inversion operator
//returns the inverse of the calling matrix
Matrix& Matrix::operator ~() const
{
	return GetInverse();
}

//equality operator
bool Matrix::operator ==(const Matrix& obj) const
{
	if(m_nRows != obj.m_nRows) return false;
	if(m_nCols != obj.m_nCols) return false;

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			if(m_pData[i][j] != obj.m_pData[i][j]) return false;
		}
	}

	return true;
}

//inequality operator
bool Matrix::operator !=(const Matrix& obj) const
{
	if(m_nRows != obj.m_nRows) return true;
	if(m_nCols != obj.m_nCols) return true;

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			if(m_pData[i][j] != obj.m_pData[i][j]) return true;
		}
	}

	return false;
}

//indexing operator
//remember, a zero (0) element DOES exist in this matrix, although not in a double matrix
double* Matrix::operator [](const int _i) const
{
	if((_i >= m_nRows) || (_i < 0)) ErrorMsg("out-of-bound index", true);

	return m_pData[_i];
}

//another indexing operator, but takes both rows and columns
//remember, a zero (0) element DOES exist in this matrix, although not in a double matrix
double& Matrix::operator ()(const int _i, const int _j) const
{
	if((_i >= m_nRows) || (_j >= m_nCols)) ErrorMsg("out-of-bounds index", true);
	if((_i < 0) || (_j < 0)) ErrorMsg("out-of-bounds index", true);

	return m_pData[_i][_j];
}

//returns true if the calling matrix is an identity matrix
bool Matrix::IsIdentity() const
{
	if(m_nCols != m_nRows) return false;

	for(int i=0; i<m_nCols; ++i) {
		for(int j=0; j<m_nRows; ++j) {
			if(i == j) {
				if(m_pData[i][j] != 1) return false;
			}
			else if(m_pData[i][j] != 0) return false;
		}
	}

	return true;
}

//returns true if every element in the calling matrix is zero (0)
bool Matrix::IsEmpty() const
{
	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			if(m_pData[i][j] != 0) return false;
		}
	}
	
	return true;
}

//finds determinant of the matrix
double Matrix::Determinant() const
{
	if(m_nRows != m_nCols) ErrorMsg("not a square matrix for determinant", true);

	double sum = 0;

	if(m_nRows == 2) {
		return((m_pData[0][0] * m_pData[1][1]) - (m_pData[1][0] * m_pData[0][1]));
	}

	for(int q=0; q<m_nCols; ++q) {
		Matrix* NewMinor = GetMinorNew(0, q);
		sum += (pow(-1, q)*(m_pData[0][q]*NewMinor->Determinant()));
		delete NewMinor;
	}

	return sum;
}

//sums all the values in the matrix
double Matrix::SumAll() const
{
	double sum = 0;

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			sum += m_pData[i][j];
		}
	}

	return sum;
}

//sums all the values in the matrix and then squares that value
double Matrix::SumAllSquared() const
{
	double d = SumAll();

	return(d * d);
}

//sums all the values in the row 'Row' of the matrix
double Matrix::SumRow(const int Row) const
{
	double sum = 0;

	for(int j=0; j<m_nCols; ++j) {
		sum += m_pData[Row][j];
	}

	return sum;
}

//sums all the values in the column 'Col' of the matrix
double Matrix::SumColumn(const int Col) const
{
	double sum = 0;

	for(int i=0; i<m_nRows; ++i) {
		sum += m_pData[i][Col];
	}

	return sum;
}

//sums all the values in the row 'Row' of the matrix and then squares that value
double Matrix::SumRowSquared(const int Row) const
{
	double d = SumRow(Row);

	return(d * d);
}

//sums all the values in the column 'Col' of the matrix and th squares that value
double Matrix::SumColumnSquared(const int Col) const
{
	double d = SumColumn(Col);

	return(d * d);
}

//returns the largest value in the matrix
double Matrix::GetMax() const
{
	double max = m_pData[0][0];

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			if(m_pData[i][j] > max) max = m_pData[i][j];
		}
	}

	return max;
}

//returns the smallest value in the matrix
double Matrix::GetMin() const
{
	double min = m_pData[0][0];

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			if(m_pData[i][j] < min) min = m_pData[i][j];
		}
	}

	return min;
}

//returns the largest value in row 'Row' in the matrix
double Matrix::GetRowMax(const int Row) const
{
	double max = m_pData[Row][0];

	for(int j=0; j<m_nCols; ++j) {
		if(m_pData[Row][j] > max) max = m_pData[Row][j];
	}

	return max;
}

//returns the smallest value in row 'Row' in the matrix
double Matrix::GetRowMin(const int Row) const
{
	double min = m_pData[Row][0];

	for(int j=0; j<m_nCols; ++j) {
		if(m_pData[Row][j] < min) min = m_pData[Row][j];
	}

	return min;
}

//returns the largest value in column 'Col' in the matrix
double Matrix::GetColumnMax(const int Col) const
{
	double max = m_pData[0][Col];

	for(int i=0; i<m_nRows; ++i) {
		if(m_pData[i][Col] > max) max = m_pData[i][Col];
	}

	return max;
}

//returns the smallest value in column 'Col' in the matrix
double Matrix::GetColumnMin(const int Col) const
{
	double min = m_pData[0][Col];

	for(int i=0; i<m_nRows; ++i) {
		if(m_pData[i][Col] < min) min = m_pData[i][Col];
	}

	return min;
}

//returns the difference of the largest and smallest values in the matrix
double Matrix::GetRange() const
{
	double min, max;

	GetNumericRange(min, max);

	return(max-min);
}

//returns the difference of the largest and smallest values in row 'Row' in the matrix
double Matrix::GetRowRange(const int Row) const
{
	double min, max;

	GetNumericRangeOfRow(min, max, Row);

	return(max-min);
}

//returns the difference of the largest and smallest values in column 'Col' in the matrix
double Matrix::GetColumnRange(const int Col) const
{
	double min, max;

	GetNumericRangeOfColumn(min, max, Col);

	return(max-min);
}

//returns the data in a one-dimensional array
//the array is dynamically allocated
double* Matrix::GetDataOneDimen() const
{
	double* newData;

	newData = new double[m_nRows * m_nCols];

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			newData[(i*m_nRows)+j] = m_pData[i][j];
		}
	}

	return newData;
}

//returns the data in a two-dimensional array
//the array is dynamically allocated
double** Matrix::GetDataTwoDimen() const
{
	double** newData;

	newData = new double*[m_nRows];
	for(int i=0; i<m_nRows; ++i) {
		newData[i] = new double[m_nCols];
		for(int j=0; j<m_nCols; ++j) {
			newData[i][j] = m_pData[i][j];
		}
	}

	return newData;
}

//returns number of rows of the matrix
int Matrix::GetRows() const
{
	return m_nRows;
}

//returns number of columns of the matrix
int Matrix::GetColumns() const
{
	return m_nCols;
}

//clears every entry in the calling matrix
Matrix& Matrix::Clear()
{
	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			m_pData[i][j] = 0;
		}
	}

	return *this;
}

//clears every entry in the row 'Row' from the calling matrix
Matrix& Matrix::ClearRow(const int Row)
{
	for(int j=0; j<m_nCols; ++j) {
		m_pData[Row][j] = 0;
	}

	return *this;
}

//clears every entry in the column 'Col' from the calling matrix
Matrix& Matrix::ClearColumn(const int Col)
{
	for(int i=0; i<m_nRows; ++i) {
		m_pData[i][Col] = 0;
	}

	return *this;
}

//fills the entry in the calling matrix to '_d'
Matrix& Matrix::SetValue(int Row,int Col,double _d)
{
	if(Row<m_nRows && Col < m_nCols)
		m_pData[Row][Col] = _d;
	return *this;
}


//fills every entry in the calling matrix to '_d'
Matrix& Matrix::Fill(const double _d)
{
	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			m_pData[i][j] = _d;
		}
	}

	return *this;
}

//fills every entry in the row 'Row' of the calling matrix to '_d'
Matrix& Matrix::FillRow(const int Row, const double _d)
{
	for(int j=0; j<m_nCols; ++j) {
		m_pData[Row][j] = _d;
	}

	return *this;
}

//fills every entry in the column 'Col' of the calling matrix to '_d'
Matrix& Matrix::FillColumn(const int Col, const double _d)
{
	for(int i=0; i<m_nRows; ++i) {
		m_pData[i][Col] = _d;
	}

	return *this;
}

//returns the inverse of the calling matrix
Matrix& Matrix::GetInverse() const
{
	if(m_nRows != m_nCols) {
		ErrorMsg("not a square matrix for inverse", true);
	}

	static Matrix temp;
	temp = *this;

	temp.RightAppendIdentity();
	temp.RREF();
	temp.LeftRemoveIdentity();

	return temp;
}

//turns the calling matrix into its inverse
Matrix& Matrix::Invert()
{
	*this = this->GetInverse();

	return *this;
}

//add 'SourceRow' by scale of 'factor' (default is one) to 'DestRow'
Matrix& Matrix::AddRows(const int SourceRow, const int DestRow, const double factor)
{
	for(int j=0; j<m_nCols; ++j) {
		m_pData[DestRow][j] += (m_pData[SourceRow][j] * factor);
	}

	return *this;
}

//multiply every element in row 'Row' by '_d'
Matrix& Matrix::MultiplyRow(const int Row, const double _d)
{
	for(int j=0; j<m_nCols; ++j) {
		m_pData[Row][j] *= _d;
	}

	return *this;
}

//divide every element in row 'Row' by '_d'
Matrix& Matrix::DivideRow(const int Row, const double _d)
{
	if(_d == 0) ErrorMsg("divide by zero in row division", true);

	for(int j=0; j<m_nCols; ++j) {
		m_pData[Row][j] /= _d;
	}

	return *this;
}

//add 'SourceCol' by scale of 'factor' (default is one) to 'DestCol'
Matrix& Matrix::AddColumns(const int SourceCol, const int DestCol, const double factor)
{
	for(int i=0; i<m_nRows; ++i) {
		m_pData[i][DestCol] += (m_pData[i][SourceCol] * factor);
	}

	return *this;
}

//multiply every element in row 'Col' by '_d'
Matrix& Matrix::MultiplyColumn(const int Col, const double _d)
{
	for(int i=0; i<m_nRows; ++i) {
		m_pData[i][Col] *= _d;
	}

	return *this;
}

//divide every element in row 'Col' by '_d'
Matrix& Matrix::DivideColumn(const int Col, const double _d)
{
	if(_d == 0) ErrorMsg("divide by zero in column division", true);

	for(int i=0; i<m_nRows; ++i) {
		m_pData[i][Col] /= _d;
	}

	return *this;
}

//puts the matrix into row-echelon form
Matrix& Matrix::REF()
{
	for(int i=0; i<m_nRows; ++i) {
		for(int j=i+1; j<m_nRows; ++j) {
			AddRows(i, j, -m_pData[j][i]/m_pData[i][i]);
		}
		DivideRow(i, m_pData[i][i]);
	}

	return *this;
}

//puts the matrix into reduced row-echelon form
Matrix& Matrix::RREF()
{
	REF();

	for(int i=m_nRows-1; i>=0; i--) {
		for(int j=i-1; j>=0; j--) {
			AddRows(i, j, -m_pData[j][i]/m_pData[i][i]);
		}
		DivideRow(i, m_pData[i][i]);
	}

	return *this;
}

//returns a matrix that is the row-echelon form of the calling matrix
Matrix& Matrix::GetREF() const
{
	static Matrix temp;

	temp = *this;
	temp.REF();

	return temp;
}

//returns a matrix that is the reduced row-echelon form of the calling matrix
Matrix& Matrix::GetRREF() const
{
	static Matrix temp;

	temp = *this;
	temp.RREF();

	return temp;
}

//returns minor around spot Row,Col
//returns a static Matrix object
Matrix& Matrix::GetMinor(const int RowSpot, const int ColSpot) const
{
	static Matrix temp;
	temp = Matrix(0.0, m_nRows-1, m_nCols-1);

	for(int i=0, k=0; i<m_nRows; ) {
		if(i == RowSpot) {
			++i;
			continue;
		}
		for(int j=0, l=0; j<m_nCols; ) {
			if(j == ColSpot) {
				++j;
				continue;
			}
			temp.m_pData[k][l] = m_pData[i][j];
			++l;
			++j;
		}
		++i;
		++k;
	}

	return temp;
}

//returns minor around spot Row,Col
//returns a Pointer to a dynamically allocated Matrix object
Matrix* Matrix::GetMinorNew(const int RowSpot, const int ColSpot) const
{
	Matrix* temp;
	temp = new Matrix(0.0, m_nRows-1, m_nCols-1);

	for(int i=0, k=0; i<m_nRows; ) {
		if(i == RowSpot) {
			++i;
			continue;
		}
		for(int j=0, l=0; j<m_nCols; ) {
			if(j == ColSpot) {
				++j;
				continue;
			}
			temp->m_pData[k][l] = m_pData[i][j];
			++l;
			++j;
		}
		++i;
		++k;
	}

	return temp;
}

//returns the submatrix starting at spot (RowSpot, ColSpot) and with lengths
//of 'RowLen' rows and 'ColLen' columns
Matrix& Matrix::GetSubMatrix(const int RowSpot, const int ColSpot, const int RowLen, const int ColLen) const
{
	static Matrix temp;
	temp = Matrix(0.0, RowLen, ColLen);

	for(int i=RowSpot, k=0; i<(RowLen+RowSpot); ++i, ++k) {
		for(int j=ColSpot, l=0; j<(ColLen+ColSpot); ++j, ++l) {
			temp.m_pData[k][l] = m_pData[i][j];
		}
	}

	return temp;
}

//changes the calling matrix into a submatrix starting at spot (RowSpot, ColSpot)
//and with lengths of 'RowLen' rows and 'ColLen' columns
Matrix& Matrix::SetSubMatrix(const int RowSpot, const int ColSpot, const int RowLen, const int ColLen)
{
	Matrix temp;

	temp = GetSubMatrix(RowSpot, ColSpot, RowLen, ColLen);

	*this = temp;

	return *this;
}

//swaps two rows, Row1 and Row2, from the calling matrix
Matrix& Matrix::SwapRows(const int Row1, const int Row2)
{
	double* temp;

	temp = new double[m_nCols];

	for(int j=0; j<m_nCols; ++j) {
		temp[j] = m_pData[Row1][j];
		m_pData[Row1][j] = m_pData[Row2][j];
		m_pData[Row2][j] = temp[j];
	}

	delete[] temp;

	return *this;
}

//swaps two columns, Col1 and Col2, from the calling matrix
Matrix& Matrix::SwapCols(const int Col1, const int Col2)
{
	double* temp;

	temp = new double[m_nRows];

	for(int i=0; i<m_nRows; ++i) {
		temp[i] = m_pData[i][Col1];
		m_pData[i][Col1] = m_pData[i][Col2];
		m_pData[i][Col2] = temp[i];
	}

	delete[] temp;

	return *this;
}

//returns the transposition of the calling matrix
Matrix& Matrix::GetTransposed() const
{
	static Matrix temp;
	temp = Matrix(0.0, m_nCols, m_nRows);

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			temp.m_pData[j][i] = m_pData[i][j];
		}
	}

	return temp;
}

//transposes the calling matrix
Matrix& Matrix::Transpose()
{
	*this = this->GetTransposed();

	return *this;
}

//sets 'Min' to the minimum and 'Max to the maximum values in the matrix
Matrix& Matrix::GetNumericRange(double& Min, double& Max) const
{
	Min = Max = m_pData[0][0];

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			if(m_pData[i][j] < Min) Min = m_pData[i][j];
			if(m_pData[i][j] > Max) Max = m_pData[i][j];
		}
	}

	return const_cast<Matrix&>(*this);
}

//sets 'Min' to the minimum and 'Max to the maximum values in row 'Row' in the matrix
Matrix& Matrix::GetNumericRangeOfRow(double& Min, double& Max, const int Row) const
{
	Min = Max = m_pData[Row][0];

	for(int j=0; j<m_nCols; ++j) {
		if(m_pData[Row][j] > Max) Max = m_pData[Row][j];
		if(m_pData[Row][j] < Min) Min = m_pData[Row][j];
	}

	return const_cast<Matrix&>(*this);
}

//sets 'Min' to the minimum and 'Max to the maximum values in column 'Col' in the matrix
Matrix& Matrix::GetNumericRangeOfColumn(double& Min, double& Max, const int Col) const
{
	Min = Max = m_pData[0][Col];

	for(int i=0; i<m_nRows; ++i) {
		if(m_pData[i][Col] > Max) Max = m_pData[i][Col];
		if(m_pData[i][Col] < Min) Min = m_pData[i][Col];
	}

	return const_cast<Matrix&>(*this);
}

// Added from various sources -- here no test for symetry
// returns an upper triangular matrix such that U*U.t() = Orignial matrix
Matrix& Matrix::CholeskyDecomposition() 
{
	int i, j, k, size = GetRows();
	Real sum=0.0;

	static Matrix temp;
	temp = Matrix(0.0, size, size);

    for (i=0; i<size; i++) {
        for (j=i; j<size; j++) {
            sum = m_pData[i][j];
            for (k=0; k<=i-1; k++) {
                sum -= temp[i][k]*temp[j][k];
            }
            if (i == j) {
                // To handle positive semi-definite matrices take the
                // square root of sum if positive, else zero.
                temp[i][i] = std::sqrt(std::max<Real>(sum, 0.0));
            } else {
                // With positive semi-definite matrices is possible
                // to have temp[i][i]==0.0
                // In this case sum happens to be zero as well
                temp[j][i] =
                    (sum==0.0 ? 0.0 : sum/temp[i][i]);
            }
        }
    }
	return temp;
}

//CMAR = Concatenate Matrix As Rows
//concatenates matrix 'obj' on to the right of the calling matrix
Matrix& Matrix::CMAR(const Matrix& obj)
{
	if(m_nCols != obj.m_nCols) ErrorMsg("mismatched matrices in row concatenation", true);

	Matrix temp(0.0, (m_nRows + obj.m_nRows), m_nCols);

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			temp.m_pData[i][j] = m_pData[i][j];
		}
	}
	for(int k=0; i<temp.m_nRows; ++i, ++k) {
		for(int j=0; j<m_nCols; ++j) {
			temp.m_pData[i][j] = obj.m_pData[k][j];
		}
	}

	*this = temp;

	return *this;
}

//CMAC = Concatenate Matrix As Columns
//concatenates matrix 'obj' on to the bottom of the calling matrix
Matrix& Matrix::CMAC(const Matrix& obj)
{
	if(m_nRows != obj.m_nRows) ErrorMsg("mismatched matrices in column concatenation", true);

	Matrix temp(0.0, m_nRows, (m_nCols + obj.m_nCols));

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			temp.m_pData[i][j] = m_pData[i][j];
		}
		for(int l=0; l<obj.m_nCols; ++l, ++j) {
			temp.m_pData[i][j] = obj.m_pData[i][l];
		}
	}

	*this = temp;

	return *this;
}

//CMAR = Concatenate Matrix As Rows
//returns a new matrix that is the calling object + 'obj' on the right
Matrix& Matrix::GetCMAR(const Matrix& obj) const
{
	static Matrix temp;

	temp = *this;
	temp.CMAR(obj);

	return temp;
}

//CMAC = Concatenate Matrix As Columns
//returns a new matrix that is the valling object + 'obj' on the bottom
Matrix& Matrix::GetCMAC(const Matrix& obj) const
{
	static Matrix temp;

	temp = *this;
	temp.CMAC(obj);

	return temp;
}

//adds a row onto the right of the calling matrix
Matrix& Matrix::ConcatenateRow(const double* RowData)
{
	Matrix temp(0.0, m_nRows+1, m_nCols);

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			temp.m_pData[i][j] = m_pData[i][j];
		}
	}
	int j;
	for(i=m_nRows, j=0; j<m_nCols; ++j) {
		temp.m_pData[i][j] = RowData[j];
	}

	*this = temp;

	return *this;
}

//adds a column onto the bottom of the calling matrix
Matrix& Matrix::ConcatenateColumn(const double* ColumnData)
{
	
	Matrix temp(0.0, m_nRows, m_nCols+1);

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			temp.m_pData[i][j] = m_pData[i][j];
		}
	}
	int j;
	for(j=m_nCols, i=0; i<m_nRows; ++i) {
		temp.m_pData[i][j] = ColumnData[i];
	}

	*this = temp;

	return *this;
}

//adds a row into the matrix in the spot 'RowSpot'
Matrix& Matrix::SpliceInRow(const double* RowData, const int RowSpot)
{
	Matrix temp(0.0, m_nRows+1, m_nCols);

	for(int i=0, k=0; i<m_nRows; ++i, ++k) {
		if(i == RowSpot) {
			for(int j=0; j<m_nCols; ++j) {
				temp.m_pData[i][j] = RowData[j];
			}
			++k;
		}
		for(int j=0; j<m_nCols; ++j) {
			temp.m_pData[k][j] = m_pData[i][j];
		}
	}

	*this = temp;

	return *this;
}

//adds a column into the matrix in the spot 'ColumnSpot'
Matrix& Matrix::SpliceInColumn(const double* ColumnData, const int ColumnSpot)
{
	Matrix temp(0.0, m_nRows, m_nCols+1);

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0, l=0; j<m_nCols; ++j, ++l) {
			if(j == ColumnSpot) {
				temp.m_pData[i][l] = ColumnData[i];
				++l;
			}
			temp.m_pData[i][l] = m_pData[i][j];
		}
	}

	*this = temp;

	return *this;
}

//removes the specified row from the calling matrix
Matrix& Matrix::RemoveRow(const int Row)
{
	Matrix temp(0.0, m_nRows-1, m_nCols);

	for(int i=0, k=0; i<m_nRows; ++i, ++k) {
		if(i == Row) ++i;
		for(int j=0; j<m_nCols; ++j) {
			temp.m_pData[k][j] = m_pData[i][j];
		}
	}

	*this = temp;

	return *this;
}

//removes the specified column from the calling matrix
Matrix& Matrix::RemoveColumn(const int Column)
{
	Matrix temp(0.0, m_nRows, m_nCols-1);

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0, l=0; j<m_nCols; ++j, ++l) {
			if(j == Column) ++j;
			temp.m_pData[i][l] = m_pData[i][j];
		}
	}

	*this = temp;

	return *this;
}


//sorts the matrix in ascending order
Matrix& Matrix::SortAscend()
{
	double* Data = GetDataOneDimen();
	int length = (m_nRows * m_nCols);
	double temp;

	for(int i=0; i<length; ++i) {
		for(int j=0; j<length-1; ++j) {
			if(Data[j] > Data[j+1]) {
				temp = Data[j];
				Data[j] = Data[j+1];
				Data[j+1] = temp;
			}
		}
	}

	*this = Matrix(Data, m_nRows, m_nCols);

	return *this;
}

//sorts the matrix in descending order
Matrix& Matrix::SortDescend()
{
	double* Data = GetDataOneDimen();
	int length = (m_nRows * m_nCols);
	double temp;

	for(int i=0; i<length; ++i) {
		for(int j=0; j<length-1; ++j) {
			if(Data[j] < Data[j+1]) {
				temp = Data[j];
				Data[j] = Data[j+1];
				Data[j+1] = temp;
			}
		}
	}

	*this = Matrix(Data, m_nRows, m_nCols);

	return *this;
}

//returns a matrix that is scaled between Min and Max of the calling matrix
Matrix& Matrix::GetNormalized(const double Min, const double Max) const
{
	static Matrix temp;

	temp = *this;
	temp.Normalize(Min, Max);

	return temp;
}

//scales the values between Min and Max
Matrix& Matrix::Normalize(const double Min, const double Max)
{
	double MatMin, MatMax;
	double Range, R_Range;

	GetNumericRange(MatMin, MatMax);

	Range = MatMax - MatMin;
	R_Range = Max - Min;

	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			m_pData[i][j] -= MatMin;
			m_pData[i][j] /= Range;
			m_pData[i][j] *= R_Range;
			m_pData[i][j] += Min;
		}
	}

	return *this;
}

//returns the covariant of the calling matrix (transposed(obj) * obj)
Matrix& Matrix::GetCovariant() const
{
	Matrix temp;

	temp = *this;
	temp.Transpose();

	return(*this * temp);
}

//turns this matrix into its covariant(obj = transposed(obj) * obj)
Matrix& Matrix::MakeCovariant()
{
	*this = this->GetCovariant();

	return *this;
}

//a way to display the matrix
//formatted
void Matrix::Display() const
{
	cout << "[";
	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			cout << "[" << m_pData[i][j] << "]";
		}
		if(m_nRows-i-1) cout << "\n ";
	}
	cout << "]\n";
}

//another way to display the matrix
//can output to a stream other than cout, which is the default
//formatted
void Matrix::Output(ostream& ostr) const
{
	ostr << "[";
	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			ostr << "[" << m_pData[i][j] << "]";
		}
		if(m_nRows-i-1) cout << "\n ";
	}
	ostr << "]\n";
}

void Matrix::Input(istream& istr)
{
	cout << "Rows: ";
	m_nRows = getint(istr);

	cout << "Columns: ";
	m_nCols = getint(istr);

	cout << "Data:\n";
	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			cout << "(" << i << "," << j << "): ";
			m_pData[i][j] = getint(istr);
		}
	}
}

//input from a file
void Matrix::Read(ifstream& istr)
{
	char str[6];

	istr >> str >> m_nRows;                     //"Rows: "
	istr >> str >> m_nCols;                     //"Cols: "
	istr >> str;                                //"Data\n"
	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			istr >> m_pData[i][j];
		}
	}
}

//output to a file
void Matrix::Write(ofstream& ostr) const
{
	ostr << "Rows: " << m_nRows << '\n';
	ostr << "Cols: " << m_nCols << '\n';
	ostr << "Data:\n";
	for(int i=0; i<m_nRows; ++i) {
		for(int j=0; j<m_nCols; ++j) {
			ostr << m_pData[i][j] << '\n';
		}
	}
}

//returns an identity matrix of size Diagonal
Matrix& Matrix::IdentityMatrix(int Diagonal)
{
	static Matrix temp;
	temp = Matrix(0.0, Diagonal, Diagonal);

	for(int q=0; q<Diagonal; ++q) {
		temp.m_pData[q][q] = 1;
	}

	return temp;
}

//uses Matrix::IdentityMatrix to get an identity matrix of size Diagonal
Matrix& IdentityMatrix(int Diagonal)
{
	return Matrix::IdentityMatrix(Diagonal);
}

//easily display the matrix, usually to the console
//formatted output
ostream& operator <<(ostream& ostr, const Matrix& obj)
{
	ostr << "[";
	for(int i=0; i<obj.GetRows(); ++i) {
		for(int j=0; j<obj.GetColumns(); ++j) {
			ostr << "[" << obj[i][j] << "]";
		}
		if(obj.GetRows()-i-1) cout << "\n ";
	}
	ostr << "]\n";

	return ostr;
}


