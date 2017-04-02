#include "window.h"

HWND global_hwnd = 0;
DWORD WINAPI gwapi::tickThreadProc(LPVOID a) {
	/* �������� ��������� ����. */
	WNDCLASSEX wc;
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC | CS_SAVEBITS; /* ������������� ����� ������. */
					// TODO ��������� ��� ��� ����
					//# CS_DROPSHADOW	������� ������ �������� ���� �� ����. ��� �������, �� ���������� ��� ���������, ��������� ���� ���� ����. #//
					//# ����� ������������ ��� ���� �� ������ WS_POPUP. #//
	wc.lpfnWndProc = gwapi::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));
	wc.lpszMenuName = NULL;
	// TODO ������� ����� ������ ��� ������� ���� ���� ���������
	wc.lpszClassName = "NADO_SDELAT_CHTOBI_KLASSI_BILI_UNIKALNI";
	wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
	/* ����������� ���� � ��������� ������. */
	/* ������� ������������ ����� ���� ��� ������������ ������������� ��� ������ ������� CreateWindow ��� CreateWindowEx. */
	eshe:
	if ( !RegisterClassEx(&wc) ) {
		//MessageBox( NULL, "Failed to register window class.", "Error", MB_OK );
		wc.lpszClassName = "KAK_TO_TAK";
		goto eshe;
	}

	global_hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW, /* ���������� ����������� ����� ������������ ����. */
								//# WS_EX_TOOLWINDOW		������� ���� ���������������� �������; �� ���� ���� �������������� ������������ ��� ��������� ���������������� ������. ���� ���������������� ������� ����� ������� ���������, ������� ������, ��� ���������� ������ ���������, � ��������� ���� ���������, � �������������� �������� ������. ���� ���������������� ������� �� ���������� � ������ ����� ��� � ���������� ����, ������� ���������� �����, ����� ������������ �������� ALT+TAB. ���� ���� ���������������� ������� ����� ��������� ����, ��� ����������� �� ������������ � ���������. ������, �� ������ ���������� �� ������ ��������� ����, ������ ������ ������� ���� ���, ����� � ���������� ALT+SPACE. #// 
								//# WS_EX_TOPMOST		����������, ��� ����, ��������� � ���� ������ ������ ���� ��������� ���� ����, �� ����� ������� ���� � ������ ������ ���� ��, ���� �����, ����� ���� ��������������. ����� �������� ��� ������� ���� �����, ����������� ������� SetWindowPos. #// 
		"NADO_SDELAT_CHTOBI_KLASSI_BILI_UNIKALNI",
		"test",//name.c_str(),
		WS_TILEDWINDOW, /* ���������� ����� ������������ ����. */
						// TODO ������� ��� ����� �� ���������� ������������� ����
						//# ���� �������� ����� ���� ����������� ������ ����, ���� ����� ������� ����������, ��������� ����: #//
						//# �����		�������������� #//
						//# WS_BORDER		������� ����, ������� ����� ������ ����� �����. #//
						//# WS_CAPTION		������� ����, ������� ����� ������ ��������� (�������� � ���� ����� WS_BORDER). #//
						//# WS_CHILD		������� �������� ����. ������������ � �������. #//
						//# WS_DISABLED		������� ����, ������� ������������� �������������. ��������������� ���� �� ����� ��������� �������� ���������� �� ������������. #//
						//# WS_DLGFRAME		������� ����, ������� ����� ����� �����, ������ ������������ � ����������� ������. ���� � ���� ������ �� ����� ����� ������ ���������. #//
						//# WS_HSCROLL		������� ����, ������� ����� �������������� ������� ���������. #//
						//# WS_MAXIMIZE		������� ����, ������� ������������� ����������. #//
						//# WS_MAXIMIZEBOX	������� ����, ������� ����� ������ ���������� (Maximize). �� ����� ���� ��������� �� ������ WS_EX_CONTEXTHELP. � ���� �� ������ ���� ��������� ����� WS_SYSMENU. #//
						//# WS_MINIMIZE		������� ����, ������� ������������� ��������. ��� �� ����� �����, ��� � WS_ICONIC. #//
						//# WS_MINIMIZEBOX	������� ����, ������� ����� ������ �������� (Minimize). �� ����� ���� ��������� �� ������ WS_EX_CONTEXTHELP. � ���� �� ������ ���� ��������� ����� WS_SYSMENU. #//
						//# WS_POPUP		������� ������������� ����. ���� ����� �� ����� �������������� �� ������ WS_CHILD. #//
						//# WS_SIZEBOX		������� ����, ������� ����� ��������� �������� ����� ����. ��� �� �����, ��� � ����� WS_THICKFRAME. #//
						//# WS_SYSMENU		������� ����, ������� ����� ���� ���� (window-menu) � ��� ������ ���������. � ���� �� ������ ���� ��������� ����� WS_CAPTION. #//
						//# WS_TABSTOP		���������� ������� ����������, ������� ����� ��������� ����� ����������, ����� ������������ �������� ������� ��������� (TAB). ������� �� ������� ��������� �������� ����� ���������� �� ��������� ������� ���������� �� ������ WS_TABSTOP. #//
						//# WS_THICKFRAME	������� ����, ������� ����� ��������� �������� ����� ����. �� �� �����, ��� � ����� WS_SIZEBOX. #//
						//# WS_TILED		������� ������������� ����. ������������� ���� ����� ������ ��������� � �����. �� �� �����, ��� � ����� WS_ OVERLAPPED. #//
						//# WS_TILEDWINDOW	������� ������������� ���� �� ������� WS_OVERLAPPED, WS_CAPTION, WS_SYSMENU, WS_THICKFRAME, WS_MINIMIZEBOX � WS_MAXIMIZEBOX. �� �� �����, ��� � ����� WS_ OVERLAPPEDWINDOW. #//
						//# WS_VISIBLE		������� ����, ������� �������� ������������� �������. ������������ � �������. #//
						//# WS_VSCROLL		������� ����, ������� ����� ������������ ������� ���������. #//
						//# WS_POPUP | WS_THICKFRAME	������� ����, ������� ������ ����� windows, ��� ���������. #//
		CW_USEDEFAULT, 0, 500, 500, GetConsoleWindow(), NULL, GetModuleHandle(NULL), NULL);

	//hdc_ = GetDC(hwnd_);

	//sizeSet(x,y);

	/* ����� ����. */
	UpdateWindow( global_hwnd );
	ShowWindow( global_hwnd, SW_SHOW );

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
	return 0;
}

