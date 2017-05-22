#include "../window.h"
#include <stdlib.h>

int main() {
	wgs::window wnd;

	wgs::bufer buf(200, 100);
	buf.penSet(wgs::Miku, 10);
	buf.draw_line(Point(0, 0), Point(200, 50));
	buf.textOut(Point(100, 50), "Hello, world!!!", wgs::text_write_style::Center);
	buf.write_in_bmp("bmp.bmp", false);

	wnd.canvas.read_from_bmp("antiprism.bmp");

	wnd.redraw();

	system("pause");
	return 0;
}