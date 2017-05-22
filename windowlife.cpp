#include <objbase.h>
//#include "vlc_windows_interfaces.h"
#include <Windowsx.h>
#include "windowlife.h"
#include "window.h"

std::map<HWND, wgs::window*> wgs::WindowLife::WindowMap_;

LRESULT CALLBACK wgs::WindowLife::currentWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	/* Указатель на объект, который относится к текущему окну. */
	extern std::map<HWND, window*> WindowMap_;
	window* This = WindowMap_[hwnd];

	taskbarRegister(This, msg, hwnd);

	switch ( msg ) {
	case WM_CREATE:{ return create(This, hwnd, wParam, lParam);
	};break;case WM_CLOSE:{	return close(This, hwnd, wParam, lParam);
	};break;case WM_PAINT:{	return paint(This, hwnd, wParam, lParam);
	};break;case WM_ACTIVATE:{ return activate(This, hwnd, wParam, lParam);			
	};break;case WM_COMMAND:{ return comand(This, hwnd, wParam, lParam);

	};break;case WM_MOVING:{ return moving(This, hwnd, wParam, lParam);

	};break;case WM_SIZE:{ return size(This, hwnd, wParam, lParam);
	};break;case WM_SIZING:{ return sizing(This, hwnd, wParam, lParam);
	};break;case WM_GETMINMAXINFO:{	return getMinMaxInfo(This, hwnd, wParam, lParam);

	};break;case WM_KEYDOWN:{	return keyboard(This, hwnd, wParam, lParam, FALSE);
	};break;case WM_KEYUP:{	return keyboard(This, hwnd, wParam, lParam, TRUE);

	/* Блок для сообщений мышки. */
	};break;case WM_LBUTTONDBLCLK:{ return mouse(This, hwnd, wParam, lParam, WinEvents::L_DBL);
	};break;case WM_LBUTTONDOWN:{ return mouse(This, hwnd, wParam, lParam, WinEvents::L_DOWN);
	};break;case WM_LBUTTONUP:{ return mouse(This, hwnd, wParam, lParam, WinEvents::L_UP);

	};break;case WM_MBUTTONDBLCLK:{ return mouse(This, hwnd, wParam, lParam, WinEvents::M_DBL);
	};break;case WM_MBUTTONDOWN:{ return mouse(This, hwnd, wParam, lParam, WinEvents::M_DOWN);
	};break;case WM_MBUTTONUP:{ return mouse(This, hwnd, wParam, lParam, WinEvents::M_UP);

	};break;case WM_RBUTTONDBLCLK:{ return mouse(This, hwnd, wParam, lParam, WinEvents::R_DBL);
	};break;case WM_RBUTTONDOWN:{ return mouse(This, hwnd, wParam, lParam, WinEvents::R_DOWN);
	};break;case WM_RBUTTONUP:{ return mouse(This, hwnd, wParam, lParam, WinEvents::R_UP);

	};break;case WM_XBUTTONDBLCLK:{ return mouse(This, hwnd, wParam, lParam, WinEvents::X1_DBL);
	};break;case WM_XBUTTONDOWN:{ return mouse(This, hwnd, wParam, lParam, WinEvents::X1_DOWN);
	};break;case WM_XBUTTONUP:{ return mouse(This, hwnd, wParam, lParam, WinEvents::X1_UP);

	};break;case WM_MOUSEMOVE:{ return mouse(This, hwnd, wParam, lParam, WinEvents::Move);
	};break;case WM_MOUSEWHEEL:{ return mouse(This, hwnd, wParam, lParam, WinEvents::Wheel);

	};break; default:
		return DefWindowProc( hwnd, msg, wParam, lParam );
	};
}

DWORD WINAPI wgs::WindowLife::windowMainThread(LPVOID typePointer) {
	window_type type = *((window_type*) typePointer);
	DWORD EX_Style = 0, WS_Style = 0;

	std::string className = registerClass(type);

	createStyle(type, WS_Style, EX_Style);

	createWindow(
		type, 
		className, 
		EX_Style, 
		WS_Style, 
		typePointer
	);

	msgCycle();

	return 0;
}

