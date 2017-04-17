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

namespace gwapi{

class WindowType {
public:
	Window *This;
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

	WindowType();
};

class Window {
	HDC hdc_;
	Bufer current_;

	LONG_PTR WS_Style;
	static UINT wm_create_mess;
	static ITaskbarList3 *pTaskbarList;

	void Init(WindowType*, HWND);
public:
	HWND hwnd_;
	WinEvents ev;
	Bufer canvas;
	Menu menu, sysMenu;

	Point MinSize;
	Point MaxSize;
	
	Window(WindowType = WindowType());
	~Window();

	void drawMenu(Point, Menu);
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

	friend LRESULT WindowLife::create(Window*, HWND&, WPARAM&, LPARAM&);
	friend DWORD WINAPI WindowLife::windowMainThread(LPVOID);
	friend void WindowLife::createStyle(WindowType&, DWORD&, DWORD&);
	friend void WindowLife::taskbarRegister(Window*, UINT, HWND);
};

}