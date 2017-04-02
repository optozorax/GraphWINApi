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

map<HWND, Window*> Stack_;

enum WindowType {DefaultWindow, NoBorder};
enum SizingType {Bottom, BottomLeft, BottomRight, Left, Right, Top, TopLeft, TopRight};

void NOPE1(int, int) {};
void NOPE2(long&, long&, long&, long&, int) {};
void NOPE3(bool, bool) {};
void NOPE4(void) {};

LRESULT CALLBACK currentWndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI windowMainThread(HANDLE);

class Window {
	HWND hwnd_;
	HDC hdc_;
	Bufer current_;
public:
	void (*funcSized)(int, int);
	void (*funcSizing)(long&, long&, long&, long&, int);
	void (*funcMove)(int, int);
	void (*funcActivate)(bool, bool);
	void (*funcDestroy)(void);

	Point MinSize;
	Point MaxSize;

	Bufer canvas;
	
	Window(int = 300, int = 300, string = "gwapi program"); //, WindowType = DefaultWindow);
	~Window();
	
	void redraw(void);
	
	void sizeSet(int, int);
	pair<int, int> sizeGet(void);
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