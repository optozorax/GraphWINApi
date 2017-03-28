#include "color.h"

gwapi::Color gwapi::rgb(int r, int g, int b) {
	Color c;
	c.m[3] = 255;
	c.m[2] = r;
	c.m[1] = g;
	c.m[0] = b;
	return c;
};
gwapi::Color gwapi::argb(int a, int r, int g, int b) {
	Color c;
	c.m[3] = a;
	c.m[2] = r;
	c.m[1] = g;
	c.m[0] = b;
	return c;
};

gwapi::Color gwapi::gray(double g) {
	Color c;
	c.m[3] = 255;
	c.m[2] = (UCHAR) (255*g);
	c.m[1] = c.m[2];
	c.m[0] = c.m[2];
	return c;
};


//gwapi::Color gwapi::rainbow(double, bool cycle, bool gray) {};
//gwapi::Color gwapi::gradient(double, vector<Color>) {};

//gwapi::Color gwapi::operator+(Color, Color) { };
//gwapi::Color gwapi::operator*(Color, unsigned char /* double??? */) {};

gwapi::Color gwapi::overlay(const Color &a, const Color &b) {
	//@ œ–Œ¬≈–»“‹  ¿  –¿¡Œ“¿≈“. @//
	Color c;
	
	UCHAR da = a.m[3];
	
	/* Ã‡ÎÂÌ¸Í‡ˇ ÓÔÚËÏËÁ‡ˆËˇ. */
	if (da != 0) {
		UCHAR dr = a.m[2];
		UCHAR dg = a.m[1];
		UCHAR db = a.m[0];
		cout << (int)da << ' ' << (int)dr << ' ' << (int)dg << ' ' << (int)db << endl;
		
		UCHAR sa = b.m[3];
		if (sa == 0 && b.clrref != 0) {
			sa = 255;
		}
		if (sa == 1) { /*  Œ—“€À‹ ƒÀﬂ –¿¡Œ“€ —“¿Õƒ¿–“Õ€’ œ–»Ã»“»¬Œ¬. */
			sa = 0;
		}
		UCHAR sr = b.m[2];
		UCHAR sg = b.m[1];
		UCHAR sb = b.m[0];
		cout << (int)sa << ' ' << (int)sr << ' ' << (int)sg << ' ' << (int)sb << endl;
		
		int ra = (int)(255.0*(da/255.0 + (1-da/255.0)*sa/255.0)) % 256;
		if (ra > 255) ra = 255;
		double dda = da/255.0;
		double dsa = sa/255.0;
		double dra = ra/255.0;
		int rr = (dr*dda + sr*dsa*(1-dda))/dra;
		int rg = (dg*dda + sg*dsa*(1-dda))/dra;
		int rb = (db*dda + sb*dsa*(1-dda))/dra;
		if (rr > 255) rr = 255;
		if (rg > 255) rg = 255;
		if (rb > 255) rb = 255;
		
		c.m[3] = ra;
		c.m[2] = rr;
		c.m[1] = rg;
		c.m[0] = rb;
		cout << (int)ra << ' ' << (int)rr << ' ' << (int)rg << ' ' << (int)rb << endl;
	}

	return c;
};