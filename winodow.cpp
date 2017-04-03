#include "window.h"

gwapi::WindowType::WindowType() {
	pos = Point(0,0);
	size = Point(300,300);
	minSize = Point(0,0);
	maxSize = Point(10000,10000);

	caption = "GraphWinAPI program";

	flags.disabled = 0;
	flags.dropShadow = 0;
	flags.icon = 0;
	flags.maximizeButton = 0;
	flags.maximized = 0;
	flags.minimizeButton = 0;
	flags.onTop = 0;

	style = Standart;
}

HWND global_hwnd = 0;
DWORD WINAPI gwapi::windowMainThread(LPVOID a) {
	WindowType curWin = *((WindowType*) a);

	/* Создание структуры окна. */
	WNDCLASSEX wc;
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC | CS_SAVEBITS | CS_DBLCLKS; /* Устанавливает стиль класса. */
	wc.lpfnWndProc = gwapi::currentWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));
	wc.lpszMenuName = NULL;
	wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );

	if (curWin.flags.dropShadow)
		wc.style |= CS_DROPSHADOW;
	
	/* Для каждого окна классы уникальны */
	long currentClass = 0;
	string className;
	do {
		currentClass++;
		className = "GraphWinAPI_" + to_string(currentClass);
		wc.lpszClassName = className.c_str();
	} while (!RegisterClassEx(&wc));

	DWORD EXstyle = 0, DEFstyle = 0;

	if (curWin.flags.disabled)
		DEFstyle |= WS_DISABLED;
	if (curWin.flags.icon)	
		DEFstyle |= WS_SYSMENU;
	if (curWin.flags.maximizeButton)
		DEFstyle |= WS_MAXIMIZEBOX;
	if (curWin.flags.maximized)
		DEFstyle |= WS_MAXIMIZE;
	if (curWin.flags.minimizeButton)	
		DEFstyle |= WS_MINIMIZEBOX;
	if (curWin.flags.onTop)
		EXstyle |= WS_EX_TOPMOST;

	switch (curWin.style) {
	case WindowType::Standart:
		DEFstyle |= WS_TILEDWINDOW;
		break;
	case WindowType::Tool:
		DEFstyle |= WS_TILEDWINDOW;
		EXstyle |= WS_EX_TOOLWINDOW;
		break;
	case WindowType::Caption:
		DEFstyle |= WS_POPUP | WS_CAPTION;
		EXstyle |= WS_EX_TOOLWINDOW;
		break;
	case WindowType::Popup:
		DEFstyle |= WS_POPUP | WS_THICKFRAME;
		break;
	case WindowType::NoBorder:
		DEFstyle |= WS_POPUP;
		break;
	}

	global_hwnd = CreateWindowEx(
		EXstyle,
		wc.lpszClassName,
		curWin.caption.c_str(),
		DEFstyle,
		curWin.pos[0], curWin.pos[1],
		curWin.size[0], curWin.size[1],
		NULL, NULL, GetModuleHandle(NULL), NULL);

	/* Показ окна. */
	UpdateWindow( global_hwnd );
	ShowWindow( global_hwnd, SW_SHOW );

	/* Обработка сообщений. */
	MSG msg;
	BOOL bRet;
	
	while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0) { 
		if (bRet == -1) {
			/* Обработка ошибки и возможный выход из программы. */
		} else {
			TranslateMessage( &msg ); 
			DispatchMessage( &msg ); 
		}
	}
	return 0;
}

