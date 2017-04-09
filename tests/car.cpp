#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "../window.h"
#include "../point.h"

class Car {
	point2 size;
	point2 pos;
	double speed; // �������� �� ����������� ��������
	double angle; // ���� �������� ������ ������������ ��
	double wheelAngle; // ���� �������� ����� ������������ ��� ������
public:
	Car(point2 = point2(20, 7), point2 = point2(), double = 0, double = 0, double = 0);

	void draw(gwapi::Bufer&, point2 = point2(), double = 1);
	void step(double, double, double = 0, double = 0);
	void getParams(double&, double&);
};

Car::Car(point2 sz, point2 ps, double sp, double an, double wh) :
	size(sz),
	pos(ps),
	speed(sp),
	angle(an),
	wheelAngle(wh) {
}

void Car::draw(gwapi::Bufer &buf, point2 center, double scale) {
	static std::vector<point2> vcs(4);
	static std::vector<point2> ps(12);
	static std::vector<Point> pps(12);

	/* �������� ��������, ��� ������ ������� ����� ������������� �����. */
	vcs[0] = point2(size[0], 0)/2.0;
	vcs[1] = point2(0, size[1])/2.0;
	vcs[2] = point2(4, 0)/2.0;
	vcs[3] = point2(4, 0)/2.0;
	vcs[3].rotate(wheelAngle);

	/* ����� ������� ������. */
	ps[0] =  vcs[0] - vcs[1];
	ps[1] = -vcs[0] - vcs[1];
	ps[2] =  vcs[0] + vcs[1];
	ps[3] = -vcs[0] + vcs[1];

	/* ����� ���� �������� ����� ������. */
	ps[4] = ps[0] + vcs[2];
	ps[5] = ps[0] - vcs[2];
	ps[6] = ps[2] + vcs[2];
	ps[7] = ps[2] - vcs[2];

	/* ����� ���� ��������� ����� ������. */
	ps[8] = ps[1] + vcs[3];
	ps[9] = ps[1] - vcs[3];
	ps[10] = ps[3] + vcs[3];
	ps[11] = ps[3] - vcs[3];

	/* ������� ���� ����� �� ���� �������� ������.
	   ���������� � ��� ��������. 
	   ����� �� �������� �������� ��������. 
	   �������� �� ��� ������������� �����. */
	for (int i = 0; i < 12; i++) {
		ps[i].rotate(angle+M_PI/2);
		ps[i] = ps[i] + pos;
		ps[i] = ps[i]*scale + center;
		to_another(pps[i], ps[i]);
	}

	/* ��������� �������. */
	buf.penSet(gwapi::Bitcoin, 2);
	buf.lineDraw(pps[0], pps[1]);
	buf.lineDraw(pps[2], pps[3]);
	buf.lineDraw(pps[0], pps[2]);
	buf.lineDraw(pps[1], pps[3]);

	/* ��������� �����. */
	buf.penSet(gwapi::Miku, 3);
	buf.lineDraw(pps[4], pps[5]);
	buf.lineDraw(pps[6], pps[7]);
	buf.lineDraw(pps[8], pps[9]);
	buf.lineDraw(pps[10], pps[11]);
}

void Car::step(double pedal, double wheel, double brake, double orient) {
	/* ���������, ������� ������������� ���������� ����� ���������. */
	const double pedalForce = 0.1; // �������� ����������� ��������
	const double brakeForce = 0.8; // �������� ����������
	const double wheelForce = 0.01; // �������� ����������� ����
	const double orientForce = 0.8; // �������� ����������� ���� �� �����
	const double speedMax = 40; // ������������ ��������
	const double angleMax = M_PI/4; // ������������ ���� �����
	const double speedFriction = 1;//0.99; // ����� ����� �������� �������� ������ �������� �� ������ 
	const double angleFriction = 1;//0.99; // ����� ����� ���� �������� ������ �������� �� ������
	const double dt = 0.1; // ���������� �������, ������� �������� �� ��������

	/* �������� ������ �� �������. */
	if (pedal > 1) pedal = 1;
	if (pedal < -1) pedal = -1;

	if (wheel > 1) wheel = 1;
	if (wheel < -1) wheel = -1;

	if (brake > 1) brake = 1;
	if (brake < 0) brake = 0;

	if (orient > 1) orient = 1;
	if (orient < 0) orient = 0;

	/* ��������� �������� � ���� �������� ����. */
	speed += pedal*pedalForce;
	speed *= 1-(1-brakeForce)*(brake);

	wheelAngle += wheel*wheelForce;
	wheelAngle *= 1-(1-orientForce)*(orient);

	if (fabs(speed) > speedMax) speed = speed/fabs(speed)*speedMax;
	if (fabs(wheelAngle) > angleMax) wheelAngle = wheelAngle/fabs(wheelAngle)*angleMax;

	/* ������ ���� ������. */
	speed *= speedFriction;
	wheelAngle *= angleFriction;

	/* ����������� ������. */
	point2 dpos;
	double dangle;
	
	if (wheelAngle == 0) { // ���� ���� ����� ����� ����, �� ��� ������ ����������
		dpos = point2(sin(angle)*speed*dt, -cos(angle)*speed*dt);
		dangle = 0;
	} else {
		double R1 = size[0]/tan(wheelAngle);
		double R2 = size[0]/sin(wheelAngle);
		double w = -speed/((R1 + R2)/2.0);
		double alpha = w*dt;

		point2 x(-R1, -size[0]/2);
		point2 p1(0,-size[0]/2), p2(0, size[0]/2);
		p1.rotate(alpha, x);
		p2.rotate(alpha, x);
	
		dpos = (p1 + p2)/2.0;
		dpos.rotate(angle);

		p2 = p2 - p1;
		dangle = atan2(p2[0], p2[1]);
	}

	pos = pos + dpos;
	angle += dangle;
}

double pd = 0, wh = 0, br = 0, or = 0;
void mykeyboard(gwapi::Window &current) {
	if (current.isKeyDown('W')) pd = 1;
	if (current.isKeyDown('A')) wh = -1;
	if (current.isKeyDown('S')) pd = -1;
	if (current.isKeyDown('D')) wh = 1;
	if (current.isKeyDown(' ')) br = or = 1;
}

int main() {
	gwapi::Window current;
	current.canvas.resize(800, 800);
	current.sizeSet(Point(800, 800));

	const int carNum = 30;
	std::vector<Car> cars(carNum);
	for (int i = 0; i < 10; i++) {
		cars[i] = Car(point2(16,7), point2(cos(M_PI*i/5.0)*40, sin(M_PI*i/5.0)*40), 0, M_PI*i/5.0 + M_PI/2.0, 0);
	}

	for (int i = 10; i < 20; i++) {
		cars[i] = Car(point2(16,7), point2(cos(M_PI*i/5.0)*60, sin(M_PI*i/5.0)*60), 0, M_PI*i/5.0, 0);
	}

	for (int i = 20; i < 30; i++) {
		cars[i] = Car(point2(16,7), point2(cos(M_PI*i/5.0)*70, sin(M_PI*i/5.0)*70), 0, -M_PI*i/5.0, 0);
	}

	while (true) {
		current.canvas.clear();
		mykeyboard(current);
		for (int i = 0; i < carNum; i++) {
			cars[i].step(pd, wh, br, or);
			cars[i].draw(current.canvas, point2(400, 400), 2);
		}

		pd = wh = br = or = 0;
		current.redraw();

		Sleep(10);
	}
	
	system("pause");

	return 0;
}