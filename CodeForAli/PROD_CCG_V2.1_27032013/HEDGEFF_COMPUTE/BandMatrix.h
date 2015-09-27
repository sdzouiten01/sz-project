/******************************************************************
* This class implements the band matrix which is like this
* 		N = 8, L = 5
*			*----------
*			**--------
*			***------
*			****----
*			*****---
*			-*****--
*			--*****-
*			---*****
*
* 31-05-2010
******************************************************************/
#pragma once
#include <iostream>

class BandMatrix{
	friend std::ostream& operator << (std::ostream& os,  BandMatrix&);
//	BandMatrix(const BandMatrix&);
public:
	BandMatrix();
	void init( unsigned int N,  unsigned int L);
	void reset();
	~BandMatrix();
	double operator()( unsigned int i,  unsigned int j)const;
	void set_val( unsigned int i,  unsigned int j, double val);
	inline  unsigned int dim()const{return _N;}
	inline  unsigned int band_len()const{return _L;}
private:
	 unsigned int _N0;
	 unsigned int _N;
	 unsigned int _L;
	double*	_val;
};
/*
#include <vector>
using  std::vector;
class BandMatrix1{
	friend std::ostream& operator << (std::ostream& os,  BandMatrix1&);
public:
	void init( unsigned int N,  unsigned int L);
	double operator()( unsigned int i,  unsigned int j)const;
	void set_val( unsigned int i,  unsigned int j, double val);
	inline  unsigned int dim()const{return _N;}
	inline  unsigned int band_len()const{return _L;}
private:
	 unsigned int _N0;
	 unsigned int _N;
	 unsigned int _L;
	vector<double>	_val;
};
class BandMatrix2{
	friend std::ostream& operator << (std::ostream& os,  BandMatrix2&);
public:
	void init( unsigned int N,  unsigned int L);
	double operator()( unsigned int i,  unsigned int j)const;
	void set_val( unsigned int i,  unsigned int j, double val);
	inline  unsigned int dim()const{return _N;}
	inline  unsigned int band_len()const{return _L;}
private:
	 unsigned int _N;
	 unsigned int _L;
	vector<vector<double> >	_val;
	double _zero_val;
};
*/