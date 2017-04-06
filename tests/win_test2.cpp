#include "D:\\CPP\\PROGRAM2\\GraphC.h\\window.h"
#include <stdlib.h>

gwapi::Window a;

void mymouse(int x, int y, gwapi::WinEvents::MouseType click, int wheel) {
	static int rnb = 0;

	switch (click) {
	case gwapi::WinEvents::L_DBL:
		a.canvas.lineDraw(Point(x-5, y+5), Point(x+5, y-5));
		a.redraw();
		break;
	case gwapi::WinEvents::R_DBL:
		a.canvas.clear(gwapi::rainbow(rnb/20.0, true));
		a.redraw();
		break;
	case gwapi::WinEvents::L_DOWN:
		a.canvas.circleDraw(Point(x, y), 10);
		a.redraw();
		break;
	case gwapi::WinEvents::R_DOWN:
		a.canvas.textOut(Point(x, y), "L");
		a.redraw();
		break;
	case gwapi::WinEvents::M_DOWN:
		a.canvas.rectDraw(Point(x-10, y-10), Point(x+10, y+10));
		a.redraw();
		break;
	case gwapi::WinEvents::M_UP:
		a.taskbarColor(gwapi::Window::Green);
		break;
	case gwapi::WinEvents::X1_UP:
		a.taskbarColor(gwapi::Window::Red);
		break;
	case gwapi::WinEvents::X2_UP:
		a.taskbarColor(gwapi::Window::Yellow);
		break;
	case gwapi::WinEvents::Wheel:
		rnb += wheel/120;
		a.canvas.brushSet(gwapi::rainbow(rnb/20.0, true));
		break;
	case gwapi::WinEvents::Move:
		static Point b;
		if (b != Point(0,0)) {
			a.canvas.lineDraw(b, Point(x, y));
		}
		b = Point(x, y);
		break;
	}
}

void myactive(bool actv, bool minz) {
	if (actv) {
		a.captionSet("ACTIVATE");
	} else {
		a.captionSet("DEACTIVATE");
	}
	if (minz) {
		a.canvas.clear(gwapi::Black);
	}
}

void mykeyboard(int key, bool state) {
	switch (key) {
	case VK_NUMPAD1:
		a.canvas.textOut(Point(0,0), "1 1 1");
		a.redraw();
		break;
	case VK_NUMPAD2:
		a.canvas.textOut(Point(0,0), "2 2 2");
		a.redraw();
		break;
	case VK_NUMPAD3:
		a.canvas.textOut(Point(0,0), "3 3 3");
		a.redraw();
		break;
	case VK_NUMPAD4:
		a.canvas.textOut(Point(0,0), "4 4 4");
		a.redraw();
		break;
	case VK_NUMPAD5:
		a.canvas.textOut(Point(0,0), "5 5 5");
		a.redraw();
		break;
	case VK_NUMPAD6:
		if (state) {
			a.canvas.textOut(Point(0,0), "6 6 6");
			a.redraw();
		} else {
			a.canvas.textOut(Point(0,0), "66666");
			a.redraw();
		}
		break;
	default:
		break;
	}
}

void mymoving(long &x, long &y, long &width, long &height) {
	a.canvas.rectDraw(Point(0,0), Point(300, 25));
	a.canvas.textOut(Point(0,0), "MOVING " + to_string(x) + " " + to_string(y) + " " + to_string(width) + " " + to_string(height));
	a.redraw();
}

void mysize(int x, int y) {
	a.canvas.textOut(Point(0,0), "SIZE   " + to_string(x) + " " + to_string(y));
	a.redraw();
}

void mysizing(long &x, long &y, long &width, long &height, gwapi::WinEvents::SizingType type) {
	a.canvas.rectDraw(Point(0,0), Point(300, 25));
	a.canvas.textOut(Point(0,11), "SIZING " + to_string(x) + " " + to_string(y) + " " + to_string(width) + " " + to_string(height));
}

int main() {
	Sleep(100);
	a.canvas.clear();
	a.canvas.brushSet(gwapi::rainbow(0));
	a.taskbarProgress(1);
	a.canvas.textStyle(12);
	a.ev.mouse = mymouse;
	a.ev.activate = myactive;
	a.ev.keyboard = mykeyboard;
	a.ev.moving = mymoving;	
	a.ev.size = mysize;
	a.ev.sizing = mysizing;

	system("pause");

	a.~Window();

	return 0;
}