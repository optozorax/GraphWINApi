#ifndef __COLORDEFINE
#define __COLORDEFINE

#include <vector>

using namespace std;

namespace gwapi{

struct Color {
	/* ??? */
	/* Union or UINT32 */
}

Color rgb(int, int, int);
Color argb(int, int, int, int);

Color gray(double);
Color rainbow(double, bool cycle = false, bool gray = false);
Color gradient(double, vector<Color>);

Color operator+(Color, Color);
Color operator*(unsigned char /* double??? */);

}

#endif