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
} Color;

const Color White = {0xFFFFFFFF};
const Color Black = {0xFF000000};

Color rgb(int, int, int);
Color argb(int, int, int, int);

Color gray(double);
//Color rainbow(double, bool cycle = false, bool gray = false);
//Color gradient(double, vector<Color>);

//Color operator+(Color, Color);
//Color operator*(Color, unsigned char /* double??? */);

Color overlay(const Color&, const Color&);

}

#endif