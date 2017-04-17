#include <windows.h>
#include <stdlib.h>
#include "../window.h"
#include "D:\\CPP\\VisualStudio\\test\\test\\resource.h"

using gwapi::MenuType;
using gwapi::MenuLine;
using gwapi::Menu;

void mdrawmenu(gwapi::Window* This, int x, int y, gwapi::WinEvents::MouseType type, int wheel) {
	if (type == gwapi::WinEvents::R_DOWN) {
		This->drawMenu(Point(x, y), *(This->menu.get(1).menu));
	}
}

void mycomand(gwapi::Window *This, int cmd) {
	if (cmd == 4) {
		This->canvas.textOut(Point(0, 0), "You clicked to Exit!!!");
		This->redraw();
	}
	if (cmd == 10) {
		MenuLine lnn = This->menu.get(3).menu->get(2);
		lnn.flags.checked = !lnn.flags.checked;
		This->menu.get(3).menu->set(2, lnn);
	}
	if (cmd == 11) {
		static bool state = true;
		This->fullscreen(state);
		state = !state;
	}
}

bool nado = true;

void myclose(gwapi::Window *This) {
	nado = false;
}

//int main() {
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	gwapi::WindowType wndtp;
	wndtp.size = Point(500, 400);
	wndtp.icon = 112;
	gwapi::Window wnd(wndtp);
	wnd.ev.mouse = mdrawmenu;
	wnd.ev.comand = mycomand;
	wnd.ev.close = myclose;

	wnd.menu.pushLine(MenuLine(Menu().
		pushLine(MenuLine(MenuType::Line, "�����\tCtrl+N", 1).SetIcon(116)).
		pushLine(MenuLine(MenuType::Line, "�������\tCtrl+O", 2).SetIcon(117)).
		pushLine(MenuLine(MenuType::Line, "���������\tCtrl+S", 3).SetIcon(110)).
		pushLine(MenuLine(MenuType::Line, "��������� ���...\tCtrl+Shift+S", 4).SetIcon(106)).
		pushLine(MenuLine(MenuType::Separator)).
		pushLine(MenuLine(Menu().
			pushLine(MenuLine(MenuType::Line, "1.c")).
			pushLine(MenuLine(MenuType::Line, "2.c")).
			pushLine(MenuLine(MenuType::Line, "3.c")).
			pushLine(MenuLine(MenuType::Line, "4.c")).
			pushLine(MenuLine(MenuType::Line, "5.c"))
			, "���������")).
		pushLine(MenuLine(MenuType::Separator)).
		pushLine(MenuLine(MenuType::Line, "�����\tAlt+F4", 4).SetIcon(111))
	, "����"));
	wnd.menu.pushLine(MenuLine(Menu().
		pushLine(MenuLine(MenuType::Line, "��������\tCtrl+Z").SetIcon(114)).
		pushLine(MenuLine(MenuType::Line, "���������\tAlt+Ctrl+Z").SetIcon(113)).
		pushLine(MenuLine(MenuType::Separator)).
		pushLine(MenuLine(MenuType::Line, "�������\tDel").SetIcon(105)).
		pushLine(MenuLine(MenuType::Line, "��������\tCtrl+V").SetIcon(109)).
		pushLine(MenuLine(MenuType::Line, "����������\tCtrl+C").SetIcon(107)).
		pushLine(MenuLine(MenuType::Line, "��������\tCtrl+X").SetIcon(108))
	, "������", 2));
	wnd.menu.pushLine(MenuLine(Menu().
		pushLine(MenuLine(MenuType::Line, "Fullscreen\tF11", 11)).
		pushLine(MenuLine(MenuType::Line, "�������� ������", 10).SetTwoIcons(101, 103))
	, "�����", 3));
	wnd.menu.pushLine(MenuLine(Menu().
		pushLine(MenuLine(Menu().
			pushLine(MenuLine(Menu().
				pushLine(MenuLine(MenuType::Line, "\t����")).
				pushLine(MenuLine(MenuType::Separator)).
				pushLine(MenuLine(MenuType::Line, "����\t����", true)).
				pushLine(MenuLine(MenuType::Line, "����\t����")).
				pushLine(MenuLine(MenuType::Separator)).
				pushLine(MenuLine(MenuType::Line, "����"))
			, "����\t����").SetIcon(104)).
			pushLine(MenuLine(MenuType::Separator)).
			pushLine(MenuLine(MenuType::Line, "�� ���� ����� ���� �� ��������.", 0, true, true)).
			pushLine(MenuLine(MenuType::Line, "����� \"�����\", \"Fullscreen\" � \"�������� ������\".", 0, true, true))
		, "� ���������\t����").SetIcon(102)).
		pushLine(MenuLine(MenuType::Separator)).
		pushLine(MenuLine(MenuType::Line, "�������\tAlt+F1").SetIcon(112))
	, "������", 3));
	wnd.redrawMenu();

	//system("pause");
	while (nado) gwapi::Window::sleep(100);
	return 0;
}