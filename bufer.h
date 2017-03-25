#ifndef __BUFERDEFINE
#define __BUFERDEFINE

#include <string>
#include <vector>
#include "color.h"
#include "point.h"

using namespace std;
using namespace point;

namespace gwapi{

enum TextStyle{LeftUp, Center};
enum BezierStyle{Default, Pieces};

struct Pen {
	Color color;
	int thickness;
};

typedef Color Brush;

class Bufer {
	HDC hdc_;
	HBMP hbmp_;
	UINT32 *mas_;
	Pen pen_;
	Brush brush_;
public:
	Bufer(int = 1000, int = 1000);
	~Bufer();
	
	void fromConsole(void);
	
	void drawTo(Bufer);
	void drawAlphaTo(Bufer);
	void drawAlphaNewTo(Bufer);
	
	void clear(Color = White);
	
	Pen penSet(Color, int);
	Brush brushSet(Color);
	
	void textOut(int, int, string, OutStyle = LeftUp);
	void textStyle(Color, int = 14, string = "Consolas");
	
	void pixelDraw(Point, Color = Black);
	Color pixelGet(Point);
	void rectDraw(Point, Point);
	void circleDraw(point2, int);
	void lineDraw(Point, Point);
	void bezierDraw(vector<Point>, BezierStyle = Default);
	
	//# Когда-нибудь написать интерфейс для работы с рисунками. #//
	
	UINT32& operator[](Point);
};

}

#endif