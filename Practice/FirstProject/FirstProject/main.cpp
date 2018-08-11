
#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include <iostream>

void ErrorCallback(int id, const char* description)
{
	std::cout << "glfw error with id: " << id << " and description: " << description << std::endl;
}

void Frame_Buffersize_Callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void InputProcess(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//tempoarily code here
unsigned int gVertexShader = 0;
unsigned int gFragmentShader = 0;
unsigned int gFragmentShader2 = 0;
unsigned int gProgram = 0;
unsigned int gProgram2 = 0;

unsigned int gVBO = 0;
unsigned int gVBO2 = 0;

unsigned int gEBO = 0;
unsigned int gEBO2 = 0;

unsigned int gVAO = 0;
unsigned int gVAO2 = 0;

void PrepareData()
{
	// first triangle
	GLfloat data[] = {
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};

	// second triangle
	GLfloat data2[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &gVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data2), data2, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &gEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PrepareShaders()
{
	char* vertexShaderStr = "#version 330 core\n"
							"layout (location = 0) in vec3 aPos;\n"
							"void main()\n"
							"{\n"
							"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
							"}\0";	
	char* fragmentShaderStr = "#version 330 core\n"
								"out vec4 FragColor;\n"
								"void main()\n"
								"{\n"
								"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
								"}\n\0";

	char* fragmentShaderStr2 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 1.0f, 0, 1.0f);\n"
		"}\n\0";

	int success = -2;
	char infoLog[512];
	gVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(gVertexShader, 1, &vertexShaderStr, NULL);
	glCompileShader(gVertexShader);
	glGetShaderiv(gVertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(gVertexShader, 512, NULL, infoLog);
		std::cout << "Compile error: " << infoLog << std::endl;
	}

	success = -2;
	gFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(gFragmentShader, 1, &fragmentShaderStr, NULL);
	glCompileShader(gFragmentShader);
	glGetShaderiv(gFragmentShader, GL_COMPILE_STATUS, &success);

	gProgram = glCreateProgram();
	glAttachShader(gProgram, gVertexShader);
	glAttachShader(gProgram, gFragmentShader);
	glLinkProgram(gProgram);

	glGetProgramiv(gProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(gProgram, 512, NULL, infoLog);
		std::cout << "Link program error: " << infoLog << std::endl;
	}

	gFragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(gFragmentShader2, 1, &fragmentShaderStr2, NULL);
	glCompileShader(gFragmentShader2);

	gProgram2 = glCreateProgram();
	glAttachShader(gProgram2, gVertexShader);
	glAttachShader(gProgram2, gFragmentShader2);
	glLinkProgram(gProgram2);
	
}

void PrepareSteps()
{
	if (gVAO == 0)
		glGenVertexArrays(1, &gVAO);

	glBindVertexArray(gVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
	glBindVertexArray(0);
}

void PrepareSteps2()
{
	if (gVAO2 == 0)
		glGenVertexArrays(1, &gVAO2);

	glBindVertexArray(gVAO2);
	
	glBindBuffer(GL_ARRAY_BUFFER, gVBO2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Draw()
{
	glUseProgram(gProgram);
	glBindVertexArray(gVAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
	glBindVertexArray(0);
}

void Draw2()
{
	glUseProgram(gProgram2);
	glBindVertexArray(gVAO2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

int main(int cout, char** args)
{
	glfwSetErrorCallback(ErrorCallback);
	glfwInit();
	//force to use OpenGL 3.3
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
	
	//Create context window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "GLFW could not create a Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to inintialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, Frame_Buffersize_Callback);

	PrepareData();
	PrepareShaders();
	PrepareSteps();
	PrepareSteps2();

	while (!glfwWindowShouldClose(window))
	{
		//Input
		glfwPollEvents();
		InputProcess(window);

		//Handle logic

		//Render
		glClearColor(0.2, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		Draw();
		Draw2();

		//SwapBuffer
		glfwSwapBuffers(window);
		//glfwPollEvents(); ???
	}

	glfwTerminate();

	return 0;
}