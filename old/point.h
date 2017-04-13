#pragma once

#include <vector>
#include <iostream>
#include <math.h>

/* TODO если хочется, чтобы класс AnyPoint для разных размеров n обладал разными ссылками
   (для 2: x, y; для 3: x, y, z; и т.д.), то придется писать для каждого n реализацию вручную.
   Ни наследование, ни специализация, ни еще что-то не избавят от лишнего написания кода.

   Так что этот универсальный шаблон остается универсальным, подходит для всех размеров n, 
   но имеет существенные недостатки вроде: излишний набор ссылок, минимальный размер = 4
   неидеальная оптимизация.

   Когда будет полностью закончен этот шаблон, и для него будут написаные все необходимые функции, 
   только тогда можно начинать писать для разных n и общий для n>4. Потому, что если это сделать
   сейчас, то потом придется в каждый класс дописывать это.
   
   Или же можно не трогать пока точки размерности три и четыре, и написать шаблон точки для
   двумерного случая. Уже только когда понадобятся размерности побольше, реализовать для них. */

/* Самое главное в этом модуле для графики - это point2 и Point. Так что можно заботиться только
   об их работоспособности. */

template<class T, int n = 2> class AnyPoint {
protected:
	std::vector<T> a_;
public:
	T &x; 
	T &y;
	T &z;
	T &w;

	AnyPoint(T = 0, T = 0, T = 0, T = 0);
	T& operator[](int);
	T length(void);

	void rotate(T, AnyPoint<T, n> = AnyPoint<T, n>(), int = 0, int = 1);

	AnyPoint<T, n>& operator=(const AnyPoint<T, n>&);
};

template<class T1, class T, int n> 
void to_another(AnyPoint<T, n>&, AnyPoint<T1, n>);

template<class T, int n> 
AnyPoint<T, n> operator+( AnyPoint<T, n> a,  AnyPoint<T, n> b);
template<class T, int n> 
AnyPoint<T, n> operator-( AnyPoint<T, n> a,  AnyPoint<T, n> b);
template<class T, int n> 
AnyPoint<T, n> operator*( AnyPoint<T, n> a,  T b);
template<class T, int n> 
AnyPoint<T, n> operator/( AnyPoint<T, n> a,  T b);
template<class T, int n> 
bool operator!=( AnyPoint<T, n> a,  AnyPoint<T, n> b);
template<class T, int n> 
bool operator==( AnyPoint<T, n> a,  AnyPoint<T, n> b);
template<class T, int n> 
std::ostream& operator<<(std::ostream& cout,  AnyPoint<T, n> a);
template<class T, int n> 
AnyPoint<T, n> operator-( AnyPoint<T, n> a);

template<class T, int n> 
bool inRectangle(AnyPoint<T, n>, AnyPoint<T, n>, AnyPoint<T, n>);

typedef AnyPoint<int,2> Point;
typedef AnyPoint<double,2> point2;
typedef AnyPoint<double,3> point3;
typedef AnyPoint<double,4> point4;

// ------------------------------------------------------------------------- //

template<class T, int n>
inline AnyPoint<T, n>::AnyPoint(T x1, T y1, T z1, T w1) : a_(max(4, n)), x(a_[0]), y(a_[1]), z(a_[2]), w(a_[3]) {
	if (n == 2) {
		a_[0] = x1;
		a_[1] = y1;
	} else
	if (n == 3) {
		a_[0] = x1;
		a_[1] = y1;
		a_[2] = z1;
	} else
	if (n == 4) {
	 	a_[0] = x1;
		a_[1] = y1;
		a_[2] = z1;
		a_[3] = w1;
	} else {
		for (int i = 0; i < n; i++) {
			a_[i] = 0;
		}
	}
}

template<class T, int n>
inline AnyPoint<T, n> & AnyPoint<T, n>::operator=(const AnyPoint<T, n> &b) {
	x = b.x;
	y = b.y;
	if (n >= 3) z = b.z;
	if (n >= 4) w = b.z;
	a_ = b.a_;
	return *this;
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
inline void AnyPoint<T, n>::rotate(T angle, AnyPoint<T, n> b, int first, int second) {
	T x = operator[](first) - b[first];
	T y = operator[](second) - b[second];
	operator[](first) = cos(angle)*x - sin(angle)*y + b[first];
	operator[](second) = sin(angle)*x + cos(angle)*y + b[second];
}

template<class T1, class T, int n>
inline void to_another(AnyPoint<T, n> &a, AnyPoint<T1, n> b) {
	for (int i = 0; i < n; i++) {
		a[i] = (T)(b[i]);
	}
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
AnyPoint<T, n> operator/(AnyPoint<T, n> a, T b) {
	for (int i = 0; i < n; i++) {
		a[i] /= b;
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
std::ostream & operator<<(std::ostream &cout, AnyPoint<T, n> a) {
	cout << "(";
	for (int i = 0; i < n-1; i++) {
		cout << a[i] << ", ";
	}
	cout << a[n-1] << ")";

	return cout;
}

template<class T, int n>
AnyPoint<T, n> operator-(AnyPoint<T, n> a) {
	return a*((T)-1);
}

template<class T, int n> 
bool inRectangle(AnyPoint<T, n> a, AnyPoint<T, n> b, AnyPoint<T, n> c) {
	for (int i = 0; i < n; i++) {
		if (!(a[i]>=b[i] && a[i]<=c[i])) {
			return false;
		}
	}
	return true;
};

//#define __POINTDEBUG

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