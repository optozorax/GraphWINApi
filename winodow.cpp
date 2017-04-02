#include "window.h"

HWND global_hwnd = 0;
DWORD WINAPI gwapi::windowMainThread(LPVOID a) {
	/* �������� ��������� ����. */
	WNDCLASSEX wc;
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC | CS_SAVEBITS; /* ������������� ����� ������. */
					// TODO ��������� ��� ��� ����
					//# CS_DROPSHADOW	������� ������ �������� ���� �� ����. ��� �������, �� ���������� ��� ���������, ��������� ���� ���� ����. #//
					//# ����� ������������ ��� ���� �� ������ WS_POPUP. #//
	wc.lpfnWndProc = gwapi::currentWndProc;
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
		CW_USEDEFAULT, 0, 500, 500, NULL, NULL, GetModuleHandle(NULL), NULL);

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

LRESULT CALLBACK gwapi::currentWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	/* ��������� �� ������, ������� ��������� � �������� ����. */
	Window* this1 = Stack_[hwnd];

	switch ( msg ) {
		case WM_ACTIVATE:{
			/* ���������� ����� ���� �������������� ��� ���������������� */
			// TODO ������ ���� �� ���, � ���� �� ���������� ���� ��� ����������� �������(������������� ����� ����� � ����)
			bool fActive = !(LOWORD(wParam) == WA_INACTIVE);
			bool fMinimized = (BOOL) HIWORD(wParam);
			
			if (this1 != 0)
				(*(this1->funcActivate))(fActive, fMinimized);

			return 0;
		};break;
		case WM_CLOSE:{
			/* ������, ����������, ��� ���������� ��������� ������ ��������� ���� ������. */
			if (this1 != 0)
				(*(this1->funcDestroy))();

			DestroyWindow( hwnd );
			return 0;
		};break;case WM_CREATE:{
			/* ����������, ����� ������ ���� ������� ����. */
			//# ��������� ����� ����, ��� �������, �� ����� ���, ��� ����� �������. #//
			
			// ����� ������ �������
			return 0;
		};break;case WM_DESTROY:{
			/* ����� ���� �����������. */
			PostQuitMessage( 0 );
			return 0;
		};break;
		case WM_GETMINMAXINFO:{
			/* ������ ������������ � ����������� ������� ����. */
			MINMAXINFO *pInfo = (MINMAXINFO *)lParam;

			if (this1 != 0) {
				pInfo->ptMinTrackSize = { this1->MinSize[0], this1->MinSize[1] };
				pInfo->ptMaxTrackSize = { this1->MaxSize[0], this1->MaxSize[1] };
			}

			return 0;
		};break;
		case WM_MOVE:{
			/* ��������� ���������� ��� ��������� ��������� ����. */
			int xPos = (int) LOWORD(lParam);
			int yPos = (int) HIWORD(lParam);

			if (this1 != 0)
				(*(this1->funcMove))(xPos, yPos);

			return 0;
		};break;case WM_PAINT:{
			/* ��������� ����������, ����� ����� ���������� ������� �������. */
			static HDC hdc;
			static PAINTSTRUCT ps;
			hdc = BeginPaint( hwnd, &ps );
			
			Bufer a_(hdc);
			if (this1 != 0)
				this1->canvas.drawTo(a_);

			EndPaint( hwnd, &ps );
			return 0;
		};break;case WM_SIZE:{
			/* ��������� ���������� ���� ����� ����, ��� ��� ������ ���������. */
			int fwSizeType = (int) wParam;
				//# ���������� ��� ������������ ��������� ��������. ���� �������� ����� ��������� ���� �� ��������� ��������: #//
				//# SIZE_MAXIMIZED	���� ���� ����������. #//
				//# SIZE_MINIMIZED	���� ���� ��������(��������������). #//
			int nWidth = LOWORD(lParam); 
			int nHeight = HIWORD(lParam); 

			if (fwSizeType != SIZE_MINIMIZED) {
				if (this1 != 0)
					(*(this1->funcSized))(nWidth, nHeight);
			}
			return 0;
		};break;case WM_SIZING:{
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
				
			if (this1 != 0)
				(*(this1->funcSizing))(lprc->left, lprc->top, lprc->right, lprc->bottom, szType);
			return TRUE;
		};break;
		//case WM_COMMAND:{
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
		//};break;
		default:
			return DefWindowProc( hwnd, msg, wParam, lParam );
	};
}
/* ------------------------------------------------------------------------- */

gwapi::Window::Window(int x, int y, string name) : 
	canvas(x+50, y+50), 
	MinSize(0,0), 
	MaxSize(10000,10000), 
	funcSized(&NOPE1), 
	funcSizing(&NOPE2), 
	funcMove(&NOPE1), 
	funcActivate(&NOPE3),
	funcDestroy(&NOPE4)
{
	/* �������� ������, ������� ������������ ��������� ������� ����. */
	CreateThread( NULL, 0, &windowMainThread, NULL, 0, NULL);	

	Sleep(50);
	hwnd_ = global_hwnd;
	Stack_[hwnd_] = this;
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

	b.canvas.penSet(gwapi::argb(128,0,0,255), 3);
	b.canvas.brushSet(gwapi::argb(128,0,0,255));
	b.canvas.lineDraw(Point(100,200), Point(200,100));
	b.canvas.rectDraw(Point(10,10), Point(200,150));
	b.redraw();
	
	system("pause");

	a.canvas.penSet(gwapi::rgb(255,0,0), 3);
	a.canvas.lineDraw(Point(100,200), Point(200,100));
	b.canvas.rectDraw(Point(130,170), Point(200,100));
	b.canvas.drawAlphaTo(a.canvas, 100);

	a.redraw();
	b.redraw();

	system("pause");

	return 0;
}
#endif

/* ~wapitaskbar - �� ��� ������ � ���������. */

/* ~wapithread - �������� ������. */

/* ~wapiscrollbar - �������� ����������. */