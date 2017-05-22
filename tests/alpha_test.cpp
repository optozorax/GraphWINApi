#include "../window.h"
#include <stdio.h>

DWORD CurrentProcessTick(void) {
	/* ѕолучение количества тиков с периодом 100 нс, которые были совершены во врем€ работы процесса. */
	FILETIME a1, a2, a3, a4;
	GetProcessTimes(GetCurrentProcess(), &a1, &a2, &a3, &a4);
	return a4.dwLowDateTime;
}

int main() {
	wgs::window a;

	//a.canvas.penSet(wgs::argb(255, 9, 237, 195), 10);
	a.canvas.penSet(wgs::Miku, 5);

	DWORD current, time = 0, n = 1;
	double sr = 0;
	//while (true) {
		current = wgs::window::timeGet();
		int i = 0;
		//for (i = 0; i < 3000; i++) {
			a.canvas.clear();
			a.canvas.draw_line(Point(20 + 50, 60.0 + i/100.0), Point(50 + 50, 66.0 + i/100.0));
			a.canvas.draw_line(Point(50 + 50, 66.0 + i/100.0), Point(10 + 50, 40.0 + i/100.0));
			a.canvas.draw_line(Point(10 + 50, 40.0 + i/100.0), Point(20 + 50, 60.0 + i/100.0));

			a.canvas.draw_line(point2(10, 30), point2(100, 50));

			a.canvas.draw_line(point2(20, 60.0 + i/100.0), point2(50, 66.0 + i/100.0));
			a.canvas.draw_line(point2(50, 66.0 + i/100.0), point2(10, 40.0 + i/100.0));
			a.canvas.draw_line(point2(10, 40.0 + i/100.0), point2(20, 60.0 + i/100.0));

			a.canvas.draw_line(point2(10, 10), point2(100, 10));
			a.canvas.draw_line(point2(10, 10), point2(200, 200));

			a.canvas.textOut(Point(0,0), std::to_string((double) sr/n));
			a.redraw();
			wgs::window::sleep(10);
		//}
		time = wgs::window::timeGet()-current;
		sr += time/1000.0;
		n++;
	//}

	a.canvas.brushSet(wgs::argb(128, 255, 0, 128));

	std::vector<point2> mas;
	mas.push_back(point2(10, 10));
	mas.push_back(point2(100, -30));
	mas.push_back(point2(50, 100));
	mas.push_back(point2(-50, 50));
	a.canvas.draw_poly(mas);

	a.canvas.draw_ellipse(point2(100, 100), point2(10,100));

	a.canvas.draw_line(Point(20, 35), Point(100, 36));
	a.redraw();

	system("pause");
	return 0;
}