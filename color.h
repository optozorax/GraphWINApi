#ifndef __COLORDEFINE
#define __COLORDEFINE

#include <vector>

namespace gwapi{

typedef union Color__ {
	DWORD clrref;
	UCHAR m[4];

	Color__() {};
	Color__(DWORD a) {clrref = a;}
} Color;

Color rgb(const int, const int, const int);
Color argb(const int, const int, const int, const int); 

Color gray(const double, const bool cycle = false);
Color rainbow(const double, const bool cycle = false);
Color gradient(const double, const bool cycle, std::vector<Color>, std::vector<double> = std::vector<double>());

Color overlay(Color, Color);

COLORREF toWindowsColor(const Color&);

/* Стандартные цвета. */
const Color White = rgb(255, 255, 255);
const Color Black = rgb(0, 0, 0);
const Color Transparent = argb(1, 0, 0, 0);

const Color Red = rgb(255,0,0);
const Color Green = rgb(0,255,0);
const Color Blue = rgb(0,0,255);
const Color Yellow = rgb(255,255,0);
const Color Purple = rgb(255,0,255);
const Color LightBlue = rgb(0,255,255);

// TODO дописать всякие еще

const Color Ubuntu = rgb(0x77, 0x21, 0x6F);
const Color UbuntuOrange = rgb(0xE9, 0x54, 0x20);
const Color Bitcoin = rgb(0xFF,	0x99, 0x00);
const Color Miku = rgb(9,237,195);
const Color Dollar = rgb(0x85, 0xbb, 0x65);

/* Monokai: можно эти цвета в пространстве имен сделать
@ghost-white: #F8F8F0;
@light-ghost-white: #F8F8F2;
@light-gray: #CCC;
@gray: #888;
@brown-gray: #49483E;
@dark-gray: #282828;

@yellow: #E6DB74;
@blue: #66D9EF;
@pink: #F92672;
@purple: #AE81FF;
@brown: #75715E;
@orange: #FD971F;
@light-orange: #FFD569;
@green: #A6E22E;
@sea-green: #529B2F; */

//#define __COLORDEBUG

}

#endif