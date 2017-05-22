#include "../window.h"
#include <stdlib.h>

int main() {
	wgs::window_type atype, btype, ctype;

	atype.caption = "First window";
	atype.size = Point(350, 400);
	atype.maxSize = Point(400, 450);
	atype.position = Point(0, 100);

	btype.caption = "SCND WNDW, Русские символы";
	btype.style = wgs::window_type::Tool;
	btype.position = atype.position + Point(atype.size.x+20,0);

	ctype.position = btype.position + Point(btype.size.x+20,0);
	ctype.style = wgs::window_type::Caption;

	wgs::window a(atype);
	wgs::window b(btype);
	wgs::window c(ctype);

	a.canvas.penSet(wgs::rgb(255,0,0), 3);
	a.canvas.clear();
	a.canvas.draw_line(Point(100,100), Point(200,200));
	a.redraw();

	b.canvas.penSet(wgs::argb(128,0,0,255), 3);
	b.canvas.brushSet(wgs::argb(128,255,0,0));
	b.canvas.draw_line(Point(100,200), Point(200,100));
	b.canvas.draw_rect(Point(10,10), Point(200,150));

	std::vector<wgs::color> clrs;
	clrs.push_back(wgs::Blue);
	clrs.push_back(wgs::Green);
	clrs.push_back(wgs::Ubuntu);
	std::vector<double> lns;
	lns.push_back(5);
	lns.push_back(1);
	for (int i = 0; i < 200; i++) {
		b.canvas.penSet(wgs::gradient(i/200.0, true, clrs, lns));
		b.canvas.draw_line(Point(i, 0), Point(i, 20));
		b.canvas.penSet(wgs::rainbow(i/200.0, true));
		b.canvas.draw_line(Point(i, 20), Point(i, 30));
	}

	b.redraw();

	system("pause");

	a.fullscreen();

	a.canvas.penSet(wgs::rgb(255,0,0), 3);
	a.canvas.draw_line(Point(100,200), Point(200,100));
	b.canvas.draw_rect(Point(130,170), Point(200,100));
	b.canvas.drawAlphaTo(a.canvas, 100);

	b.sizeSet(Point(200,200));

	c.positionSet(Point(200,200));

	a.redraw();
	b.redraw();

	system("pause");

	return 0;
}