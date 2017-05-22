#include <string>
#include <windows.h>

namespace wgs {

class menu;

enum menu_type {
	PopupMenu, Separator, 
	Line, Line2BMP
};

class menu_line {
	HBITMAP bmp1_, bmp2_, bmp3_;
public:
	menu_type type;
	std::string name;
	menu *popup_menu;
	unsigned int id;

	struct {
		bool checked;
		bool grayed;
		bool rightAlign;
	} flags;
	
	menu_line(menu, std::string, bool = false, bool = false, bool = false);
	menu_line(menu_type, std::string = std::string(), unsigned int = 0, bool = false, bool = false, bool = false);
	
	menu_line& SetIcon(int);
	menu_line& SetTwoIcons(int, int);

	friend class menu;
};

class menu{
	HMENU hmenu_;
public:
	menu();

	void destroy(void);
	
	menu& pushLine(menu_line);
	menu& insertLine(int, menu_line);
	menu& deleteLine(int);
	menu& set(int, menu_line);
	menu_line get(int);	
	int size(void);

	friend class window;
	friend class menu_line;
};

}