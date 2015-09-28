#include "BandMatrix.h"
using namespace std;

BandMatrix::BandMatrix():_L(0), _N(0), _N0(0), _val(0){}
BandMatrix::~BandMatrix()
{
	if ( _val != NULL )
	{
		delete [] _val;
	}
	_val = NULL;
}
void BandMatrix::init( unsigned int N,  unsigned int L)
{
	if ( _N != N || _L != L )
	{
		_N = N;
		_L = L;
		if ( _L >= _N )
		{
			_L = _N;
		}
		_N0 = (1+_L)*_L/2;
		if ( _val == NULL )
		{
			_val = new double[_N0+(_N-_L)*_L]();
		}
	}
	else
		this->reset();
}
void BandMatrix::reset()
{
	unsigned int len = _N0+(_N-_L)*_L;
	for (int i = 0; i < len; ++i)
	{
		_val[i] = 0;
	}
}
double BandMatrix::operator ()( unsigned int i,  unsigned int j)const
{
	unsigned int pos = i - _L + 1;
	if ( i >= _L)
	{
		if ( j <= i && j >= pos )
		{
			return _val[j - pos + _N0+(i-_L)*_L];
		}else
			return 0;
	}
	else// i < _L
	{
		if ( j <= i && j >= 0)
		{
			return _val[ (1+i)*i/2+j ];
		}
		else
			return 0;
	}
}
void BandMatrix::set_val( unsigned int i,  unsigned int j, double v)
{
	unsigned int pos = i - _L + 1;
	if ( i >= _L)
	{
		if ( j <= i && j >= pos )
		{
			_val[j - pos + _N0+(i-_L)*_L] = v;
		}else
			return;
	}
	else// i < _L
	{
		if ( j <= i && j >= 0)
		{
			_val[ (1+i)*i/2+j ] = v;
		}
		else
			return;
	}
}
ostream& operator << (ostream& os,  BandMatrix& mat)
{
	for ( unsigned int i = 0; i < mat.dim(); ++i)
	{
		for ( unsigned int j = 0; j < mat.dim(); ++j)
		{
			os << mat(i, j) << ' ';
		}
		os << endl;
	}
	return os;
}

/*
void BandMatrix1::init( unsigned int N,  unsigned int L)
{
	_N = N;
	_L = L;
	_N0 = (1+_L)*_L/2;
	_val.resize(_N0+(_N-_L)*_L);

	unsigned int count = 1;
	for ( unsigned int i = 0; i < _N; ++i)
	{
		for ( unsigned int j = 0; j < _N; ++j)
		{
			this->set_val(i, j, count++);
		}
	}
}
double BandMatrix1::operator ()( unsigned int i,  unsigned int j)const
{
	unsigned int pos = i - _L + 1;
	if ( i >= _L)
	{
		if ( j <= i && j >= pos )
		{
			return _val[j - pos + _N0+(i-_L)*_L];
		}else
			return 0;
	}
	else// i < _L
	{
		if ( j <= i && j >= 0)
		{
			return _val[ (1+i)*i/2+j ];
		}
		else
			return 0;
	}
}
void BandMatrix1::set_val( unsigned int i,  unsigned int j, double v)
{
	unsigned int pos = i - _L + 1;
	if ( i >= _L)
	{
		if ( j <= i && j >= pos )
		{
			_val[j - pos + _N0+(i-_L)*_L] = v;
		}else
			return;
	}
	else// i < _L
	{
		if ( j <= i && j >= 0)
		{
			_val[ (1+i)*i/2+j ] = v;
		}
		else
			return;
	}
}
ostream& operator << (ostream& os,  BandMatrix1& mat)
{
	for ( unsigned int i = 0; i < mat.dim(); ++i)
	{
		for ( unsigned int j = 0; j < mat.dim(); ++j)
		{
			os << mat(i, j) << ' ';
		}
		os << endl;
	}
	return os;
}

void BandMatrix2::init( unsigned int N,  unsigned int L)
{
	_zero_val = 0;
	_N = N;
	_L = L;
	this->_val.resize(_N);
	for ( unsigned int i = 0; i < _L; ++i) 
	{
		_val[i].resize(i+1);
	}
	for (  unsigned int i = _L; i < N; ++i)
	{
		_val[i].resize(_L);
	}
	unsigned int count = 1;
	for ( unsigned int i = 0; i < _N; ++i)
	{
		for ( unsigned int j = 0; j < _N; ++j)
		{
			this->set_val(i, j, count++);
		}
	}
}
double BandMatrix2::operator ()( unsigned int i,  unsigned int j)const
{
	unsigned int pos = i - _L + 1;
	if ( i >= _L)
	{
		if ( j <= i && j >= pos )
		{
			return _val[i][j-pos];
		}else
			return 0;
	}
	else// i < _L
	{
		if ( j <= i && j >= 0)
		{
			return _val[i][j];
		}
		else
			return 0;
	}
}
void BandMatrix2::set_val( unsigned int i,  unsigned int j, double v)
{
	unsigned int pos = i - _L + 1;
	if ( i >= _L)
	{
		if ( j <= i && j >= pos )
		{
			_val[i][j-pos] = v;
		}else
			return;
	}
	else// i < _L
	{
		if ( j <= i && j >= 0)
		{
			_val[i][j] = v;
		}
		else
			return;
	}
}
ostream& operator << (ostream& os,  BandMatrix2& mat)
{
	for ( unsigned int i = 0; i < mat.dim(); ++i)
	{
		for ( unsigned int j = 0; j < mat.dim(); ++j)
		{
			os << mat(i, j) << ' ';
		}
		os << endl;
	}
	return os;
}
*/