#include <GL/glew.h>
#include <GLFW/glfw3.h>  
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <string>
#include <functional>
#include <vector>
#include <iostream>
#include "framework.h"

using namespace fw;
using namespace std;

Application* Application::instance_ = NULL;

void Application::Init()
{
	instance_ = this;
	bool glfw_inited = false;
	try{
		GLenum err = glfwInit();
		if (!err){
			throw runtime_error((const char*)glewGetErrorString(err));
		}

		glfw_inited = true;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glEnable(GL_MULTISAMPLE);

		// create window
		window_ = glfwCreateWindow(window_width_, window_height_, title_.c_str(), NULL, NULL);
		if (!window_)
			throw runtime_error("create window failed");

		/* Make the window's context current */
		glfwMakeContextCurrent(window_);

		if (glewInit() != GLEW_OK)
			throw runtime_error("init glew failed");

		// input proc
		glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetKeyCallback(window_, KeyCallback);
		glfwSetCursorPosCallback(window_, MouseCallback);
		double xpos, ypos;
		glfwGetCursorPos(window_, &xpos, &ypos);
		pre_xpos_ = float(xpos);
		pre_ypos_ = float(ypos);
	}
	catch (const exception& e){
		if (glfw_inited)
			glfwTerminate();
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}

void Application::Shutdown()
{
	glfwDestroyWindow(window_);
	glfwTerminate();
}

void Application::Run()
{
	Init();
	double lasttime = glfwGetTime();
	try{
		// the window is closed?
		while (!glfwWindowShouldClose(window_)){
			// time calculate
			double curtime = glfwGetTime();
			double deltatime = curtime - lasttime;
			lasttime = curtime;

			// Poll for and process events
			glfwPollEvents();

			this->OnUpdate(float(deltatime));
			
			// Swap front and back buffers
			glfwSwapBuffers(window_);

		}
	}
	catch (const exception& e){
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	catch (...){
		std::cerr << "unknow error" << std::endl;
		exit(1);
	}
	Shutdown();
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* instance = Application::instance_;
	InputEvent e;
	if (action == GLFW_PRESS)
		e.action = KEY_DOWN;
	else if (action == GLFW_RELEASE)
		e.action = KEY_UP;
	else
		return;
	e.key = key;
	instance->OnInput(e);
}


void Application::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Application* instance = Application::instance_;
	
	InputEvent e;
	e.action = MOUSE_MOVE;
	e.xpos = float(xpos);
	e.ypos = float(ypos);
	e.dx = float(xpos - instance->pre_xpos_);
	e.dy = float(ypos - instance->pre_ypos_);
	instance->pre_xpos_ = float(xpos);
	instance->pre_ypos_ = float(ypos);
	instance->OnInput(e);
}
