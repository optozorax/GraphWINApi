#pragma once

/* ��� ����, ����� �������� ����������� �������. */
#define _WIN32_IE 0x301
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501

#include <string>
//#include <map>
#include <windows.h>
#include <objbase.h>
//#include "vlc_windows_interfaces.h"
#include <Shobjidl.h>
#include "bufer.h"
#include "windowlife.h"

using namespace std;

namespace gwapi{

class WindowType {
public:
	Window *This;
	Point position, size;
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

class Window {
	HWND hwnd_;
	HDC hdc_;
	Bufer current_;

	UINT wm_create_mess;
	LONG_PTR WS_Style;
	ITaskbarList3 *pTaskbarList;
public:
	// TODO ������� ��������� ����� � ��������� ����
		// http://vsokovikov.narod.ru/New_MSDN_API/Window/notify_wm_moving.htm
	// TODO �������� ������� �� ������� ����
	// TODO �������� ������� �� �����
		// http://www.vsokovikov.narod.ru/New_MSDN_API/Mouse_input/notify_mouse.htm
	// TODO �������� ������� �� ����������
		// http://vsokovikov.narod.ru/New_MSDN_API/Keyb_input/msg_wm_sethotkey.htm
		// http://vsokovikov.narod.ru/New_MSDN_API/Keyb_input/notify_wm_keydown.htm
		// http://vsokovikov.narod.ru/New_MSDN_API/Keyb_input/notify_wm_keyup.htm
	// TODO �������� ������� �� �������
	void (*funcSized)(int, int);
	void (*funcSizing)(long&, long&, long&, long&, int);
	void (*funcMove)(int, int);
	void (*funcActivate)(bool, bool);
	void (*funcDestroy)(void);

	// TODO ������� ��������� ��� ��������

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
	
	//# �����-������ ������� ��������� ��� ������ � ��������, �������� ��������� �����. #//
	//# �����-������ ������� ��������� ��� ���������� ������� �� �������. #//

	friend LRESULT WindowLife::create(Window*, HWND&, WPARAM&, LPARAM&);
	friend DWORD WINAPI WindowLife::windowMainThread(LPVOID);
	friend void WindowLife::createStyle(WindowType&, DWORD&, DWORD&);
	friend void WindowLife::taskbarRegister(Window*, UINT);
};

#define __WINDOWTEST

}