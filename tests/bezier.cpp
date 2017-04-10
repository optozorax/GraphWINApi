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

	void draw(gwapi::Bufer&, bool = true);
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

void Bezier::draw(gwapi::Bufer &buf, bool color) {
	gwapi::Color crclClr = gwapi::rgb(255, 0, 0);
	gwapi::Color lineClr = gwapi::rgb(0, 0, 255);
	gwapi::Color bezrClr = gwapi::rgb(0, 0, 0);
	if (!color) {
		crclClr.m[3] = 128;
		lineClr.m[3] = 128;
		bezrClr.m[3] = 128;
		crclClr = gwapi::overlay(crclClr, gwapi::White);
		lineClr = gwapi::overlay(lineClr, gwapi::White);
		bezrClr = gwapi::overlay(bezrClr, gwapi::White);
	}

	/* Рисуем кривую Безье. */
	buf.penSet(bezrClr, color + 1);
	buf.bezierDraw(a_);

	/* Рисуем линии. */
	buf.penSet(lineClr, color + 1);
	for (int i = 0; i < a_.size()-1; i++) {
		buf.lineDraw(a_[i], a_[i+1]);
	}

	/* Рисуем окружности. */
	buf.penSet(crclClr, color + 1);
	for (int i = 0; i < a_.size(); i++) {
		buf.circleDraw(a_[i], poRadius);
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
	
	void draw(gwapi::Bufer&, Point, bool = true);
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

void BezierInterface::draw(gwapi::Bufer &buf, Point mousePos, bool color) {
	bz.draw(buf, color);
	if (color) {
		int num = bz.numPoint(mousePos);
		if (num != -1) {
			buf.penSet(gwapi::Bitcoin, 2);
			buf.circleDraw(bz.getCenter(num), poRadius);
		}
	}
}

void BezierInterface::mouseWheel(Point x, int wh) {
	point2 b, c;
	to_another(c, x);
	for (int i = 0; i < bz.size(); i++) {
		to_another(b, bz.getCenter(i));
		b.rotate((wh)/120.0/180.0*M_PI, c);
		to_another(bz.getCenter(i), b);
	}
}

std::vector<BezierInterface> bzs(10);
Point mousePos;
int currentBez = 0;
bool drawFlag = true;

void bezmouse(gwapi::Window *This, int x, int y, gwapi::WinEvents::MouseType type, int wheel) {
	switch (type)
	{
	case gwapi::WinEvents::L_DOWN:
		bzs[currentBez].mouseLeft(Point(x,y), false);
		drawFlag = true;
		break;
	case gwapi::WinEvents::R_DOWN:
		bzs[currentBez].mouseRight(Point(x,y), false);
		drawFlag = true;
		break;
	case gwapi::WinEvents::L_UP:
		bzs[currentBez].mouseLeft(Point(x,y), true);
		drawFlag = true;
		break;
	case gwapi::WinEvents::R_UP:
		bzs[currentBez].mouseRight(Point(x,y), true);
		drawFlag = true;
		break;
	case gwapi::WinEvents::Move:
		bzs[currentBez].mouseMove(Point(x,y));
		mousePos = Point(x, y);
		drawFlag = true;
		break;
	case gwapi::WinEvents::Wheel:
		//bzs[currentBez].mouseWheel(mousePos, wheel);
		//drawFlag = true;
		break;
	default:
		break;
	}
}

void bezkey(gwapi::Window *This, int key, bool state) {
	if (!state) {
		if (key >= '0' && key <='9')
			currentBez = key-'0';
		else
			currentBez = 0;
		drawFlag = true;
	}
}

int main() {
	gwapi::WindowType currType;
	currType.caption = "Bezier Creater v0.1";
	currType.size = Point(800, 800);
	gwapi::Window current(currType);

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