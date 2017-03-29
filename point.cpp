#include "point.h"

template<class T, int n>
inline AnyPoint<T, n>::AnyPoint(T x, T y, T z, T w) : a_(n) {
	if (n == 2) {
		a_[0] = x;
		a_[1] = y;
	} else
	if (n == 3) {
		a_[0] = x;
		a_[1] = y;
		a_[2] = z;
	} else
	if (n == 4) {
	 	a_[0] = x;
		a_[1] = y;
		a_[2] = z;
		a_[3] = w;
	} else {
		for (int i = 0; i < n; i++) {
			a_[i] = 0;
		}
	}
}

template<class T, int n>
inline T& AnyPoint<T, n>::operator[](int i) {
	return a_[i];
}

template<class T, int n>
T AnyPoint<T, n>::length(void) {
	T sum = 0;
	for (int i = 0; i<n; i++) {
		sum += a_[i]*a_[i];
	}
	sum = sqrt(sum);
	return sum;
}

template<class T, int n>
inline AnyPoint<T,n> operator+(AnyPoint<T,n> a, AnyPoint<T,n> b) {
	for (int i = 0; i < n; i++) {
		a[i] += b[i];
	}
	return a;
}

template<class T, int n>
AnyPoint<T, n> operator-(AnyPoint<T, n> a, AnyPoint<T, n> b) {
	return a + (-b);
}

template<class T, int n>
AnyPoint<T, n> operator*(AnyPoint<T, n> a, T b) {
	for (int i = 0; i < n; i++) {
		a[i] *= b;
	}

	return a;
}

template<class T, int n>
bool operator!=(AnyPoint<T, n> a, AnyPoint<T, n> b) {
	return !(a == b);
}

template<class T, int n>
bool operator==(AnyPoint<T, n> a, AnyPoint<T, n> b) {
	bool cond = true;
	for (int i = 0; i < n; i++) {
		cond = cond && (a[i] == b[i]);
	}
	return cond;
}

template<class T, int n>
ostream & operator<<(ostream &cout, AnyPoint<T, n> a)
{
	cout << "(";
	for (int i = 0; i < n; i++) {
		cout << a[i];
		if (i == (n-1)) 
			cout << ")"; 
		else 
			cout << ", ";
	}

	return cout;
}

template<class T, int n>
AnyPoint<T, n> operator-(AnyPoint<T, n> a) {
	return a*((T)-1);
}

#define __POINTDEBUG

#ifdef __POINTDEBUG
#include <stdio.h>
#include <stdlib.h>
int main() {
	point2 a1, a2(2, 4); a1[1] = 1;
	point3 b1(1, 2, 3), b2(3, 3, 3);
	point4 c1(1,2,3,4), c2(-1,1,-1,1);
	Point d1(2,4), d2(1,-1);

	cout.precision(1);

	cout << "This is test of Point module." << endl;

	cout << "a1 = " << a1 << endl;
	cout << "a2 = " << a2 << endl;
	cout << "a1 + a2 = " << a1 + a2 << endl;
	cout << "a1 - a2 = " << a1 - a2 << endl;
	cout << "-a1 = " << -a1 << endl;
	cout << "a2*2 = " << a2*2.0 << endl;
	cout << "(a2 != a1) = " << ((a2 != a1) ?  "true" : "false") << endl;
	cout << "length(a1) = " << a1.length() << endl;
	cout << "a1.x, a1.y = " << a1[0] << ", " << a1[1] << endl << endl; 

	cout << "b1 = " << b1 << endl;
	cout << "b2 = " << b2 << endl;
	cout << "b1 + b2 = " << b1 + b2 << endl;
	cout << "b1 - b2 = " << b1 - b2 << endl;
	cout << "-b1 = " << -b1 << endl;
	cout << "b2*2 = " << b2*2.0 << endl;
	cout << "(b2 != b1) = " << ((b2 != b1) ?  "true" : "false") << endl;
	cout << "length(b1) = " << b1.length() << endl << endl;

	cout << "c1 = " << c1 << endl;
	cout << "c2 = " << c2 << endl;
	cout << "c1.x, c1.y, c1.z, c1.w = " << c1[0] << ", " << c1[1] << ", " << c1[2] << ", " << c1[3] << endl << endl;

	cout << "d1 = " << d1 << endl;
	cout << "d2 = " << d2 << endl;
	cout << "d1 + d2*2 = " << d1 + d2*2 << endl;

	system("pause");
}
#endif