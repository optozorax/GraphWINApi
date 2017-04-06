#include "../window.h"
#include <stdlib.h>

int main() {
	gwapi::WindowType atype, btype, ctype;

	atype.caption = "First window";
	atype.size = Point(350, 400);
	atype.maxSize = Point(400, 450);
	atype.position = Point(0, 100);

	btype.caption = "SCND WNDW, Русские символы";
	btype.style = gwapi::WindowType::Tool;
	btype.position = atype.position + Point(atype.size[0]+20,0);

	ctype.position = btype.position + Point(btype.size[0]+20,0);
	ctype.style = gwapi::WindowType::Caption;

	gwapi::Window a(atype);
	gwapi::Window b(btype);
	gwapi::Window c(ctype);

	a.canvas.penSet(gwapi::rgb(255,0,0), 3);
	a.canvas.clear();
	a.canvas.lineDraw(Point(100,100), Point(200,200));
	a.redraw();

	b.canvas.penSet(gwapi::argb(128,0,0,255), 3);
	b.canvas.brushSet(gwapi::argb(128,255,0,0));
	b.canvas.lineDraw(Point(100,200), Point(200,100));
	b.canvas.rectDraw(Point(10,10), Point(200,150));
	b.redraw();

	system("pause");

	a.fullscreen();

	a.canvas.penSet(gwapi::rgb(255,0,0), 3);
	a.canvas.lineDraw(Point(100,200), Point(200,100));
	b.canvas.rectDraw(Point(130,170), Point(200,100));
	b.canvas.drawAlphaTo(a.canvas, 100);

	b.sizeSet(Point(200,200));

	c.positionSet(Point(200,200));

	a.redraw();
	b.redraw();

	system("pause");

	return 0;
}