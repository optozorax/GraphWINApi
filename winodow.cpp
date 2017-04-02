#include "window.h"

HWND global_hwnd = 0;
DWORD WINAPI gwapi::tickThreadProc(LPVOID a) {
	/* Создание структуры окна. */
	WNDCLASSEX wc;
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC | CS_SAVEBITS; /* Устанавливает стиль класса. */
					// TODO проверить что это дает
					//# CS_DROPSHADOW	Создает эффект падающей тени на окно. Как правило, он включается для маленьких, временных окон типа меню. #//
					//# Можно использовать для окон со стилем WS_POPUP. #//
	wc.lpfnWndProc = gwapi::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));
	wc.lpszMenuName = NULL;
	// TODO сделать чтобы классы для каждого окна были уникальны
	wc.lpszClassName = "NADO_SDELAT_CHTOBI_KLASSI_BILI_UNIKALNI";
	wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
	/* Регистрация окна и обработка ошибки. */
	/* Функция регистрирует класс окна для последующего использования при вызове функции CreateWindow или CreateWindowEx. */
	eshe:
	if ( !RegisterClassEx(&wc) ) {
		//MessageBox( NULL, "Failed to register window class.", "Error", MB_OK );
		wc.lpszClassName = "KAK_TO_TAK";
		goto eshe;
	}

	global_hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW, /* Определяет расширенный стиль создаваемого окна. */
								//# WS_EX_TOOLWINDOW		Создает окно инструментальных средств; то есть окно предполагается использовать как плавающую инструментальную панель. Окно инструментальных средств имеет область заголовка, которая короче, чем нормальная строка заголовка, а заголовок окна выводится, с использованием меньшего шрифта. Окно инструментальных средств не появляется в панели задач или в диалоговом окне, которое появляется тогда, когда пользователь нажимает ALT+TAB. Если окно инструментальных средств имеет системное меню, его пиктограмма не отображается в заголовке. Однако, Вы можете показывать на экране системное меню, щелкая правой кнопкой мыши или, вводя с клавиатуры ALT+SPACE. #// 
								//# WS_EX_TOPMOST		Определяет, что окно, созданное с этим стилем должно быть размещено выше всех, не самых верхних окон и должно стоять выше их, даже тогда, когда окно деактивировано. Чтобы добавить или удалить этот стиль, используйте функцию SetWindowPos. #// 
		"NADO_SDELAT_CHTOBI_KLASSI_BILI_UNIKALNI",
		"test",//name.c_str(),
		WS_TILEDWINDOW, /* Определяет стиль создаваемого окна. */
						// TODO разбить эти стили на конкретные представления окна
						//# Этот параметр может быть комбинацией стилей окна, плюс стили органов управления, указанных ниже: #//
						//# Стиль		Предназначение #//
						//# WS_BORDER		Создает окно, которое имеет тонкую линию рамки. #//
						//# WS_CAPTION		Создает окно, которое имеет строку заголовка (включает в себя стиль WS_BORDER). #//
						//# WS_CHILD		Создает дочернее окно. Использовать в кнопках. #//
						//# WS_DISABLED		Создает окно, которое первоначально заблокировано. Заблокированное окно не может принимать вводимую информацию от пользователя. #//
						//# WS_DLGFRAME		Создает окно, которое имеет стиль рамки, обычно используемый с диалоговыми окнами. Окно с этим стилем не может иметь строку заголовка. #//
						//# WS_HSCROLL		Создает окно, которое имеет горизонтальную линейку прокрутки. #//
						//# WS_MAXIMIZE		Создает окно, которое первоначально развернуто. #//
						//# WS_MAXIMIZEBOX	Создает окно, которое имеет кнопку Развернуть (Maximize). Не может быть объединен со стилем WS_EX_CONTEXTHELP. К тому же должен быть определен стиль WS_SYSMENU. #//
						//# WS_MINIMIZE		Создает окно, которое первоначально свернуто. Тот же самый стиль, что и WS_ICONIC. #//
						//# WS_MINIMIZEBOX	Создает окно, которое имеет кнопку Свернуть (Minimize). Не может быть объединен со стилем WS_EX_CONTEXTHELP. К тому же должен быть определен стиль WS_SYSMENU. #//
						//# WS_POPUP		Создает выскакивающее окно. Этот стиль не может использоваться со стилем WS_CHILD. #//
						//# WS_SIZEBOX		Создает окно, которое имеет установку размеров рамки окна. Тот же самое, что и стиль WS_THICKFRAME. #//
						//# WS_SYSMENU		Создает окно, которое имеет меню окна (window-menu) в его строке заголовка. К тому же должен быть определен стиль WS_CAPTION. #//
						//# WS_TABSTOP		Определяет элемент управления, который может принимать фокус клавиатуры, когда пользователь нажимает клавишу ТАБУЛЯЦИИ (TAB). Нажатие на клавиши табуляции передает фокус клавиатуры на следующий элемент управления со стилем WS_TABSTOP. #//
						//# WS_THICKFRAME	Создает окно, которое имеет установку размеров рамки окна. То же самое, что и стиль WS_SIZEBOX. #//
						//# WS_TILED		Создает перекрывающее окно. Перекрывающее окно имеет строку заголовка и рамку. То же самое, что и стиль WS_ OVERLAPPED. #//
						//# WS_TILEDWINDOW	Создает перекрывающее окно со стилями WS_OVERLAPPED, WS_CAPTION, WS_SYSMENU, WS_THICKFRAME, WS_MINIMIZEBOX и WS_MAXIMIZEBOX. То же самое, что и стиль WS_ OVERLAPPEDWINDOW. #//
						//# WS_VISIBLE		Создает окно, которое является первоначально видимым. Использовать в кнопках. #//
						//# WS_VSCROLL		Создает окно, которое имеет вертикальную линейку прокрутки. #//
						//# WS_POPUP | WS_THICKFRAME	Создает окно, имеющее только рамку windows, без заголовка. #//
		CW_USEDEFAULT, 0, 500, 500, GetConsoleWindow(), NULL, GetModuleHandle(NULL), NULL);

	//hdc_ = GetDC(hwnd_);

	//sizeSet(x,y);

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

