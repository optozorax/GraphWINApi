#include <windows.h>
#include <stdlib.h>
#include "../window.h"

gwapi::Menu a2;

void mdrawmenu(gwapi::Window* This, int x, int y, gwapi::WinEvents::MouseType type, int wheel) {
	if (type == gwapi::WinEvents::R_DOWN) {
		This->drawMenu(Point(x, y), a2);
	}
}

void mycomand(gwapi::Window *This, int cmd) {
	if (cmd == 4) {
		This->canvas.textOut(Point(0, 0), "You clicked to Exit!!!");
		This->redraw();
	}
}

using gwapi::MenuType;
using gwapi::MenuLine;
using gwapi::Menu;

int main() {
	gwapi::Window wnd;
	wnd.ev.mouse = mdrawmenu;
	wnd.ev.comand = mycomand;

	a2.pushLine(MenuLine(MenuType::Line, "Open", 1));
	a2.pushLine(MenuLine(MenuType::Separator));
	a2.pushLine(MenuLine(MenuType::Line, "Save", 2, true));
	a2.pushLine(MenuLine(MenuType::Line, "Save as...", 3, false, true));
	a2.pushLine(MenuLine(MenuType::Separator));
	a2.pushLine(MenuLine(MenuType::Line, "Exit", 4));

	wnd.menu.pushLine(MenuLine(a2, "File"));
	wnd.menu.pushLine(MenuLine(MenuType::Line, "Edit", 2));
	wnd.menu.pushLine(MenuLine(MenuType::Line, "Help", 3));
	wnd.sysMenu.pushLine(MenuLine(MenuType::Line, "Help", 3));
	wnd.redrawMenu();

	system("pause");
	return 0;
}