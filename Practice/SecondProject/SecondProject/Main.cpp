
#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include "math.h"
#include "GLContext.h"
#include "GLShader.h"

unsigned int gVBO;
unsigned int gVBO2;
unsigned int gEBO;
unsigned int gVAO;

void PrepareData()
{
	if (gVAO == 0)
		glGenVertexArrays(1, &gVAO);

	// first triangle
	//GLfloat data[] = {
	//	-0.9f, -0.5f, 0.0f,  // left 
	//	-0.0f, -0.5f, 0.0f,  // right
	//	-0.45f, 0.5f, 0.0f,  // top 
	//};

	// second triangle
	//GLfloat data2[] = {
	//	0.0f, -0.5f, 0.0f,  // left
	//	0.9f, -0.5f, 0.0f,  // right
	//	0.45f, 0.5f, 0.0f   // top 
	//};

	/*glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &gVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data2), data2, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	//vertex and color
	GLfloat vertices[] = {
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom left
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top 
	};

	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PrepareSteps()
{
	glBindVertexArray(gVAO);

	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (char*)0 + 3 * sizeof(float));

	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBindVertexArray(0);
}

void updateSomething(GLShader* shader)
{
	int vertexColor = glGetUniformLocation(shader->GetProgramId(), "vertexColor");
	if (vertexColor != -1)
	{
		float time = glfwGetTime();
		float green = (sin(time) / 2.0f) + 0.5f;
		glUniform4f(vertexColor, 0, green, 0, 1);
	}

	int OffsetX = glGetUniformLocation(shader->GetProgramId(), "OffsetX");
	if (OffsetX != -1)
	{
		float time = glfwGetTime();
		float delta = cos(time) / 2.0;
		glUniform1f(OffsetX, delta);
	}
}

void Draw()
{
	glBindVertexArray(gVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

int main(int args, char** argv)
{
	//const char* vertexStr = "#version 330 core\n"
	//	"layout(location = 0) in vec3 aPos;\n"
	//	//"out vec4 vertexColor;\n"
	//	"void main()\n"
	//	"{\n"
	//	"gl_Position = vec4(aPos, 1.0);\n"
	//	"//vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
	//	"}\n";

	//const char* fragmentStr = "#version 330 core\n"
	//	"out vec4 FragColor;\n"
	//	"uniform vec4 vertexColor;\n"
	//	"void main()\n"
	//	"{\n"
	//	"FragColor = vertexColor;\n"
	//	"}\n";

	const char* vertexPath = "VertexShader.glsl";
	const char* fragmentPath = "FragmentShader.glsl";
	GLContext::GetInstance()->CreateContextWindow(800, 600, "Hello Window");
	GLContext::GetInstance()->MakeViewport(0, 0, 800, 600);

	GLShader* shader;

	shader = new GLShader(vertexPath, fragmentPath);

	PrepareData();
	PrepareSteps();

	//Press escape to terminate context
	while (GLContext::GetInstance()->ShouldTerminateContext())
	{
		glfwPollEvents();
		GLContext::GetInstance()->HandleExit();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		shader->ActiveProgram();

		updateSomething(shader);
		Draw();

		GLContext::GetInstance()->SwapBuffers();
	}

	GLContext::ShutDown();
	delete shader;
	return 0;
}