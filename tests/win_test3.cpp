#define _USE_MATH_DEFINES
#include "D:\\CPP\\PROGRAM2\\GraphC.h\\window.h"
#include <stdlib.h>
#include <math.h>

/* Common consts */
const int scrX = 1280, scrY = 1024;
const double angleSpeed = 0.1;

/* Circles and rect consts. */
const int crclRadius = 15;
const int crclNum = 15;
const double crclStep = 2.3;

/* Color conus consts */
const int cnsRadius = 145;
const int cnsStep = 4;
const int cnsSpeed = 10;

/* Text consts */
const int textH = 14;
const int textW = 5*textH/2;
const int textSpeed = 4;

/* Other consts. NOT FOR EDIT. */
const double radius = min(scrX, scrY)/2 - 50;
const double rectCount = (M_PI*radius/(crclRadius+6));
const Point cntr = Point(scrX/2, scrY/2);
const Point d = Point(crclRadius, crclRadius);
const double txtX = scrX/textW + 3, txtY = scrY / textH + 3;
const double cnsNum = cnsRadius/cnsStep;

inline double toRad(double a) { return a/180.0*M_PI; }

void drawText(gwapi::Window &a, double angle) {
	int i1 = 1;
	double xp = 0, yp = 0;
	for (int i = -3; i < txtY; i++) {
		i1 *= -1;
		for (int j = -3; j < txtX; j++) {
			xp = j*textW + i1*((int)(angle*textSpeed))%textW;
			yp = i*textH;
			a.canvas.penSet(gwapi::rainbow((xp/scrX + yp/scrY + angle/50)*2.0, true));
			a.canvas.textOut(Point(xp, yp), "TEXT");
		}
	}
}

void drawDisk(gwapi::Window &a, double angle) {
	a.canvas.brushSet(gwapi::White);
	a.canvas.penSet(gwapi::Black);
	a.canvas.circleDraw(cntr, radius + crclRadius + 5);
	int i1 = 1;
	Point b;
	for (int i = 0; i < rectCount; i++) {
		i1 = 1;
		a.canvas.brushSet(gwapi::rainbow((i+angle)/rectCount*2.0, true));
		a.canvas.penSet(gwapi::rainbow((i+angle)/rectCount*2.0, true));

		for (int j = 0; j < crclNum; j++) {
			i1 *= -1;
			b = Point(cos(toRad(i1*angle + i*360/rectCount))*(radius-j*(crclRadius*crclStep-j)), 
				sin(toRad(i1*angle + i*360/rectCount))*(radius-j*(crclRadius*crclStep-j)));

			if (i1 == 1) {
				a.canvas.rectDraw(cntr+b - (d - Point(j,j)), cntr+b + (d - Point(j,j)));
			} else {
				a.canvas.circleDraw(cntr+b, crclRadius-j);
			}
		}
	}
}

void drawConus(gwapi::Window &a, double angle) {
	static int i1 = 1;
	double beta = 0;
	for (int i = 0; i < cnsNum; i++) {
		i1 *= -1;
		beta = toRad(angle*cnsSpeed + i*cnsSpeed);
		a.canvas.brushSet(gwapi::rainbow((angle/50.0 + i/cnsNum)*2, true));
		a.canvas.circleDraw(cntr + Point(sin(beta)*i, cos(beta)*i), cnsRadius - i*cnsStep);
	}
}

void fpsInit(gwapi::Window &a, DWORD &time, double angle) {
	if ((int)(angle/angleSpeed)%50 == 0) {
		time = a.timeGet();
	}
}

void fpsDraw(gwapi::Window &a, DWORD &time, double angle) {
	static double fps = 0;
	if ((int)(angle/angleSpeed)%50 == 0) {
		time = a.timeGet() - time; if (time == 0) time = 1;
		fps = 1000.0*50.0/time;
		printf((to_string(fps) + " FPS\n").c_str());
	}
	a.canvas.penSet(gwapi::Black);
	a.canvas.textOut(Point(0,0), to_string(fps) + " FPS");
};

int main() {
	gwapi::WindowType atype;
	atype.size = Point(scrX, scrY);
	gwapi::Window a(atype);
	
	/* Костыль. */
	Sleep(100);

	a.fullscreen();
	a.canvas.textStyle(gwapi::StyleText(textH - 2));

	double angle = 0;
	DWORD time = 0;

	while (true) {
		fpsInit(a, time, angle);

		a.canvas.clear();

		angle += angleSpeed;
		if (angle > 360) angle -= 360;
		
		drawText(a, angle);
		drawDisk(a, angle);
		drawConus(a, angle);

		fpsDraw(a, time, angle);

		a.redraw();
	}

	return 0;
}