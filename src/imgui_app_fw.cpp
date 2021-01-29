#include "imgui_app_fw_impl.h"

namespace imgui_app_fw
{
	static bool init_gui_null()
	{
		return false;
	}

	static bool pump_gui_null()
	{
		return false;
	}

	static void begin_frame_gui_null()
	{
	}

	static void end_frame_gui_null(ImVec4 clear_color)
	{
	}

	static void destroy_gui_null()
	{
	}

	void set_window_title_null(const char* title)
	{
	}

	static auto init_gui_impl = init_gui_null;
	static auto pump_gui_impl = pump_gui_null;
	static auto begin_frame_gui_impl = begin_frame_gui_null;
	static auto end_frame_gui_impl = end_frame_gui_null;
	static auto destroy_gui_impl = destroy_gui_null;
	static auto set_window_title_impl = set_window_title_null;

	bool select_platform()
	{
		init_gui_impl = init_gui_glfw_vulkan;
		pump_gui_impl = pump_gui_glfw_vulkan;
		begin_frame_gui_impl = begin_frame_gui_glfw_vulkan;
		end_frame_gui_impl = end_frame_gui_glfw_vulkan;
		destroy_gui_impl = destroy_gui_glfw_vulkan;
		set_window_title_impl = set_window_title_glfw_vulkan;
		return true;
	}

	bool init()
	{
		return init_gui_impl();
	}

	bool pump()
	{
		return pump_gui_impl();
	}

	void begin_frame()
	{
		begin_frame_gui_impl();
	}

	void end_frame(ImVec4 clear_color)
	{
		end_frame_gui_impl(clear_color);
	}

	void destroy()
	{
		return destroy_gui_impl();
	}

	void set_window_title(const char* title)
	{
		set_window_title_impl(title);
	}
}

