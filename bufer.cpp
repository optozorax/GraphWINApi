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

void Bufer::drawTo(Bufer &a, int x, int y, int width, int height) {
	boardsToCorrect(sizex, sizey, x, y, width, height);
	BitBlt(a.hdc_, x, y, width, height, hdc_, 0, 0, SRCCOPY);
}

void Bufer::drawAlphaTo(Bufer &a, int x, int y, int width, int height) {
	boardsToCorrect(sizex, sizey, x, y, width, height);

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
		TextOut(hdc_, x.x, x.y, first.c_str(), first.size()); 
		x = x + Point(0, textSize(first).y);
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
		x = Point(max(x.x, sz->cx), max(x.y, sz->cy));
		str.erase(0, first.size()+1);
	} while (str.size() != 0);
	delete sz;

	return x;
}

void Bufer::textStyle(StyleText stl) {
	// TODO Понять: надо ли через эту функцию получать такие параметры, как: толщина, курсив и т.д. что задается через структуру
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
	DeleteObject(SelectObject(hdc_, hfont));
}

void Bufer::pixelDraw(Point x, Color c) {
	operator[](x) = c.clrref;
}

void Bufer::rectDraw(Point a, Point b) {
	/*for (int i = a.x; i < b.x; i++) {
		for (int j = a.y; j < b.y; j++) {
			operator[](Point(i, j)) = brush_.clrref;
		}
	}*/
	PatBlt(hdc_, a.x, a.y, b.x-a.x, b.y-a.y, PATCOPY);
	lineDraw(a, Point(b.x, a.y));
	lineDraw(a, Point(a.x, b.y));
	lineDraw(b, Point(b.x, a.y-1));
	lineDraw(b, Point(a.x-1, b.y));
}

void Bufer::circleDraw(Point c, int r) {
	Ellipse(hdc_, c.x - r, c.y - r, c.x + r, c.y + r);
}

void Bufer::lineDraw(Point a, Point b) {
	MoveToEx(hdc_, a.x, a.y, NULL);
	LineTo(hdc_, b.x, b.y);
}

void gwapi::Bufer::polyDraw(std::vector<Point> mas) {
	POINT *mas1 = new POINT[mas.size()];
	for (int i = 0; i < mas.size(); i++) {
		mas1[i] = {mas[i].x, mas[i].y};
	}
	Polyline(hdc_, mas1, mas.size());
	delete mas1;
}

inline UINT32& Bufer::operator[](Point a) {
	return mas_[a.x + sizey*a.y];
}

UINT32& Bufer::pixelGet(Point a) {
	if (inRectangle(a, Point(sizex,sizey),Point(0,0))) {
		return operator[](a);
	} else {
		// TODO Понять: тут точно надо исключение? Скорее всего можно просто ничего не делать
		throw Bad_Coordinate();
	}
}

void Bufer::bezierDraw(std::vector<Point> a, BezierStyle stl) {
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
		for (int j = 0; j < a.size(); j++) b[j] = a[j];

		for (int j = 1; j < a.size(); j++) {
			for (int k = 0; k < a.size()-j; k++) {
				b[k] = b[k] + (b[k+1]-b[k])*((double)(i)/iters);
			}
		}
		b2 = b[0];
		if (i != 0) lineDraw(b2, b1);
		b1 = b2;
	}
}

void gwapi::Bufer::pixelDraw(point2, Color) {
	// TODO реализовать
	// Это вещественный пиксель. 
	// Если координата целая - он закрашивает один пиксель.
	// Если координата нецелая, то считается, что это квадрат, центр которого находится +point2(0.5,0.5), 
	//		и там уже считается какую площадь занимает некоторая часть этого пикселя. В итоге он рисует на четыре пикселя.
}

void gwapi::Bufer::rectDraw(point2, point2) {
	// TODO реализовать
	// Для того, чтобы нарисовать прямоугольник с вещественными координатами надо: 
	//		Нарисовать его внутреннюю часть
	//		Аналогичным образом с окружностью рисовать границу и внутреннюю часть.
}

