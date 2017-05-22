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
		pushLine(menu_line(menu_type::Line, "Новый\tCtrl+N", 1).SetIcon(116)).
		pushLine(menu_line(menu_type::Line, "Открыть\tCtrl+O", 2).SetIcon(117)).
		pushLine(menu_line(menu_type::Line, "Сохранить\tCtrl+S", 3).SetIcon(110)).
		pushLine(menu_line(menu_type::Line, "Сохранить как...\tCtrl+Shift+S", 4).SetIcon(106)).
		pushLine(menu_line(menu_type::Separator)).
		pushLine(menu_line(menu().
			pushLine(menu_line(menu_type::Line, "1.c")).
			pushLine(menu_line(menu_type::Line, "2.c")).
			pushLine(menu_line(menu_type::Line, "3.c")).
			pushLine(menu_line(menu_type::Line, "4.c")).
			pushLine(menu_line(menu_type::Line, "5.c"))
			, "Последние")).
		pushLine(menu_line(menu_type::Separator)).
		pushLine(menu_line(menu_type::Line, "Выход\tAlt+F4", 4).SetIcon(111))
	, "Файл"));
	wnd.main_menu.pushLine(menu_line(menu().
		pushLine(menu_line(menu_type::Line, "Отменить\tCtrl+Z").SetIcon(114)).
		pushLine(menu_line(menu_type::Line, "Повторить\tAlt+Ctrl+Z").SetIcon(113)).
		pushLine(menu_line(menu_type::Separator)).
		pushLine(menu_line(menu_type::Line, "Удалить\tDel").SetIcon(105)).
		pushLine(menu_line(menu_type::Line, "Вставить\tCtrl+V").SetIcon(109)).
		pushLine(menu_line(menu_type::Line, "Копировать\tCtrl+C").SetIcon(107)).
		pushLine(menu_line(menu_type::Line, "Вырезать\tCtrl+X").SetIcon(108))
	, "Правка", 2));
	wnd.main_menu.pushLine(menu_line(menu().
		pushLine(menu_line(menu_type::Line, "Fullscreen\tF11", 11)).
		pushLine(menu_line(menu_type::Line, "Поменять иконку", 10).SetTwoIcons(101, 103))
	, "Опции", 3));
	wnd.main_menu.pushLine(menu_line(menu().
		pushLine(menu_line(menu().
			pushLine(menu_line(menu().
				pushLine(menu_line(menu_type::Line, "\tЕсть")).
				pushLine(menu_line(menu_type::Separator)).
				pushLine(menu_line(menu_type::Line, "Нету\tЕсть", true)).
				pushLine(menu_line(menu_type::Line, "Есть\tНету")).
				pushLine(menu_line(menu_type::Separator)).
				pushLine(menu_line(menu_type::Line, "Нету"))
			, "Нету\tЕсть").SetIcon(104)).
			pushLine(menu_line(menu_type::Separator)).
			pushLine(menu_line(menu_type::Line, "Ни один пункт меню не работает.", 0, true, true)).
			pushLine(menu_line(menu_type::Line, "Кроме \"Выход\", \"Fullscreen\" и \"Поменять иконку\".", 0, true, true))
		, "О программе\tНету").SetIcon(102)).
		pushLine(menu_line(menu_type::Separator)).
		pushLine(menu_line(menu_type::Line, "Справка\tAlt+F1").SetIcon(112))
	, "Помощь", 3));
	wnd.redrawMenu();

	//system("pause");
	while (nado) wgs::window::sleep(100);
	return 0;
}