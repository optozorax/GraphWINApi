#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "../window.h"
#include "../point.h"
#include "D:/CPP/agg/include/agg_font_win32_tt.h"

class Car {
	point2 size;
	point2 pos;
	double speed; // Скорость по направлению движения
	double angle; // Угол поворота машины относительно ОХ
	double wheelAngle; // Угол поворота колес относительно оси машины
public:
	Car(point2 = point2(20, 7), point2 = point2(), double = 0, double = 0, double = 0);

	void draw(wgs::bufer&, point2 = point2(), double = 1);
	void step(double, double, double = 0, double = 0);
	//void getParams(double&, double&);
};

Car::Car(point2 sz, point2 ps, double sp, double an, double wh) :
	size(sz),
	pos(ps),
	speed(sp),
	angle(an),
	wheelAngle(wh) {
}

void Car::draw(wgs::bufer &buf, point2 center, double scale) {
	static std::vector<point2> vcs(4);
	static std::vector<point2> ps(12);

	/* Создание векторов, при помощи которых будут формироваться точки. */
	vcs[0] = point2(size.x, 0)/2.0;
	vcs[1] = point2(0, size.y)/2.0;
	vcs[2] = point2(4, 0)/2.0;
	vcs[3] = point2(4, 0)/2.0;
	vcs[3].rotate(wheelAngle);

	/* Точки корпуса машины. */
	ps[0] =  vcs[0] - vcs[1];
	ps[1] = -vcs[0] - vcs[1];
	ps[2] =  vcs[0] + vcs[1];
	ps[3] = -vcs[0] + vcs[1];

	/* Точки двух передних колес машины. */
	ps[4] = ps[0] + vcs[2];
	ps[5] = ps[0] - vcs[2];
	ps[6] = ps[2] + vcs[2];
	ps[7] = ps[2] - vcs[2];

	/* Точки двух последних колес машины. */
	ps[8] = ps[1] + vcs[3];
	ps[9] = ps[1] - vcs[3];
	ps[10] = ps[3] + vcs[3];
	ps[11] = ps[3] - vcs[3];

	/* Поворот всех точек на угол поворота машины.
	   Применение к ним масштаба. 
	   Сдвиг на заданную величину пикселей. 
	   Создание из них целочисленных точек. */
	for (int i = 0; i < 12; i++) {
		ps[i].rotate(angle+M_PI/2);
		ps[i] = ps[i] + pos;
		ps[i] = ps[i]*scale + center;
	}

	/* Рисование корпуса. */
	buf.penSet(wgs::Bitcoin, 1);
	buf.draw_line(ps[0], ps[1]);
	buf.draw_line(ps[2], ps[3]);
	buf.draw_line(ps[0], ps[2]);
	buf.draw_line(ps[1], ps[3]);

	/* Рисование колес. */
	buf.penSet(wgs::Miku, 3);
	buf.draw_line(ps[4], ps[5]);
	buf.draw_line(ps[6], ps[7]);
	buf.draw_line(ps[8], ps[9]);
	buf.draw_line(ps[10], ps[11]);
}

void Car::step(double pedal, double wheel, double brake, double orient) {
	/* Константы, которые контроллируют физическую часть симуляции. */
	const double pedalForce = 0.1; // Скорость возрастания скорости
	const double brakeForce = 0.97; // Скорость торможения
	const double wheelForce = 0.01; // Скорость возрастания угла
	const double orientForce = 1;//0.97; // Скорость возвращения угла на место
	const double speedMax = 40; // Максимальная скорость
	const double angleMax = M_PI/4; // Максимальный угол колес
	const double speedFriction = 1;//0.99; // Какая часть скорости теряется каждую итерацию от трения 
	const double angleFriction = 1;//0.99; // Какая часть угла теряется каждую итерацию от трения
	const double dt = 0.1; // Количество времени, которое проходит за итерацию

	/* Контроль выхода за границы. */
	if (pedal > 1) pedal = 1;
	if (pedal < -1) pedal = -1;

	if (wheel > 1) wheel = 1;
	if (wheel < -1) wheel = -1;

	if (brake > 1) brake = 1;
	if (brake < 0) brake = 0;

	if (orient > 1) orient = 1;
	if (orient < 0) orient = 0;

	/* Изменение скорости и угла согласно силе. */
	speed += pedal*pedalForce;
	speed *= 1-(1-brakeForce)*(brake);

	wheelAngle += wheel*wheelForce;
	wheelAngle *= 1-(1-orientForce)*(orient);

	if (fabs(speed) > speedMax) speed = speed/fabs(speed)*speedMax;
	if (fabs(wheelAngle) > angleMax) wheelAngle = wheelAngle/fabs(wheelAngle)*angleMax;

	/* Работа силы трения. */
	speed *= speedFriction;
	wheelAngle *= angleFriction;

	/* Перемещение машины. */
	point2 dpos;
	double dangle;
	
	if (wheelAngle == 0) { // Если угол колес равен нулю, то там особое управление
		dpos = point2(sin(angle)*speed*dt, -cos(angle)*speed*dt);
		dangle = 0;
	} else {
		double R1 = size.x/tan(wheelAngle);
		double R2 = size.x/sin(wheelAngle);
		double w = -speed/((R1 + R2)/2.0);
		double alpha = w*dt;

		point2 x(-R1, -size.x/2);
		point2 p1(0,-size.x/2), p2(0, size.x/2);
		p1.rotate(alpha, x);
		p2.rotate(alpha, x);
	
		dpos = (p1 + p2)/2.0;
		dpos.rotate(angle);

		p2 = p2 - p1;
		dangle = atan2(p2.x, p2.y);
	}

	pos = pos + dpos;
	angle += dangle;
}

