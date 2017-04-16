#include "../window.h"
#include <stdio.h>

/* Это сравнение целого рассчета прозрачности линии с вещественным. */

DWORD CurrentProcessTick(void) {
	/* Получение количества тиков с периодом 100 нс, которые были совершены во время работы процесса. */
	FILETIME a1, a2, a3, a4;
	GetProcessTimes(GetCurrentProcess(), &a1, &a2, &a3, &a4);
	return a4.dwLowDateTime;
}

inline void floatR(gwapi::Window &wnd, double a, double b, double c) {
	double alpha, sum, r;
	int x,y,i,j;
	gwapi::Color newc;
	point2 y1;
	for (int k = 0; k < 30; k++){
		for (x = 0; x < 300; x++) {
			for (y = 0; y < 300; y++) {
				sum = 0;
				for (i = 0; i <= 15; i++) {
					for (j = 0; j <= 15; j++) {
						y1 = point2(x + i/15.0, y + j/15.0);
						r = fabs(a*y1.x + b*y1.y + c)/sqrt(a*a + b*b);
						if (r<=5) sum++;
					}
				}
				alpha = sum/256;

				newc = gwapi::Black;
				newc.m[3] = newc.m[3]*alpha;
				if (newc.m[3] == 0) newc.m[3] = 1;
				wnd.canvas[Point(x, y)] = gwapi::overlay(newc, wnd.canvas[Point(x, y)]).clrref;
			}
		}
	}
}

inline void intR(gwapi::Window &wnd, double a, double b, double c) {
	int x,y,i,j, sum;
	gwapi::Color newc;
	int a1 = a*255, b1 = b*255, c1 = c*255, sab = sqrt(a1*a1 + b1*b1)*15, rr;

	for (int k = 0; k < 30; k++){
		for (x = 0; x < 300; x++) {
			for (y = 0; y < 300; y++) {
				sum = 0;
				rr = 15*(a1*x + b1*y + c1);
				for (i = 0; i < 16; i++) {
					for (j = 0; j < 16; j++) {
						if (fabs(rr + a1*i + b1*j) <= (5) *sab) sum++;
					}
				}

				newc = gwapi::Black;
				newc.m[3] = newc.m[3]*sum/256;
				if (newc.m[3] == 0) newc.m[3] = 1;
				wnd.canvas[Point(x, y)] = gwapi::overlay(newc, wnd.canvas[Point(x, y)]).clrref;
			}
		}
	}
}

int main() {
	gwapi::Window wnd;
	DWORD current = 0, time = 0;

	double a = 1, b = -1, c = 0;

	current = CurrentProcessTick();	
	floatR(wnd, a, b, c);
	time = CurrentProcessTick()-current;
	printf("%.5lf\n", time/10000000.0);

	wnd.redraw();
	system("pause");

	current = CurrentProcessTick();	
	intR(wnd, a, b, c);
	time = CurrentProcessTick()-current;
	printf("%.5lf\n", time/10000000.0);

	wnd.redraw();
	system("pause");
	return 0;
}