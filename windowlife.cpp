#include "windowlife.h"
#include "window.h"

map<HWND, gwapi::Window*> gwapi::WindowLife::WindowMap_;

LRESULT CALLBACK gwapi::WindowLife::currentWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	/* ��������� �� ������, ������� ��������� � �������� ����. */
	extern map<HWND, Window*> WindowMap_;
	Window* This = WindowMap_[hwnd];

	taskbarRegister(This, msg);

	switch ( msg ) {
	case WM_CREATE:{ return create(This, hwnd, wParam, lParam);
	};break;case WM_ACTIVATE:{ return activate(This, hwnd, wParam, lParam);			
	};break;case WM_CLOSE:{	return close(This, hwnd, wParam, lParam);
	};break;case WM_GETMINMAXINFO:{	return getMinMaxInfo(This, hwnd, wParam, lParam);
	};break;case WM_MOVE:{ return move(This, hwnd, wParam, lParam);
	};break;case WM_PAINT:{	return paint(This, hwnd, wParam, lParam);
	};break;case WM_SIZE:{ return size(This, hwnd, wParam, lParam);
	};break;case WM_SIZING:{ return sizing(This, hwnd, wParam, lParam);
	};break;case WM_COMMAND:{ return comand(This, hwnd, wParam, lParam);
	//};break;case WM_DESTROY:{ PostQuitMessage(0); return 0;
	};break; default:
		return DefWindowProc( hwnd, msg, wParam, lParam );
	};
}

DWORD WINAPI gwapi::WindowLife::windowMainThread(LPVOID typePointer) {
	WindowType type = *((WindowType*) typePointer);
	DWORD EX_Style = 0, WS_Style = 0;

	string className = registerClass(type);

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

void gwapi::WindowLife::createStyle(WindowType &type, DWORD &WS_Style, DWORD &EX_Style) {
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
	case WindowType::Standart:
		WS_Style |= WS_TILEDWINDOW;
		break;
	case WindowType::Tool:
		WS_Style |= WS_TILEDWINDOW;
		EX_Style |= WS_EX_TOOLWINDOW;
		break;
	case WindowType::Caption:
		WS_Style |= WS_POPUP | WS_CAPTION;
		EX_Style |= WS_EX_TOOLWINDOW;
		break;
	case WindowType::Popup:
		WS_Style |= WS_POPUP | WS_THICKFRAME;
		break;
	case WindowType::NoBorder:
		WS_Style |= WS_POPUP;
		break;
	}

	type.This->WS_Style = WS_Style;
}

string gwapi::WindowLife::registerClass(WindowType &type) {
	/* �������� ��������� ����. */
	WNDCLASSEX wc;
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC | CS_SAVEBITS | CS_DBLCLKS;
	wc.lpfnWndProc = gwapi::WindowLife::currentWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));
	wc.lpszMenuName = NULL;
	wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );

	if (type.flags.dropShadow)
		wc.style |= CS_DROPSHADOW;

	/* ��� ������� ���� ������ ��������� */
	long currentClassN = 0;
	string className;
	do {
		currentClassN++;
		className = "GraphWinAPI_" + to_string(currentClassN);
		wc.lpszClassName = className.c_str();
	} while (!RegisterClassEx(&wc));

	return className;
}

void gwapi::WindowLife::createWindow(WindowType &type, string className, DWORD EX_Style, DWORD WS_Style, LPVOID typePointer) {
	HWND hwnd = CreateWindowEx(
		EX_Style,
		className.c_str(),
		type.caption.c_str(),
		WS_Style,
		type.position[0], type.position[1],
		type.size[0], type.size[1],
		NULL, NULL, GetModuleHandle(NULL), 
		typePointer
	);

	UpdateWindow( hwnd );
	ShowWindow( hwnd, SW_SHOW );
}

void gwapi::WindowLife::msgCycle(void) {
	/* ��������� ���������. */
	MSG msg;
	BOOL bRet;

	while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0) { 
		if (bRet == -1) {
			/* ��������� ������ � ��������� ����� �� ���������. */
		} else {
			TranslateMessage( &msg ); 
			DispatchMessage( &msg ); 
		}
	}
}

