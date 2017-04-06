#include "D:\\CPP\\PROGRAM2\\GraphC.h\\window.h"
#include <stdlib.h>

gwapi::Window a;

void mymouse(int x, int y, gwapi::WinEvents::MouseType click, int wheel) {
	static int rnb = 0;
	if (click == gwapi::WinEvents::L_DOWN) {
		a.canvas.circleDraw(Point(x, y), 10);
		a.redraw();
	}
	if (click == gwapi::WinEvents::L_DBL) {
		a.canvas.lineDraw(Point(x-5, y+5), Point(x+5, y-5));
		a.redraw();
	}
	if (click == gwapi::WinEvents::R_DOWN) {
		//a.canvas.lineDraw(Point(x-5, y-5), Point(x+5, y+5));
		a.canvas.textOut(Point(x, y), "L");
		a.redraw();
	}
	if (click == gwapi::WinEvents::R_DBL) {
		a.canvas.clear(gwapi::rainbow(rnb/20.0, true));
		a.redraw();
	}
	if (click == gwapi::WinEvents::M_DOWN) {
		a.canvas.rectDraw(Point(x-10, y-10), Point(x+10, y+10));
		a.redraw();
	}
	if (click == gwapi::WinEvents::Wheel) {
		rnb += wheel/120;
		a.canvas.brushSet(gwapi::rainbow(rnb/20.0, true));
	}
	if (click == gwapi::WinEvents::Move) {
		static Point b;
		if (b != Point(0,0)) {
			a.canvas.lineDraw(b, Point(x, y));
		}
		b = Point(x, y);
	}
}

int main() {
	a.canvas.clear();
	a.canvas.brushSet(gwapi::rainbow(0));
	a.ev.mouse = mymouse;

	system("pause");

	a.~Window();

	return 0;
}