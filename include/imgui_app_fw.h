#pragma once

#include <imgui.h>

namespace imgui_app_fw
{
	bool select_platform();
	void set_window_title(const char* title);

	bool init();
	bool pump();
	void begin_frame();
	void end_frame(ImVec4 clear_color);
	void destroy();
}
