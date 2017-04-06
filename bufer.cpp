#include "bufer.h"

namespace gwapi{

Bufer::Bufer(int x, int y) : sizex(x), sizey(y) {
	VOID *pvBits;

	/* ��������� ��� ����, ����� ������� �������� � ��������. */
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

	brushSet(White);
	penSet(Black);
}

Bufer::Bufer(HDC hdc) {
	hdc_ = hdc;
	hbmp_ = NULL;
	mas_ = NULL;
	sizex = 1000;
	sizey = 1000;

	brushSet(White);
	penSet(Black);
}

Bufer::~Bufer() {
	DeleteObject(hbmp_);
	DeleteDC(hdc_);
}

void Bufer::boardsToCorrect(Bufer &a, int &x, int &y, int &width, int &height) {
	// TODO �������������� �� ��� ������ �����.
	/* ���� ������ � ������ ����� ����, �� ��� ������, ��� ���� ���������� ���� �����. */
	if (width == 0) width = sizex;	
	if (height == 0) height = sizey;

	/* �������� ������ �� ������� ������ ��� ���������. */
	if (width > sizex) width = sizex;
	if (height > sizey) height = sizey;

	if (a.sizex != -1) {
		/* ��������, �� ������� �� x y �� ������� ������� ������. */
		if (x > a.sizex) x = sizex;
		if (y > a.sizey) y = sizey;

		/* �������� �� ������� �� ������� �������� �� ������� ������� ������. */
		if (x+width > a.sizex) width = sizex-x;
		if (y+width > a.sizey) height = sizey-y;
	}
}

void Bufer::drawTo(Bufer &a, int x, int y, int width, int height) {
	boardsToCorrect(a, x, y, width, height);
	BitBlt(a.hdc_, x, y, width, height, hdc_, 0, 0, SRCCOPY);
}

void Bufer::drawAlphaTo(Bufer &a, int x, int y, int width, int height) {
	boardsToCorrect(a, x, y, width, height);

	/* ���� ��������� � �������������. */
	Point c, d;
	for (int x1 = x; x1<x+width; x1++) {
		for (int y1 = y; y1<y+height; y1++) {
			c = Point(x1, y1);
			d = Point(x1 - x, y1 - y);
			a[c] = overlay(operator[](d), a[c]).clrref;
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

void Bufer::textOut(Point x, string str, TextWriteStyle stl) {
	// TODO ������� ��� ������ ������.
	// TODO ������� ����� �������� ��� ��������� �����.
	// GetTextExtentPoint32 - ��� �������
	TextOut(hdc_, x[0], x[1], (LPCSTR)str.c_str(), strlen(str.c_str())); 
}

void Bufer::textStyle(int size, string name) {
	// TODO ������: ���� �� ����� ��� ������� �������� ����� ���������, ���: �������, ������ � �.�. ��� �������� ����� ���������
	LOGFONT font;
	font.lfHeight 			= -size; /* ������ ������. */ 
	font.lfWidth 			= 0; /* ������ �������� � ������. */
	font.lfEscapement 		= 0; /* ���� ������� ������������ ���������. */ 
	font.lfOrientation 		= 0; /* ���� ����� �������� ������ ������� ������� � ���� X ����������. */ 
	font.lfWeight 			= 0; /* ������� ������ � ��������� �� 0 �� 1000. */ 
	font.lfItalic 			= FALSE; /* ��������� �����. */
	font.lfUnderline 		= FALSE; /* ������������ �����. */
	font.lfStrikeOut 		= FALSE; /* ����������� �����. */
	font.lfCharSet 			= RUSSIAN_CHARSET; /* ����� ��������. */
	font.lfOutPrecision 	= 0; /* �������� ������. */
	font.lfClipPrecision 	= CLIP_DEFAULT_PRECIS; /* �������� ���������. */
	font.lfQuality 			= PROOF_QUALITY; /* �������� ������. */
	font.lfPitchAndFamily 	= 0; /* ������ �������� � ��������� ������. */

	/* �������� ������. */
	for (int i = 0; i < name.size(); i++) {
		font.lfFaceName[i] = name[i];
	}
	for (int i = name.size(); i < 32; i++) {
		font.lfFaceName[i] = 0;
	}

	/* ���������� ������ � ����������� ����. */
	HFONT hfont = CreateFontIndirect(&font);
	SelectObject(hdc_, hfont);
}

void Bufer::pixelDraw(Point x, Color c) {
	operator[](x) = c.clrref;
}

void Bufer::rectDraw(Point a, Point b) {
	/*for (int i = a[0]; i < b[0]; i++) {
		for (int j = a[1]; j < b[1]; j++) {
			operator[](Point(i, j)) = brush_.clrref;
		}
	}*/
	PatBlt(hdc_, a[0], a[1], b[0]-a[0], b[1]-a[1], PATCOPY);
	lineDraw(a, Point(b[0], a[1]));
	lineDraw(a, Point(a[0], b[1]));
	lineDraw(b, Point(b[0], a[1]-1));
	lineDraw(b, Point(a[0]-1, b[1]));
}

void Bufer::circleDraw(Point c, int r) {
	Ellipse(hdc_, c[0] - r, c[1] - r, c[0] + r, c[1] + r);
}

void Bufer::lineDraw(Point a, Point b) {
	MoveToEx(hdc_, a[0], a[1], NULL);
	LineTo(hdc_, b[0], b[1]);
}

inline UINT32& Bufer::operator[](Point a) {
	return mas_[a[0] + sizey*a[1]];
}

UINT32& Bufer::pixelGet(Point a) {
	if (inRectangle(a, Point(sizex,sizey),Point(0,0))) {
		return operator[](a);
	} else {
		// TODO ������: ��� ����� ���� ����������? ������ ����� ����� ������ ������ �� ������
		throw Bad_Coordinate();
	}
}

void Bufer::bezierDraw(vector<Point>, BezierStyle) {
	// TODO �����������
	// ������������������ ������:
	//		���������� ����� ������� �� ������.
	//		����� ���������� �������� = �����\5.
	//		�������� ������ ����� ������� ������� �� ����������� ����� �������� ��� ������ ������������� ������.
}

void gwapi::Bufer::pixelDraw(point2, Color) {
	// TODO �����������
	// ��� ������������ �������. 
	// ���� ���������� ����� - �� ����������� ���� �������.
	// ���� ���������� �������, �� ���������, ��� ��� �������, ����� �������� ��������� +point2(0.5,0.5), 
	//		� ��� ��� ��������� ����� ������� �������� ��������� ����� ����� �������. � ����� �� ������ �� ������ �������.
}

void gwapi::Bufer::rectDraw(point2, point2) {
	// TODO �����������
	// ��� ����, ����� ���������� ������������� � ������������� ������������ ����: 
	//		���������� ��� ���������� �����
	//		����������� ������� � ����������� �������� ������� � ���������� �����.
}

void gwapi::Bufer::circleDraw(point2, double) {
	// TODO �����������
	// ���������� �������� ���������� ������ � ������� �������� � ������� ����.
	// ������� ����� ����� ������� ������� ��� ������������. ������� ���������� ����� ����� ���������� �� ���������� ����� �������.
	// ������ ���������� ��� ��� �� ������� - �� ���������� �����, ������� � ����� ��� ����� ������������ ������ �������.
	// ������� ���������� ����� ������������ ������ �� ��������, ��� � �����
}

void gwapi::Bufer::lineDraw(point2, point2) {
	// TODO �����������
	// ��� ����, ����� ���������� ����� �� ������������, ����� ������� � ������������ ������� ������������ ����� � ������ ������:
	//		������������ ��������� ��� ��������� ������� ������� �������� ������������� ����� � �� �������� � ������ ������� � ������� �������
	//		������ �������� �������:
	//			������������ n x n ����� ������ �������-�������� � ��� ������ ��������������, ����������� �� ��� ������ �����, � ��� ������� ��������� ��� ���������� ������������� �����
	//			������� ��������� ������, � ������� ������ ������������� ��� ������� ���������������� �������� �����
	//			������� ��������� ��� ������, ���������� ���������� x^n + y^n = 1, ��� n - ������� ������ �����. ��� ��� ������� ��������� ����� ������� �����.
	//			������������ ���������� ������� ��� �������� ��������� ������� ������� ����� �������� � �������.
	//		� ������� ����� ����� ����������� ��� ��� ������ � �������� �������� �������� � �������� ������.
	//		� ��� � �������� ������� ������ ����������� �� �������� � ��������.
}

}

#ifdef __BUFERTEST
using namespace gwapi;
#include <windows.h>
#include <iostream>
#include <stdlib.h>
int main() {
	Bufer a(300,300), b(300, 300), currConsole(GetDC(GetConsoleWindow()));

	b.clear(Transparent);
	b.brushSet(argb(128,128,0,128));
	b.penSet(rgb(255,0,0), 2);
	b.rectDraw(Point(100, 100), Point(170, 170));

	a.clear();
	a.penSet(rgb(128,0,128), 3);
	a.lineDraw(Point(100,100), Point(200,200));
	a.lineDraw(Point(100,200), Point(200,100));
	a.circleDraw(Point(150,150), 50);
	a.textStyle(14, "Times New Roman");
	a.textOut(Point(100,100), string("TEXT TEST"));

	b.drawAlphaTo(a);

	a.drawTo(currConsole);
	b.drawTo(currConsole, 300);
	system("pause");
	return 0;
}
#endif