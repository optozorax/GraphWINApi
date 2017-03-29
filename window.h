#ifndef __WINDOWDEFINE
#define __WINDOWDEFINE

#include <string>
#include <windows.h>
#include "bufer.h"

using namespace std;

namespace gwapi{

class Window {
	HWND hwnd_;
	HDC hdc_;
public:
	Bufer canvas;
	
	Window(int = 300, int = 300, string = "gwapi program");
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

}
#endif