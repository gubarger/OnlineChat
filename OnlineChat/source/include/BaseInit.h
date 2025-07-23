#ifndef BASE_INIT_H
#define BASE_INIT_H

// default includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Interface.h"

class BaseInit {
public:
	BaseInit() {}
	int initialize(const char* MainWindowTitle, int& wDisplay, int& hDisplay, Interface* _interface);
	~BaseInit() = default;

private:
	int wDisplay{}, hDisplay{};
};
#endif