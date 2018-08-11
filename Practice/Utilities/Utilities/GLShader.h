#ifndef __GLSHADER__
#define __GLSHADER__

class GLShader
{
private:
	unsigned int m_VertexShader;
	unsigned int m_FragmentShader;
	unsigned int m_Program;

	void CompileShader(const char* str, unsigned int id);
	void LinkProgram();
public:
	~GLShader();
	GLShader(const char* vertexStr, const char* fragmentStr);

	void ActiveProgram();

	unsigned int GetVertexId() { return m_VertexShader; }
	unsigned int GetFragmentId() { return m_FragmentShader; }
	unsigned int GetProgramId() { return m_Program; }

};

#endif