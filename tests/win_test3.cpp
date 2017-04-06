#define _USE_MATH_DEFINES
#include "D:\\CPP\\PROGRAM2\\GraphC.h\\window.h"
#include <stdlib.h>
#include <math.h>

const int scrX = 1280, scrY = 1024;
const double rectCount =  70;
const double radius = 450;
const int cellRadius = 15;
const Point c = Point(scrX/2, scrY/2);
const Point d = Point(cellRadius, cellRadius);

inline double toRad(double a) { return a/180.0*M_PI; }

void drawText(gwapi::Window &a, double angle) {
	int i1 = 1;
	for (int i = 0; i < 73; i++) {
		i1 *= -1;
		for (int j = -3; j < 70; j++) {
			a.canvas.textOut(Point(j*35 + i1*25 + i1*((int)(angle*4))%35, i*14), "TEXT");
		}
	}
}

void drawDisk(gwapi::Window &a, double angle) {
	int i1 = 1;
	Point b;
	for (int i = 0; i < rectCount; i++) {
		i1 = 1;
		a.canvas.brushSet(gwapi::rainbow((i+angle)/rectCount*2.0, true));
		a.canvas.penSet(gwapi::rainbow((i+angle)/rectCount*2.0, true));

		for (int j = 0; j < 15; j++) {
			i1 *= -1;
			b = Point(cos(toRad(i1*angle + i*360/rectCount))*(radius-j*(cellRadius*2.3-j)), 
				sin(toRad(i1*angle + i*360/rectCount))*(radius-j*(cellRadius*2.3-j)));

			if (i1 == 1) {
				a.canvas.rectDraw(c+b - (d - Point(j,j)), c+b + (d - Point(j,j)));
			} else {
				a.canvas.circleDraw(c+b, cellRadius-j);
			}
		}
	}
}

void drawConus(gwapi::Window &a, double angle) {
	static int i1 = 1;
	for (int i = 0; i < 36; i++) {
		i1 *= -1;
		a.canvas.brushSet(gwapi::rainbow((angle/50.0 + i/36.0)*2, true));
		a.canvas.circleDraw(c + Point(sin(toRad(angle*10 + i*10))*i, 
			cos(toRad(angle*10 + i*10))*i), 145 - i*4);
	}
}

int main() {
	gwapi::WindowType atype;
	atype.size = Point(scrX, scrY);
	gwapi::Window a(atype);

	Sleep(100);

	a.fullscreen();
	a.canvas.textStyle(12);

	double angle = 0;
	Point b;
	int i1 = 1;
	DWORD time = 0;
	double fps = 0;

	while (true) {
		if ((int)(angle*10)%50 == 0) {
			time = a.timeGet();
		}

		a.canvas.clear();

		angle += 0.1;
		if (angle > 360) angle -= 360;
		
		drawText(a, angle);

		a.canvas.brushSet(gwapi::White);
		a.canvas.penSet(gwapi::Black);
		a.canvas.circleDraw(c, radius + cellRadius + 5);

		drawDisk(a, angle);
		drawConus(a, angle);

		if ((int)(angle*10)%50 == 0) {
			time = a.timeGet() - time; if (time == 0) time = 1;
			fps = 1000.0*50.0/time;
			printf((to_string(fps) + " FPS\n").c_str());
		}
		a.canvas.textOut(Point(0,0), to_string(fps) + " FPS");

		a.redraw();
	}

	return 0;
}