void wgs::WindowLife::createStyle(window_type &type, DWORD &WS_Style, DWORD &EX_Style) {
	if (type.flags.disabled)
		WS_Style |= WS_DISABLED;
	if (type.flags.icon)	
		WS_Style |= WS_SYSMENU;
	if (type.flags.maximizeButton)
		WS_Style |= WS_MAXIMIZEBOX;
	if (type.flags.maximized)
		WS_Style |= WS_MAXIMIZE;
	if (type.flags.minimizeButton)	
		WS_Style |= WS_MINIMIZEBOX;
	if (type.flags.onTop)
		EX_Style |= WS_EX_TOPMOST;

	switch (type.style) {
	case window_type::Standart:
		WS_Style |= WS_TILEDWINDOW;
		break;
	case window_type::Tool:
		WS_Style |= WS_TILEDWINDOW;
		EX_Style |= WS_EX_TOOLWINDOW;
		break;
	case window_type::Caption:
		WS_Style |= WS_POPUP | WS_CAPTION;
		EX_Style |= WS_EX_TOOLWINDOW;
		break;
	case window_type::Popup:
		WS_Style |= WS_POPUP | WS_THICKFRAME;
		break;
	case window_type::NoBorder:
		WS_Style |= WS_POPUP;
		break;
	}

	type.This->WS_Style = WS_Style;
}

std::string wgs::WindowLife::registerClass(window_type &type) {
	/* Создание структуры окна. */
	WNDCLASSEX wc;
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC | CS_SAVEBITS | CS_DBLCLKS;
	wc.lpfnWndProc = wgs::WindowLife::currentWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	if (type.icon == -1)
		wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	else
		wc.hIcon =  LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(type.icon));
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));
	wc.lpszMenuName = NULL;
	wc.hIconSm = wc.hIcon;

	if (type.flags.dropShadow)
		wc.style |= CS_DROPSHADOW;

	/* Для каждого окна классы уникальны */
	long currentClassN = 0;
	std::string className;
	do {
		currentClassN++;
		className = "GraphWinAPI_" + std::to_string(currentClassN);
		wc.lpszClassName = className.c_str();
	} while (!RegisterClassEx(&wc));

	return className;
}

void wgs::WindowLife::createWindow(window_type &type, std::string className, DWORD EX_Style, DWORD WS_Style, LPVOID typePointer) {
	HWND hwnd = CreateWindowEx(
		EX_Style,
		className.c_str(),
		type.caption.c_str(),
		WS_Style,
		type.position.x, type.position.y,
		type.size.x, type.size.y,
		NULL, NULL, GetModuleHandle(NULL), 
		typePointer
	);

	UpdateWindow( hwnd );
	ShowWindow( hwnd, SW_SHOW );
}

void wgs::WindowLife::msgCycle(void) {
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
}

void wgs::WindowLife::taskbarRegister(window *This, UINT msg, HWND hwnd) {
	if (This != NULL) {
		if (msg == This->wm_create_mess) {
			HRESULT const Status = CoCreateInstance(
				CLSID_TaskbarList,
				NULL,
				CLSCTX_ALL,
				IID_ITaskbarList3,
				(void **)&(This->sm_taskbar)
			);

			This->sm_taskbar->HrInit();

			if (!SUCCEEDED(Status)) {
				/* Обработка ошибки. */

			}
		}
	}
}

LRESULT wgs::WindowLife::getMinMaxInfo(window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* Выдает максимальные и минимальные размеры окна. */
	MINMAXINFO *pInfo = (MINMAXINFO *)lParam;

	if (This != 0) {
		RECT rcClient, rcWindow;
		POINT ptDiff;
		GetClientRect( This->hwnd_, &rcClient );
		GetWindowRect( This->hwnd_, &rcWindow );
		ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
		ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
		pInfo->ptMinTrackSize = { This->MinSize.x + ptDiff.x, This->MinSize.y+ptDiff.y };
		pInfo->ptMaxTrackSize = { This->MaxSize.x + ptDiff.x, This->MaxSize.y+ptDiff.y };
	}

	return 0;
}

LRESULT wgs::WindowLife::activate(window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* Посылается когда окно активизируется или деактивизируется */
	// TODO понять надо ли это, и надо ли возвращать ноль или стандартную функцию(устанавливает фокус клавы в окно)
	bool fActive = !(LOWORD(wParam) == WA_INACTIVE);
	bool fMinimized = (BOOL) HIWORD(wParam);

	if (This->ev.activate != NULL)
		(*(This->ev.activate))(This, fActive, fMinimized);

	return 0;
}

LRESULT wgs::WindowLife::size(window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* Сообщение посылается окну после того, как его размер изменился. */
	int fwSizeType = (int) wParam;
	int nWidth = LOWORD(lParam); 
	int nHeight = HIWORD(lParam); 

	if (fwSizeType != SIZE_MINIMIZED) {
		if (This->ev.size != NULL)
			(*(This->ev.size))(This, nWidth, nHeight);
	}
	return 0;
}