/* ------------------------------------------------------------------------- */
/* ��������� ��������� ���������. */
LRESULT CALLBACK gwapi::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch ( msg ) {
		//case WM_ACTIVATE:{
			/* ���������� ����� ���� �������������� ��� ���������������� */
			//# ���� ��������� ���������� ��� ���������, ��� ������ ���������� ����. #//
				//# fActive = LOWORD(wParam);           // ������ ����������� #//
				//# fMinimized = (BOOL) HIWORD(wParam); // ������ ����������� #//
				//# hwndPrevious = (HWND) lParam;       // ���������� ���� #//
			//# fActive #//
			//# �������� �������� ����� ����� wParam. �������������, �������������� �� ���� ��� ��� ����������������. #// 
			//# ���� �������� ����� ���� ����� �� ��������� ��������: #//
				//# WA_ACTIVE - �������������� ��������� ������� ������, ��� ������ ����. #//
					//# (��������, ���������� � ������� SetActiveWindow ��� �������������� ���������� ���������� ��� ������ ����) #//
				//# WA_CLICKACTIVE - �������������� ������� ����. #//
				//# WA_INACTIVE - ����������������. #//
			//# fMinimized #//
				//# �������� �������� ����� ����� wParam. ������������� ��������� ��������� ����, ��������������� ��� �����������������. #// 
				//# ��������, ������������ �� ����, ���������, ��� ���� �������� (��������������). #//
			//# hwndPrevious #//
				//# �������� lParam. �������������� ����, �������������� ��� ����������������, � ����������� �� �������� ��������� fActive. #// 
				//# ���� �������� fActive - WA_INACTIVE, hwndPrevious - ���������� ��������������� ����. #// 
				//# ���� �������� fActive - WA_ACTIVE ��� WA_CLICKACTIVE, hwndPrevious - ���������� ����������������� ����. #// 
				//# ���� ���������� ����� ���� ����� (NULL). #//
			//# ----------------------------------------------------------------------- #//
			//# �������� �� ��������� #//
				//# ���� ���� �������������� � �� �������� (��������������), ������� DefWindowProc ������������� ����� ���������� � ����. #//
			return 0; 
		//};break;
		case WM_CLOSE:{
			/* ������, ����������, ��� ���������� ��������� ������ ��������� ���� ������. */
			//# ���� ���������� ������������ ��� ���������, ��� ������ ���������� ����. #//
			//# �������� �� ��������� #//
				//# ������� DefWindowProc ���������� � ������� DestroyWindow, ����� ���������� ����. #//
			//# ����������: #//
				//# ���������� ��������� ����� ��������� ������������ � �������������, �� ���������� ����, #// 
				//# � ���� ��������� ��������� WM_CLOSE � �������� ������� DestroyWindow ������ �����, ���� ������������ ������������ �����. #//
			DestroyWindow( hwnd );
			return 0;
		};break;case WM_CREATE:{
			/* ����������, ����� ������ ���� ������� ����. */
			//# ��������� ����� ����, ��� �������, �� ����� ���, ��� ����� �������. #//
			//# ���� ���������� ������������ ��� ���������, ��� ���������� 0, ����� ���������� �������� ����. #//
			//# ���� ���������� ��������� ���������� -1, �� ���� �����������, � ������� CreateWindow ���������� �������� ����������� ����� (NULL). #//
			return 0;
		};break;case WM_DESTROY:{
			/* ����� ���� �����������. */
			PostQuitMessage( 0 );
			return 0;
		};break;
		//case WM_GETMINMAXINFO:{
			/* ������ ������������ � ����������� ������� ����. */
			// TODO ������� ���������, ����� ����� ���� �������� ���� � ����������� ������� ����
			//MINMAXINFO *pInfo = (MINMAXINFO *)lParam;
			//pInfo->ptMinTrackSize = {100,	100	}; /* ���������� ����������� ������. */
			//pInfo->ptMaxTrackSize = {500,	500	}; /* ���������� ������������ ������. */
			//return 0;
		//};break;
		case WM_MOVE:{
			/* ��������� ���������� ��� ��������� ��������� ����. */
			// TODO ������� ����� ����� ���� �� ���� ������ ��������� �������
			//# ������������ �������� #//
			//# ���� ��������� ������������ ��� ���������, ��� ������ ���������� ����. #//
			int xPos = (int) LOWORD(lParam);    /* ������� �� ����������� */
			int yPos = (int) HIWORD(lParam);    /* ������� �� ��������� */
			return 0;
		};break;case WM_PAINT:{
			/* ��������� ����������, ����� ����� ���������� ������� �������. */
			static HDC hdc;
			static PAINTSTRUCT ps;
			
			hdc = BeginPaint( hwnd, &ps );
			/* ��������� �����. */
			EndPaint( hwnd, &ps );
			return 0;
		};break;
		//case WM_ERASEBKGND:{
			// TODO ������: ����� �� ���
			//# ������������ ������ ���� ��������� ���� ��������� ������������ ��� � ������. #//
			//return TRUE;
		//};break;
		case WM_SIZE:{
			/* ��������� ���������� ���� ����� ����, ��� ��� ������ ���������. */
			// TODO ������� ����� ����� ���� �� ���� ������ ��������� �������
			int fwSizeType = (int) wParam;
				//# ���������� ��� ������������ ��������� ��������. ���� �������� ����� ��������� ���� �� ��������� ��������: #//
				//# SIZE_MAXHIDE	��������� ���������� ���� ������������� �����, ����� ���������� ��������� ������ ����. #//
				//# SIZE_MAXIMIZED	���� ���� ����������. #//
				//# SIZE_MAXSHOW	��������� ���������� ���� ������������� �����, ����� ��������� ������ ���� ���� ������������� � ��� ������� ��������. #//
				//# SIZE_MINIMIZED	���� ���� ��������(��������������). #//
				//# SIZE_RESTORED	���� ���� ��������, �� �� ���� �������� SIZE_MINIMIZED �� SIZE_MAXIMIZED �� �����������. #// 
			int nWidth = LOWORD(lParam); 
				//# ������������� ����� ������ ������� �������. #//
			int nHeight = HIWORD(lParam); 
				//# ������������� ����� ������ ������� �������. #//
			return 0;
		};break;case WM_SIZING:{
			/* ��������� ���������� �� ����� ��������� �������� ����. */
			// TODO ������� ����� ����� ���� �� ���� ������ ��������� �������
			int fwSide = wParam;
				//# ���������, ������� ���� ���� �������������. ���� �������� ����� ���� ����������� ��������� ��������: #//
	
				//# WMSZ_BOTTOM		������ ���� #//
				//# WMSZ_BOTTOMLEFT	����� ������ ���� #//
				//# WMSZ_BOTTOMRIGHT	������ ������ ���� #//
				//# WMSZ_LEFT		����� ���� #//
				//# WMSZ_RIGHT		������ ���� #//
				//# WMSZ_TOP		������� ���� #//
				//# WMSZ_TOPLEFT	������� ����� ���� #//
				//# WMSZ_TOPRIGHT	����� ������ ���� #// 
			RECT* lprc = (LPRECT) lParam;
				//# ����� ��������� RECT � ��������� ������������ ���������������� ��������������. #// 
				//# ����� �������� ������ ��� ������� ���������������� ��������������, ���������� ��������� ������ �������� ����� ���� ���������. #//
				
			//# ������������ �������� #//
				//# ��������� ������ ���������� ������ (TRUE), ���� ��� ������������ ��� ���������. #//
			return TRUE;
		};break;case WM_COMMAND:{
			/* ��������� ������������ �����, ����� ������������ �������� ��������� ����� �� ���� ��� ���������� ������� �� ������ � �.�. */
			// TODO ������� ����� ����� ���� �� ���� ������ ��������� �������
			//switch ( LOWORD(wParam) ) {
				//case /* �����. */:{
					/* ���������. */
				//};break;case /* �����. */:{
					/* ���������. */
				//};break;
					/* ... */
			//};  
		};break;default:
			return DefWindowProc( hwnd, msg, wParam, lParam );
	};
}
/* ------------------------------------------------------------------------- */

