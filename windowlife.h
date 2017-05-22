#pragma once

/* Для того, чтобы работалы современные функции. */
#define _WIN32_IE 0x301
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501

#include <map>
#include <windows.h>
#include "winevents.h"

namespace wgs {

class window;
class window_type;

namespace WindowLife {

extern std::map<HWND, window*> WindowMap_;

/* Блок функций, связанных с сообщениями окна. */
LRESULT CALLBACK currentWndProc(HWND, UINT, WPARAM, LPARAM);

// TODO убрать из интерфейса всю эту фигню
void taskbarRegister(window*, UINT, HWND);
LRESULT getMinMaxInfo(window*, HWND&, WPARAM&, LPARAM&);
LRESULT activate(window*, HWND&, WPARAM&, LPARAM&);
LRESULT size(window*, HWND&, WPARAM&, LPARAM&);
LRESULT sizing(window*, HWND&, WPARAM&, LPARAM&);
LRESULT close(window*, HWND&, WPARAM&, LPARAM&);
LRESULT create(window*, HWND&, WPARAM&, LPARAM&);
LRESULT paint(window*, HWND&, WPARAM&, LPARAM&);
LRESULT comand(window*, HWND&, WPARAM&, LPARAM&);
LRESULT moving(window*, HWND&, WPARAM&, LPARAM&);
LRESULT mouse(window*, HWND&, WPARAM&, LPARAM&, WinEvents::MouseType);
LRESULT keyboard(window*, HWND&, WPARAM&, LPARAM&, BOOL);

/* Блок функций, связанных с созданием окна. */
DWORD WINAPI windowMainThread(LPVOID);
std::string registerClass(window_type&);
void createStyle(window_type&, DWORD&, DWORD&);
void createWindow(window_type&, std::string, DWORD, DWORD, LPVOID);
void msgCycle(void);

}

}