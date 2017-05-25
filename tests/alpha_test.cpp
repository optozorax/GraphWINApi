#include "../window.h"

int main() {
	wgs::window_type wndt;
	wndt.style = wgs::window_type::Tool;
	wndt.size = Point(130, 130);
	wndt.caption = "Double coordinates test";
	wgs::window a(wndt);

	a.canvas.penSet(wgs::Miku, 5);

	double sr = 0;
	while (true) {
		int i = 0;
		for (i = -3000; i < 3000; i++) {
			a.canvas.clear();
			a.canvas.draw_line(Point(20 + 50, 60.0 + i/100.0), Point(50 + 50, 66.0 + i/100.0));
			a.canvas.draw_line(Point(50 + 50, 66.0 + i/100.0), Point(10 + 50, 40.0 + i/100.0));
			a.canvas.draw_line(Point(10 + 50, 40.0 + i/100.0), Point(20 + 50, 60.0 + i/100.0));

			a.canvas.draw_line(point2(20, 60.0 + i/100.0), point2(50, 66.0 + i/100.0));
			a.canvas.draw_line(point2(50, 66.0 + i/100.0), point2(10, 40.0 + i/100.0));
			a.canvas.draw_line(point2(10, 40.0 + i/100.0), point2(20, 60.0 + i/100.0));

			a.redraw();
			wgs::window::sleep(10);
		}
	}

	return 0;
}