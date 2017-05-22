#include <windows.h>
#include <stdlib.h>
#include "../window.h"
#include "D:\\CPP\\VisualStudio\\test\\test\\resource.h"

using wgs::menu_type;
using wgs::menu_line;
using wgs::menu;

void mdrawmenu(wgs::window* This, int x, int y, wgs::WinEvents::MouseType type, int wheel) {
	if (type == wgs::WinEvents::R_DOWN) {
		This->drawMenu(Point(x, y), *(This->main_menu.get(1).popup_menu));
	}
}

void mycomand(wgs::window *This, int cmd) {
	if (cmd == 4) {
		This->canvas.textOut(Point(0, 0), "You clicked to Exit!!!");
		This->redraw();
	}
	if (cmd == 10) {
		menu_line lnn = This->main_menu.get(3).popup_menu->get(2);
		lnn.flags.checked = !lnn.flags.checked;
		This->main_menu.get(3).popup_menu->set(2, lnn);
	}
	if (cmd == 11) {
		static bool state = true;
		This->fullscreen(state);
		state = !state;
	}
}

bool nado = true;

void myclose(wgs::window *This) {
	nado = false;
}

//int main() {
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	wgs::window_type wndtp;
	wndtp.size = Point(500, 400);
	wndtp.icon = 112;
	wgs::window wnd(wndtp);
	wnd.ev.mouse = mdrawmenu;
	wnd.ev.comand = mycomand;
	wnd.ev.close = myclose;

	wnd.main_menu.pushLine(menu_line(menu().
		pushLine(menu_line(menu_type::Line, "�����\tCtrl+N", 1).SetIcon(116)).
		pushLine(menu_line(menu_type::Line, "�������\tCtrl+O", 2).SetIcon(117)).
		pushLine(menu_line(menu_type::Line, "���������\tCtrl+S", 3).SetIcon(110)).
		pushLine(menu_line(menu_type::Line, "��������� ���...\tCtrl+Shift+S", 4).SetIcon(106)).
		pushLine(menu_line(menu_type::Separator)).
		pushLine(menu_line(menu().
			pushLine(menu_line(menu_type::Line, "1.c")).
			pushLine(menu_line(menu_type::Line, "2.c")).
			pushLine(menu_line(menu_type::Line, "3.c")).
			pushLine(menu_line(menu_type::Line, "4.c")).
			pushLine(menu_line(menu_type::Line, "5.c"))
			, "���������")).
		pushLine(menu_line(menu_type::Separator)).
		pushLine(menu_line(menu_type::Line, "�����\tAlt+F4", 4).SetIcon(111))
	, "����"));
	wnd.main_menu.pushLine(menu_line(menu().
		pushLine(menu_line(menu_type::Line, "��������\tCtrl+Z").SetIcon(114)).
		pushLine(menu_line(menu_type::Line, "���������\tAlt+Ctrl+Z").SetIcon(113)).
		pushLine(menu_line(menu_type::Separator)).
		pushLine(menu_line(menu_type::Line, "�������\tDel").SetIcon(105)).
		pushLine(menu_line(menu_type::Line, "��������\tCtrl+V").SetIcon(109)).
		pushLine(menu_line(menu_type::Line, "����������\tCtrl+C").SetIcon(107)).
		pushLine(menu_line(menu_type::Line, "��������\tCtrl+X").SetIcon(108))
	, "������", 2));
	wnd.main_menu.pushLine(menu_line(menu().
		pushLine(menu_line(menu_type::Line, "Fullscreen\tF11", 11)).
		pushLine(menu_line(menu_type::Line, "�������� ������", 10).SetTwoIcons(101, 103))
	, "�����", 3));
	wnd.main_menu.pushLine(menu_line(menu().
		pushLine(menu_line(menu().
			pushLine(menu_line(menu().
				pushLine(menu_line(menu_type::Line, "\t����")).
				pushLine(menu_line(menu_type::Separator)).
				pushLine(menu_line(menu_type::Line, "����\t����", true)).
				pushLine(menu_line(menu_type::Line, "����\t����")).
				pushLine(menu_line(menu_type::Separator)).
				pushLine(menu_line(menu_type::Line, "����"))
			, "����\t����").SetIcon(104)).
			pushLine(menu_line(menu_type::Separator)).
			pushLine(menu_line(menu_type::Line, "�� ���� ����� ���� �� ��������.", 0, true, true)).
			pushLine(menu_line(menu_type::Line, "����� \"�����\", \"Fullscreen\" � \"�������� ������\".", 0, true, true))
		, "� ���������\t����").SetIcon(102)).
		pushLine(menu_line(menu_type::Separator)).
		pushLine(menu_line(menu_type::Line, "�������\tAlt+F1").SetIcon(112))
	, "������", 3));
	wnd.redrawMenu();

	//system("pause");
	while (nado) wgs::window::sleep(100);
	return 0;
}