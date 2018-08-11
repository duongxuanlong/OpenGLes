#include "glad\glad.h"
#include "GLFW\glfw3.h"


#include "GLShader.h"

#include <iostream>
#include <fstream>
#include <sstream>

GLShader::GLShader(const char* vertexStr, const char* fragmentStr)
{
	m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	m_Program = glCreateProgram();

	std::fstream vertexStream, fragmentStream;
	vertexStream.open(vertexStr);
	fragmentStream.open(fragmentStr);

	std::stringstream vstr, fstr;
	vstr << vertexStream.rdbuf();
	fstr << fragmentStream.rdbuf();

	CompileShader(vstr.str().c_str(), m_VertexShader);
	CompileShader(fstr.str().c_str(), m_FragmentShader);
	LinkProgram();
}

GLShader::~GLShader()
{
	glDetachShader(m_Program, m_VertexShader);
	glDetachShader(m_Program, m_FragmentShader);
	
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
	glDeleteProgram(m_Program);
}

void GLShader::CompileShader(const char* str, unsigned int id)
{
	glShaderSource(id, 1, &str, NULL);
	glCompileShader(id);

	int success;
	char infoLog[512];

	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "Shader compile error with log: " << infoLog << std::endl;
	}
}

void GLShader::LinkProgram()
{
	glAttachShader(m_Program, m_VertexShader);
	glAttachShader(m_Program, m_FragmentShader);
	glLinkProgram(m_Program);

	int success;
	char infoLog[512];
	glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
		std::cout << "Link program error with log: " << infoLog << std::endl;
	}
}

void GLShader::ActiveProgram()
{
	if (m_Program == 0)
		return;

	glUseProgram(m_Program);
}