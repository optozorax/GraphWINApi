#define _USE_MATH_DEFINES
#include <math.h>
#include "bufer.h"
#include "bufer2.h"
#include "color.h"

vertex_src_poly_line::vertex_src_poly_line(int num1) : num(0), m_count(0), poly(true) { 
	m_x = new double[num1+5];
	m_y = new double[num1+5];
	memset(m_x, 0, sizeof(double)*(num1+5));
	memset(m_y, 0, sizeof(double)*(num1+5));
}

vertex_src_poly_line::~vertex_src_poly_line() {
	delete[] m_x;
	delete[] m_y;
}

void vertex_src_poly_line::reset(void) {
	num = 0; 
}

void vertex_src_poly_line::add_point(double x, double y) {	
	m_x[num] = x;	
	m_y[num] = y;	
	num++; 
}

void vertex_src_poly_line::rewind(unsigned) { 
	m_count = 0; 
}

void vertex_src_poly_line::polygon(bool x) { 
	poly = x; 
}

unsigned vertex_src_poly_line::vertex(double* x, double* y)
{
	if(m_count == num) {
		m_count++;
		if (poly) {
			if (num == 2)	return agg::path_cmd_stop;
			else			return agg::path_cmd_end_poly | agg::path_flags_close;
		} else 
			return agg::path_cmd_stop;
	}
	if(m_count > num) return agg::path_cmd_stop;
	*x = m_x[m_count];
	*y = m_y[m_count];
	m_count++;
	return ((m_count == 1) ? agg::path_cmd_move_to : agg::path_cmd_line_to);
}

namespace wgs{

	void wgs::bufer::draw_rect(point2 a, point2 b) {	
		std::vector<point2> mas;
		mas.push_back(a);
		mas.push_back(point2(b.x, a.y));
		mas.push_back(b);
		mas.push_back(point2(a.x, b.y));

		draw_poly(mas);
	}

	void wgs::bufer::draw_ellipse(point2 pos, point2 size) {
		agg::rendering_buffer	rbuf((agg::int8u*) buf(), width(), height(), width()*4);
		pixfmt					pf(rbuf);
		renderer_base			ren_base(pf);
		renderer_scanline		ren_sl(ren_base);
		rasterizer_scanline		ras;
		scanline				sl;

		agg::ellipse ell;
		ell.init(pos.x, pos.y, size.x, size.y);

		ren_sl.color(agg::rgba8(this->m_brush.m[2], this->m_brush.m[1], this->m_brush.m[0], this->m_brush.m[3]));

		ras.reset();
		ras.add_path(ell);
		agg::render_scanlines(ras, sl, ren_sl);

		agg::conv_stroke<agg::ellipse> stroke(ell);
		stroke.line_cap(agg::round_cap);
		stroke.line_join(agg::round_join);
		stroke.width(this->m_pen.thickness);

		ren_sl.color(agg::rgba8(this->m_pen.color.m[2], this->m_pen.color.m[1], 
			this->m_pen.color.m[0], this->m_pen.color.m[3]));

		ras.reset();
		ras.add_path(stroke);
		agg::render_scanlines(ras, sl, ren_sl);
	}

	void wgs::bufer::draw_line(point2 first, point2 second) {
		agg::rendering_buffer	rbuf((agg::int8u*) buf(), width(), height(), width()*4);
		pixfmt					pf(rbuf);
		renderer_base			ren_base(pf);
		renderer_scanline		ren_sl(ren_base);
		rasterizer_scanline		ras;
		scanline				sl;

		vertex_src_poly_line line;
		line.add_point(first.x, first.y);
		line.add_point(second.x, second.y);

		agg::conv_stroke<vertex_src_poly_line> stroke(line);
		stroke.line_cap(agg::round_cap);
		stroke.line_join(agg::round_join);
		stroke.width(this->m_pen.thickness);

		ren_sl.color(agg::rgba8(this->m_pen.color.m[2], this->m_pen.color.m[1], 
					 this->m_pen.color.m[0], this->m_pen.color.m[3]));

		ras.reset();
		ras.add_path(stroke);
		agg::render_scanlines(ras, sl, ren_sl);
	}

	void wgs::bufer::draw_poly(std::vector<point2> mas) {
		agg::rendering_buffer	rbuf((agg::int8u*) buf(), width(), height(), width()*4);
		pixfmt					pf(rbuf);
		renderer_base			ren_base(pf);
		renderer_scanline		ren_sl(ren_base);
		rasterizer_scanline		ras;
		scanline				sl;

		vertex_src_poly_line poly;
		for (int i = 0; i < mas.size(); i++) {
			poly.add_point(mas[i].x, mas[i].y);
		}

		ren_sl.color(agg::rgba8(this->m_brush.m[2], this->m_brush.m[1], this->m_brush.m[0], this->m_brush.m[3]));

		ras.reset();
		ras.add_path(poly);
		agg::render_scanlines(ras, sl, ren_sl);

		agg::conv_stroke<vertex_src_poly_line> stroke(poly);
		stroke.line_cap(agg::round_cap);
		stroke.line_join(agg::round_join);
		stroke.width(this->m_pen.thickness);

		ren_sl.color(agg::rgba8(this->m_pen.color.m[2], this->m_pen.color.m[1], 
			this->m_pen.color.m[0], this->m_pen.color.m[3]));

		ras.reset();
		ras.add_path(stroke);
		agg::render_scanlines(ras, sl, ren_sl);
	}

}