double pd = 0, wh = 0, br = 0, or = 0;
void mykeyboard(wgs::window &current) {
	if (current.isKeyDown('W')) pd = 1;
	if (current.isKeyDown('A')) wh = -1;
	if (current.isKeyDown('S')) pd = -1;
	if (current.isKeyDown('D')) wh = 1;
	if (current.isKeyDown(' ')) br = or = 1;
}

int main() {
	wgs::window current;
	current.canvas.resize(Point(500, 500));
	current.sizeSet(Point(500, 500));

	const int carLayers = 5;
	const int carInCircle = 2;
	const int carNum = carLayers*carInCircle;
	std::vector<Car> cars(carNum);
	for (int j = 0; j < carLayers; j++) {
		for (int i = j*carInCircle; i < (j+1)*carInCircle; i++) {
			cars[i] = Car(point2(16,7), point2(cos(M_PI*i/carInCircle*2.0)*(j+1)*10, sin(M_PI*i/carInCircle*2.0)*(j+1)*10), 0, M_PI*i/carInCircle*2.0 + M_PI/6.0*j, 0);
		}
	}

	/* typedef agg::font_engine_win32_tt_int16 font_engine_type;
	typedef agg::font_cache_manager<font_engine_type> font_manager_type;

	int carNum = 0;
	std::vector<Car> cars;

	font_engine_type		m_feng(::GetDC(0));
	font_manager_type		m_fman(m_feng);
	m_feng.height(100.0);
	m_feng.italic(true);
	m_feng.weight(10);

	if(m_feng.create_font("Ubuntu Light", agg::glyph_ren_outline))
	{
		double x = 0.0;
		double y = -200.0;
		const char* p = "Test text";

		while(*p)
		{
			const agg::glyph_cache* glyph = m_fman.glyph(*p);
			if(glyph)
			{

				m_fman.add_kerning(&x, &y);
				m_fman.init_embedded_adaptors(glyph, x, y);

				// PATH
				auto path = m_fman.path_adaptor();
				path.rewind(0);
				double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
				double first = 0, second = 0;

				int cmd = agg::path_cmd_line_to;
				while (cmd != agg::path_cmd_stop) {
					cmd = path.vertex(&x1, &y1); y1 = -y1;

					if (cmd == agg::path_cmd_move_to) {
						first = x1;
						second = y1;
					}

					int cmd1 = cmd & agg::path_flags_close;
					if (cmd1) {
						x1 = first;
						y1 = second;

						cmd = agg::path_cmd_line_to;
					}

					if (cmd == 3 || cmd == agg::path_cmd_line_to) {
						cars.push_back(Car(
							Point((point2(x2, y2)-point2(x1, y1)).length()+1, 1), 
							(Point(x2, y2)+Point(x1, y1))/2,
							0,
							atan2(y2-y1, x2-x1)+M_PI/2,
							M_PI/2
						));
						current.canvas.draw_line(Point(x1, y1), Point(x2, y2));
						current.redraw();
						carNum++;
					}

					x2 = x1;
					y2 = y1;
				}

				// increment pen position
				x += glyph->advance_x;
				y += glyph->advance_y;
			}
			++p;
		}

	} */

	while (true) {
		current.canvas.clear();
		mykeyboard(current);
		for (int i = 0; i < carNum; i++) {
			cars[i].step(pd, wh, br, or);
			cars[i].draw(current.canvas, point2(250, 250), 2.5);
		}

		pd = wh = br = or = 0;
		current.redraw();

		Sleep(10);
	}
	
	system("pause");

	return 0;
}