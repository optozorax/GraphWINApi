#ifndef __POINTDEFINE
#define __POINTDEFINE

#include <vector>

using namespace std;

typedef AnyPoint<int,2> Point;
typedef AnyPoint<double,2> point2;
typedef AnyPoint<double,3> point3;
typedef AnyPoint<double,4> point4;

template<class T, int n = 2> class AnyPoint {
	vector<T> a_;
public:
	AnyPoint(int = 0, int = 0, int = 0, int = 0);
	~point2();
	
	T& operator[](int);
	
	T lenght(void);
	
	AnyPoint operator+(AnyPoint b);
	bool operator!=(AnyPoint b);
	bool operator==(AnyPoint b);
	
	AnyPoint operator-();
}

template<class T, int n> AnyPoint<T, n> operator*(AnyPoint<T, n>, double);

#endif