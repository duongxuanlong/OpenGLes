#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include "GLContext.h"
#include <iostream>

GLContext* GLContext::m_Instace = nullptr;

typedef void(*DrawCall) (GLContext& context);

GLContext::GLContext()
{
	m_IsInit = false;
	//Init glfw
	glfwInit();
}

GLContext::~GLContext()
{
	glfwTerminate();
}

GLContext* GLContext::GetInstance()
{
	if (GLContext::m_Instace == nullptr)
		GLContext::m_Instace = new GLContext();

	
	return GLContext::m_Instace;
}

void GLContext::CreateContextWindow(int width, int height, const char* title)
{
	if (m_IsInit)
		return;
	m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (m_Window == NULL)
	{
		std::cout << "Error with could not create glfw window" << std::endl;
		return;
	}
	glfwMakeContextCurrent(m_Window);

	//Init glad
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	m_IsInit = true;
}

void GLContext::MakeViewport(int offsetX, int offsetY, int width, int height)
{
	glViewport(offsetX, offsetY, width, height);
}

void GLContext::SwapBuffers()
{
	glfwSwapBuffers(m_Window);
}

GLFWwindow* GLContext::GetContextWindow()
{
	return m_Window;
}

void GLContext::HandleExit()
{
	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_Window, true);
	}
}

bool GLContext::ShouldTerminateContext()
{
	return !glfwWindowShouldClose(m_Window);
}

void GLContext::ShutDown()
{
	if (m_Instace != nullptr)
	{
		delete m_Instace;
		m_Instace = nullptr;
	}
}

