#include "bufer.h"

namespace gwapi{

Bufer::Bufer(int x, int y) : sizex(x), sizey(y) {
	VOID *pvBits;

	/* Структура для того, чтобы связать картинку с массивом. */
	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	bmi.bmiHeader.biWidth = x;
	bmi.bmiHeader.biHeight = -y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = x * y * 4;
	
	hdc_ = CreateCompatibleDC(NULL);
	hbmp_ = CreateDIBSection(hdc_, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
	
	mas_ = (UINT32*) pvBits;
	SelectObject(hdc_, hbmp_);
}

Bufer::Bufer(HDC hdc) {
	hdc_ = hdc;
	hbmp_ = NULL;
	mas_ = NULL;
	sizex = 1000;
	sizey = 1000;
}

Bufer::~Bufer() {
	DeleteObject(hbmp_);
	DeleteDC(hdc_);
}

void Bufer::drawTo(Bufer a, int x, int y, int width, int height) {
	/* Обработка слишком больших границ рисования. */
	x = min(a.sizex, x);
	y = min(a.sizey, y);
	
	if (width == 0) width = a.sizex;	
	if (height == 0) height = a.sizey;
	
	//width = min(x + width, a.sizex) - width;
	//height = min(y + height, a.sizey) - height;
	
	BitBlt(a.hdc_, x, y, width, height, hdc_, 0, 0, SRCCOPY);
}

void Bufer::drawAlphaTo(Bufer a, int x, int y, int width, int height) {
	// TODO протестировать как работает
	for (int x = 0; x<sizex; x++) {
		for (int y = 0; y<sizey; y++) {
			a[Point(x, y)] = overlay(a[Point(x, y)], operator[](Point(x,y))).clrref;
		}
	}
}

void Bufer::clear(Color cls) {
	for (int x = 0; x<sizex; x++) {
		for (int y = 0; y<sizey; y++) {
			operator[](Point(x, y)) = cls.clrref;
		}
	}
}

Pen Bufer::penSet(Color clr, int thick) {
	Pen a1 = pen_;
	
	pen_.color = clr;
	pen_.thickness = thick;
	
	SelectObject(hdc_, CreatePen(PS_SOLID, thick, toWindowsColor(clr)));
	
	return a1;
}

Brush Bufer::brushSet(Color clr) {
	Brush a1 = brush_;
	
	brush_ = clr;
	
	SelectObject(hdc_, CreateSolidBrush(toWindowsColor(clr)));
	
	return a1;
}

void Bufer::textOut(Point x, string str, TextStyle stl) {
	// TODO сделать для разных стилей
	TextOut(hdc_, x[0], x[1], (LPCSTR)str.c_str(), strlen(str.c_str())); 
}

void Bufer::textStyle(Color c, int size, string name) {
	// TODO протестировать
	LOGFONT font;
	HFONT hfont;
	font.lfHeight 			= -size; /* Высота шрифта. */ 
	font.lfWidth 			= 0; /* Ширина символов в шрифте. */
	font.lfEscapement 		= 0; /* Угол наклона относительно горизонта. */ 
	font.lfOrientation 		= 0; /* Угол между основной линией каждого символа и осью X устройства. */ 
	font.lfWeight 			= 0; /* Толщина шрифта в диапазоне от 0 до 1000. */ 
	font.lfItalic 			= FALSE; /* Курсивный шрифт. */
	font.lfUnderline 		= FALSE; /* Подчеркнутый шрифт. */
	font.lfStrikeOut 		= FALSE; /* Зачеркнутый шрифт. */
	font.lfCharSet 			= RUSSIAN_CHARSET; /* Набор символов. */
	font.lfOutPrecision 		= 0; /* Точность вывода. */
	font.lfClipPrecision 		= CLIP_DEFAULT_PRECIS; /* Точность отсечения. */
	font.lfQuality 			= PROOF_QUALITY; /* Качество вывода. */
	font.lfPitchAndFamily 		= 0; /* Ширина символов и семейство шрифта. */

	/* Название шрифта. */
	for (int i = 0; i < name.size(); i++) {
		font.lfFaceName[i] = name[i];
	}
	for (int i = name.size(); i < 32; i++) {
		font.lfFaceName[i] = 0;
	}

	/* Применение шрифта к дескриптору окна. */
	hfont = ::CreateFontIndirect(&font);
	SelectObject(hdc_, hfont);
}

void Bufer::pixelDraw(Point x, Color c) {
	operator[](x) = c.clrref;
}

Color Bufer::pixelGet(Point a) {
	return operator[](a);
}

void Bufer::rectDraw(Point a, Point b) {
	for (int i = a[0]; i < b[0]; i++) {
		for (int j = a[1]; j < b[1]; j++) {
			operator[](Point(i, j)) = brush_.clrref;
		}
	}
	lineDraw(a, Point(b[0], a[1]));
	lineDraw(a, Point(a[0], b[1]));
	lineDraw(b, Point(b[0], a[1]));
	lineDraw(b, Point(a[0], b[1]));
}

void Bufer::circleDraw(Point c, int r) {
	Ellipse(hdc_, c[0] - r, c[1] - r, c[0] + r, c[1] + r);
}

void Bufer::lineDraw(Point a, Point b) {
	MoveToEx(hdc_, a[0], a[1], NULL);
	LineTo(hdc_, b[0], b[1]);
}

void Bufer::bezierDraw(vector<Point>, BezierStyle) {
	// TODO сделать потом
}

inline UINT32& Bufer::operator[](Point a) {
	return mas_[a[0] + sizey*a[1]];
}

void gwapi::Bufer::pixelDraw(point2, Color) {
	// TODO реализовать в далеком будущем
}

Color gwapi::Bufer::pixelGet(point2) {
	// TODO реализовать в далеком будущем
	return Color();
}

void gwapi::Bufer::rectDraw(point2, point2) {
	// TODO реализовать в далеком будущем
}

void gwapi::Bufer::circleDraw(point2, double) {
	// TODO реализовать в далеком будущем
}

void gwapi::Bufer::lineDraw(point2, point2) {
	// TODO реализовать в далеком будущем
}

}

#define __BUFERTEST
#ifdef __BUFERTEST
using namespace gwapi;
#include <windows.h>
#include <iostream>
#include <stdlib.h>
int main() {
	Bufer a(300,300);
	a.clear();
	a.penSet(rgb(128,0,128), 3);
	a.lineDraw(Point(100,100), Point(200,200));
	a.lineDraw(Point(100,200), Point(200,100));
	a.circleDraw(Point(150,150), 50);
	a.drawTo(GetDC(GetConsoleWindow()));
	system("pause");
	return 0;
}
#endif