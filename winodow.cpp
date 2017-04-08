#include "window.h"

ITaskbarList3 *gwapi::Window::pTaskbarList;
UINT gwapi::Window::wm_create_mess;

gwapi::WinEvents::WinEvents() : 
	size(NULL),
	sizing(NULL),
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
		maxSize(Point(2000,2000)),
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

	/* —оздание потока, который создает окно и обрабатывает его сообщени€. */
	CreateThread( NULL, 0, &WindowLife::windowMainThread, type1, 0, NULL);

	// TODO сделать костыль, чтобы программа выходила из инициализации, только когда получит таскбар
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

void gwapi::Window::captionSet(std::string cap) {
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
	/* ≈сли ввести символы A..Z, a..z, и другие, которые 
	€вно отображаютс€ на клавиатуре, то функци€ проверит
	их на нажати€. »наче надо указывать коды виртуальных 
	клавиш из WinAPI. */
	return (-(GetKeyState(key) >> 8));
}

unsigned long gwapi::Window::timeGet(void) {
	/* ¬озвращает количество миллисекунд с того момента, 
	как система была запущена. »змер€ет врем€ в миллисек.
	ƒл€ того, чтобы узнать сколько времени прошло между 
	двум€ событи€ми, надо получить разность времени 
	между его началом и концом.*/
	return GetTickCount();
}

/* ~wapithread - создание потока. */