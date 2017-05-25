#include "D:/CPP/agg/include/agg_scanline_u.h"
#include "D:/CPP/agg/include/agg_rasterizer_scanline_aa.h"
#include "D:/CPP/agg/include/agg_pixfmt_rgba.h"
#include "D:/CPP/agg/include/agg_rendering_buffer.h"
#include "D:/CPP/agg/include/agg_conv_stroke.h"
#include "D:/CPP/agg/include/agg_renderer_scanline.h"
#include "D:/CPP/agg/include/agg_ellipse.h"
#include "D:/CPP/agg/include/agg_basics.h"

typedef agg::pixfmt_bgra32								pixfmt;
typedef agg::renderer_base<pixfmt>						renderer_base;
typedef agg::renderer_scanline_aa_solid<renderer_base>	renderer_scanline;
typedef agg::rasterizer_scanline_aa<>					rasterizer_scanline;
typedef agg::scanline_u8								scanline;
typedef agg::rasterizer_sl_clip_int_3x					rass;
typedef pixfmt::color_type								color_type;

class vertex_src_poly_line {
public:
	vertex_src_poly_line(int num1 = 100);
	~vertex_src_poly_line();
	void reset(void);
	void add_point(double x, double y);
	void rewind(unsigned);
	void polygon(bool x);
	unsigned vertex(double* x, double* y);

private:
	unsigned num;
	unsigned m_count;
	double   *m_x;
	double   *m_y;
	bool	 poly;
};