#ifndef __BUFERDEFINE
#define __BUFERDEFINE

#include <string>
#include <vector>
#include <windows.h>
#include "color.h"
#include "point.h"

using namespace std;

namespace gwapi{

/* Классы исключений: */
class Bad_Coordinate{};

/* Классы типов рисования. */
enum TextWriteStyle{LeftUp, Center};
enum BezierStyle{Default, Pieces};

/* Структуры данных для внутреннего представления. */
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

	// TODO подумать насчет копирования буфера
	
	/* Работа с другими буферами. */
	void drawTo(Bufer&, int x = 0, int y = 0, int width = 0, int height = 0);
	void drawAlphaTo(Bufer&, int x = 0, int y = 0, int width = 0, int height = 0);
	// Если величины width, height равны нулю, то это значит, что надо нарисовать весь буфер
	
	/* Очистка буфера заданным цветом. */
	void clear(Color = White);
	void clearM(Color = White);
	
	/* Работа с рисующими устройствами. */
	Pen penSet(Color, int = 1);
	Brush brushSet(Color);
	
	/* Работа с текстом. */
	void textOut(Point, string, TextWriteStyle = LeftUp);
	void textStyle(int = 14, string = "Consolas");
	
	// TODO Понять: надо ли объединить два последующих блока, и различать их только дефайнами или флагами?

	/* Примитивы на основе функций Windows. Без сглаживания и прозрачных цветов. */
	/* Максимальная скорость рисования. */
	void pixelDraw(Point, Color = Black);
	void rectDraw(Point, Point);
	void circleDraw(Point, int);
	void lineDraw(Point, Point);
	void bezierDraw(vector<Point>, BezierStyle = Default);
	
	/* Примитивы на основе моей собственная реализация со сглаживанием и прозрачными цветами. */
	/* Скорее всего очень медленно. */
	void pixelDraw(point2, Color = Black);
	void rectDraw(point2, point2);
	void circleDraw(point2, double);
	void lineDraw(point2, point2);
	void bezierDraw(vector<point2>, BezierStyle = Default);

	/* Обращение к соответствующему пикселю для изменения или чтения. */
	UINT32& pixelGet(Point); // С проверкой выхода за границы
	UINT32& operator[](Point); // Без проверки выхода за границы

	//# Когда-нибудь написать интерфейс для работы с рисунками. #//

	friend class Window;
	friend LRESULT gwapi::WindowLife::create(Window*, HWND&, WPARAM&, LPARAM&);
};

/* Если объявлен этот дефайн, то в файле реализации имеется main, в котором запускаются тесты этого модуля. */
//#define __BUFERTEST

}

#endif