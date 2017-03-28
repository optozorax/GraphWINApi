#ifndef __COLORDEFINE
#define __COLORDEFINE

#include <vector>
#include <iostream>
#include <windows.h>

using namespace std;

namespace gwapi{

typedef union Color__ {
	DWORD clrref;
	UCHAR m[4];

	Color__() {};
	Color__(DWORD a) {clrref = a;}
} Color;

const Color White = {0xFFFFFFFF};
const Color Black = {0xFF000000};

Color rgb(const int, const int, const int);
Color argb(const int, const int, const int, const int); 

Color gray(const double, const bool cycle = false);
Color rainbow(const double, const bool cycle = false);
Color gradient(const double, const bool cycle, vector<Color>);

Color overlay(const Color&, const Color&);

COLORREF toWindowsColor(const Color&);

}

#endif