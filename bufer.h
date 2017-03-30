#ifndef __BUFERDEFINE
#define __BUFERDEFINE

#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>
#include "color.h"
#include "point.h"

using namespace std;

namespace gwapi{

enum TextWriteStyle{LeftUp, Center};
enum BezierStyle{Default, Pieces};

struct Pen {
	Color color;
	int thickness;
};

typedef Color Brush;

class Bufer {
	HDC hdc_;
	HBITMAP hbmp_;
	UINT32 *mas_;
	int sizex, sizey;
	Pen pen_;
	Brush brush_;
public:
	Bufer(int = 1000, int = 1000);
	Bufer(HDC);
	~Bufer();
	
	void drawTo(Bufer, int x = 0, int y = 0, int width = 0, int height = 0);
	void drawAlphaTo(Bufer&, int x = 0, int y = 0, int width = 0, int height = 0);
	
	void clear(Color = White);
	
	Pen penSet(Color, int = 1);
	Brush brushSet(Color);
	
	void textOut(Point, string, TextWriteStyle = LeftUp);
	void textStyle(int = 14, string = "Consolas");
	
	/* Реализация на основе примитивов виндоус. */
	void pixelDraw(Point, Color = Black);
	Color pixelGet(Point);
	void rectDraw(Point, Point);
	void circleDraw(Point, int);
	void lineDraw(Point, Point);
	void bezierDraw(vector<Point>, BezierStyle = Default);
	
	/* Моя собственная реализация со сглаживанием и прозрачностью. */
	void pixelDraw(point2, Color = Black);
	Color pixelGet(point2);
	void rectDraw(point2, point2);
	void circleDraw(point2, double);
	void lineDraw(point2, point2);
	void bezierDraw(vector<point2>, BezierStyle = Default);
	
	//# Когда-нибудь написать интерфейс для работы с рисунками. #//
	
	UINT32& operator[](Point);
};

}

#endif