cmake_minimum_required(VERSION 3.15)
project(RGS LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(HEADERS

	"rgs/Application.h" 
	"rgs/Window/Window.h" 
	"rgs/Window/WindowsWindow.h" 
	"rgs/Base/Base.h"
	"rgs/Window/InputCode.h"
	"rgs/Window/Framebuffer.h"
	"rgs/Base/Maths.h"
	"rgs/Shader/BaseShader.h"
	"rgs/Window/Renderer.h"
	"rgs/Shader/BlinnShader.h")

set(SOURCES
	"rgs/Main.cpp"
	"rgs/Application.cpp" 
	"rgs/Window/WindowsWindow.cpp" 
	"rgs/Window/Window.cpp"
	"rgs/Window/Framebuffer.cpp"
	"rgs/Base/Maths.cpp"
	"rgs/Shader/BlinnShader.cpp" "rgs/Window/Renderer.cpp")

set(INCLUDE_PATH
	"src"
)

set(TARGET RGS)
add_executable(${TARGET} ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PRIVATE ${INCLUDE_PATH})