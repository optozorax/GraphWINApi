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
}

Bufer::~Bufer() {
	DeleteObject(hbmp_);
	DeleteDC(hdc_);
}

void Bufer::drawTo(Bufer a, int x, int y, int width, int height) {
	/* ��������� ������� ������� ������ ���������. */
	x = min(a.sizex, x);
	y = min(a.sizey, y);
	
	if (width == 0) width = a.sizex;	
	if (height == 0) height = a.sizey;
	
	width = min(x + width, a.sizex) - width;
	height = min(y + width, a.sizey) - height;
	
	BitBlt(a.hdc_, x, y, width, height, hdc_, 0, 0, SRCCOPY);
}

void Bufer::drawAlphaTo(Bufer a, int x, int y, int width, int height) {
	// TODO �������������� ��� ��������
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

void Bufer::textOut(int x, int y, string str, TextStyle stl) {
	
	TextOut(hdc_, x, y, (LPCWSTR)str.c_str(), strlen(str.c_str())); 
}

void Bufer::textStyle(Color c, int size, string name) {
	// TODO ��������������
	LOGFONT font;
	HFONT hfont;
	font.lfHeight 			= -size; /* ������ ������. */ 
	font.lfWidth 			= 0; /* ������ �������� � ������. */
	font.lfEscapement 		= 0; /* ���� ������� ������������ ���������. */ 
	font.lfOrientation 		= 0; /* ���� ����� �������� ������ ������� ������� � ���� X ����������. */ 
	font.lfWeight 			= 0; /* ������� ������ � ��������� �� 0 �� 1000. */ 
	font.lfItalic 			= FALSE; /* ��������� �����. */
	font.lfUnderline 		= FALSE; /* ������������ �����. */
	font.lfStrikeOut 		= FALSE; /* ����������� �����. */
	font.lfCharSet 			= RUSSIAN_CHARSET; /* ����� ��������. */
	font.lfOutPrecision 		= 0; /* �������� ������. */
	font.lfClipPrecision 		= CLIP_DEFAULT_PRECIS; /* �������� ���������. */
	font.lfQuality 			= PROOF_QUALITY; /* �������� ������. */
	font.lfPitchAndFamily 		= 0; /* ������ �������� � ��������� ������. */
	strcpy(font.lfFaceName,		name.c_str()); /* �������� ������. */
	/* ���������� ������ � ����������� ����. */
	hfont = ::CreateFontIndirect(&font);
	SelectObject(hdc_, hfont);
}

void Bufer::pixelDraw(Point x, Color c) {
	operator[](x) = c.clrref;
}

Color Bufer::pixelGet(Point a) {
	return operator[](a);
}

void Bufer::rectDraw(Point, Point) {

}

void Bufer::circleDraw(Point, int) {

}

void Bufer::lineDraw(Point, Point) {

}

void Bufer::bezierDraw(vector<Point>, BezierStyle) {

}

inline UINT32& Bufer::operator[](Point a) {
	return mas_[a[0] + sizey*a[1]];
}

void gwapi::Bufer::pixelDraw(point2, Color) {

}

Color gwapi::Bufer::pixelGet(point2) {

	return Color();
}

void gwapi::Bufer::rectDraw(point2, point2) {

}

void gwapi::Bufer::circleDraw(point2, double) {

}

void gwapi::Bufer::lineDraw(point2, point2) {

}

}