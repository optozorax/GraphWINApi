#pragma once

/* ��� ����, ����� �������� ����������� �������. */
#define _WIN32_IE 0x301
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501

#include <map>
#include <windows.h>
#include "winevents.h"

namespace gwapi {

class Window;
class WindowType;

namespace WindowLife {

extern std::map<HWND, Window*> WindowMap_;

/* ���� �������, ��������� � ����������� ����. */
LRESULT CALLBACK currentWndProc(HWND, UINT, WPARAM, LPARAM);

// TODO ������ �� ���������� ��� ��� �����
void taskbarRegister(Window*, UINT, HWND);
LRESULT getMinMaxInfo(Window*, HWND&, WPARAM&, LPARAM&);
LRESULT activate(Window*, HWND&, WPARAM&, LPARAM&);
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
std::string registerClass(WindowType&);
void createStyle(WindowType&, DWORD&, DWORD&);
void createWindow(WindowType&, std::string, DWORD, DWORD, LPVOID);
void msgCycle(void);

}

}