/* ------------------------------------------------------------------------- */
/* Процедура обработки сообщений. */
LRESULT CALLBACK gwapi::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch ( msg ) {
		//case WM_ACTIVATE:{
			/* Посылается когда окно активизируется или деактивизируется */
			//# Если программа обработала это сообщение, она должна возвратить нуль. #//
				//# fActive = LOWORD(wParam);           // флажок активизации #//
				//# fMinimized = (BOOL) HIWORD(wParam); // флажок минимизации #//
				//# hwndPrevious = (HWND) lParam;       // дескриптор окна #//
			//# fActive #//
			//# Значение младшего байта слова wParam. Устанавливает, активизируется ли окно или оно деактивизируется. #// 
			//# Этот параметр может быть одним из следующих значений: #//
				//# WA_ACTIVE - Активизировано некоторым методом другим, чем щелчок мыши. #//
					//# (например, обращением к функции SetActiveWindow или использованием интерфейса клавиатуры для выбора окна) #//
				//# WA_CLICKACTIVE - Активизировано щелчком мыши. #//
				//# WA_INACTIVE - Деактивизировано. #//
			//# fMinimized #//
				//# Значение старшего байта слова wParam. Устанавливает свернутое состояние окна, активизируемого или деактивизируемого. #// 
				//# Значение, отличающееся от нуля, указывает, что окно свернуто (минимизировано). #//
			//# hwndPrevious #//
				//# Значение lParam. Идентифицирует окно, активизируемое или деактивизируемое, в зависимости от значения параметра fActive. #// 
				//# Если значение fActive - WA_INACTIVE, hwndPrevious - дескриптор активизируемого окна. #// 
				//# Если значение fActive - WA_ACTIVE или WA_CLICKACTIVE, hwndPrevious - дескриптор деактивизируемого окна. #// 
				//# Этот дескриптор может быть ПУСТО (NULL). #//
			//# ----------------------------------------------------------------------- #//
			//# Действие по умолчанию #//
				//# Если окно активизируется и не свернуто (минимизировано), функция DefWindowProc устанавливает фокус клавиатуры в окно. #//
			return 0; 
		//};break;
		case WM_CLOSE:{
			/* Сигнал, означающий, что прикладная программа должна завершить свою работу. */
			//# Если приложение обрабатывает это сообщение, оно должно возвратить ноль. #//
			//# Действие по умолчанию #//
				//# Функция DefWindowProc обращается к функции DestroyWindow, чтобы уничтожить окно. #//
			//# Примечания: #//
				//# Прикладная программа может запросить пользователя о подтверждении, до разрушения окна, #// 
				//# в ходе обработки сообщение WM_CLOSE и вызывает функцию DestroyWindow только тогда, если пользователь подтверждает выбор. #//
			DestroyWindow( hwnd );
			return 0;
		};break;case WM_CREATE:{
			/* Вызывается, когда должно быть создано окно. */
			//# Принимает после того, как создано, но перед тем, как стать видимым. #//
			//# Если приложение обрабатывает это сообщение, оно возвращает 0, чтобы продолжить создание окна. #//
			//# Если прикладная программа возвращает -1, то окно разрушается, и функция CreateWindow возвращает значение дескриптора ПУСТО (NULL). #//
			return 0;
		};break;case WM_DESTROY:{
			/* Когда окно разрушается. */
			PostQuitMessage( 0 );
			return 0;
		};break;
		//case WM_GETMINMAXINFO:{
			/* Выдает максимальные и минимальные размеры окна. */
			// TODO сделать процедуру, чтобы можно было задавать макс и минимальные размеры окна
			//MINMAXINFO *pInfo = (MINMAXINFO *)lParam;
			//pInfo->ptMinTrackSize = {100,	100	}; /* Установили минимальный размер. */
			//pInfo->ptMaxTrackSize = {500,	500	}; /* Установили максимальный размер. */
			//return 0;
		//};break;
		case WM_MOVE:{
			/* Сообщение посылается при изменении положения окна. */
			// TODO сделать чтобы можно было на этот случай назначить функцию
			//# Возвращаемые значения #//
			//# Если программа обрабатывает это сообщение, она должна возвратить ноль. #//
			int xPos = (int) LOWORD(lParam);    /* позиция по горизонтали */
			int yPos = (int) HIWORD(lParam);    /* позиция по вертикали */
			return 0;
		};break;case WM_PAINT:{
			/* Сообщение посылается, когда нужно отрисовать рабочую область. */
			static HDC hdc;
			static PAINTSTRUCT ps;
			
			hdc = BeginPaint( hwnd, &ps );
			/* Рисование здесь. */
			EndPaint( hwnd, &ps );
			return 0;
		};break;
		//case WM_ERASEBKGND:{
			// TODO понять: нужно ли это
			//# Использовать только если программа сама полностью отрисовывает фон и прочее. #//
			//return TRUE;
		//};break;
		case WM_SIZE:{
			/* Сообщение посылается окну после того, как его размер изменился. */
			// TODO сделать чтобы можно было на этот случай назначить функцию
			int fwSizeType = (int) wParam;
				//# Определяет тип запрошенного изменения размеров. Этот параметр может принимать одно из следующих значений: #//
				//# SIZE_MAXHIDE	Сообщение посылается всем выскакивающим окнам, когда развернуто некоторое другое окно. #//
				//# SIZE_MAXIMIZED	Окно было развернуто. #//
				//# SIZE_MAXSHOW	Сообщение посылается всем выскакивающим окнам, когда некоторое другое окно было восстановлено в его прежних размерах. #//
				//# SIZE_MINIMIZED	Окно было свернуто(минимизировано). #//
				//# SIZE_RESTORED	Окно было изменено, но ни одно значение SIZE_MINIMIZED ни SIZE_MAXIMIZED не применяется. #// 
			int nWidth = LOWORD(lParam); 
				//# Устанавливает новую ширину рабочей области. #//
			int nHeight = HIWORD(lParam); 
				//# Устанавливает новую высоту рабочей области. #//
			return 0;
		};break;case WM_SIZING:{
			/* Сообщение посылается во время изменении размеров окна. */
			// TODO сделать чтобы можно было на этот случай назначить функцию
			int fwSide = wParam;
				//# Указывает, который край окна передвигается. Этот параметр может быть комбинацией следующих значений: #//
	
				//# WMSZ_BOTTOM		Нижний край #//
				//# WMSZ_BOTTOMLEFT	Левый нижний угол #//
				//# WMSZ_BOTTOMRIGHT	Правый нижний угол #//
				//# WMSZ_LEFT		Левый край #//
				//# WMSZ_RIGHT		Правый край #//
				//# WMSZ_TOP		Верхний край #//
				//# WMSZ_TOPLEFT	Верхний левый угол #//
				//# WMSZ_TOPRIGHT	Левый правый угол #// 
			RECT* lprc = (LPRECT) lParam;
				//# Адрес структуры RECT с экранными координатами перетаскиваемого прямоугольника. #// 
				//# Чтобы изменять размер или позицию перетаскиваемого прямоугольника, прикладная программа должна изменять члены этой структуры. #//
				
			//# Возвращаемое значение #//
				//# Программа должна возвратить ИСТИНУ (TRUE), если она обрабатывает это сообщение. #//
			return TRUE;
		};break;case WM_COMMAND:{
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
		};break;default:
			return DefWindowProc( hwnd, msg, wParam, lParam );
	};
}
/* ------------------------------------------------------------------------- */

gwapi::Window::Window(int x, int y, string name) : canvas(x+50, y+50) {
	/* Создание потока, который обрабатывает сообщения данного окна. */
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
	/* Процедура задает размер окна с учетом рамки. */
	RECT rcClient, rcWindow;
	POINT ptDiff;

	GetClientRect( hwnd_, &rcClient );
	GetWindowRect( hwnd_, &rcWindow );

	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;

	/* Изменяется размер. */
	MoveWindow( hwnd_, rcWindow.left, rcWindow.top, width + ptDiff.x, height + ptDiff.y, false);
}

pair<int, int> gwapi::Window::sizeGet(void) {

	return pair<int, int>();
}

void gwapi::Window::captionSet(string) {
	// TODO реализовать
}

void gwapi::Window::positionSet(Point) {
	// TODO реализовать
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

/* ~wapitaskbar - всё для работы с таскбаром. */

/* ~wapithread - создание потока. */

/* ~wapiscrollbar - создание скроллбара. */