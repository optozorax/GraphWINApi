#ifndef __BUFERDEFINE
#define __BUFERDEFINE

#include <string>
#include <vector>
#include <windows.h>
#include "color.h"
#include "point.h"

namespace wgs{

/* Классы типов рисования. */
enum text_write_style { LeftUp, Center };

/* Структуры данных для внутреннего представления. */
struct pen {
	color color;
	double thickness;
};

typedef color brush;

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

class bufer {
public:
	bufer(int = 1000, int = 1000);
	bufer(HDC hdc, Point size);
	~bufer();

	void resize(Point nsize);

	/* Работа низкоуровневой частью. */
	unsigned width(void) const;
	unsigned height(void) const;
	color* buf(void);

	/* Работа с изображениями. */
	void read_from_bmp(std::string name);
	void write_in_bmp(std::string name, bool is32bit);

	/* Работа с другими буферами. */
	void drawTo(bufer&, int x = 0, int y = 0, int width = 0, int height = 0);
	void drawAlphaTo(bufer&, int x = 0, int y = 0, int width = 0, int height = 0);
	// Если величины width, height равны нулю, то это значит, что надо нарисовать весь буфер
	
	/* Очистка буфера заданным цветом. */
	void clear(color = White);
	
	/* Работа с рисующими устройствами. */
	pen penSet(color = Black, double = 1);
	brush brushSet(color = White);
	
	/* Работа с текстом. */
	void textOut(Point, std::string, text_write_style = LeftUp);
	Point textSize(std::string);
	void textStyle(StyleText);

	/* Примитивы на основе функций Windows. Без сглаживания и прозрачных цветов. */
	/* Максимальная скорость рисования. */
	void draw_pixel(Point, color = Black);
	void draw_rect(Point, Point);
	void draw_ellipse(Point, Point);
	void draw_line(Point, Point);
	void draw_poly(std::vector<Point>);
	
	/* Примитивы на основе моей собственная реализация со сглаживанием и прозрачными цветами. */
	/* Скорее всего очень медленно. */
	void draw_rect(point2, point2);
	void draw_ellipse(point2, point2);
	void draw_line(point2, point2);
	void draw_poly(std::vector<point2>);

	/* Обращение к соответствующему пикселю для изменения или чтения. */
	color& get_pixel(Point); // С проверкой выхода за границы
	color& operator[](Point); // Без проверки выхода за границы
private:
	HDC		m_hdc;
	HBITMAP	m_hbmp;
	UINT32	*m_mas;
	pen		m_pen;
	brush	m_brush;
	int		m_width;
	int		m_height;
};

}

#endif