#pragma once

#include "define.h"
#include "FBO.h"

//UIデザインパーツ格納クラス
class UI_DESIGN {
public:
	FBO fbo;
	char x_pointer_cursor = 0;
	char xy_pointer_cursor = 1;
	char zoom_cursor = 2;
	char unit_line = 3;

	UI_DESIGN();
};