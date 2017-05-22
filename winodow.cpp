#include "window.h"

ITaskbarList3 *wgs::window::sm_taskbar;
UINT wgs::window::wm_create_mess;

wgs::WinEvents::WinEvents() : 
	size(NULL),
	sizing(NULL),
	moving(NULL),
	keyboard(NULL),
	mouse(NULL),
	activate(NULL),
	comand(NULL),
	close(NULL) {
}


wgs::window_type::window_type() : 
		position(Point(0,0)), 
		size(Point(300,300)),
		minSize(Point(0,0)), 
		maxSize(Point(2000,2000)),
		caption("GraphWinAPI program"),
		style(Standart),
		icon(-1)
{
	flags = {};
}

void wgs::window::taskbarProgress(double x) {
	sm_taskbar->SetProgressValue(hwnd_, (DWORD) 100*x, (DWORD) 100); 
}

void wgs::window::taskbarColor(TaskbarColor clr) {
	switch (clr) {
	case wgs::window::Loading:
		sm_taskbar->SetProgressState(hwnd_, TBPF_INDETERMINATE); 
		break;
	case wgs::window::Green:
		sm_taskbar->SetProgressState(hwnd_, TBPF_NORMAL); 
		break;
	case wgs::window::Yellow:
		sm_taskbar->SetProgressState(hwnd_, TBPF_PAUSED); 
		break;
	case wgs::window::Red:
		sm_taskbar->SetProgressState(hwnd_, TBPF_ERROR); 
		break;
	case wgs::window::Blink:
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

void wgs::window::Init(window_type *curWin, HWND hwnd) {
	hwnd_ = hwnd;
	m_hdc = GetDC(hwnd);

	sizeSet(curWin->size);
	current_ = bufer(m_hdc, Point(10000, 10000));

	main_menu.hmenu_ = CreateMenu();
	sysMenu.hmenu_ = GetSystemMenu(hwnd_, false);
	SetMenu(hwnd_, main_menu.hmenu_);

	if (sm_taskbar == NULL) {
		CoInitialize(NULL);
		wm_create_mess = RegisterWindowMessage("TaskbarButtonCreated");
	} 

	delete curWin;
}

wgs::window::window(window_type type) :
	canvas(type.size.x+50, type.size.y+50), 
	MinSize(type.minSize), 
	MaxSize(type.maxSize)
{
	type.This = this;
	window_type *type1 = new window_type; 
	*type1 = type;

	/* —оздание потока, который создает окно и обрабатывает его сообщени€. */
	CreateThread( NULL, 0, &WindowLife::windowMainThread, type1, 0, NULL);

	//  остыль, чтобы программа выходила из инициализации, только когда получит таскбар
	if (type.style == window_type::Standart)
		while (sm_taskbar == 0) Sleep(1);
}

wgs::window::~window() {
	SendMessage(hwnd_, WM_CLOSE, NULL, NULL);
	DeleteDC(m_hdc);
}

void wgs::window::drawMenu(Point x, menu mn) {
	RECT rcClient, rcWindow;
	GetWindowRect(hwnd_, &rcWindow);
	GetClientRect(hwnd_, &rcClient);
	int xx = ((rcWindow.right-rcWindow.left) - rcClient.right)/2;
	int yy = ((rcWindow.bottom-rcWindow.top) - rcClient.bottom) - xx;
	TrackPopupMenuEx(mn.hmenu_, TPM_LEFTALIGN | TPM_TOPALIGN, 
		x.x + rcWindow.left + xx, x.y + rcWindow.top + yy, hwnd_, NULL);
}

void wgs::window::redraw(void) {
	canvas.drawTo(current_);
}

void wgs::window::redrawMenu(void) {
	DrawMenuBar(hwnd_);
}

void wgs::window::sizeSet(Point sz) {
	RECT rcClient, rcWindow;
	POINT ptDiff;
	GetClientRect( hwnd_, &rcClient );
	GetWindowRect( hwnd_, &rcWindow );
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow( hwnd_, rcWindow.left, rcWindow.top, sz.x + ptDiff.x, sz.y + ptDiff.y, false);
}

Point wgs::window::sizeGet(void) {
	RECT rcClient;
	GetClientRect( hwnd_, &rcClient );
	return Point(rcClient.right, rcClient.bottom);
}

void wgs::window::captionSet(std::string cap) {
	SetWindowText(hwnd_, cap.c_str());
}

void wgs::window::positionSet(Point ps) {
	RECT rcClient;
	GetClientRect( hwnd_, &rcClient );
	MoveWindow( hwnd_, ps.x, ps.y, rcClient.right, rcClient.bottom, false);
}

void wgs::window::fullscreen(bool state) {
	static WINDOWPLACEMENT wnplc;
	wnplc.length = sizeof(WINDOWPLACEMENT);
	static bool nowState = false;

	if (sm_taskbar != NULL) {
		if (state) {
			if (!nowState) {
				sm_taskbar->MarkFullscreenWindow(hwnd_, TRUE);
				GetWindowPlacement(hwnd_, &wnplc);
				SetWindowLongPtr(hwnd_, GWL_STYLE, WS_POPUP | (WS_Style & (WS_VSCROLL | WS_HSCROLL)));
				UpdateWindow(hwnd_);
				ShowWindow(hwnd_, SW_MAXIMIZE);
				SetFocus(hwnd_);	
				nowState = TRUE;
			};
		} else {
			if (nowState) {
				sm_taskbar->MarkFullscreenWindow(hwnd_, FALSE);
				SetWindowLongPtr(hwnd_, GWL_STYLE, WS_Style);
				UpdateWindow(hwnd_);
				SetWindowPlacement(hwnd_, &wnplc);
				SetFocus(hwnd_);
				nowState = FALSE;
			};
		};
	}
}

bool wgs::window::isKeyDown(int key) {
	/* ≈сли ввести символы A..Z, a..z, и другие, которые 
	€вно отображаютс€ на клавиатуре, то функци€ проверит
	их на нажати€. »наче надо указывать коды виртуальных 
	клавиш из WinAPI. */
	return (-(GetKeyState(key) >> 8));
}

unsigned long wgs::window::timeGet(void) {
	/* ¬озвращает количество миллисекунд с того момента, 
	как система была запущена. »змер€ет врем€ в миллисек.
	ƒл€ того, чтобы узнать сколько времени прошло между 
	двум€ событи€ми, надо получить разность времени 
	между его началом и концом.*/
	return GetTickCount();
}

void wgs::window::sleep(unsigned long t) {
	Sleep(t);
}