#pragma once

#include "imgui_app_fw.h"
#include "imgui_app_fw_rendering.h"


#if IMGUI_APP_WIN32_DX11
bool init_gui_win32_dx11();
bool pump_gui_win32_dx11();
void begin_frame_gui_win32_dx11();
void end_frame_gui_win32_dx11(ImVec4 clear_color);
void destroy_gui_win32_dx11();
void set_window_title_win32_dx11(const char* title);
#endif

#if IMGUI_APP_WIN32_DX12
bool init_gui_win32_dx12();
bool pump_gui_win32_dx12();
void begin_frame_gui_win32_dx12();
void end_frame_gui_win32_dx12(ImVec4 clear_color);
void destroy_gui_win32_dx12();
void set_window_title_win32_dx12(const char* title);
#endif

#if IMGUI_APP_GLFW_VULKAN
bool init_gui_glfw_vulkan();
bool pump_gui_glfw_vulkan();
void begin_frame_gui_glfw_vulkan();
void end_frame_gui_glfw_vulkan(ImVec4 clear_color);
void destroy_gui_glfw_vulkan();
void set_window_title_glfw_vulkan(const char* title);
#endif

