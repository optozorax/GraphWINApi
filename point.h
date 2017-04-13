#pragma once

#include <iostream>
#include <math.h>

template<class T> class _point {
public:
	T x, y;
	_point(T = 0, T = 0);
	
	T length(void);
	void rotate(T, _point<T>);

	template<class T1>
	operator _point<T1>();
};

template<class T> 
_point<T> operator+(_point<T> a, _point<T> b);
template<class T> 
_point<T> operator-(_point<T> a, _point<T> b);
template<class T> 
_point<T> operator*(_point<T> a,  T b);
template<class T>
_point<T> operator/(_point<T> a,  T b);
template<class T> 
bool operator!=(_point<T> a, _point<T> b);
template<class T> 
bool operator==(_point<T> a, _point<T> b);
template<class T> 
std::ostream& operator<<(std::ostream& cout, _point<T> a);
template<class T> 
_point<T> operator-(_point<T> a);

template<class T> 
bool inRectangle(_point<T>, _point<T>, _point<T>);

typedef _point<int> Point;
typedef _point<double> point2;

// ------------------------------------------------------------------------- //

template<class T>
inline _point<T>::_point(T x1, T y1) {
	x = x1;
	y = y1;
}

template<class T>
inline T _point<T>::length(void) {
	return sqrt(x*x + y*y);
}

template<class T>
inline void _point<T>::rotate(T angle, _point<T> center) {
	T x1 = x - center.x;
	T y1 = y - center.y;
	x = cos(angle)*x1 - sin(angle)*y1 + center.x;
	y = sin(angle)*x1 + cos(angle)*y1 + center.y;
}

template<class T>
template<class T1>
inline _point<T>::operator _point<T1>() {
	return _point<T1>((T1) x, (T1) y);
}

template<class T>
inline _point<T> operator+(_point<T> a, _point<T> b) {
	return _point<T>(a.x + b.x, a.y + b.y);
}

template<class T>
inline _point<T> operator-(_point<T> a, _point<T> b) {
	return _point<T>(a.x - b.x, a.y - b.y);
}

template<class T>
inline _point<T> operator*(_point<T> a, T b) {
	return _point<T>(a.x*b, a.y*b);
}

template<class T>
inline _point<T> operator/(_point<T> a, T b) {
	return _point<T>(a.x/b, a.y/b);
}

template<class T>
inline _point<T> operator-(_point<T> a) {
	return _point<T>(-a.x, -a.y);
}

template<class T>
inline bool inRectangle(_point<T> x, _point<T> a, _point<T> b) {
	return (x.x >= a.x) && (x.x <= b.x) && (x.y >= a.y) && (x.y <= b.y);
}

template<class T>
inline std::ostream & operator<<(std::ostream & cout, _point<T> a) {
	cout << "(" << a.x << ", " << a.y << ")";
	return cout;
}

template<class T>
inline bool operator!=(_point<T> a, _point<T> b) {
	return (a.x != b.x) && (a.y != b.y);
}

template<class T>
inline bool operator==(_point<T> a, _point<T> b) {
	return (a.x == b.x) && (a.y == b.y);
}