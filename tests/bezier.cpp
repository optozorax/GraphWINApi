#define _USE_MATH_DEFINES
#include "../window.h"
#include <math.h>
#include <vector>
#include <string>

const double poRadius = 5;

class Bezier {
	std::vector<Point> a_;
public:
	Bezier();

	int numPoint(Point);
	Point& getCenter(int);
	int size();

	void pushPoint(Point);
	void deletePoint(int);
	void movePoint(int, Point);
	void move(Point);

	void draw(wgs::bufer&, bool = true);
};

Bezier::Bezier() : a_(1) {
	a_[0] = Point(100, 100);
}

int Bezier::numPoint(Point x) {
	for (int i = 0; i < a_.size(); i++) {
		if ((a_[i]-x).length() <= poRadius) {
			return i;
		}
	}
	return -1;
}

Point& Bezier::getCenter(int i) {
	return a_[i];
}

int Bezier::size() {
	return a_.size();
}

void Bezier::pushPoint(Point a) {
	a_.push_back(a);
}

void Bezier::deletePoint(int i) {
	a_.erase(a_.begin()+i);
}

void Bezier::movePoint(int i, Point dp) {
	a_[i] = a_[i] + dp;
}

void Bezier::move(Point dp) {
	for (int i = 0; i < a_.size(); i++)	{
		movePoint(i, dp);
	}
}

void Bezier::draw(wgs::bufer &buf, bool color) {
	wgs::color crclClr = wgs::rgb(255, 0, 0);
	wgs::color lineClr = wgs::rgb(0, 0, 255);
	wgs::color bezrClr = wgs::rgb(0, 0, 0);
	if (!color) {
		crclClr.m[3] = 128;
		lineClr.m[3] = 128;
		bezrClr.m[3] = 128;
		crclClr = wgs::overlay(crclClr, wgs::White);
		lineClr = wgs::overlay(lineClr, wgs::White);
		bezrClr = wgs::overlay(bezrClr, wgs::White);
	}

	/* Рисуем кривую Безье. */
	buf.penSet(bezrClr, color + 1);
	buf.draw_bezier(a_);

	/* Рисуем линии. */
	buf.penSet(lineClr, color + 1);
	for (int i = 0; i < a_.size()-1; i++) {
		buf.draw_line(a_[i], a_[i+1]);
	}

	/* Рисуем окружности. */
	buf.penSet(crclClr, color + 1);
	for (int i = 0; i < a_.size(); i++) {
		buf.draw_ellipse(a_[i], Point(poRadius,poRadius));
	}
}

class BezierInterface {
	Bezier bz;

	bool pointMove;
	bool allMove;

	int numMove;
	Point lastMove;
public:
	BezierInterface();

	void mouseLeft(Point, bool);
	void mouseRight(Point, bool);
	void mouseMove(Point);
	void mouseWheel(Point, int);
	
	void draw(wgs::bufer&, Point, bool = true);
};

BezierInterface::BezierInterface() : 
	bz(),
	pointMove(false),
	allMove(false),
	numMove(0),
	lastMove() {
}

void BezierInterface::mouseLeft(Point x, bool state) {
	if (!state) { // DOWN
		numMove = bz.numPoint(x);
		if (numMove == -1) {
			allMove = true;
			pointMove = false;
		} else {
			allMove = false;
			pointMove = true;
		}
		lastMove = x;
	} else { // UP
		allMove = false;
		pointMove = false;
	}
}

void BezierInterface::mouseRight(Point x, bool state) {
	if (!state) { // DOWN
		/* Nothing. */
	} else { // UP
		int num = bz.numPoint(x);
		if (num == -1) {
			bz.pushPoint(x);
		} else {
			bz.deletePoint(num);
		}
	}
}

void BezierInterface::mouseMove(Point x) {
	if (allMove) {
		bz.move(x-lastMove);
		lastMove = x;
	} else if (pointMove){
		bz.movePoint(numMove, x-lastMove);
		lastMove = x;
	}
}

void BezierInterface::draw(wgs::bufer &buf, Point mousePos, bool color) {
	bz.draw(buf, color);
	if (color) {
		int num = bz.numPoint(mousePos);
		if (num != -1) {
			buf.penSet(wgs::Bitcoin, 2);
			buf.draw_ellipse(bz.getCenter(num), Point(poRadius,poRadius));
		}
	}
}

void BezierInterface::mouseWheel(Point x, int wh) {
	point2 b, c;
	c = x;
	for (int i = 0; i < bz.size(); i++) {
		b = bz.getCenter(i);
		b.rotate((wh)/120.0/180.0*M_PI, c);
		bz.getCenter(i) = b;
	}
}

std::vector<BezierInterface> bzs(10);
Point mousePos;
int currentBez = 0;
bool drawFlag = true;

void bezmouse(wgs::window *This, int x, int y, wgs::WinEvents::MouseType type, int wheel) {
	switch (type)
	{
	case wgs::WinEvents::L_DOWN:
		bzs[currentBez].mouseLeft(Point(x,y), false);
		drawFlag = true;
		break;
	case wgs::WinEvents::R_DOWN:
		bzs[currentBez].mouseRight(Point(x,y), false);
		drawFlag = true;
		break;
	case wgs::WinEvents::L_UP:
		bzs[currentBez].mouseLeft(Point(x,y), true);
		drawFlag = true;
		break;
	case wgs::WinEvents::R_UP:
		bzs[currentBez].mouseRight(Point(x,y), true);
		drawFlag = true;
		break;
	case wgs::WinEvents::Move:
		bzs[currentBez].mouseMove(Point(x,y));
		mousePos = Point(x, y);
		drawFlag = true;
		break;
	case wgs::WinEvents::Wheel:
		//bzs[currentBez].mouseWheel(mousePos, wheel);
		//drawFlag = true;
		break;
	default:
		break;
	}
}

void bezkey(wgs::window *This, int key, bool state) {
	if (!state) {
		if (key >= '0' && key <='9')
			currentBez = key-'0';
		else
			currentBez = 0;
		drawFlag = true;
	}
}

int main() {
	wgs::window_type currType;
	currType.caption = "Bezier Creater v0.1";
	currType.size = Point(800, 800);
	wgs::window current(currType);

	current.ev.mouse = bezmouse;
	current.ev.keyboard = bezkey;

	while (true) {
		if (drawFlag) {
			current.canvas.clear();
			for (int i = 0; i < 10; i++) {
				bzs[i].draw(current.canvas, mousePos, i == currentBez);
			}
			current.redraw();
			drawFlag = false;
			Sleep(3);
		}
	}
}