LRESULT wgs::WindowLife::sizing(window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* Сообщение посылается во время изменении размеров окна. */
	int fwSide = wParam;
	RECT* lprc = (LPRECT) lParam;

	WinEvents::SizingType szType;
	switch (fwSide) {
	case (WMSZ_BOTTOM): szType = WinEvents::Bottom; break;
	case (WMSZ_BOTTOMLEFT): szType = WinEvents::BottomLeft; break;
	case (WMSZ_BOTTOMRIGHT): szType = WinEvents::BottomRight; break;
	case (WMSZ_LEFT): szType = WinEvents::Left; break;
	case (WMSZ_RIGHT): szType = WinEvents::Right; break;
	case (WMSZ_TOP): szType = WinEvents::Top; break;
	case (WMSZ_TOPLEFT): szType = WinEvents::TopLeft; break;
	case (WMSZ_TOPRIGHT): szType = WinEvents::TopRight; break;
	}

	if (This->ev.sizing != NULL)
		(*(This->ev.sizing))(This, lprc->left, lprc->top, lprc->right, lprc->bottom, szType);
	return TRUE;
}

LRESULT wgs::WindowLife::close(window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* Сигнал, означающий, что прикладная программа должна завершить свою работу. */
	if (This->ev.close != NULL)
		(*(This->ev.close))(This);

	DestroyWindow( hwnd );
	return 0;
}

LRESULT wgs::WindowLife::create(window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* Вызывается, когда должно быть создано окно. */
	extern std::map<HWND, window*> WindowMap_;

	window_type *curWin = ((window_type*) ((CREATESTRUCT*)lParam)->lpCreateParams);
	This = curWin->This;

	WindowMap_[hwnd] = This;

	This->Init(curWin, hwnd);

	return 0;
}

LRESULT wgs::WindowLife::paint(window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* Сообщение посылается, когда нужно отрисовать рабочую область. */
	static HDC hdc;
	static PAINTSTRUCT ps;
	hdc = BeginPaint( hwnd, &ps );

	bufer a_(hdc, Point(10000, 10000));
	if (This != 0)
		This->canvas.drawTo(a_);

	EndPaint( hwnd, &ps );
	return 0;
}

LRESULT wgs::WindowLife::comand(window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* Сообщение отправляется тогда, когда пользователь выбирает командный пункт из меню или происходит нажатие на кнопку и т.д. */
	if (This->ev.comand != NULL)
		(*(This->ev.comand))(This, LOWORD(wParam));
	return 0;
}

LRESULT wgs::WindowLife::moving(window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	RECT* lprc = (LPRECT) lParam;
	if (This->ev.moving != NULL)
		(*(This->ev.moving))(This, lprc->left, lprc->top, lprc->right, lprc->bottom);
	return TRUE;
}

LRESULT wgs::WindowLife::mouse(window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam, WinEvents::MouseType mtype) {
	/* Обрабатывает данные с мышки. */
	int xPos = GET_X_LPARAM(lParam); 
	int yPos = GET_Y_LPARAM(lParam); 
	int wheel = 0;
	
	if (mtype == wgs::WinEvents::X1_DBL || 
		mtype == wgs::WinEvents::X1_DOWN ||
		mtype == wgs::WinEvents::X1_UP
	) {
		int fwButton = GET_XBUTTON_WPARAM (wParam);
		if (fwButton == XBUTTON2) {
			switch (mtype) {
			case wgs::WinEvents::X1_DBL:
				mtype = wgs::WinEvents::X2_DBL;
				break;
			case wgs::WinEvents::X1_DOWN:
				mtype = wgs::WinEvents::X2_DOWN;
				break;
			case wgs::WinEvents::X1_UP:
				mtype = wgs::WinEvents::X2_UP;
				break;
			}
		}
	} else if (mtype == wgs::WinEvents::Wheel) {
		wheel = GET_WHEEL_DELTA_WPARAM(wParam);
	}

	if (This->ev.mouse != NULL)
		(*(This->ev.mouse))(This, xPos, yPos, mtype, wheel);

	return 0;
}

LRESULT wgs::WindowLife::keyboard(window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam, BOOL state) {
	if (This->ev.keyboard != NULL)
		(*(This->ev.keyboard))(This, (int) wParam, state);
	return 0;
}