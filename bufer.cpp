#define _USE_MATH_DEFINES
#include <math.h>
#include "bufer.h"
#include "EasyBMP\\EasyBMP.h"
#include "color.h"

namespace wgs{

wgs::StyleText::StyleText(int size1, std::string name1, int thick1, bool italic1, bool underline1, bool strikedOut1, int symbolSlope1, int textSlope1) :
	size(size1),
	name(name1),
	symbolSlope(symbolSlope1),
	textSlope(textSlope1),
	thick(thick1),
	italic(italic1),
	underline(underline1),
	strikedOut(strikedOut1) {
}

bufer::bufer(int x, int y) : m_width(x), m_height(y) {
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
	
	m_hdc = CreateCompatibleDC(nullptr);
	m_hbmp = CreateDIBSection(m_hdc, &bmi, DIB_RGB_COLORS, &pvBits, nullptr, 0);
	SetGraphicsMode(m_hdc, GM_ADVANCED);
	
	m_mas = (UINT32*) pvBits;
	SelectObject(m_hdc, m_hbmp);

	clear();
	brushSet();
	penSet();
	textStyle(StyleText());
}

void wgs::bufer::resize(Point nsize) {
	DeleteObject(m_hbmp);
	DeleteDC(m_hdc);

	m_width = nsize.x;
	m_height = nsize.y;

	VOID *pvBits;

	/* Структура для того, чтобы связать картинку с массивом. */
	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	bmi.bmiHeader.biWidth = nsize.x;
	bmi.bmiHeader.biHeight = -nsize.y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = nsize.x * nsize.y * 4;

	m_hdc = CreateCompatibleDC(nullptr);
	m_hbmp = CreateDIBSection(m_hdc, &bmi, DIB_RGB_COLORS, &pvBits, nullptr, 0);

	SetGraphicsMode(m_hdc, GM_ADVANCED);

	m_mas = (UINT32*) pvBits;
	DeleteObject(SelectObject(m_hdc, m_hbmp));

	clear();
	brushSet();
	penSet();
	textStyle(StyleText());
}

bufer::bufer(HDC hdc, Point size) {
	m_hdc = hdc;
	m_hbmp = nullptr;
	m_mas = nullptr;
	m_width = size.x;
	m_height = size.y;

	brushSet(White);
	penSet(Black);
}

unsigned bufer::width(void) const {
	return m_width;
}

unsigned bufer::height(void) const {
	return m_height;
}

color* bufer::buf(void) {
	return (color*)(m_mas);
}

void wgs::bufer::read_from_bmp(std::string file) {
	BMP AnImage;
	AnImage.ReadFromFile(file.c_str());

	resize(Point(AnImage.TellWidth(), AnImage.TellHeight()));

	if (AnImage.TellBitDepth() <= 24) {
		AnImage.SetBitDepth(32);
		for (size_t i = 0; i < AnImage.TellWidth(); i++) {
			for (size_t j = 0; j < AnImage.TellHeight(); j++) {
				RGBApixel pix = AnImage.GetPixel(i, j);
				pix.Alpha = 255;

				operator[](Point(i, j)) = rgb(pix.Red, pix.Green, pix.Blue).clrref;
			}
		}
	} else {
		for (size_t i = 0; i < AnImage.TellWidth(); i++) {
			for (size_t j = 0; j < AnImage.TellHeight(); j++) {
				RGBApixel pix = AnImage.GetPixel(i, j);

				operator[](Point(i, j)) = argb(pix.Alpha, pix.Red, pix.Green, pix.Blue).clrref;
			}
		}
	}
}

void wgs::bufer::write_in_bmp(std::string file, bool writeAlpha) {
	if (writeAlpha) {
		BMP AnImage;
		AnImage.SetBitDepth(32);
		AnImage.SetSize(m_width, m_height);

		RGBApixel pix;
		for (size_t i = 0; i < AnImage.TellWidth(); i++) {
			for (size_t j = 0; j < AnImage.TellHeight(); j++) {
				color mypix = operator[](Point(i,j));

				pix.Alpha = mypix.m[3];
				pix.Blue = mypix.m[0];
				pix.Red = mypix.m[2];
				pix.Green = mypix.m[1];

				AnImage.SetPixel(i, j, pix);
			}
		}

		AnImage.WriteToFile(file.c_str());
	} else {
		BMP AnImage;
		AnImage.SetSize(m_width, m_height);

		RGBApixel pix;
		for (size_t i = 0; i < AnImage.TellWidth(); i++) {
			for (size_t j = 0; j < AnImage.TellHeight(); j++) {
				color mypix = operator[](Point(i,j));

				pix.Blue = mypix.m[0];
				pix.Red = mypix.m[2];
				pix.Green = mypix.m[1];

				AnImage.SetPixel(i, j, pix);
			}
		}

		AnImage.WriteToFile(file.c_str());
	}
}

bufer::~bufer() {
	DeleteObject(m_hbmp);
	if (m_mas != nullptr) {
		DeleteDC(m_hdc);
	}
}

void boardsToCorrect(int sizex, int sizey, int &x, int &y, int &width, int &height) {
	// TODO протестировать на все случаи жизни.
	/* Если высота и ширина равны нулю, то это значит, что надо нарисовать весь буфер. */
	if (width == 0) width = sizex;	
	if (height == 0) height = sizey;

	/* Проверка хватит ли первого буфера для рисования. */
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (width > sizex) width = sizex;
	if (height > sizey) height = sizey;

	if (sizex != -1) {
		/* Проверка, не выходят ли x y за пределы второго буфера. */
		if (x > sizex) x = sizex;
		if (y > sizey) y = sizey;

		/* Проверка не выходят ли границы квадрата за пределы второго буфера. */
		if (x+width > sizex) width = sizex-x;
		if (y+width > sizey) height = sizey-y;
	}
}

void bufer::drawTo(bufer &a, int x, int y, int width, int height) {
	boardsToCorrect(a.m_width, a.m_height, x, y, width, height);
	BitBlt(a.m_hdc, x, y, width, height, m_hdc, 0, 0, SRCCOPY);
}

void bufer::drawAlphaTo(bufer &a, int x, int y, int width, int height) {
	boardsToCorrect(a.m_width, a.m_height, x, y, width, height);

	/* Само рисование с прозрачностью. */
	Point c, d;
	for (int x1 = x; x1<x+width; x1++) {
		for (int y1 = y; y1<y+height; y1++) {
			c = Point(x1, y1);
			d = Point(x1 - x, y1 - y);
			a[c] = overlay(operator[](d), a[c]).clrref;
		}
	}
}

void bufer::clear(color cls) {
	if (cls.m[3] == 255) {
		color oldBrush = m_brush;
		brushSet(cls);
		PatBlt(m_hdc, 0, 0, m_width, m_height, PATCOPY);
		brushSet(oldBrush);
	} else {
		DWORD clr1 = cls.clrref;
		for (register int i = 0; i < m_height; i++) {
			for (register int j = 0; j < m_width; j++) {
				operator[](Point(j, i)) = clr1;
			}
		}
	}
}

pen bufer::penSet(color clr, double thick) {
	pen a1 = m_pen;
	
	m_pen.color = clr;
	m_pen.thickness = thick;
	
	if (clr.clrref == Transparent.clrref)
		DeleteObject(SelectObject(m_hdc, CreatePen(PS_NULL, 0, 0)));
	else 
		DeleteObject(SelectObject(m_hdc, CreatePen(PS_SOLID, (int) thick, toWindowsColor(clr))));

	SetTextColor(m_hdc, toWindowsColor(clr));
	
	return a1;
}

brush bufer::brushSet(color clr) {
	brush a1 = m_brush;
	
	m_brush = clr;
	SetBkMode(m_hdc, TRANSPARENT);
	if (clr.clrref == Transparent.clrref)
		DeleteObject(SelectObject(m_hdc, GetSysColorBrush(NULL_BRUSH)));
	else 
		DeleteObject(SelectObject(m_hdc, CreateSolidBrush(toWindowsColor(clr))));
	
	return a1;
}

void bufer::textOut(Point x, std::string str, text_write_style stl) {
	if (stl == Center) x = x - textSize(str)/2;
	std::string first;
	do {
		first = str.substr(0, str.find("\n"));
		TextOut(m_hdc, x.x, x.y, first.c_str(), first.size()); 
		x = x + Point(0, textSize(first).y);
		str.erase(0, first.size()+1);
	} while (str.size() != 0);
}

Point wgs::bufer::textSize(std::string str) {
	SIZE *sz = new SIZE;
	std::string first;
	Point x(0,0);

	do {
		first = str.substr(0, str.find("\n"));
		GetTextExtentPoint32(m_hdc, first.c_str(), first.size(), sz);
		x = Point(max(x.x, sz->cx), max(x.y, sz->cy));
		str.erase(0, first.size()+1);
	} while (str.size() != 0);
	delete sz;

	return x;
}

void bufer::textStyle(StyleText stl) {
	LOGFONT font;
	font.lfHeight 			= -stl.size; /* Высота шрифта. */ 
	font.lfWidth 			= 0; /* Ширина символов в шрифте. */
	font.lfEscapement 		= stl.textSlope; /* Угол наклона относительно горизонта. */ 
	font.lfOrientation 		= stl.symbolSlope; /* Угол между основной линией каждого символа и осью X устройства. */ 
	font.lfWeight 			= stl.thick*100; /* Толщина шрифта в диапазоне от 0 до 1000. */ 
	font.lfItalic 			= stl.italic; /* Курсивный шрифт. */
	font.lfUnderline 		= stl.underline; /* Подчеркнутый шрифт. */
	font.lfStrikeOut 		= stl.strikedOut; /* Зачеркнутый шрифт. */
	font.lfCharSet 			= RUSSIAN_CHARSET; /* Набор символов. */
	font.lfOutPrecision 	= OUT_STROKE_PRECIS; /* Точность вывода. */
	font.lfClipPrecision 	= CLIP_DEFAULT_PRECIS; /* Точность отсечения. */
	font.lfQuality 			= ANTIALIASED_QUALITY; /* Качество вывода. */
	font.lfPitchAndFamily 	= 0; /* Ширина символов и семейство шрифта. */

	/* Название шрифта. */
	for (int i = 0; i < stl.name.size(); i++) {
		font.lfFaceName[i] = stl.name[i];
	}
	for (int i = stl.name.size(); i < 32; i++) {
		font.lfFaceName[i] = 0;
	}

	/* Применение шрифта к дескриптору окна. */
	HFONT hfont = CreateFontIndirect(&font);
	DeleteObject(SelectObject(m_hdc, hfont));
}

void bufer::draw_pixel(Point x, color c) {
	operator[](x) = c.clrref;
}

void bufer::draw_rect(Point a, Point b) {
	PatBlt(m_hdc, a.x, a.y, b.x-a.x, b.y-a.y, PATCOPY);
	draw_line(a, Point(b.x, a.y));
	draw_line(a, Point(a.x, b.y));
	draw_line(b, Point(b.x, a.y-1));
	draw_line(b, Point(a.x-1, b.y));
}

void bufer::draw_ellipse(Point c, Point r) {
	Ellipse(m_hdc, c.x - r.x, c.y - r.y, c.x + r.x, c.y + r.y);
}

void bufer::draw_line(Point a, Point b) {
	MoveToEx(m_hdc, a.x, a.y, NULL);
	LineTo(m_hdc, b.x, b.y);
}

void wgs::bufer::draw_poly(std::vector<Point> mas) {
	POINT *mas1 = new POINT[mas.size()];
	for (int i = 0; i < mas.size(); i++) {
		mas1[i] = {mas[i].x, mas[i].y};
	}
	Polygon(m_hdc, mas1, mas.size());
	delete mas1;
}

inline color& bufer::operator[](Point a) {
	return (color) m_mas[a.x + m_width*a.y];
}

color& bufer::get_pixel(Point a) {
	if (inRectangle(a, Point(m_width,m_height),Point(0,0))) {
		return operator[](a);
	} else {
	}
}

}