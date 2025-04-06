cmake_minimum_required(VERSION 3.15)
project(RGS LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(HEADERS

	"rgs/Application.h" 
	"rgs/Window.h" 
	"rgs/WindowsWindow.h")

set(SOURCES
	"rgs/Main.cpp"
	"rgs/Application.cpp")

set(TARGET RGS)
add_executable(${TARGET} ${HEADERS} ${SOURCES})