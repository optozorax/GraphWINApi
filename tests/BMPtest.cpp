#include "../window.h"
#include <stdlib.h>

int main() {
	gwapi::Window wnd;

	gwapi::Bufer buf(200, 100);
	buf.penSet(gwapi::Miku, 10);
	buf.lineDraw(Point(0, 0), Point(200, 50));
	buf.textOut(Point(100, 50), "Hello, world!!!", gwapi::TextWriteStyle::Center);
	buf.writeToBMP("bmp.bmp", false);

	wnd.canvas.readFromBMP("antiprism.bmp");

	wnd.redraw();

	system("pause");
	return 0;
}