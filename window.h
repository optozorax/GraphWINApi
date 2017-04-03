#ifndef __WINDOWDEFINE
#define __WINDOWDEFINE

/* Для того, чтобы работалы современные функции. */
#define _WIN32_IE 0x301
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501

#include <string>
#include <map>
#include <windows.h>
#include "bufer.h"

using namespace std;

namespace gwapi{

map<HWND, Window*> WindowMap_;

enum SizingType {Bottom, BottomLeft, BottomRight, Left, Right, Top, TopLeft, TopRight};

class WindowType {
public:
	Point pos, size;
	Point minSize, maxSize;
	string caption;

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

void NOPE1(int, int) {};
void NOPE2(long&, long&, long&, long&, int) {};
void NOPE3(bool, bool) {};
void NOPE4(void) {};

// TODO Вынести всю обработку сообщений в отдельное пространство имен
LRESULT CALLBACK currentWndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI windowMainThread(HANDLE);

class Window {
	HWND hwnd_;
	HDC hdc_;
	Bufer current_;
public:
	// TODO создать отдельный класс с событиями окна
		// http://vsokovikov.narod.ru/New_MSDN_API/Window/notify_wm_moving.htm
	// TODO добавить событие на двойной клик
	// TODO добавить событие на мышку
		// http://www.vsokovikov.narod.ru/New_MSDN_API/Mouse_input/notify_mouse.htm
	// TODO добавить собыите на клавиатуру
		// http://vsokovikov.narod.ru/New_MSDN_API/Keyb_input/msg_wm_sethotkey.htm
		// http://vsokovikov.narod.ru/New_MSDN_API/Keyb_input/notify_wm_keydown.htm
		// http://vsokovikov.narod.ru/New_MSDN_API/Keyb_input/notify_wm_keyup.htm
	// TODO добавить событие на таскбар
	void (*funcSized)(int, int);
	void (*funcSizing)(long&, long&, long&, long&, int);
	void (*funcMove)(int, int);
	void (*funcActivate)(bool, bool);
	void (*funcDestroy)(void);

	Point MinSize;
	Point MaxSize;

	Bufer canvas;
	
	Window(WindowType = WindowType());
	~Window();
	
	void redraw(void);
	
	void sizeSet(Point);
	Point sizeGet(void);
	void captionSet(string);
	void positionSet(Point);
	
	void fullscreen(bool = true);
	
	bool isKeyDown(int); /* ?????? */
	
	//# Когда-нибудь сделать интерфейс для работы с потоками, возможно отдельный класс. #//
	//# Когда-нибудь сделать интерфейс для назначения функций на события. #//
};

#define __WINDOWTEST

}
#endif