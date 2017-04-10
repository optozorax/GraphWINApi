#define _USE_MATH_DEFINES
#include <math.h>
#include "bufer.h"

namespace gwapi{

gwapi::StyleText::StyleText(int size1, std::string name1, int thick1, bool italic1, bool underline1, bool strikedOut1, int symbolSlope1, int textSlope1) :
	size(size1),
	name(name1),
	symbolSlope(symbolSlope1),
	textSlope(textSlope1),
	thick(thick1),
	italic(italic1),
	underline(underline1),
	strikedOut(strikedOut1) {
}

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

	SetGraphicsMode(hdc_, GM_ADVANCED);
	
	mas_ = (UINT32*) pvBits;
	SelectObject(hdc_, hbmp_);

	clear();
	brushSet();
	penSet();
	textStyle(StyleText());
}

void gwapi::Bufer::resize(int x, int y) {
	DeleteObject(hbmp_);
	DeleteDC(hdc_);

	sizex = x;
	sizey = y;

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

	SetGraphicsMode(hdc_, GM_ADVANCED);

	mas_ = (UINT32*) pvBits;
	SelectObject(hdc_, hbmp_);

	clear();
	brushSet();
	penSet();
	textStyle(StyleText());
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

void boardsToCorrect(int sizex, int sizey, int &x, int &y, int &width, int &height) {
	// TODO �������������� �� ��� ������ �����.
	/* ���� ������ � ������ ����� ����, �� ��� ������, ��� ���� ���������� ���� �����. */
	if (width == 0) width = sizex;	
	if (height == 0) height = sizey;

	/* �������� ������ �� ������� ������ ��� ���������. */
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (width > sizex) width = sizex;
	if (height > sizey) height = sizey;

	if (sizex != -1) {
		/* ��������, �� ������� �� x y �� ������� ������� ������. */
		if (x > sizex) x = sizex;
		if (y > sizey) y = sizey;

		/* �������� �� ������� �� ������� �������� �� ������� ������� ������. */
		if (x+width > sizex) width = sizex-x;
		if (y+width > sizey) height = sizey-y;
	}
}

void Bufer::drawTo(Bufer &a, int x, int y, int width, int height) {
	boardsToCorrect(sizex, sizey, x, y, width, height);
	BitBlt(a.hdc_, x, y, width, height, hdc_, 0, 0, SRCCOPY);
}

void Bufer::drawAlphaTo(Bufer &a, int x, int y, int width, int height) {
	boardsToCorrect(sizex, sizey, x, y, width, height);

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
	if (cls.m[3] == 255) {
		Color oldBrush = brush_;
		brushSet(cls);
		PatBlt(hdc_, 0, 0, sizex, sizey, PATCOPY);
		brushSet(oldBrush);
	} else {
		DWORD clr1 = cls.clrref;
		for (register int i = 0; i < sizey; i++) {
			for (register int j = 0; j < sizex; j++) {
				operator[](Point(j, i)) = clr1;
			}
		}
	}
}

Pen Bufer::penSet(Color clr, double thick) {
	Pen a1 = pen_;
	
	pen_.color = clr;
	pen_.thickness = thick;
	
	DeleteObject(SelectObject(hdc_, CreatePen(PS_SOLID, (int) thick, toWindowsColor(clr))));

	SetTextColor(hdc_, toWindowsColor(clr));
	
	return a1;
}

Brush Bufer::brushSet(Color clr) {
	Brush a1 = brush_;
	
	brush_ = clr;
	
	DeleteObject(SelectObject(hdc_, CreateSolidBrush(toWindowsColor(clr))));
	
	return a1;
}

void Bufer::textOut(Point x, std::string str, TextWriteStyle stl) {
	if (stl == Center) x = x - textSize(str)/2;
	std::string first;
	do {
		first = str.substr(0, str.find("\n"));
		TextOut(hdc_, x[0], x[1], first.c_str(), first.size()); 
		x = x + Point(0, textSize(first)[1]);
		str.erase(0, first.size()+1);
	} while (str.size() != 0);
}

Point gwapi::Bufer::textSize(std::string str) {
	SIZE *sz = new SIZE;
	std::string first;
	Point x(0,0);

	do {
		first = str.substr(0, str.find("\n"));
		GetTextExtentPoint32(hdc_, first.c_str(), first.size(), sz);
		x = Point(max(x[0], sz->cx), max(x[1], sz->cy));
		str.erase(0, first.size()+1);
	} while (str.size() != 0);
	delete sz;

	return x;
}

void Bufer::textStyle(StyleText stl) {
	// TODO ������: ���� �� ����� ��� ������� �������� ����� ���������, ���: �������, ������ � �.�. ��� �������� ����� ���������
	LOGFONT font;
	font.lfHeight 			= -stl.size; /* ������ ������. */ 
	font.lfWidth 			= 0; /* ������ �������� � ������. */
	font.lfEscapement 		= stl.textSlope; /* ���� ������� ������������ ���������. */ 
	font.lfOrientation 		= stl.symbolSlope; /* ���� ����� �������� ������ ������� ������� � ���� X ����������. */ 
	font.lfWeight 			= stl.thick*100; /* ������� ������ � ��������� �� 0 �� 1000. */ 
	font.lfItalic 			= stl.italic; /* ��������� �����. */
	font.lfUnderline 		= stl.underline; /* ������������ �����. */
	font.lfStrikeOut 		= stl.strikedOut; /* ����������� �����. */
	font.lfCharSet 			= RUSSIAN_CHARSET; /* ����� ��������. */
	font.lfOutPrecision 	= OUT_STROKE_PRECIS; /* �������� ������. */
	font.lfClipPrecision 	= CLIP_DEFAULT_PRECIS; /* �������� ���������. */
	font.lfQuality 			= ANTIALIASED_QUALITY; /* �������� ������. */
	font.lfPitchAndFamily 	= 0; /* ������ �������� � ��������� ������. */

	/* �������� ������. */
	for (int i = 0; i < stl.name.size(); i++) {
		font.lfFaceName[i] = stl.name[i];
	}
	for (int i = stl.name.size(); i < 32; i++) {
		font.lfFaceName[i] = 0;
	}

	/* ���������� ������ � ����������� ����. */
	HFONT hfont = CreateFontIndirect(&font);
	DeleteObject(SelectObject(hdc_, hfont));
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

void gwapi::Bufer::polyDraw(std::vector<Point> mas) {
	POINT *mas1 = new POINT[mas.size()];
	for (int i = 0; i < mas.size(); i++) {
		mas1[i] = {mas[i][0], mas[i][1]};
	}
	Polyline(hdc_, mas1, mas.size());
	delete mas1;
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

void Bufer::bezierDraw(std::vector<Point> a, BezierStyle stl) {
	// TODO �����������
	// ������������������ ������:
	//		���������� ����� ������� �� ������.
	//		����� ���������� �������� = �����\5.
	//		�������� ������ ����� ������� ������� �� ����������� ����� �������� ��� ������ ������������� ������.

	double len = 0;
	for (int i = 0; i < a.size()-1; i++) {
		len += (a[i]-a[i+1]).length();
	}

	int iters = (int)(len/5.0);
	double t;
	std::vector<point2> b(a.size());
	Point b2;
	Point b1;
	for (int i = 0; i <= iters; i++) {
		for (int j = 0; j < a.size(); j++) to_another(b[j], a[j]);

		for (int j = 1; j < a.size(); j++) {
			for (int k = 0; k < a.size()-j; k++) {
				b[k] = b[k] + (b[k+1]-b[k])*((double)(i)/iters);
			}
		}
		to_another(b2, b[0]);
		if (i != 0) lineDraw(b2, b1);
		b1 = b2;
	}
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

void createBorders(point2 p1, point2 p2, Point &p3, Point &p4, double thick, int sizex, int sizey) {
	p3 = Point(min(p1[0], p2[0]), min(p1[1], p2[1]));
	p4 = Point(max(p1[0], p2[0]), max(p1[1], p2[1]));

	p3[0] -= thick*1.5;
	p3[1] -= thick*1.5;
	p4[0] += thick*1.5;
	p4[1] += thick*1.5;

	p4 = p4 - p3;
	boardsToCorrect(sizex, sizey, p3[0], p3[1], p4[0], p4[1]);
	p4 = p4 + p3;
}

double getAlpha(Point x, point2 p1, point2 p2, double thick) {
	double a, b, c, d, r, r1;

	// ��������� ���������� ������
	if ((d = p2[0]*p1[1]-p1[0]*p2[1]) != 0) {
		a = (p2[1]-p1[1])/d;
		b = (p1[0]-p2[0])/d;
		c = 1;
	} else if ((d = p2[1]-p1[1]) != 0) {
		a = 1;
		b = (p1[0]-p2[0])/d;
		c = (p2[0]*p1[1]-p1[0]*p2[1])/d;
	} else {
		std::cout << "Points not to line: " << p1 << " " << p2 << std::endl;
	}

	// ���������� �� ����� �� ������
	r = fabs(a*(x[0]+0.5) + b*(x[1]+0.5) + c)/sqrt(a*a + b*b);

	// �������� ����� �� ������
	point2 pr = point2(
		(b*(b*(x[0]+0.5)-a*(x[1]+0.5))-a*c)/(a*a + b*b), 
		(a*(-b*(x[0]+0.5)+a*(x[1]+0.5))-b*c)/(a*a + b*b));

	if (r < (thick+1)) { // ����� ��������� � ����������� �������������� ������, ������������ ��������
		bool inRect = inRectangle(pr, p1, p2) || inRectangle(pr, p2, p1);

		if (!inRect && ((r1 = min((p1-pr).length(),(p2-pr).length())) < 1) ) { 
			// ������ ����� �� ����� ������ � �������������
			return r1;
		}
		if (inRect) {
			if (r > thick) {
				// ����� �� ����� ��������������
				r -= thick;
				return 1-r;
			} else {
				// ����� ������ ��������������
				return 1;
			}
		}
	}
	return 0;
}

void gwapi::Bufer::lineDraw(point2 p1, point2 p2) {
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
	Point p3, p4;
	createBorders(p1, p2, p3, p4, pen_.thickness, sizex, sizey);

	Color newc;
	double alpha;
	for (int i = p3[0]; i < p4[0]; i++) {
		for (int j = p3[1]; j < p4[1]; j++) {
			alpha = getAlpha(Point(i, j), p1, p2, pen_.thickness/2.0);

			newc = pen_.color;
			newc.m[3] = newc.m[3]*alpha;
			if (newc.m[3] == 0) newc.m[3] = 1;
			operator[](Point(i, j)) = gwapi::overlay(newc, operator[](Point(i, j))).clrref;
		}
	}
}

void gwapi::Bufer::polyDraw(std::vector<point2> mas) {
	// TODO �����������
	// ��� ����, ����� ���������� ������������� ���� ������������ ����� XOR ��� ������.
	// ������������ ����� ��������������� ����� ��� ��, ��� � � ������� ������.
	// ��� ����� ���� �� ����� �������.
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