gwapi::Window::Window(int x, int y, string name) : canvas(x+50, y+50) {
	/* �������� ������, ������� ������������ ��������� ������� ����. */
	hTickThread = CreateThread( NULL, 0, &tickThreadProc, NULL, 0, NULL);	

	Sleep(50);
	hwnd_ = global_hwnd;
	hdc_ = GetDC(hwnd_);

	sizeSet(x, y);
	current_.hdc_ = hdc_;
}

gwapi::Window::~Window() {
	SendMessage(hwnd_, WM_CLOSE, NULL, NULL);
	DeleteDC(hdc_);
}

void gwapi::Window::redraw(void) {
	canvas.drawTo(current_);
}

void gwapi::Window::sizeSet(int width, int height) {
	/* ��������� ������ ������ ���� � ������ �����. */
	RECT rcClient, rcWindow;
	POINT ptDiff;

	GetClientRect( hwnd_, &rcClient );
	GetWindowRect( hwnd_, &rcWindow );

	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;

	/* ���������� ������. */
	MoveWindow( hwnd_, rcWindow.left, rcWindow.top, width + ptDiff.x, height + ptDiff.y, false);
}

pair<int, int> gwapi::Window::sizeGet(void) {

	return pair<int, int>();
}

void gwapi::Window::captionSet(string) {
	// TODO �����������
}

void gwapi::Window::positionSet(Point) {
	// TODO �����������
}

void gwapi::Window::fullscreen(bool) {
	/* ~wapifullscreen - ����� ���������. */
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
	gwapi::Window a(500,500,"Test window, nigga :)"), b;
	
	a.canvas.penSet(gwapi::rgb(255,0,0), 3);
	a.canvas.clear();
	a.canvas.lineDraw(Point(100,100), Point(200,200));
	a.redraw();

	b.canvas.penSet(gwapi::rgb(0,0,255), 3);
	b.canvas.lineDraw(Point(100,200), Point(200,100));
	b.redraw();
	
	system("pause");

	a.canvas.penSet(gwapi::rgb(255,0,0), 3);
	a.canvas.lineDraw(Point(100,200), Point(200,100));
	a.redraw();

	system("pause");

	return 0;
}
#endif

/* ~wapitaskbar - �� ��� ������ � ���������. */

/* ~wapithread - �������� ������. */

/* ~wapiscrollbar - �������� ����������. */