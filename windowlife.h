#pragma once

#include <map>
#include <windows.h>
using namespace std;

namespace gwapi {

class Window;
class WindowType;

namespace WindowLife {

extern map<HWND, Window*> WindowMap_;

enum SizingType {Bottom, BottomLeft, BottomRight, Left, Right, Top, TopLeft, TopRight};

LRESULT CALLBACK currentWndProc(HWND, UINT, WPARAM, LPARAM);
void taskbarRegister(Window*, UINT);
LRESULT getMinMaxInfo(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT activate(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT move(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT size(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT sizing(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT close(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT create(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT paint(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT comand(Window*, HWND&, WPARAM&, LPARAM&);

DWORD WINAPI windowMainThread(LPVOID);
string registerClass(WindowType&);
void createStyle(WindowType&, DWORD&, DWORD&);
void createWindow(WindowType&, string, DWORD, DWORD, LPVOID);
void msgCycle(void);

}

}