void gwapi::WindowLife::taskbarRegister(Window *This, UINT msg) {
	if (This != NULL) {
		if (msg == This->wm_create_mess) {
			HRESULT const Status = CoCreateInstance(
				CLSID_TaskbarList,
				NULL,
				CLSCTX_ALL,
				IID_ITaskbarList3,
				(void **)&(This->pTaskbarList)
			);

			if (!SUCCEEDED(Status)) {
				/* ��������� ������. */

			}
		}
	}
}

LRESULT gwapi::WindowLife::getMinMaxInfo(Window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* ������ ������������ � ����������� ������� ����. */
	MINMAXINFO *pInfo = (MINMAXINFO *)lParam;

	if (This != 0) {
		pInfo->ptMinTrackSize = { This->MinSize[0], This->MinSize[1] };
		pInfo->ptMaxTrackSize = { This->MaxSize[0], This->MaxSize[1] };
	}

	return 0;
}

LRESULT gwapi::WindowLife::activate(Window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* ���������� ����� ���� �������������� ��� ���������������� */
	// TODO ������ ���� �� ���, � ���� �� ���������� ���� ��� ����������� �������(������������� ����� ����� � ����)
	bool fActive = !(LOWORD(wParam) == WA_INACTIVE);
	bool fMinimized = (BOOL) HIWORD(wParam);

	if (This->funcActivate != NULL)
		(*(This->funcActivate))(fActive, fMinimized);

	return 0;
}

LRESULT gwapi::WindowLife::move(Window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* ��������� ���������� ��� ��������� ��������� ����. */
	int xPos = (int) LOWORD(lParam);
	int yPos = (int) HIWORD(lParam);

	if (This->funcMove != NULL)
		(*(This->funcMove))(xPos, yPos);

	return 0;
}

LRESULT gwapi::WindowLife::size(Window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* ��������� ���������� ���� ����� ����, ��� ��� ������ ���������. */
	int fwSizeType = (int) wParam;
	int nWidth = LOWORD(lParam); 
	int nHeight = HIWORD(lParam); 

	if (fwSizeType != SIZE_MINIMIZED) {
		if (This->funcSized != NULL)
			(*(This->funcSized))(nWidth, nHeight);
	}
	return 0;
}

LRESULT gwapi::WindowLife::sizing(Window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* ��������� ���������� �� ����� ��������� �������� ����. */
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

	if (This->funcSizing != NULL)
		(*(This->funcSizing))(lprc->left, lprc->top, lprc->right, lprc->bottom, szType);
	return TRUE;
}

LRESULT gwapi::WindowLife::close(Window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* ������, ����������, ��� ���������� ��������� ������ ��������� ���� ������. */
	if (This->funcDestroy != NULL)
		(*(This->funcDestroy))();

	DestroyWindow( hwnd );
	return 0;
}

LRESULT gwapi::WindowLife::create(Window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* ����������, ����� ������ ���� ������� ����. */

	extern map<HWND, Window*> WindowMap_;

	WindowType curWin = *((WindowType*) ((CREATESTRUCT*)lParam)->lpCreateParams);
	This = curWin.This;

	WindowMap_[hwnd] = This;

	This->hwnd_ = hwnd;
	This->hdc_ = GetDC(hwnd);

	This->sizeSet(curWin.size);
	This->current_.hdc_ = This->hdc_;

	CoInitialize(NULL);
	This->wm_create_mess = RegisterWindowMessage("TaskbarButtonCreated");

	delete (WindowType*) ((CREATESTRUCT*)lParam)->lpCreateParams;

	return 0;
}

LRESULT gwapi::WindowLife::paint(Window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* ��������� ����������, ����� ����� ���������� ������� �������. */
	static HDC hdc;
	static PAINTSTRUCT ps;
	hdc = BeginPaint( hwnd, &ps );

	Bufer a_(hdc);
	if (This != 0)
		This->canvas.drawTo(a_);

	EndPaint( hwnd, &ps );
	return 0;
}

LRESULT gwapi::WindowLife::comand(Window *This, HWND &hwnd, WPARAM &wParam, LPARAM &lParam) {
	/* ��������� ������������ �����, ����� ������������ �������� ��������� ����� �� ���� ��� ���������� ������� �� ������ � �.�. */
	// TODO ������� ����� ����� ���� �� ���� ������ ��������� �������
	switch ( LOWORD(wParam) ) {
	}; 
	return 0;
}