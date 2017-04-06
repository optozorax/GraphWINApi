#ifndef __BUFERDEFINE
#define __BUFERDEFINE

#include <string>
#include <vector>
#include <windows.h>
#include "color.h"
#include "point.h"

using namespace std;

namespace gwapi{

/* ������ ����������: */
class Bad_Coordinate{};

/* ������ ����� ���������. */
enum TextWriteStyle{LeftUp, Center};
enum BezierStyle{Default, Pieces};

/* ��������� ������ ��� ����������� �������������. */
struct Pen {
	Color color;
	int thickness;
};
typedef Color Brush;

class Window;

namespace WindowLife {
	LRESULT create(Window*, HWND&, WPARAM&, LPARAM&);
}

class Bufer {
	HDC hdc_;
	HBITMAP hbmp_;
	UINT32 *mas_;
	int sizex, sizey;
	Pen pen_;
	Brush brush_;

	void Bufer::boardsToCorrect(Bufer&, int&, int&, int&, int&);
public:
	Bufer(int = 1000, int = 1000);
	Bufer(HDC);
	~Bufer();

	// TODO �������� ������ ����������� ������
	
	/* ������ � ������� ��������. */
	void drawTo(Bufer&, int x = 0, int y = 0, int width = 0, int height = 0);
	void drawAlphaTo(Bufer&, int x = 0, int y = 0, int width = 0, int height = 0);
	// ���� �������� width, height ����� ����, �� ��� ������, ��� ���� ���������� ���� �����
	
	/* ������� ������ �������� ������. */
	void clear(Color = White);
	void clearM(Color = White);
	
	/* ������ � ��������� ������������. */
	Pen penSet(Color, int = 1);
	Brush brushSet(Color);
	
	/* ������ � �������. */
	void textOut(Point, string, TextWriteStyle = LeftUp);
	void textStyle(int = 14, string = "Consolas");
	
	// TODO ������: ���� �� ���������� ��� ����������� �����, � ��������� �� ������ ��������� ��� �������?

	/* ��������� �� ������ ������� Windows. ��� ����������� � ���������� ������. */
	/* ������������ �������� ���������. */
	void pixelDraw(Point, Color = Black);
	void rectDraw(Point, Point);
	void circleDraw(Point, int);
	void lineDraw(Point, Point);
	void bezierDraw(vector<Point>, BezierStyle = Default);
	
	/* ��������� �� ������ ���� ����������� ���������� �� ������������ � ����������� �������. */
	/* ������ ����� ����� ��������. */
	void pixelDraw(point2, Color = Black);
	void rectDraw(point2, point2);
	void circleDraw(point2, double);
	void lineDraw(point2, point2);
	void bezierDraw(vector<point2>, BezierStyle = Default);

	/* ��������� � ���������������� ������� ��� ��������� ��� ������. */
	UINT32& pixelGet(Point); // � ��������� ������ �� �������
	UINT32& operator[](Point); // ��� �������� ������ �� �������

	//# �����-������ �������� ��������� ��� ������ � ���������. #//

	friend class Window;
	friend LRESULT gwapi::WindowLife::create(Window*, HWND&, WPARAM&, LPARAM&);
};

/* ���� �������� ���� ������, �� � ����� ���������� ������� main, � ������� ����������� ����� ����� ������. */
//#define __BUFERTEST

}

#endif