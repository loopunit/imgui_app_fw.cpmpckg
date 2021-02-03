cmake_minimum_required( VERSION 3.12 )

project(imgui_app_fw
	VERSION 1.7.8)

include(CMakeDependentOption)

option(IMGUI_BUILD_EXAMPLES "Build examples." OFF)
cmake_dependent_option(IMGUI_BUILD_APP_FW "Build app framework." OFF "IMGUI_BUILD_EXAMPLES" ON)

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

include(ExternalProject)

####

if (NOT DEFINED CPM_SCRIPTS)
	include(cmake/CPM.cmake)

	# One frustrating part of this project setup is the circular dependency between cpmpckg and this repo.
	# GIT_TAG will always lag cpmpckg @ HEAD when this project is updated there.
	CPMAddPackage(
		NAME cpmpckg
		GITHUB_REPOSITORY loopunit/cpmpckg
		GIT_TAG 137aed86a29e21a791420f5e454cfd0d8c0e30dc
		DOWNLOAD_ONLY true)

	include(${cpmpckg_SOURCE_DIR}/cmake/add_cpm_module.cmake)
else()
	include(${CPM_SCRIPTS}/CPM.cmake)
	include(${CPM_SCRIPTS}/add_cpm_module.cmake)
endif()

####

CPMAddPackage(
  NAME PackageProject.cmake
  GITHUB_REPOSITORY loopunit/PackageProject.cmake
  GIT_TAG e5ec20069766f4f078f9f01a86e250e20da0817c)

####

CPMAddBaseModule(imgui)
CPMAddBaseModule(framegraph)
CPMAddBaseModule(basis_universal)
CPMAddBaseModule(glfw)
CPMAddBaseModule(mu_stdlib)

set(imgui_app_fw_SOURCE_ROOT ${CMAKE_CURRENT_LIST_DIR})

file(GLOB app_fw_impl_headers)
file(GLOB app_fw_impl_sources)

# Hijacking source from the imgui project, so the imgui cpm module isn't applicable
CPMAddPackage(
	NAME imgui
	GITHUB_REPOSITORY ocornut/imgui
	GIT_TAG ac08593b9645aee7e086b1e9b98a6a1d79d09210
	DOWNLOAD_ONLY TRUE)

if(NOT ${imgui_ADDED})
	message( FATAL_ERROR "Could not find imgui" )
endif()

file(GLOB app_fw_impl_sources2 
	"${imgui_app_fw_SOURCE_ROOT}/src/glfw_vulkan/VulkanDevice2.h"
	"${imgui_app_fw_SOURCE_ROOT}/src/glfw_vulkan/VulkanDevice2.cpp"
	"${imgui_app_fw_SOURCE_ROOT}/src/glfw_vulkan/main.cpp")

list(APPEND app_fw_impl_sources ${app_fw_impl_sources2})

file(GLOB app_fw_headers
	"${imgui_app_fw_SOURCE_ROOT}/include/*.h")

file(GLOB app_fw_sources
	"${imgui_app_fw_SOURCE_ROOT}/src/imgui_app_fw.cpp"
	"${imgui_app_fw_SOURCE_ROOT}/src/imgui_app_fw_impl.h")

add_library(imgui_app_fw STATIC ${app_fw_sources} ${app_fw_impl_sources} ${app_fw_headers} ${app_fw_impl_headers})

target_include_directories(imgui_app_fw 
	PRIVATE 
		${imgui_SOURCE_DIR}/backends)

target_include_directories(imgui_app_fw PUBLIC $<BUILD_INTERFACE:${imgui_app_fw_SOURCE_ROOT}/include>
					$<INSTALL_INTERFACE:imgui_app_fw>)

target_link_libraries(imgui_app_fw
	PUBLIC
		cpm_runtime::imgui)

target_link_libraries(imgui_app_fw
	PUBLIC
		cpm_runtime::glfw cpm_runtime::framegraph cpm_runtime::basis_universal cpm_runtime::mu_stdlib)

set_target_properties(imgui_app_fw PROPERTIES CXX_STANDARD 17)

packageProject(
	NAME imgui_app_fw
	VERSION ${PROJECT_VERSION}
	BINARY_DIR ${PROJECT_BINARY_DIR}
	INCLUDE_DIR ${imgui_app_fw_SOURCE_ROOT}/include
	INCLUDE_DESTINATION include
	DEPENDENCIES "imgui")

add_library(cpm_runtime::imgui_app_fw ALIAS imgui_app_fw)

if(IMGUI_BUILD_EXAMPLES)
	file(GLOB example_sources 
		${CMAKE_CURRENT_LIST_DIR}/examples/main.cpp)

	add_executable(imgui_example
		${example_sources})

	set_target_properties(imgui_example PROPERTIES CXX_STANDARD 17)

	target_include_directories(imgui_example 
		PRIVATE 
			${imgui_SOURCE_DIR}/examples)

	target_link_libraries(imgui_example 
		PUBLIC
			cpm_runtime::imgui_app_fw)
endif()
