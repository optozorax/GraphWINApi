#include "bufer.h"

namespace gwapi{

Bufer::Bufer(int x, int y) : sizex(x), sizey(y) {
	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	bmi.bmiHeader.biWidth = x;
	bmi.bmiHeader.biHeight = -y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = x * y * 4;
	
	VOID *pvBits;
	
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
	
	if (width = 0) width = a.sizex;	
	if (height = 0) height = a.sizey;
	
	width = min(x + width, a.sizex) - width;
	height = min(y + width, a.sizey) - height;
	
	BitBlt(a.hdc_, x, y, width, height, hdc_, 0, 0, SRCCOPY);
}

void Bufer::drawAlphaTo(Bufer) {
	//@ Алгоритм, который учитывает прозрачность двух слоев. @//
}

void Bufer::clear(Color cls = White) {
	for (int x = 0; x<sizex; x++) {
		for (int y = 0; y<sizey; y++) {
			(*this)[] = cls.clrref;
		}
	}
}

Pen Bufer::penSet(Color clr, int thick) {
	Pen a1 = pen_;
	
	pen_.color = clr;
	pen_.thickness = thick;
	
	SelectObject(hdc_, CreatePen(PS_SOLID, thick, clr.clrref));
	
	return a1;
}

Brush Bufer::brushSet(Color clr) {
	brush_ = clr;
}

void Bufer::textOut(int, int, string, OutStyle = LeftUp) {
	
}

void Bufer::textStyle(Color, int = 14, string = "Consolas") {
}

void Bufer::pixelDraw(Point, Color = Black) {
}

Color Bufer::pixelGet(Point) {
}

void Bufer::rectDraw(Point, Point) {
}

void Bufer::circleDraw(point2, int) {
}

void Bufer::lineDraw(Point, Point) {
}

void Bufer::bezierDraw(vector<Point>, BezierStyle = Default) {
}

UINT32& Bufer::operator[](Point) {
}

}