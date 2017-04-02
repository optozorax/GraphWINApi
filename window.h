#ifndef __WINDOWDEFINE
#define __WINDOWDEFINE

/* Для того, чтобы работалы современные функции. */
#define _WIN32_IE 0x301
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501

#include <string>
#include <windows.h>
#include "bufer.h"

using namespace std;

namespace gwapi{

enum WindowType {DefaultWindow, NoBorder};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI tickThreadProc(HANDLE);

class Window {
	HWND hwnd_;
	HDC hdc_;
	Bufer current_;

	HANDLE hTickThread;
public:
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