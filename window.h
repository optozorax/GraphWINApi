#pragma once

/* Для того, чтобы работалы современные функции. */
#define _WIN32_IE 0x301
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501

#include <string>
#include <windows.h>
#include <Shobjidl.h>
#include "bufer.h"
#include "windowlife.h"
#include "winevents.h"
#include "menu.h"

namespace wgs{

class window_type {
public:
	window *This;
	Point position, size;
	Point minSize, maxSize;
	std::string caption;
	int icon;

	struct {
		unsigned int dropShadow : 1;
		unsigned int onTop : 1;		
		unsigned int disabled : 1;	
		unsigned int maximized : 1;
		unsigned int maximizeButton : 1;
		unsigned int minimizeButton : 1;
		unsigned int icon : 1;		
	} flags;

	enum WindowStyle{Standart, Tool, Caption, Popup, NoBorder} 
		style;

	window_type();
};

class window {
	HDC m_hdc;
	bufer current_;

	LONG_PTR WS_Style;
	static UINT wm_create_mess;
	static ITaskbarList3 *sm_taskbar;

	HWND	hwnd_;

	void Init(window_type*, HWND);
public:
	WinEvents ev;
	bufer	canvas;
	menu	main_menu;
	menu	sysMenu;

	Point	MinSize;
	Point	MaxSize;
	
	window(window_type = window_type());
	~window();

	void drawMenu(Point, menu);
	void redraw(void);
	void redrawMenu(void);
	
	void sizeSet(Point);
	Point sizeGet(void);
	void captionSet(std::string);
	void positionSet(Point);
	
	enum TaskbarColor{Loading, Green, Yellow, Red, Blink};
	void fullscreen(bool = true);
	void taskbarProgress(double);
	void taskbarColor(TaskbarColor = Loading);

	static unsigned long timeGet(void);
	static bool isKeyDown(int);
	static void sleep(unsigned long);

	friend LRESULT WindowLife::create(window*, HWND&, WPARAM&, LPARAM&);
	friend DWORD WINAPI WindowLife::windowMainThread(LPVOID);
	friend void WindowLife::createStyle(window_type&, DWORD&, DWORD&);
	friend void WindowLife::taskbarRegister(window*, UINT, HWND);
};

}