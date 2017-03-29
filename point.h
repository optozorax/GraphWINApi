#ifndef __POINTDEFINE
#define __POINTDEFINE

#include <vector>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

template<class T, int n = 2> class AnyPoint {
	vector<T> a_;
public:
	AnyPoint(T = 0, T = 0, T = 0, T = 0);
	T& operator[](int);
	T length(void);
};

template<class T, int n> 
AnyPoint<T, n> operator+( AnyPoint<T, n> a,  AnyPoint<T, n> b);
template<class T, int n> 
AnyPoint<T, n> operator-( AnyPoint<T, n> a,  AnyPoint<T, n> b);
template<class T, int n> 
AnyPoint<T, n> operator*( AnyPoint<T, n> a,  T b);
template<class T, int n> 
bool operator!=( AnyPoint<T, n> a,  AnyPoint<T, n> b);
template<class T, int n> 
bool operator==( AnyPoint<T, n> a,  AnyPoint<T, n> b);
template<class T, int n> 
ostream& operator<<(ostream& cout,  AnyPoint<T, n> a);
template<class T, int n> 
AnyPoint<T, n> operator-( AnyPoint<T, n> a);

typedef AnyPoint<int,2> Point;
typedef AnyPoint<double,2> point2;
typedef AnyPoint<double,3> point3;
typedef AnyPoint<double,4> point4;

#endif