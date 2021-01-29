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

	CPMAddPackage(
	  NAME cpmpckg
	  SOURCE_DIR "C:/projects/loopunit/cpmpckg"
	  #GITHUB_REPOSITORY loopunit/cpmpckg
	  #GIT_TAG xxx
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
		cpm_runtime::glfw cpm_runtime::framegraph cpm_runtime::basis_universal)

target_compile_definitions(imgui_app_fw
	PUBLIC
		IMGUI_APP_GLFW_VULKAN=1)

set_target_properties(imgui_app_fw PROPERTIES CXX_STANDARD 17)

packageProject(
	# the name of the target to export
	NAME imgui_app_fw
	# the version of the target to export
	VERSION ${PROJECT_VERSION}
	# a temporary directory to create the config files
	BINARY_DIR ${PROJECT_BINARY_DIR}
	# location of the target's public headers
	INCLUDE_DIR ${imgui_app_fw_SOURCE_ROOT}/include
	# should match the target's INSTALL_INTERFACE include directory
	INCLUDE_DESTINATION include
	# semicolon separated list of the project's dependencies
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

CPMAddPackage(
	NAME implot
	GITHUB_REPOSITORY epezent/implot
	GIT_TAG 862b8c44ea6d62d736e320b7a11be8438ecbac5d
	DOWNLOAD_ONLY true)

if (NOT implot_ADDED)
	message( FATAL_ERROR "Could not find implot" )
endif()

file(GLOB implot_public_headers 
	"${implot_SOURCE_DIR}/implot.h"
	"${implot_SOURCE_DIR}/implot_internal.h"
)

file(GLOB implot_source
	"${implot_SOURCE_DIR}/*.cpp"
)

#############

CPMAddPackage(
	NAME ImGuiColorTextEdit
	GITHUB_REPOSITORY BalazsJako/ImGuiColorTextEdit
	GIT_TAG 0a88824f7de8d0bd11d8419066caa7d3469395c4
	DOWNLOAD_ONLY true)

if (NOT ImGuiColorTextEdit_ADDED)
	message( FATAL_ERROR "Could not find ImGuiColorTextEdit" )
endif()

file(GLOB ImGuiColorTextEdit_public_headers 
	"${ImGuiColorTextEdit_SOURCE_DIR}/TextEditor.h"
)

file(GLOB ImGuiColorTextEdit_source
	"${ImGuiColorTextEdit_SOURCE_DIR}/*.cpp"
)

#############

CPMAddPackage(
	NAME imgui_markdown
	GITHUB_REPOSITORY juliettef/imgui_markdown
	GIT_TAG 2b33c435b4463999438a5532f1dc201c142a2aab
	DOWNLOAD_ONLY true)

if (NOT imgui_markdown_ADDED)
	message( FATAL_ERROR "Could not find imgui_markdown" )
endif()

file(GLOB imgui_markdown_public_headers 
	"${imgui_markdown_SOURCE_DIR}/imgui_markdown.h"
)

#############

CPMAddPackage(
	NAME imgui_node_editor
	GITHUB_REPOSITORY thedmd/imgui-node-editor
	GIT_TAG 687a72f940c76cf5064e13fe55fa0408c18fcbe4
	DOWNLOAD_ONLY true)

if (NOT imgui_node_editor_ADDED)
	message( FATAL_ERROR "Could not find imgui_node_editor" )
endif()

file(GLOB imgui_node_editor_public_headers 
	"${imgui_node_editor_SOURCE_DIR}/*.h"
	"${imgui_node_editor_SOURCE_DIR}/*.inl"
)

file(GLOB imgui_node_editor_source
	"${imgui_node_editor_SOURCE_DIR}/*.cpp"
)

#############

CPMAddPackage(
	NAME imgui_console
	GITHUB_REPOSITORY rmxbalanque/imgui-console
	GIT_TAG a9a1f83b016c3b455f2b88361aa2b79f25f3f961
	DOWNLOAD_ONLY true)


if (NOT imgui_console_ADDED)
	message( FATAL_ERROR "Could not find imgui_console" )
endif()

file(GLOB imgui_console_public_headers 
	"${imgui_console_SOURCE_DIR}/include/imgui_console/imgui_console.h"
)

file(GLOB imgui_console_public_headers_csys
	"${imgui_console_SOURCE_DIR}/include/csys/*.*"
)

file(GLOB imgui_console_source
	"${imgui_console_SOURCE_DIR}/src/imgui_console.cpp"
)

#############

CPMAddPackage(
	NAME ImGuiFileDialog
	GITHUB_REPOSITORY aiekick/ImGuiFileDialog
	GIT_TAG 27cf103c3be5fe11084a07c902c4c180d4e0fff6
	DOWNLOAD_ONLY true)

if (NOT ImGuiFileDialog_ADDED)
	message( FATAL_ERROR "Could not find ImGuiFileDialog" )
endif()

file(GLOB ImGuiFileDialog_public_headers 
	"${ImGuiFileDialog_SOURCE_DIR}/ImGuiFileDialog/ImGuiFileDialog.h"
	"${ImGuiFileDialog_SOURCE_DIR}/ImGuiFileDialog/ImGuiFileDialogConfig.h"
	"${ImGuiFileDialog_SOURCE_DIR}/3rdparty/dirent/include/dirent.h")

file(GLOB ImGuiFileDialog_source
	"${ImGuiFileDialog_SOURCE_DIR}/ImGuiFileDialog/*.cpp")

#############

set(STAGING_ROOT ${CMAKE_CURRENT_BINARY_DIR}/imgui_addons_staging)

file(COPY 
	${implot_public_headers}
	${ImGuiColorTextEdit_public_headers}
	${imgui_markdown_public_headers}
	${imgui_node_editor_public_headers}
	${ImGuiFileDialog_public_headers} 
	${imgui_console_public_headers}
	DESTINATION ${STAGING_ROOT}/include)

file(COPY 
	${ImTerm_public_headers}
	DESTINATION ${STAGING_ROOT}/include/ImTerm)

file(COPY 
	${imgui_console_public_headers_csys}
	DESTINATION ${STAGING_ROOT}/include/csys)


file(COPY 
	${implot_source}
	${ImGuiColorTextEdit_source}
	${imgui_node_editor_source}
	${ImGuiFileDialog_source}
	${imgui_console_source}
	DESTINATION ${STAGING_ROOT}/src)

file(GLOB sources ${STAGING_ROOT}/src/*.*)

add_library(imgui_addons STATIC ${sources})

set_target_properties(imgui_addons PROPERTIES CXX_STANDARD 20)

target_link_libraries(imgui_addons PRIVATE cpm_runtime::imgui)

target_include_directories(imgui_addons PUBLIC $<BUILD_INTERFACE:${STAGING_ROOT}/include>
					$<INSTALL_INTERFACE:imgui_addons>)

target_compile_definitions(imgui_addons PUBLIC SPDLOG_COMPILED_LIB SPDLOG_FMT_EXTERNAL)

packageProject(
	# the name of the target to export
	NAME imgui_addons
	# the version of the target to export
	VERSION ${PROJECT_VERSION}
	# a temporary directory to create the config files
	BINARY_DIR ${PROJECT_BINARY_DIR}
	# location of the target's public headers
	INCLUDE_DIR ${STAGING_ROOT}/include
	# should match the target's INSTALL_INTERFACE include directory
	INCLUDE_DESTINATION include
	# semicolon separated list of the project's dependencies
	DEPENDENCIES ""
)

add_library(cpm_runtime::imgui_addons ALIAS imgui_addons)

################

if(IMGUI_BUILD_EXAMPLES)
	file(GLOB addon_example_sources 
		${CMAKE_CURRENT_LIST_DIR}/examples/addon_main.cpp)

	add_executable(imgui_addons_example
		${addon_example_sources})

	set_target_properties(imgui_addons_example PROPERTIES CXX_STANDARD 20)

	target_include_directories(imgui_addons_example
		PRIVATE 
			${imgui_SOURCE_DIR}/examples)

	target_link_libraries(imgui_addons_example
		PUBLIC
			cpm_runtime::imgui_app_fw imgui_addons)
endif()

###################

file(GLOB imgui_app_sources 
	${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_app_internal.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_app_logger.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_app_logger.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_app.cpp)

###################

CPMAddPackage(
NAME atomic_queue
	GITHUB_REPOSITORY max0x7ba/atomic_queue
	GIT_TAG df794032353a9fbf6204481bdd53ef0263ce51f3 
	DOWNLOAD_ONLY true)

if( NOT ${atomic_queue_ADDED} )
	message( FATAL_ERROR "Could not find atomic_queue" )
endif()

install(
	DIRECTORY ${atomic_queue_SOURCE_DIR}/include/atomic_queue
	DESTINATION include)

list(APPEND imgui_public_include_paths ${atomic_queue_SOURCE_DIR}/include)

#############

CPMAddPackage(
	NAME spdlog
	GITHUB_REPOSITORY gabime/spdlog
	GIT_TAG 18e3f07f7d333d33c0ef8fbea02299dbe5d49f97 
	DOWNLOAD_ONLY true)

if( NOT ${spdlog_ADDED} )
	message( FATAL_ERROR "Could not find spdlog" )
endif()

install(
	DIRECTORY ${spdlog_SOURCE_DIR}/include/spdlog
	DESTINATION include)

file(GLOB spdlog_source 
	${spdlog_SOURCE_DIR}/src/*.cpp)

list(APPEND imgui_public_include_paths ${spdlog_SOURCE_DIR}/include)
list(APPEND imgui_app_sources ${spdlog_source})

#############

CPMAddPackage(
	NAME fmt
	GITHUB_REPOSITORY fmtlib/fmt
	GIT_TAG af283059612dd06c321dc509255ae9b003ca5e69
	DOWNLOAD_ONLY true)

if( NOT ${fmt_ADDED} )
	message( FATAL_ERROR "Could not find fmt" )
endif()

install(
	DIRECTORY ${fmt_SOURCE_DIR}/include/fmt
	DESTINATION include)

file(GLOB fmt_source 
	${fmt_SOURCE_DIR}/src/*.cc)

list(APPEND imgui_public_include_paths ${fmt_SOURCE_DIR}/include)
list(APPEND imgui_app_sources ${fmt_source})

#############

CPMAddPackage(
	NAME tinyfsm
	GITHUB_REPOSITORY digint/tinyfsm
	GIT_TAG 06f27072266d6c34d990f910c4c43fb11240501c
	DOWNLOAD_ONLY true)

if( NOT ${tinyfsm_ADDED} )
	message( FATAL_ERROR "Could not find tinyfsm" )
endif()

install(
	FILES ${tinyfsm_SOURCE_DIR}/include/tinyfsm.hpp
	DESTINATION include)

list(APPEND imgui_public_include_paths ${tinyfsm_SOURCE_DIR}/include)

#############

CPMAddPackage(
	NAME nfd
	GITHUB_REPOSITORY native-toolkit/nfd
	GIT_TAG e9e3636b031797396aa502b78432ae1347c2afe3
	DOWNLOAD_ONLY true)

if( NOT ${nfd_ADDED} )
	message( FATAL_ERROR "Could not find nfd" )
endif()

install(
	FILES ${nfd_SOURCE_DIR}/src/include/nfd.h
	DESTINATION include)

file(GLOB nfd_source 
	${nfd_SOURCE_DIR}/src/nfd_win.cpp
	${nfd_SOURCE_DIR}/src/common.h
	${nfd_SOURCE_DIR}/src/nfd_common.h
	${nfd_SOURCE_DIR}/src/nfd_common.c)

list(APPEND imgui_public_include_paths ${nfd_SOURCE_DIR}/src)
list(APPEND imgui_public_include_paths ${nfd_SOURCE_DIR}/src/include)

list(APPEND imgui_app_sources ${nfd_source})

#############

CPMAddPackage(
	NAME boxer
	GITHUB_REPOSITORY aaronmjacobs/Boxer
	GIT_TAG 01c17f6a9cd66068f7890ea887ab3b9a673f0434
	DOWNLOAD_ONLY true)

if( NOT ${boxer_ADDED} )
	message( FATAL_ERROR "Could not find boxer" )
endif()

install(
	DIRECTORY ${boxer_SOURCE_DIR}/include/boxer
	DESTINATION include)

if(APPLE)
	file(GLOB boxer_source 
		${boxer_SOURCE_DIR}/src/boxer_osx.mm)
elseif (WIN32)
	file(GLOB boxer_source 
		${boxer_SOURCE_DIR}/src/boxer_win.cpp)
elseif (LINUX)
	file(GLOB boxer_source 
		${boxer_SOURCE_DIR}/src/boxer_linux.cpp)
endif()

list(APPEND imgui_public_include_paths ${boxer_SOURCE_DIR}/include)

list(APPEND imgui_app_sources ${boxer_source})

#############

list(APPEND imgui_private_include_paths ${PROJECT_BINARY_DIR}/include)

add_library(imgui_app STATIC ${imgui_app_sources})

target_link_libraries(imgui_app PUBLIC cpm_runtime::imgui_app_fw cpm_runtime::imgui_addons)

target_include_directories(imgui_app PRIVATE ${imgui_private_include_paths})
target_include_directories(imgui_app PUBLIC ${imgui_public_include_paths})

set_target_properties(imgui_app PROPERTIES CXX_STANDARD 17)

packageProject(
	# the name of the target to export
	NAME imgui_app
	# the version of the target to export
	VERSION ${PROJECT_VERSION}
	# a temporary directory to create the config files
	BINARY_DIR ${PROJECT_BINARY_DIR}
	# location of the target's public headers
	INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include
	# should match the target's INSTALL_INTERFACE include directory
	INCLUDE_DESTINATION include
	# semicolon separated list of the project's dependencies
	DEPENDENCIES "")
  
	if(IMGUI_BUILD_EXAMPLES)
	file(GLOB example_sources 
		${CMAKE_CURRENT_SOURCE_DIR}/examples/app_main.cpp)

	add_executable(imgui_app_example
		${example_sources})
    
	set_target_properties(imgui_app_example PROPERTIES CXX_STANDARD 17)

	target_include_directories(imgui_app_example
		PRIVATE 
			${imgui_SOURCE_DIR}/examples)
    
	target_link_libraries(imgui_app_example
		PUBLIC
			imgui_app)
endif()