LRESULT CALLBACK gwapi::currentWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	/* Указатель на объект, который относится к текущему окну. */
	Window* this1 = WindowMap_[hwnd];

	switch ( msg ) {
		case WM_ACTIVATE:{
			/* Посылается когда окно активизируется или деактивизируется */
			// TODO понять надо ли это, и надо ли возвращать ноль или стандартную функцию(устанавливает фокус клавы в окно)
			bool fActive = !(LOWORD(wParam) == WA_INACTIVE);
			bool fMinimized = (BOOL) HIWORD(wParam);
			
			if (this1 != 0)
				(*(this1->funcActivate))(fActive, fMinimized);

			return 0;
		};break;
		case WM_CLOSE:{
			/* Сигнал, означающий, что прикладная программа должна завершить свою работу. */
			if (this1 != 0)
				(*(this1->funcDestroy))();

			DestroyWindow( hwnd );
			return 0;
		};break;case WM_CREATE:{
			/* Вызывается, когда должно быть создано окно. */
			
			// здесь особые вещички

			return 0;
		};break;case WM_DESTROY:{
			/* Когда окно разрушается. */
			PostQuitMessage( 0 );
			return 0;
		};break;
		case WM_GETMINMAXINFO:{
			/* Выдает максимальные и минимальные размеры окна. */
			MINMAXINFO *pInfo = (MINMAXINFO *)lParam;

			if (this1 != 0) {
				pInfo->ptMinTrackSize = { this1->MinSize[0], this1->MinSize[1] };
				pInfo->ptMaxTrackSize = { this1->MaxSize[0], this1->MaxSize[1] };
			}

			return 0;
		};break;
		case WM_MOVE:{
			/* Сообщение посылается при изменении положения окна. */
			int xPos = (int) LOWORD(lParam);
			int yPos = (int) HIWORD(lParam);

			if (this1 != 0)
				(*(this1->funcMove))(xPos, yPos);

			return 0;
		};break;case WM_PAINT:{
			/* Сообщение посылается, когда нужно отрисовать рабочую область. */
			static HDC hdc;
			static PAINTSTRUCT ps;
			hdc = BeginPaint( hwnd, &ps );
			
			Bufer a_(hdc);
			if (this1 != 0)
				this1->canvas.drawTo(a_);

			EndPaint( hwnd, &ps );
			return 0;
		};break;case WM_SIZE:{
			/* Сообщение посылается окну после того, как его размер изменился. */
			int fwSizeType = (int) wParam;
			int nWidth = LOWORD(lParam); 
			int nHeight = HIWORD(lParam); 

			if (fwSizeType != SIZE_MINIMIZED) {
				if (this1 != 0)
					(*(this1->funcSized))(nWidth, nHeight);
			}
			return 0;
		};break;case WM_SIZING:{
			/* Сообщение посылается во время изменении размеров окна. */
			int fwSide = wParam;
			RECT* lprc = (LPRECT) lParam;
			
			SizingType szType;
			switch (fwSide) {
				case (WMSZ_BOTTOM): szType = Bottom; break;
				case (WMSZ_BOTTOMLEFT): szType = BottomLeft; break;
				case (WMSZ_BOTTOMRIGHT): szType = BottomRight; break;
				case (WMSZ_LEFT): szType = Left; break;
				case (WMSZ_RIGHT): szType = Right; break;
				case (WMSZ_TOP): szType = Top; break;
				case (WMSZ_TOPLEFT): szType = TopLeft; break;
				case (WMSZ_TOPRIGHT): szType = TopRight; break;
			}
				
			if (this1 != 0)
				(*(this1->funcSizing))(lprc->left, lprc->top, lprc->right, lprc->bottom, szType);
			return TRUE;
		};break;
		//case WM_COMMAND:{
			/* Сообщение отправляется тогда, когда пользователь выбирает командный пункт из меню или происходит нажатие на кнопку и т.д. */
			// TODO сделать чтобы можно было на этот случай назначить функцию
			//switch ( LOWORD(wParam) ) {
				//case /* Номер. */:{
					/* Программа. */
				//};break;case /* Номер. */:{
					/* Программа. */
				//};break;
					/* ... */
			//};  
		//};break;
		default:
			return DefWindowProc( hwnd, msg, wParam, lParam );
	};
}
/* ------------------------------------------------------------------------- */

gwapi::Window::Window(WindowType type) : 
	canvas(type.size[0]+50, type.size[0]+50), 
	MinSize(type.minSize), 
	MaxSize(type.maxSize), 
	funcSized(&NOPE1), 
	funcSizing(&NOPE2), 
	funcMove(&NOPE1), 
	funcActivate(&NOPE3),
	funcDestroy(&NOPE4)
{
	/* Создание потока, который обрабатывает сообщения данного окна. */
	CreateThread( NULL, 0, &windowMainThread, &type, 0, NULL);	

	// TODO как-нибудь избавиться от этого костыля, опасно
	Sleep(50);
	hwnd_ = global_hwnd;
	WindowMap_[hwnd_] = this;
	hdc_ = GetDC(hwnd_);

	sizeSet(type.size);
	current_.hdc_ = hdc_;
}

gwapi::Window::~Window() {
	SendMessage(hwnd_, WM_CLOSE, NULL, NULL);
	DeleteDC(hdc_);
}

void gwapi::Window::redraw(void) {
	canvas.drawTo(current_);
}

void gwapi::Window::sizeSet(Point sz) {
	/* Процедура задает размер окна с учетом рамки. */
	RECT rcClient, rcWindow;
	POINT ptDiff;

	GetClientRect( hwnd_, &rcClient );
	GetWindowRect( hwnd_, &rcWindow );

	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;

	/* Изменяется размер. */
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
	/* Процедура задает размер окна с учетом рамки. */
	RECT rcClient;
	GetClientRect( hwnd_, &rcClient );
	MoveWindow( hwnd_, ps[0], ps[1], rcClient.right, rcClient.bottom, false);
}

void gwapi::Window::fullscreen(bool) {
	/* ~wapifullscreen - вызов фулскрина. */
}

bool gwapi::Window::isKeyDown(int) {
	// TODO
	return false;
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
	atype.pos = Point(0, 100);

	btype.caption = "SCND WNDW УПРЛС";
	btype.style = gwapi::WindowType::Tool;
	btype.pos = atype.pos + Point(atype.size[0]+20,0);

	ctype.pos = btype.pos + Point(btype.size[0]+20,0);
	ctype.style = gwapi::WindowType::Caption;

	gwapi::Window a(atype), b(btype), c(ctype);
	
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

/* ~wapitaskbar - всё для работы с таскбаром. */

/* ~wapithread - создание потока. */