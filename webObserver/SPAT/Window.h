#pragma once
#include <glad/glad.h>
#include <iostream>
#include <ostream>

#include <utility>
#include "WindowProps.h"
#include "Input.h"
#include "Logger.h"

class Window final
{
	WindowProps properties;
	GLFWwindow* window;

	void initGLFW()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
public:
	bool running = true;

	Window(WindowProps&& props);
	
	void onUpdate();
	void clear();
	GLFWwindow* getGLFWwindow() const;

	void close();
	~Window();
};

namespace Events
{
	inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	
}

inline Window::Window(WindowProps&& props): properties(std::move(props))
{
	initGLFW();
	window = glfwCreateWindow(props.width, props.height, props.name.c_str(), NULL, NULL);
	
	if (window == nullptr)
	{
		Logger::log("Failed to create GLFW window");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		Logger::log("Failed to initialize GLAD");
	}

	glfwSetFramebufferSizeCallback(window, Events::framebuffer_size_callback);
	glfwSetKeyCallback(window, Input::keyCallback);
}

inline void Window::onUpdate()
{
	Input::poll();
	running = !glfwWindowShouldClose(window);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

inline void Window::clear()
{
	glClearColor(0.2f, 0.2f, 0.22f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

inline GLFWwindow* Window::getGLFWwindow() const
{
	return window;
}

inline void Window::close()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

inline Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
