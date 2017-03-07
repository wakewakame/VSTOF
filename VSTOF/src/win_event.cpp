#include "win_event.h"

WIN_EVENT::WIN_EVENT() {
	l_click = 0;
	b_l_click = 0;
}

bool WIN_EVENT::in(RECT area) {
	if (
		(area.left <= mouse.x) &&
		(area.top <= mouse.y) &&
		(area.right >= mouse.x) &&
		(area.bottom >= mouse.y)
		) {
		return 1;
	}
	return 0;
}

bool WIN_EVENT::in(POINT pos, int size) {
	return in({
		pos.x - size / 2,
		pos.y - size / 2,
		pos.x + size / 2,
		pos.y + size / 2
	});
}

bool WIN_EVENT::get_l_click() {
	return (l_click == 1 && b_l_click == 0);
}

bool WIN_EVENT::l_click_in(RECT area) {
	return (in(area) && get_l_click());
}

void WIN_EVENT::loop() {
	b_l_click = l_click;
	b_mouse = mouse;
}