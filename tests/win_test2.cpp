#include "D:\\CPP\\PROGRAM2\\GraphC.h\\window.h"
#include <stdlib.h>

using namespace std;

void mymouse(gwapi::Window *This, int x, int y, gwapi::WinEvents::MouseType click, int wheel) {
	static int rnb = 0;

	switch (click) {
	case gwapi::WinEvents::L_DBL:
		This->canvas.lineDraw(Point(x-5, y+5), Point(x+5, y-5));
		This->redraw();
		break;
	case gwapi::WinEvents::R_DBL:
		This->canvas.clear(gwapi::rainbow(rnb/20.0, true));
		This->redraw();
		break;
	case gwapi::WinEvents::L_DOWN:
		This->canvas.circleDraw(Point(x, y), 10);
		This->redraw();
		break;
	case gwapi::WinEvents::R_DOWN:
		This->canvas.textOut(Point(x, y), "L");
		This->redraw();
		break;
	case gwapi::WinEvents::M_DOWN:
		This->canvas.rectDraw(Point(x-10, y-10), Point(x+10, y+10));
		This->redraw();
		break;
	case gwapi::WinEvents::M_UP:
		This->taskbarColor(gwapi::Window::Green);
		break;
	case gwapi::WinEvents::X1_UP:
		This->taskbarColor(gwapi::Window::Red);
		break;
	case gwapi::WinEvents::X2_UP:
		This->taskbarColor(gwapi::Window::Yellow);
		break;
	case gwapi::WinEvents::Wheel:
		rnb += wheel/120;
		This->canvas.brushSet(gwapi::rainbow(rnb/20.0, true));
		break;
	case gwapi::WinEvents::Move:
		static Point b;
		if (b != Point(0,0)) {
			This->canvas.lineDraw(b, Point(x, y));
		}
		b = Point(x, y);
		break;
	}
}

void myactive(gwapi::Window *This, bool actv, bool minz) {
	if (actv) {
		This->captionSet("ACTIVATE");
	} else {
		This->captionSet("DEACTIVATE");
	}
	if (minz) {
		This->canvas.clear(gwapi::Black);
	}
}

void mykeyboard(gwapi::Window *This, int key, bool state) {
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

void mymoving(gwapi::Window *This, long &x, long &y, long &width, long &height) {
	This->canvas.rectDraw(Point(0,0), Point(300, 25));
	This->canvas.textOut(Point(0,0), "MOVING " + to_string(x) + " " + to_string(y) + " " + to_string(width) + " " + to_string(height));
	This->redraw();
}

void mysize(gwapi::Window *This, int x, int y) {
	This->canvas.textOut(Point(0,0), "SIZE   " + to_string(x) + " " + to_string(y));
	This->redraw();
}

void mysizing(gwapi::Window *This, long &x, long &y, long &width, long &height, gwapi::WinEvents::SizingType type) {
	This->canvas.rectDraw(Point(0,0), Point(300, 25));
	This->canvas.textOut(Point(0,11), "SIZING " + to_string(x) + " " + to_string(y) + " " + to_string(width) + " " + to_string(height));
}

int main() {
	gwapi::Window a;
	a.taskbarProgress(1);

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

	return 0;
}