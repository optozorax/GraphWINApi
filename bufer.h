#ifndef __BUFERDEFINE
#define __BUFERDEFINE

#include <string>
#include <vector>
#include <windows.h>
#include "color.h"
#include "point.h"

namespace gwapi{

/* ������ ����������: */
class Bad_Coordinate{};

/* ������ ����� ���������. */
enum TextWriteStyle{LeftUp, Center};
enum BezierStyle{Default, Pieces};

/* ��������� ������ ��� ����������� �������������. */
struct Pen {
	Color color;
	double thickness;
};
typedef Color Brush;

class Window;

namespace WindowLife {
	LRESULT create(Window*, HWND&, WPARAM&, LPARAM&);
}

class StyleText {
public:
	int size;
	std::string name;
	int symbolSlope;
	int textSlope;
	int thick;

	bool italic;
	bool underline;
	bool strikedOut;

	StyleText(int = 14, std::string = "Consolas", int = 0, bool = false, bool = false, bool = false, int = 0, int = 0);
};

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
	//Bufer(const Bufer&);
	~Bufer();

	void resize(int = 1000, int = 1000);

	// TODO �������� ������ ����������� ������
	
	/* ������ � ������� ��������. */
	void drawTo(Bufer&, int x = 0, int y = 0, int width = 0, int height = 0);
	void drawAlphaTo(Bufer&, int x = 0, int y = 0, int width = 0, int height = 0);
	// ���� �������� width, height ����� ����, �� ��� ������, ��� ���� ���������� ���� �����
	
	/* ������� ������ �������� ������. */
	void clear(Color = White);
	
	/* ������ � ��������� ������������. */
	Pen penSet(Color = Black, double = 1);
	Brush brushSet(Color = White);
	
	/* ������ � �������. */
	void textOut(Point, std::string, TextWriteStyle = LeftUp);
	Point textSize(std::string);
	void textStyle(StyleText);
	
	// TODO ������: ���� �� ���������� ��� ����������� �����, � ��������� �� ������ ��������� ��� �������?

	/* ��������� �� ������ ������� Windows. ��� ����������� � ���������� ������. */
	/* ������������ �������� ���������. */
	void pixelDraw(Point, Color = Black);
	void rectDraw(Point, Point);
	void circleDraw(Point, int);
	void lineDraw(Point, Point);
	void polyDraw(std::vector<Point>);
	void bezierDraw(std::vector<Point>, BezierStyle = Default);
	
	/* ��������� �� ������ ���� ����������� ���������� �� ������������ � ����������� �������. */
	/* ������ ����� ����� ��������. */
	void pixelDraw(point2, Color = Black);
	void rectDraw(point2, point2);
	void circleDraw(point2, double);
	void lineDraw(point2, point2);
	void polyDraw(std::vector<point2>);
	void bezierDraw(std::vector<point2>, BezierStyle = Default);

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