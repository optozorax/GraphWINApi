#include <string>
#include <windows.h>

namespace gwapi {

class Menu;

enum MenuType {
	PopupMenu, Line, Line2BMP, 
	Separator, Break
};

class MenuLine {
	HBITMAP bmp1_, bmp2_, bmp3_;
public:
	MenuType type;
	std::string name;
	Menu *menu;
	unsigned int id;

	struct {
		bool checked;
		bool grayed;
		bool disabled;
		bool rightAlign;
	} flags;
	
	MenuLine(Menu, std::string, bool = false, bool = false, bool = false, bool = false);
	MenuLine(MenuType, std::string = std::string(), unsigned int = 0, bool = false, bool = false, bool = false, bool = false);
	
	MenuLine& SetIcon(int);
	MenuLine& SetTwoIcons(int, int);

	friend class Menu;
};

class Menu{
	HMENU hmenu_;
public:
	Menu();

	void destroy(void);
	
	Menu& pushLine(MenuLine);
	Menu& insertLine(int, MenuLine);
	Menu& deleteLine(int);
	Menu& set(int, MenuLine);
	MenuLine get(int);	
	int size(void);

	friend class Window;
	friend class MenuLine;
};

}