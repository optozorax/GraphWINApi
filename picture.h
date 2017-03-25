#ifndef __ANIMATIONDEFINE
#define __ANIMATIONDEFINE

#include <string>
#include "window.h"

using namespace std;

namespace gwapi{

class Animation {
	
public:
	WindowSave(Window);
	~WindowSave();
	
	void savePicture(string = "");
	void makeAnimation(string name, int first = 1000, int each = 33);
}

}

#endif