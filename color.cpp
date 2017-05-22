#include <windows.h>
#include "color.h"

wgs::color wgs::rgb(const int r, const int g, const int b) {
	color c;
	c.m[3] = 255;
	c.m[2] = r;
	c.m[1] = g;
	c.m[0] = b;
	return c;
};
wgs::color wgs::argb(const int a, const int r, const int g, const int b) {
	color c;
	c.m[3] = a;
	c.m[2] = r;
	c.m[1] = g;
	c.m[0] = b;
	return c;
};

wgs::color wgs::gray(const double a, const bool cycle) {
	/* Начинается с черного, заканчивается белым. */
	std::vector<color> clrs(2);
	clrs[0] = Black;
	clrs[1] = White;

	return gradient(a, cycle, clrs);
};


wgs::color wgs::rainbow(const double a, const bool cycle) {
	/* Начинается с красного, заканчивается фиолетовым. */
	std::vector<color> clrs(7);
	clrs[0] = Red;
	clrs[1] = Orange;
	clrs[2] = Yellow;
	clrs[3] = Green;
	clrs[4] = LightBlue;
	clrs[5] = DarkBlue;
	clrs[6] = Violet;//rgb(128,0,255);

	return gradient(a, cycle, clrs);
};

wgs::color wgs::gradient(const double a, const bool cycle, const std::vector<color> clrs, std::vector<double> lengths) {
	register int n = clrs.size();
	register double col = abs(a);
	if (cycle) {
		while (col>1) col = abs(col-2);
	}

	if (lengths.size() == 0) {
		for (int i = 0; i < n-1; i++) lengths.push_back(1.0/(n-1));
	}

	double sum = 0;
	for (int i = 0; i < n-1; i++) sum += lengths[i];
	for (int i = 0; i < n-1; i++) lengths[i] /= sum;

	register double c;
	register int r, g, b;
	for (register int i = 0; i < n-1; i++) {
		if (col<=lengths[i]) {
			c = col/lengths[i];
			b = clrs[i].m[0]+(clrs[i+1].m[0]-clrs[i].m[0])*c;
			g = clrs[i].m[1]+(clrs[i+1].m[1]-clrs[i].m[1])*c;
			r = clrs[i].m[2]+(clrs[i+1].m[2]-clrs[i].m[2])*c;
			break;
		}
		col -= lengths[i];
	}

	return rgb(r,g,b);
};

wgs::color wgs::overlay(color a, color b) {
	/* Первый пиксель находится над вторым. */
	color c;
	
	UCHAR da = a.m[3];
	if (da == 0 && a.clrref != 0) da = 255;
	if (da == 1) da = 0;
	
	/* Маленькая оптимизация. */
	if (da != 0) {
		UCHAR dr = a.m[2];
		UCHAR dg = a.m[1];
		UCHAR db = a.m[0];

		UCHAR sa = b.m[3];
		UCHAR sr = b.m[2];
		UCHAR sg = b.m[1];
		UCHAR sb = b.m[0];

		/* КОСТЫЛЬ ДЛЯ РАБОТЫ СТАНДАРТНЫХ ПРИМИТИВОВ. */
		if (sa == 0 && b.clrref != 0) sa = 255;
		if (sa == 1) sa = 0;
		
		int sada = sa*da;
		int ra = da + sa - da*sa/255;				if (ra > 255) ra = 255;
		int rr = (dr*da + sr*sa-sr*sada/255)/ra;	if (rr > 255) rr = 255;
		int rg = (dg*da + sg*sa-sg*sada/255)/ra;	if (rg > 255) rg = 255;
		int rb = (db*da + sb*sa-sb*sada/255)/ra;	if (rb > 255) rb = 255;
		
		c.m[3] = ra;
		c.m[2] = rr;
		c.m[1] = rg;
		c.m[0] = rb;
	} else 
		c = b;

	return c;
};

COLORREF wgs::toWindowsColor(const color& a) {
	return RGB(a.m[2],a.m[1],a.m[0]);
}

#ifdef __COLORDEBUG
#include <stdlib.h>
#include <windows.h>

using namespace wgs;

void Line(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}


int main() {
	color a(0xFFFF2211), b(0x55330017), c(0x110066FD), d;
	d = overlay(b, a);
	printf("%x + %x = %x \n", b.clrref, a.clrref, d.clrref);

	d = overlay(c, b);
	printf("%x + %x = %x \n", c.clrref, b.clrref, d.clrref);

	d = overlay(c, a);
	printf("%x + %x = %x \n", c.clrref, a.clrref, d.clrref);

	d = rgb(0xFF, 0x22, 0x11);
	printf("%x %x %x\n", a.clrref, d.clrref, RGB(0xFF, 0x22, 0x11));

	HWND hwnd = GetConsoleWindow();
	HDC hdc = GetDC(hwnd);

	for (int i = 0; i < 300; i++) {
		a = rainbow((i-100)/100.0, true);
		a.m[3] = (255*i/300) % 255;
		a = overlay(a, Black);
		SelectObject(hdc, CreatePen(PS_SOLID, 2, toWindowsColor(a)));
		Line(hdc, i, 100, i, 110);
	} 

	for (int i = 0; i < 300; i++) {
		a = gray((i-100)/100.0, true);
		SelectObject(hdc, CreatePen(PS_SOLID, 2, toWindowsColor(a)));
		Line(hdc, i, 110, i, 120);
	} 

	for (int i = 0; i < 300; i++) {
		a = rgb(255,0,0);
		SelectObject(hdc, CreatePen(PS_SOLID, 2, toWindowsColor(a)));
		Line(hdc, i, 120, i, 130);
	} 

	system("pause");
}
#endif