void gwapi::Bufer::circleDraw(point2, double) {
	// TODO реализовать
	// Окружность рисуется внутренней кистью и внешней границей с помощью пера.
	// Граница может иметь немалую толщину или прозрачность. Поэтому внутренняя часть будет рисоваться до внутренней части границы.
	// Внутри окружности все что не граница - то внутренняя часть, поэтому и цвета там будут складываться особым образом.
	// Пиксель окружности можно представлять такими же методами, как и снизу
}

double pa_, pb_, pc_, ab_, sab_;
inline double getAlpha(Point &x1, const point2 &first, const point2 &second, double thick) {
	static point2 pp3(0.3, 0.3);
	point2 x = x1;
	x.x += 0.5;
	x.y += 0.5;

	double r, r1;

	thick = thick-1;
	if (thick < 0) thick = 0;

	// Расстояние от точки до прямой
	r = fabs(pa_*x.x + pb_*x.y + pc_)/sab_;

	if (r < (thick+1)) { // Точка находится в бесконечном прямоугольнике прямой, образованной отрезком
		// Проекция точки на прямую
		point2 pr = point2(
			(pb_*(pb_*x.x-pa_*x.y)-pa_*pc_)/ab_, 
			(pa_*(-pb_*x.x+pa_*x.y)-pb_*pc_)/ab_);

		bool inRect = inRectangle(pr, first - pp3, second + pp3) || 
			inRectangle(pr, second - pp3, first + pp3);

		// Дает закругление на концах
		if (!inRect && ((r1 = min((first-x).length(), (second-x).length())) <= thick+1)) { 
			if (r1 > thick) {
				// Точка на краях прямоугольника
				r1 -= thick;
				return 1-r1;
			} else {
				// Точка внутри прямоугольника
				return 1;
			}
		}

		if (inRect) {
			if (r > thick) {
				// Точка на краях прямоугольника
				r -= thick;
				return 1-r;
			} else {
				// Точка внутри прямоугольника
				return 1;
			}
		}
	}
	return 0;
}

inline double getAlpha1(Point &x, const point2 &first, const point2 &second, double thick) {
	/* Тупой алгоритм перебора всех точек внутри пикселя для расчета площади. */
	/* В десять раз медленней, чем альтернативный. */
	const double num = 15; // от 0.1 до 15 вклюительно
	double r;
	point2 pr, y;
	bool inRect;
	int sum = 0;
	thick *= sab_;

	for (int i = 0; i <= num; i++) {
		for (int j = 0; j <= num; j++) {
			y = point2(x.x + i/num, x.y + j/num);
			r = fabs(pa_*y.x + pb_*y.y + pc_);
			pr = point2( (pb_*(pb_*y.x-pa_*y.y)-pa_*pc_)/ab_, 
					(pa_*(-pb_*y.x+pa_*y.y)-pb_*pc_)/ab_);
			inRect = inRectangle(pr, first - point2(0.3,0.3), second + point2(0.3,0.3)) || 
				inRectangle(pr, second - point2(0.3,0.3), first + point2(0.3,0.3));
			if (inRect && r<=thick) {
				sum++;
			}
		}
	}
		
	return sum/((num+1)*(num+1));
}

inline void getABCParams(point2 first, point2 second) {
	// Получение параметров прямой
	static double d;
	if ((d = second.x*first.y-first.x*second.y) != 0) {
		pa_ = (second.y-first.y)/d;
		pb_ = (first.x-second.x)/d;
		pc_ = 1;
	} else if ((d = second.y-first.y) != 0) {
		pa_ = 1;
		pb_ = (first.x-second.x)/d;
		pc_ = (second.x*first.y-first.x*second.y)/d;
	} else {
		std::cout << "Points not to line: " << first << " " << second << std::endl;
	}
	ab_ = pa_*pa_ + pb_*pb_;
	sab_ = sqrt(ab_);
}

