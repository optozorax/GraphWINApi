#pragma once

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
const Color Fuchsia = rgb(255,0,255);
const Color Aqua = rgb(0,255,255);

const Color Maroon = rgb(128, 0, 0);
const Color Oliv = rgb(128, 128, 0);
const Color Purple = rgb(128, 0, 128);
const Color Teal = rgb(0, 128, 128);
const Color Navy = rgb(0, 0, 128);
const Color Gray = rgb(128, 128, 128);

const Color DarkBlue = rgb(77,77,255);
const Color LightBlue = rgb(0,191,255);
const Color Orange = rgb(255,128,0);
const Color Gold = rgb(0xFF, 0xD7, 0);
const Color Indigo = rgb(0x4B, 0, 0x80);
const Color Silver = rgb(0xC0, 0xC0, 0xC0);
const Color Violet = rgb(0xEE, 0x80, 0xEE);

const Color Ubuntu = rgb(0x77, 0x21, 0x6F);
const Color UbuntuOrange = rgb(0xE9, 0x54, 0x20);
const Color Bitcoin = rgb(0xFF,	0x99, 0x00);
const Color Miku = rgb(9,237,195);
const Color Dollar = rgb(0x85, 0xbb, 0x65);
const Color SeaGreen = rgb(0x52, 0x9B, 0x2F);
const Color Pink = rgb(0xF9, 0x26, 0x72);
const Color GreenYellow = rgb(0xAD, 0xFF, 0x2F);

}