#include "D:\\CPP\\PROGRAM2\\GraphC.h\\window.h"
#include <stdlib.h>

using namespace std;

void mymouse(wgs::window *This, int x, int y, wgs::WinEvents::MouseType click, int wheel) {
	static int rnb = 0;

	switch (click) {
	case wgs::WinEvents::L_DBL:
		This->canvas.draw_line(Point(x-5, y+5), Point(x+5, y-5));
		This->redraw();
		break;
	case wgs::WinEvents::R_DBL:
		This->canvas.clear(wgs::rainbow(rnb/20.0, true));
		This->redraw();
		break;
	case wgs::WinEvents::L_DOWN:
		This->canvas.draw_ellipse(Point(x, y), 10);
		This->redraw();
		break;
	case wgs::WinEvents::R_DOWN:
		This->canvas.textOut(Point(x, y), "L");
		This->redraw();
		break;
	case wgs::WinEvents::M_DOWN:
		This->canvas.draw_rect(Point(x-10, y-10), Point(x+10, y+10));
		This->redraw();
		break;
	case wgs::WinEvents::M_UP:
		This->taskbarColor(wgs::window::Green);
		break;
	case wgs::WinEvents::X1_UP:
		This->taskbarColor(wgs::window::Red);
		break;
	case wgs::WinEvents::X2_UP:
		This->taskbarColor(wgs::window::Yellow);
		break;
	case wgs::WinEvents::Wheel:
		rnb += wheel/120;
		This->canvas.brushSet(wgs::rainbow(rnb/20.0, true));
		break;
	case wgs::WinEvents::Move:
		static Point b;
		if (b != Point(0,0)) {
			This->canvas.draw_line(b, Point(x, y));
		}
		b = Point(x, y);
		break;
	}
}

void myactive(wgs::window *This, bool actv, bool minz) {
	if (actv) {
		This->captionSet("ACTIVATE");
	} else {
		This->captionSet("DEACTIVATE");
	}
	if (minz) {
		This->canvas.clear(wgs::Black);
	}
}

void mykeyboard(wgs::window *This, int key, bool state) {
	switch (key) {
	case VK_NUMPAD1:
		This->canvas.textOut(Point(0,0), "1 1 1");
		This->redraw();
		break;
	case VK_NUMPAD2:
		This->canvas.textOut(Point(0,0), "2 2 2");
		This->redraw();
		break;
	case VK_NUMPAD3:
		This->canvas.textOut(Point(0,0), "3 3 3");
		This->redraw();
		break;
	case VK_NUMPAD4:
		This->canvas.textOut(Point(0,0), "4 4 4");
		This->redraw();
		break;
	case VK_NUMPAD5:
		This->canvas.textOut(Point(0,0), "5 5 5");
		This->redraw();
		break;
	case VK_NUMPAD6:
		if (state) {
			This->canvas.textOut(Point(0,0), "6 6 6");
			This->redraw();
		} else {
			This->canvas.textOut(Point(0,0), "66666");
			This->redraw();
		}
		break;
	default:
		break;
	}
}

void mymoving(wgs::window *This, long &x, long &y, long &width, long &height) {
	This->canvas.draw_rect(Point(0,0), Point(300, 25));
	This->canvas.textOut(Point(0,0), "MOVING " + to_string(x) + " " + to_string(y) + " " + to_string(width) + " " + to_string(height));
	This->redraw();
}

void mysize(wgs::window *This, int x, int y) {
	This->canvas.textOut(Point(0,0), "SIZE   " + to_string(x) + " " + to_string(y));
	This->redraw();
}

void mysizing(wgs::window *This, long &x, long &y, long &width, long &height, wgs::WinEvents::SizingType type) {
	This->canvas.draw_rect(Point(0,0), Point(300, 25));
	This->canvas.textOut(Point(0,11), "SIZING " + to_string(x) + " " + to_string(y) + " " + to_string(width) + " " + to_string(height));
}

int main() {
	wgs::window a;
	a.taskbarProgress(1);

	a.canvas.clear();
	a.canvas.brushSet(wgs::rainbow(0));
	a.taskbarProgress(1);
	a.canvas.textStyle(12);
	a.ev.mouse = mymouse;
	a.ev.activate = myactive;
	a.ev.keyboard = mykeyboard;
	a.ev.moving = mymoving;	
	a.ev.size = mysize;
	a.ev.sizing = mysizing;

	system("pause");

	return 0;
}