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
	comand(NULL),
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

void gwapi::Window::Init(WindowType *curWin, HWND hwnd) {
	hwnd_ = hwnd;
	hdc_ = GetDC(hwnd);

	sizeSet(curWin->size);
	current_.hdc_ = hdc_;

	menu.hmenu_ = CreateMenu();
	sysMenu.hmenu_ = GetSystemMenu(hwnd_, false);
	SetMenu(hwnd_, menu.hmenu_);

	if (pTaskbarList == NULL) {
		CoInitialize(NULL);
		wm_create_mess = RegisterWindowMessage("TaskbarButtonCreated");
	} 

	delete curWin;
}

gwapi::Window::Window(WindowType type) :
	canvas(type.size.x+50, type.size.y+50), 
	MinSize(type.minSize), 
	MaxSize(type.maxSize)
{
	type.This = this;
	WindowType *type1 = new WindowType; 
	*type1 = type;

	/* —оздание потока, который создает окно и обрабатывает его сообщени€. */
	CreateThread( NULL, 0, &WindowLife::windowMainThread, type1, 0, NULL);

	//  остыль, чтобы программа выходила из инициализации, только когда получит таскбар
	while (pTaskbarList == 0) Sleep(1);
}

gwapi::Window::~Window() {
	SendMessage(hwnd_, WM_CLOSE, NULL, NULL);
	DeleteDC(hdc_);
}

void gwapi::Window::drawMenu(Point x, Menu mn) {
	RECT rcClient, rcWindow;
	GetWindowRect(hwnd_, &rcWindow);
	GetClientRect(hwnd_, &rcClient);
	int xx = ((rcWindow.right-rcWindow.left) - rcClient.right)/2;
	int yy = ((rcWindow.bottom-rcWindow.top) - rcClient.bottom) - xx;
	TrackPopupMenuEx(mn.hmenu_, TPM_LEFTALIGN | TPM_TOPALIGN, 
		x.x + rcWindow.left + xx, x.y + rcWindow.top + yy, hwnd_, NULL);
}

void gwapi::Window::redraw(void) {
	canvas.drawTo(current_);
}

void gwapi::Window::redrawMenu(void) {
	DrawMenuBar(hwnd_);
}

void gwapi::Window::sizeSet(Point sz) {
	RECT rcClient, rcWindow;
	POINT ptDiff;
	GetClientRect( hwnd_, &rcClient );
	GetWindowRect( hwnd_, &rcWindow );
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow( hwnd_, rcWindow.left, rcWindow.top, sz.x + ptDiff.x, sz.y + ptDiff.y, false);
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
	MoveWindow( hwnd_, ps.x, ps.y, rcClient.right, rcClient.bottom, false);
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

void gwapi::Window::sleep(unsigned long t) {
	Sleep(t);
}

/* ~wapithread - создание потока. */