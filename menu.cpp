#include "menu.h"

void gwapi::Menu::destroy(void) {
	DestroyMenu(hmenu_);
}

gwapi::Menu& gwapi::Menu::deleteLine(int i) {
	RemoveMenu(hmenu_, i, MF_BYPOSITION);
	return *this;
}

int gwapi::Menu::size(void) {
	return GetMenuItemCount(hmenu_);
}

gwapi::MenuLine::MenuLine(MenuType tp, std::string nm, unsigned int ID, bool chck, bool grd, bool dsb, bool ral) : 
	bmp1_(NULL), 
	bmp2_(NULL),
	bmp3_(NULL)
{
	type = tp;
	if (type != Separator && type != Break) {
		name = nm;
		if (type != PopupMenu) {
			menu = NULL;
			id = ID;
			flags.checked = chck;
			flags.grayed = grd;
			flags.disabled = dsb;
			flags.rightAlign = ral;
		}
	}
}

gwapi::MenuLine::MenuLine(Menu mn, std::string nm, bool chck, bool grd, bool dsb, bool ral) {
	type = PopupMenu;
	menu = new Menu;
	menu->hmenu_ = mn.hmenu_;
	name = nm;
	flags.checked = chck;
	flags.grayed = grd;
	flags.disabled = dsb;
	flags.rightAlign = ral;
}

gwapi::Menu::Menu() {
	hmenu_ = CreateMenu();
}

HBITMAP Icon2Bmp(HICON hicon) {
	/* Преобразует иконку hicon в формат точечного рисунка, причем рисунок имеет размеры 18x18, и его фон такой же, как и у пункта меню. */
	HBITMAP hbmp;
	HDC hdcBmp;
	HBRUSH menuBrush;
	MENUITEMINFO minf;        

	/* Создание дексриптора для рисунка. */
	hdcBmp = CreateCompatibleDC(NULL);

	/* Создание кисти. */
	menuBrush = CreateSolidBrush(GetSysColor(COLOR_MENU));
	SelectObject(hdcBmp, menuBrush); 

	/* Создание самого рисунка. */
	hbmp = CreateCompatibleBitmap(hdcBmp, 18, 18); 
	SelectObject(hdcBmp, hbmp);

	/* Закраска фона цветом меню. */
	ExtFloodFill(hdcBmp, 0, 0, GetSysColor(COLOR_MENU), FLOODFILLBORDER); 

	/* Рисование иконки. */
	SelectObject(hdcBmp, hbmp); 
	DrawIconEx(hdcBmp, 1,1, hicon, 16,16, NULL, CreateSolidBrush( GetSysColor(COLOR_MENU) ), DI_NORMAL | DI_COMPAT);

	/* Очищение памяти. */
	DeleteDC(hdcBmp);

	/* Возвращение рисунка. */
	return hbmp;
}

gwapi::MenuLine& gwapi::MenuLine::SetIcon(int x) {
	bmp1_ = Icon2Bmp(LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(x)));
	return *this;
}

gwapi::MenuLine& gwapi::MenuLine::SetTwoIcons(int x, int y) {
	bmp2_ = Icon2Bmp(LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(x)));
	bmp3_ = Icon2Bmp(LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(y)));
	return *this;
}

gwapi::Menu& gwapi::Menu::pushLine(MenuLine ln) {
	if (ln.type == Separator)
		AppendMenu(hmenu_, MF_SEPARATOR, 0, "");
	else
	if (ln.type == Break)
		AppendMenu(hmenu_, MF_MENUBARBREAK, 0, "");
	else {
		if (ln.type == PopupMenu)
			AppendMenu(hmenu_, MF_STRING | MF_POPUP, (UINT)(ln.menu->hmenu_), ln.name.c_str());
		if ((ln.type == Line) || (ln.type == Line2BMP))
			AppendMenu(hmenu_, MF_STRING, ln.id, ln.name.c_str());
		set(size(), ln);
	}
	return *this;
}

gwapi::Menu& gwapi::Menu::insertLine(int x, MenuLine ln) {
	if (ln.type == Separator)
		InsertMenu(hmenu_, x, MF_BYPOSITION | MF_SEPARATOR, 0, "");
	else
		if (ln.type == Break)
			InsertMenu(hmenu_, x, MF_BYPOSITION | MF_MENUBARBREAK, 0, "");
		else {
			if (ln.type == PopupMenu)
				InsertMenu(hmenu_, x, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT)(ln.menu->hmenu_), ln.name.c_str());
			if (ln.type == Line || ln.type == Line2BMP)
				InsertMenu(hmenu_, x, MF_BYPOSITION | MF_STRING, ln.id, ln.name.c_str());
			set(size(), ln);
		}
		return *this;
}

gwapi::Menu& gwapi::Menu::set(int x, MenuLine ln) {
	MENUITEMINFO lpmii;
	ZeroMemory(&lpmii, sizeof(MENUITEMINFO));
	lpmii.cbSize = sizeof(MENUITEMINFO);
	if (ln.bmp1_ != NULL)
		lpmii.fMask |= MIIM_BITMAP;
	if (ln.bmp2_ != NULL)
		lpmii.fMask |= MIIM_CHECKMARKS;
	if (ln.menu != NULL)
		lpmii.fMask |= MIIM_SUBMENU;
	lpmii.fMask |= MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING;
	if (ln.type == Break)
		lpmii.fType |= MFT_MENUBARBREAK;
	if (ln.type == Separator)
		lpmii.fType |= MFT_SEPARATOR;
	if (ln.flags.rightAlign)
		lpmii.fType |= MFT_RIGHTJUSTIFY;
	if (ln.flags.checked)
		lpmii.fState |= MFS_CHECKED;
	else
		lpmii.fState |= MFS_UNCHECKED;
	if (ln.flags.disabled)
		lpmii.fState |= MFS_DISABLED;
	else
		lpmii.fState |= MFS_ENABLED;
	if (ln.flags.grayed)
		lpmii.fState |= MFS_GRAYED;
	lpmii.wID = ln.id;
	if (ln.type == PopupMenu)
		lpmii.hSubMenu = ln.menu->hmenu_;
	if (ln.bmp2_ != NULL) {
		lpmii.hbmpChecked = ln.bmp2_;
		lpmii.hbmpUnchecked = ln.bmp3_;
	}
	lpmii.dwTypeData = (LPSTR)ln.name.c_str();
	lpmii.cch = ln.name.size();
	if (ln.bmp1_ != NULL)
		lpmii.hbmpItem = ln.bmp1_;
	SetMenuItemInfo(hmenu_, x-1, TRUE, &lpmii);
	return *this;
}