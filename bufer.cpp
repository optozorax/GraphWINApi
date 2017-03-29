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
	
	width = min(x + width, a.sizex) - width;
	height = min(y + width, a.sizey) - height;
	
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

void Bufer::textOut(int, int, string str, TextStyle stl) {
	
	TextOut(hdcMem, x, y, str.c_str(), strlen(str.c_str())); 
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
	strcpy(font.lfFaceName,		name.c_str()); /* Название шрифта. */
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

}