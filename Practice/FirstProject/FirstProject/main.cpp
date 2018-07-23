
#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include <iostream>

void ErrorCallback(int id, const char* description)
{
	std::cout << "glfw error with id: " << id << " and description: " << description << std::endl;
}

int main(int cout, char** args)
{
	glfwSetErrorCallback(&ErrorCallback);
	glfwInit();
	//force to use OpenGL 3.3
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "GLFW could not create a Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	return 0;
}