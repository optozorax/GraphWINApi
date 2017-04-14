#include "../window.h"
#include <stdio.h>

DWORD CurrentProcessTick(void) {
	/* ѕолучение количества тиков с периодом 100 нс, которые были совершены во врем€ работы процесса. */
	FILETIME a1, a2, a3, a4;
	GetProcessTimes(GetCurrentProcess(), &a1, &a2, &a3, &a4);
	return a4.dwLowDateTime;
}

int main() {
	gwapi::Window a;

	//a.canvas.penSet(gwapi::argb(255, 9, 237, 195), 10);
	a.canvas.penSet(gwapi::Black, 5);

	DWORD current, time = 0, n = 0;
	double sr = 0;
	while (true) {
		current = CurrentProcessTick();
		for (int i = 0; i < 3000; i++) {
			a.canvas.clear();
			a.canvas.lineDraw(Point(20 + 50, 60.0 + i/100.0), Point(50 + 50, 66.0 + i/100.0));
			a.canvas.lineDraw(Point(50 + 50, 66.0 + i/100.0), Point(10 + 50, 40.0 + i/100.0));
			a.canvas.lineDraw(Point(10 + 50, 40.0 + i/100.0), Point(20 + 50, 60.0 + i/100.0));

			a.canvas.lineDraw(point2(10, 30), point2(100, 50));

			a.canvas.lineDraw(point2(20, 60.0 + i/100.0), point2(50, 66.0 + i/100.0));
			a.canvas.lineDraw(point2(50, 66.0 + i/100.0), point2(10, 40.0 + i/100.0));
			a.canvas.lineDraw(point2(10, 40.0 + i/100.0), point2(20, 60.0 + i/100.0));

			a.canvas.lineDraw(point2(10, 10), point2(100, 10));
			a.canvas.lineDraw(point2(10, 10), point2(200, 200));
			a.canvas.textOut(Point(0,0), std::to_string((double) sr/n));
			a.redraw();
		}
		time = CurrentProcessTick()-current;
		sr += time/10000000.0;
		n++;
	}
	a.canvas.penSet(gwapi::Ubuntu, 2);

	a.canvas.penSet(gwapi::Miku, 2);
	a.canvas.lineDraw(Point(20, 35), Point(100, 36));
	a.redraw();

	system("pause");
	return 0;
}