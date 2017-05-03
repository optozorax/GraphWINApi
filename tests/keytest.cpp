#include "../window.h"

int main() {
	gwapi::WindowType wndtype;
	wndtype.size = Point(490, 490);
	wndtype.maxSize = wndtype.size;
	wndtype.minSize = wndtype.size;
	wndtype.caption = "Keys Test";
	wndtype.style = gwapi::WindowType::Tool;
	gwapi::Window wnd(wndtype);

	wnd.canvas.penSet(gwapi::White);
	wnd.canvas.textStyle(gwapi::StyleText(12));

	wnd.fullscreen(true);

	while (true) {
		wnd.canvas.clear(gwapi::Bitcoin);
		wnd.canvas.penSet(gwapi::White);
		for (int i = 0; i < 256; i++) {
			if (wnd.isKeyDown(i))
				wnd.canvas.brushSet(gwapi::Ubuntu);
			else
				wnd.canvas.brushSet(gwapi::Bitcoin);
			wnd.canvas.rectDraw(Point(i%16, i/16)*30 + Point(2,2) + Point(5,5), 
				Point(i%16+1, i/16+1)*30 - Point(2,2) + Point(5,5));
			wnd.canvas.textOut(Point(i%16, i/16)*30 + Point(15, 15) + Point(5,5), 
				std::to_string(i), gwapi::TextWriteStyle::Center);
		}
		wnd.redraw();
		wnd.sleep(100);

		#define func(A) if (wnd.isKeyDown(A)) printf(#A "\n");
		func(VK_LBUTTON)
		func(VK_RBUTTON)
		func(VK_CANCEL)
		func(VK_MBUTTON)
		func(VK_XBUTTON1)
		func(VK_XBUTTON2)
		func(VK_BACK)
		func(VK_TAB)
		func(VK_CLEAR)
		func(VK_RETURN)
		func(VK_SHIFT)
		func(VK_CONTROL)
		func(VK_MENU)
		func(VK_PAUSE)
		func(VK_CAPITAL)
		func(VK_KANA)
		func(VK_HANGEUL)
		func(VK_HANGUL)
		func(VK_JUNJA)
		func(VK_FINAL)
		func(VK_HANJA)
		func(VK_KANJI)
		func(VK_ESCAPE)
		func(VK_CONVERT)
		func(VK_NONCONVERT)
		func(VK_ACCEPT)
		func(VK_MODECHANGE)
		func(VK_SPACE)
		func(VK_PRIOR)
		func(VK_NEXT)
		func(VK_END)
		func(VK_HOME)
		func(VK_LEFT)
		func(VK_UP)
		func(VK_RIGHT)
		func(VK_DOWN)
		func(VK_SELECT)
		func(VK_PRINT)
		func(VK_EXECUTE)
		func(VK_SNAPSHOT)
		func(VK_INSERT)
		func(VK_DELETE)
		func(VK_HELP)
		func(VK_LWIN)
		func(VK_RWIN)
		func(VK_APPS)
		func(VK_SLEEP)
		func(VK_NUMPAD0)
		func(VK_NUMPAD1)
		func(VK_NUMPAD2)
		func(VK_NUMPAD3)
		func(VK_NUMPAD4)
		func(VK_NUMPAD5)
		func(VK_NUMPAD6)
		func(VK_NUMPAD7)
		func(VK_NUMPAD8)
		func(VK_NUMPAD9)
		func(VK_MULTIPLY)
		func(VK_ADD)
		func(VK_SEPARATOR)
		func(VK_SUBTRACT)
		func(VK_DECIMAL)
		func(VK_DIVIDE)
		func(VK_F1)
		func(VK_F2)
		func(VK_F3)
		func(VK_F4)
		func(VK_F5)
		func(VK_F6)
		func(VK_F7)
		func(VK_F8)
		func(VK_F9)
		func(VK_F10)
		func(VK_F11)
		func(VK_F12)
		func(VK_F13)
		func(VK_F14)
		func(VK_F15)
		func(VK_F16)
		func(VK_F17)
		func(VK_F18)
		func(VK_F19)
		func(VK_F20)
		func(VK_F21)
		func(VK_F22)
		func(VK_F23)
		func(VK_F24)
		func(VK_NUMLOCK)
		func(VK_SCROLL)
		func(VK_LSHIFT)
		func(VK_RSHIFT)
		func(VK_LCONTROL)
		func(VK_RCONTROL)
		func(VK_LMENU)
		func(VK_RMENU)
		func(VK_BROWSER_BACK)
		func(VK_BROWSER_FORWARD)
		func(VK_BROWSER_REFRESH)
		func(VK_BROWSER_STOP)
		func(VK_BROWSER_SEARCH)
		func(VK_BROWSER_FAVORITES)
		func(VK_BROWSER_HOME)
		func(VK_VOLUME_MUTE)
		func(VK_VOLUME_DOWN)
		func(VK_VOLUME_UP)
		func(VK_MEDIA_NEXT_TRACK)
		func(VK_MEDIA_PREV_TRACK)
		func(VK_MEDIA_STOP)
		func(VK_MEDIA_PLAY_PAUSE)
		func(VK_LAUNCH_MAIL)
		func(VK_LAUNCH_MEDIA_SELECT)
		func(VK_LAUNCH_APP1)
		func(VK_LAUNCH_APP2)
		func(VK_OEM_1)
		func(VK_OEM_PLUS)
		func(VK_OEM_COMMA)
		func(VK_OEM_MINUS)
		func(VK_OEM_PERIOD)
		func(VK_OEM_2)
		func(VK_OEM_3)
		func(VK_OEM_4)
		func(VK_OEM_5)
		func(VK_OEM_6)
		func(VK_OEM_7)
		func(VK_OEM_8)
		func(VK_OEM_102)
		func(VK_PROCESSKEY)
		func(VK_PACKET)
		func(VK_ATTN)
		func(VK_CRSEL)
		func(VK_EXSEL)
		func(VK_EREOF)
		func(VK_PLAY)
		func(VK_ZOOM)
		func(VK_NONAME)
		func(VK_PA1)
		func(VK_OEM_CLEAR)
	}
}