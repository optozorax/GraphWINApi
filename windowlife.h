#pragma once

/* ��� ����, ����� �������� ����������� �������. */
#define _WIN32_IE 0x301
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501

#include <map>
#include <windows.h>
#include <Windowsx.h>
#include "winevents.h"
using namespace std;

namespace gwapi {

class Window;
class WindowType;

namespace WindowLife {

extern map<HWND, Window*> WindowMap_;

/* ���� �������, ��������� � ����������� ����. */
LRESULT CALLBACK currentWndProc(HWND, UINT, WPARAM, LPARAM);
void taskbarRegister(Window*, UINT, HWND);
LRESULT getMinMaxInfo(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT activate(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT move(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT size(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT sizing(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT close(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT create(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT paint(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT comand(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT moving(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT mouse(Window*, HWND&, WPARAM&, LPARAM&, WinEvents::MouseType);
LRESULT keyboard(Window*, HWND&, WPARAM&, LPARAM&, BOOL);

/* ���� �������, ��������� � ��������� ����. */
DWORD WINAPI windowMainThread(LPVOID);
string registerClass(WindowType&);
void createStyle(WindowType&, DWORD&, DWORD&);
void createWindow(WindowType&, string, DWORD, DWORD, LPVOID);
void msgCycle(void);

}

}