void createBorders(point2 first, point2 second, Point &start, Point &end, double thick, int sizex, int sizey) {
	start = Point(min(first.x, second.x), min(first.y, second.y));
	end = Point(max(first.x, second.x), max(first.y, second.y));

	start.x -= thick*1.5 + 3;
	start.y -= thick*1.5 + 3;
	end.x += thick*1.5 + 3;
	end.y += thick*1.5 + 3;

	if (start.x<0) start.x = 0;
	if (start.y<0) start.y = 0;
	if (end.x>sizex) end.x = sizex;
	if (end.y>sizey) end.y = sizey;
}

Color newc;
inline void drawPix(gwapi::Bufer *This, gwapi::Color &pen, Point pos, double &alpha) {
	newc = pen;
	newc.m[3] = newc.m[3]*alpha;
	if (newc.m[3] == 0) newc.m[3] = 1;
	This->operator[](pos) = gwapi::overlay(newc, This->operator[](pos)).clrref;
}

inline void getYc(double &yc, int &i, int sizey) {
	// Получение точки на прямой с текущей координатой x
	yc = (-pc_-pa_*i)/pb_;
	if (yc < 0) yc = 0;
	if (yc > sizey) yc = sizey-1;
	if (yc != yc) yc = 0;
}

inline void getXc(double &yc, int &i, int sizey) {
	// Получение точки на прямой с текущей координатой x
	yc = (-pc_-pb_*i)/pa_;
	if (yc < 0) yc = 0;
	if (yc >= sizey) yc = sizey-1;
	if (yc != yc) yc = 0;
}

void gwapi::Bufer::lineDraw(point2 first, point2 second) {
	// TODO сравнить как будет, если все числа представить в виде дробей с основанием 255

	if (first.x>second.x) std::swap(first, second);
	double thick = pen_.thickness/2.0;
	double (*gtAlpha)(Point&, const point2&, const point2&, double) = &getAlpha;
	//if (thick<1) gtAlpha = &getAlpha1;

	first = first + point2(0.5, 0.5); 
	second = second + point2(0.5, 0.5);
	getABCParams(first, second);

	Point start, end;
	createBorders(first, second, start, end, thick, sizex-1, sizey-1);

	double alpha, yc;
	int i, j;
	if (abs(second.x-first.x)>abs(second.y-first.y)) {
		for (i = start.x; i < end.x; i++) {
			// Алгоритм движения из центра к бокам, пока прозрачность не станет равна нулю
			// Эффективнее по сравнению с обычным во много раз(как минимум в три раза)
			getYc(yc, i, sizey-1);

			for (j = yc; j < sizey-1; j++) {
				alpha = gtAlpha(Point(i, j), first, second, thick);

				drawPix(this, pen_.color, Point(i,j), alpha);
				if (alpha == 0) break;
			}

			for (j = yc; j > 1; j--) {
				alpha = gtAlpha(Point(i, j), first, second, thick);

				drawPix(this, pen_.color, Point(i,j), alpha);
				if (alpha == 0) break;
			}
		}
	} else {
		for (j = start.y; j < end.y; j++) {
			// Алгоритм движения из центра к бокам, пока прозрачность не станет равна нулю
			// Эффективнее по сравнению с обычным во много раз(как минимум в три раза)
			getXc(yc, j, sizex-1);

			for (i = yc; i < sizex-1; i++) {
				alpha = gtAlpha(Point(i, j), first, second, thick);

				drawPix(this, pen_.color, Point(i,j), alpha);
				if (alpha == 0) break;
			}

			for (i = yc; i > 1; i--) {
				alpha = gtAlpha(Point(i, j), first, second, thick);

				drawPix(this, pen_.color, Point(i,j), alpha);
				if (alpha == 0) break;
			}
		}
	}
}

void gwapi::Bufer::polyDraw(std::vector<point2> mas) {
	// TODO реализовать
	// Для того, чтобы нарисовать многоугольник надо использовать метод XOR или другие.
	// Прозрачность будет рассматриваться точно так же, как и в верхнем методе.
	// Это будет одно из самых сложных.
}

}