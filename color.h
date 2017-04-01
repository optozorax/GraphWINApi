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
const Color Transparent = {0x01000000};

// TODO сделать много различных стандартных цветов
/* const Color Red = rgb(255,0,0);
const Color Green = rgb(0,255,0);
const Color Blue = rgb(0,0,255);
const Color Yellow = rgb(255,255,0);
const Color Purple = rgb(255,0,255);

const Color LightBlue = rgb(0,255,255);
const Color Miku = rgb(9,237,195);  */

Color rgb(const int, const int, const int);
Color argb(const int, const int, const int, const int); 

Color gray(const double, const bool cycle = false);
Color rainbow(const double, const bool cycle = false);
Color gradient(const double, const bool cycle, vector<Color>);

Color overlay(Color, Color);

COLORREF toWindowsColor(const Color&);

//#define __COLORDEBUG

}

#endif