#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include "GLContext.h"
#include "GLShader.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include "windows.h"

#include <iostream>

unsigned int TextureID_1;
unsigned int TextureID_2;

unsigned int gVBO;
unsigned int gEBO;
unsigned int gVAO;

int location = -1;
int color = -1;
int texlocation1 = -1;
//int texlocation2 = -1;

int uniformtex1 = -1;
int uniformtex2 = -1;
float MixParam = -1;

void ErrorCallback(int id, const char* description)
{
	std::cout << "Error with id: " << id << " and description: " << description << std::endl;
}

void PrePareTexture()
{
	glGenTextures(1, &TextureID_2);
	glGenTextures(1, &TextureID_2);

	stbi_set_flip_vertically_on_load(true);
	//Texture 1
	int width, height, channel;
	unsigned char* data = stbi_load("Brick.jpg", &width, &height, &channel, 0);

	glBindTexture(GL_TEXTURE_2D, TextureID_1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
	
	//Texture2
	data = stbi_load("awesomeface.png", &width, &height, &channel, 0);
	
	glBindTexture(GL_TEXTURE_2D, TextureID_2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

}

void PrepareData()
{
	//Generate vertex buffer object
	glGenVertexArrays(1, &gVAO);

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//draw by indices
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	glGenBuffers(1, &gEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AccessShaderParam(GLShader* shader)
{
	if (location == -1)
		location = glGetAttribLocation(shader->GetProgramId(), "aPos");
	if (color == -1)
		color = glGetAttribLocation(shader->GetProgramId(), "aColor");
	if (texlocation1 == -1)
		texlocation1 = glGetAttribLocation(shader->GetProgramId(), "aTexCoordinate1");
	/*if (texlocation2 == -1)
	texlocation2 = glGetAttribLocation(shader->GetProgramId(), "aTexCoordinate2");*/
	if (uniformtex1 == -1)
		uniformtex1 = glGetUniformLocation(shader->GetProgramId(), "Texture1");
	if (uniformtex2 == -1)
		uniformtex2 = glGetUniformLocation(shader->GetProgramId(), "Texture2");

	if (MixParam == -1)
		MixParam = glGetUniformLocation(shader->GetFragmentId(), "MixParam");
}

void PrepareSteps()
{
	glBindVertexArray(gVAO);

	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	if (location != -1)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	}

	if (texlocation1 != -1)
	{
		glEnableVertexAttribArray(texlocation1);
		glVertexAttribPointer(texlocation1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	}

	if (color != -1)
	{
		glEnableVertexAttribArray(color);
		glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*int textureUnit = 0;
	if (uniformtex1 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, TextureID_1);
		glUniform1i(uniformtex1, textureUnit);
		++textureUnit;
	}
	if (uniformtex2 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, TextureID_2);
		glUniform1i(uniformtex2, textureUnit);
		++textureUnit;
	}*/

	glBindVertexArray(0);
}

void Draw(GLShader* shader)
{
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	if (location != -1)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	}

	if (texlocation1 != -1)
	{
		glEnableVertexAttribArray(texlocation1);
		glVertexAttribPointer(texlocation1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	}

	if (color != -1)
	{
		glEnableVertexAttribArray(color);
		glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	int textureUnit = 0;
	if (uniformtex1 != -1)
	{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, TextureID_1);
	glUniform1i(uniformtex1, textureUnit);
	++textureUnit;
	}
	if (uniformtex2 != -1)
	{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, TextureID_2);
	glUniform1i(uniformtex2, textureUnit);
	++textureUnit;
	}
	//glBindVertexArray(gVAO);
	//glDrawArrays(GL_TRIANGLES, 0, 4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

int main(int args, char** argv)
{
	glfwSetErrorCallback(ErrorCallback);
	GLContext::GetInstance()->CreateContextWindow(800, 600, "Texture Practice");
	GLContext::GetInstance()->MakeViewport(0, 0, 800, 600);

	GLShader* shader = new GLShader("VertexShader.glsl", "FragmentShader.glsl");

	PrepareData();
	PrePareTexture();
	AccessShaderParam(shader);
	shader->ActiveProgram();
	//PrepareSteps();

	while (GLContext::GetInstance()->ShouldTerminateContext())
	{
		glfwPollEvents();
		GLContext::GetInstance()->HandleExit();

		glClearColor(0.2, 0.5, 0.3, 1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		shader->ActiveProgram();
		Draw(shader);

		GLContext::GetInstance()->SwapBuffers();
	}

	GLContext::GetInstance()->ShutDown();
	delete shader;
	return 1;
}