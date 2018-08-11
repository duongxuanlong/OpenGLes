#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include "GLContext.h"
#include "GLShader.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int gVBO;
unsigned int gEBO;

unsigned int gTextureID1;
unsigned int gTextureID2;

int gPos = -1;
int gColor = -1;
int gTexCoordinate = -1;
int gUniformTex1 = -1;
int gUniformTex2 = -1;
int gUniformTranFormMatrix = -1;

void PrepareData()
{
	glGenBuffers(1, &gVBO);
	glGenBuffers(1, &gEBO);

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//draw by indices
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PrepareTexture()
{
	glGenTextures(1, &gTextureID1);
	glGenTextures(1, &gTextureID2);

	stbi_set_flip_vertically_on_load(true);
	int width, height, channel;

	glBindTexture(GL_TEXTURE_2D, gTextureID1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* data = stbi_load("Brick.jpg", &width, &height, &channel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, gTextureID2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	data = stbi_load("awesomeface.png", &width, &height, &channel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void AccessLoation(GLShader* shader)
{
	if (gPos == -1)
	{
		gPos = glGetAttribLocation(shader->GetProgramId(), "aPos");
	}

	if (gColor == -1)
		gColor = glGetAttribLocation(shader->GetProgramId(), "aColor");

	if (gTexCoordinate == -1)
		gTexCoordinate = glGetAttribLocation(shader->GetProgramId(), "aTexCoordinate");

	if (gUniformTex1 == -1)
		gUniformTex1 = glGetUniformLocation(shader->GetProgramId(), "Texture1");

	if (gUniformTex2 == -1)
		gUniformTex2 = glGetUniformLocation(shader->GetProgramId(), "Texture2");

	if (gUniformTranFormMatrix == -1)
		gUniformTranFormMatrix = glGetUniformLocation(shader->GetProgramId(), "TranFormMatrix");
	
}

void UpdateSomething(GLShader* shader)
{
	if (gUniformTranFormMatrix != -1)
	{
		// Scale * (RotateZ * RotateX * RotateY) * Translate
		glm::mat4 matrix;
		//matrix = glm::rotate(matrix, (float)glfwGetTime(), glm::vec3(0, 0, 1.0f));
		matrix = glm::translate(matrix, glm::vec3(0.5f, -0.5f, 0.0f));
		matrix = glm::rotate(matrix, (float)glfwGetTime(), glm::vec3(0, 0, 1.0f));
		
		//matrix = glm::scale(matrix, glm::vec3(0.5f, 0.5f, 0.5f));

		glUniformMatrix4fv(gUniformTranFormMatrix, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}

void Draw(GLShader* shader)
{
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	if (gPos != -1)
	{
		glEnableVertexAttribArray(gPos);
		glVertexAttribPointer(gPos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	}

	if (gColor != -1)
	{
		glEnableVertexAttribArray(gColor);
		glVertexAttribPointer(gColor, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	}

	if (gTexCoordinate != -1)
	{
		glEnableVertexAttribArray(gTexCoordinate);
		glVertexAttribPointer(gTexCoordinate, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//shader->ActiveProgram();
	int textureUnit = 0;
	if (gUniformTex1 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, gTextureID1);
		glUniform1i(gUniformTex1, textureUnit);
		++textureUnit;
	}

	if (gUniformTex2 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, gTextureID2);
		glUniform1i(gUniformTex2, textureUnit);
		++textureUnit;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


int main(int cout, char** argv)
{
	GLContext::GetInstance()->CreateContextWindow(800, 600, "Transformation Matrix");
	GLContext::GetInstance()->MakeViewport(0, 0, 800, 600);

	GLShader* shader = new GLShader("GLSL_VertesShader.glsl", "GLSL_FragmentShader.glsl");

	PrepareData();
	PrepareTexture();
	
	AccessLoation(shader);

	while (GLContext::GetInstance()->ShouldTerminateContext())
	{
		glfwPollEvents();
		GLContext::GetInstance()->HandleExit();

		glClearColor(0.2, 0.5, 0.3, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		shader->ActiveProgram();
		UpdateSomething(shader);
		Draw(shader);

		GLContext::GetInstance()->SwapBuffers();
	}

	GLContext::GetInstance()->ShutDown();
	delete shader;
	return -1;
}