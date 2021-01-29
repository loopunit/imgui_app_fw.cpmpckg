#pragma once

#include "imgui_app_fw.h"
#include "imgui_app_fw_rendering.h"

bool init_gui_glfw_vulkan();
bool pump_gui_glfw_vulkan();
void begin_frame_gui_glfw_vulkan();
void end_frame_gui_glfw_vulkan(ImVec4 clear_color);
void destroy_gui_glfw_vulkan();
void set_window_title_glfw_vulkan(const char* title);
