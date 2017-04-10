#include "../window.h"
#include <stdio.h>

int main() {
	gwapi::Window a;

	a.canvas.penSet(gwapi::Miku, 2);
	a.canvas.lineDraw(point2(-20, -20), point2(1000, 1000));

	Sleep(500);

	while (true) {
		for (int i = 0; i < 3000; i++) {
			a.canvas.clear();
			a.canvas.lineDraw(point2(20, 60.0 + i/100.0), point2(50, 66.0 + i/100.0));
			a.canvas.lineDraw(point2(50, 66.0 + i/100.0), point2(10, 40.0 + i/100.0));
			a.canvas.lineDraw(point2(10, 40.0 + i/100.0), point2(20, 60.0 + i/100.0));

			a.canvas.lineDraw(Point(20 + 50, 60.0 + i/100.0), Point(50 + 50, 66.0 + i/100.0));
			a.canvas.lineDraw(Point(50 + 50, 66.0 + i/100.0), Point(10 + 50, 40.0 + i/100.0));
			a.canvas.lineDraw(Point(10 + 50, 40.0 + i/100.0), Point(20 + 50, 60.0 + i/100.0));
			a.redraw();
			Sleep(3);
		}
	}
	a.canvas.penSet(gwapi::Ubuntu, 2);

	a.canvas.penSet(gwapi::Miku, 2);
	a.canvas.lineDraw(Point(20, 35), Point(100, 36));
	a.redraw();

	system("pause");
	return 0;
}