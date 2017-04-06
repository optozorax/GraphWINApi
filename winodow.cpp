#include "window.h"

ITaskbarList3 *gwapi::Window::pTaskbarList;
UINT gwapi::Window::wm_create_mess;

gwapi::WinEvents::WinEvents() : 
	size(NULL),
	sizing(NULL),
	move(NULL),
	moving(NULL),
	keyboard(NULL),
	mouse(NULL),
	activate(NULL),
	close(NULL) {
}


gwapi::WindowType::WindowType() : 
		position(Point(0,0)), 
		size(Point(300,300)),
		minSize(Point(0,0)), 
		maxSize(Point(10000,10000)),
		caption("GraphWinAPI program"),
		style(Standart)
{
	flags = {};
}

void gwapi::Window::taskbarProgress(double x) {
	pTaskbarList->SetProgressValue(hwnd_, (DWORD) 100*x, (DWORD) 100); 
}

void gwapi::Window::taskbarColor(TaskbarColor clr) {
	switch (clr) {
	case gwapi::Window::Loading:
		pTaskbarList->SetProgressState(hwnd_, TBPF_INDETERMINATE); 
		break;
	case gwapi::Window::Green:
		pTaskbarList->SetProgressState(hwnd_, TBPF_NORMAL); 
		break;
	case gwapi::Window::Yellow:
		pTaskbarList->SetProgressState(hwnd_, TBPF_PAUSED); 
		break;
	case gwapi::Window::Red:
		pTaskbarList->SetProgressState(hwnd_, TBPF_ERROR); 
		break;
	case gwapi::Window::Blink:
		FLASHWINFO pfwi;
		ZeroMemory( &pfwi, sizeof( FLASHWINFO ) );
		pfwi.cbSize = sizeof( FLASHWINFO );
		pfwi.hwnd = hwnd_;
		pfwi.dwFlags = FLASHW_TIMERNOFG | FLASHW_TRAY;
		pfwi.uCount = 5;
		pfwi.dwTimeout = 300;
		FlashWindowEx( &pfwi );
		break;
	}
}

gwapi::Window::Window(WindowType type) :
	canvas(type.size[0]+50, type.size[0]+50), 
	MinSize(type.minSize), 
	MaxSize(type.maxSize)
{
	type.This = this;
	WindowType *type1 = new WindowType; 
	*type1 = type;

	/* �������� ������, ������� ������� ���� � ������������ ��� ���������. */
	CreateThread( NULL, 0, &WindowLife::windowMainThread, type1, 0, NULL);	
}

gwapi::Window::~Window() {
	SendMessage(hwnd_, WM_CLOSE, NULL, NULL);
	DeleteDC(hdc_);
}

void gwapi::Window::redraw(void) {
	canvas.drawTo(current_);
}

void gwapi::Window::sizeSet(Point sz) {
	RECT rcClient, rcWindow;
	POINT ptDiff;
	GetClientRect( hwnd_, &rcClient );
	GetWindowRect( hwnd_, &rcWindow );
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow( hwnd_, rcWindow.left, rcWindow.top, sz[0] + ptDiff.x, sz[1] + ptDiff.y, false);
}

Point gwapi::Window::sizeGet(void) {
	RECT rcClient;
	GetClientRect( hwnd_, &rcClient );
	return Point(rcClient.right, rcClient.bottom);
}

void gwapi::Window::captionSet(string cap) {
	SetWindowText(hwnd_, cap.c_str());
}

void gwapi::Window::positionSet(Point ps) {
	RECT rcClient;
	GetClientRect( hwnd_, &rcClient );
	MoveWindow( hwnd_, ps[0], ps[1], rcClient.right, rcClient.bottom, false);
}

void gwapi::Window::fullscreen(bool state) {
	static WINDOWPLACEMENT wnplc;
	wnplc.length = sizeof(WINDOWPLACEMENT);
	static bool nowState = false;

	if (state) {
		if (!nowState) {
			pTaskbarList->MarkFullscreenWindow(hwnd_, TRUE);
			GetWindowPlacement(hwnd_, &wnplc);
			SetWindowLongPtr(hwnd_, GWL_STYLE, WS_POPUP | (WS_Style & (WS_VSCROLL | WS_HSCROLL)));
			UpdateWindow(hwnd_);
			ShowWindow(hwnd_, SW_MAXIMIZE);
			SetFocus(hwnd_);	
			nowState = TRUE;
		};
	} else {
		if (nowState) {
			pTaskbarList->MarkFullscreenWindow(hwnd_, FALSE);
			SetWindowLongPtr(hwnd_, GWL_STYLE, WS_Style);
			UpdateWindow(hwnd_);
			SetWindowPlacement(hwnd_, &wnplc);
			SetFocus(hwnd_);
			nowState = FALSE;
		};
	};
}

bool gwapi::Window::isKeyDown(int key) {
	/* ���� ������ ������� A..Z, a..z, � ������, ������� 
	���� ������������ �� ����������, �� ������� ��������
	�� �� �������. ����� ���� ��������� ���� ����������� 
	������ �� WinAPI. */
	return (-(GetKeyState(key) >> 8));
}

#ifdef __WINDOWTEST
#include "bufer.h"
#include "color.h"
#include <stdlib.h>
int main() {
	gwapi::WindowType atype, btype, ctype;

	atype.caption = "First window, nigga";
	atype.size = Point(350, 400);
	atype.maxSize = Point(400, 450);
	atype.position = Point(0, 100);

	btype.caption = "SCND WNDW �����";
	btype.style = gwapi::WindowType::Tool;
	btype.position = atype.position + Point(atype.size[0]+20,0);

	ctype.position = btype.position + Point(btype.size[0]+20,0);
	ctype.style = gwapi::WindowType::Caption;

	gwapi::Window a(atype);
	system("pause");
	gwapi::Window b(btype);
	system("pause");
	gwapi::Window c(ctype);
	
	a.canvas.penSet(gwapi::rgb(255,0,0), 3);
	a.canvas.clear();
	a.canvas.lineDraw(Point(100,100), Point(200,200));
	a.redraw();

	b.canvas.penSet(gwapi::argb(128,0,0,255), 3);
	b.canvas.brushSet(gwapi::argb(128,255,0,0));
	b.canvas.lineDraw(Point(100,200), Point(200,100));
	b.canvas.rectDraw(Point(10,10), Point(200,150));
	b.redraw();
	
	system("pause");

	a.fullscreen();

	a.canvas.penSet(gwapi::rgb(255,0,0), 3);
	a.canvas.lineDraw(Point(100,200), Point(200,100));
	b.canvas.rectDraw(Point(130,170), Point(200,100));
	b.canvas.drawAlphaTo(a.canvas, 100);

	b.sizeSet(Point(200,200));

	c.positionSet(Point(200,200));

	a.redraw();
	b.redraw();

	system("pause");

	return 0;
}
#endif

/* ~wapithread - �������� ������. */