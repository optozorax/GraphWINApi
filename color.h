#pragma once

#include <vector>

namespace wgs{

union color {
	DWORD clrref;
	UCHAR m[4];

	color() {};
	color(DWORD a) {clrref = a;}
};

color rgb(const int, const int, const int);
color argb(const int, const int, const int, const int); 

color gray(const double, const bool cycle = false);
color rainbow(const double, const bool cycle = false);
color gradient(const double, const bool cycle, std::vector<color>, std::vector<double> = std::vector<double>());

color overlay(color, color);

COLORREF toWindowsColor(const color&);

/* Стандартные цвета. */
const color White = rgb(255, 255, 255);
const color Black = rgb(0, 0, 0);
const color Transparent = argb(1, 0, 0, 0);

const color Red = rgb(255,0,0);
const color Green = rgb(0,255,0);
const color Blue = rgb(0,0,255);

const color Yellow = rgb(255,255,0);
const color Fuchsia = rgb(255,0,255);
const color Aqua = rgb(0,255,255);

const color Maroon = rgb(128, 0, 0);
const color Oliv = rgb(128, 128, 0);
const color Purple = rgb(128, 0, 128);
const color Teal = rgb(0, 128, 128);
const color Navy = rgb(0, 0, 128);
const color Gray = rgb(128, 128, 128);

const color DarkBlue = rgb(77,77,255);
const color LightBlue = rgb(0,191,255);
const color Orange = rgb(255,128,0);
const color Gold = rgb(0xFF, 0xD7, 0);
const color Indigo = rgb(0x4B, 0, 0x80);
const color Silver = rgb(0xC0, 0xC0, 0xC0);
const color Violet = rgb(0xEE, 0x80, 0xEE);

const color Ubuntu = rgb(0x77, 0x21, 0x6F);
const color UbuntuOrange = rgb(0xE9, 0x54, 0x20);
const color Bitcoin = rgb(0xFF,	0x99, 0x00);
const color Miku = rgb(9,237,195);
const color Dollar = rgb(0x85, 0xbb, 0x65);
const color SeaGreen = rgb(0x52, 0x9B, 0x2F);
const color Pink = rgb(0xF9, 0x26, 0x72);
const color GreenYellow = rgb(0xAD, 0xFF, 0x2F);

}