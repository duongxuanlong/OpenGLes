#ifndef __GLCONTEXT__
#define __GLCONTEXT__

class GLContext
{
private:
	GLFWwindow*  m_Window;
	static GLContext* m_Instace;
	int m_Width;
	int m_Height;
	bool m_IsInit;

	GLContext();
public:
	~GLContext();
	static GLContext* GetInstance();
	
	GLFWwindow* GetContextWindow();
	void CreateContextWindow(int width = 800, int height = 600, const char* title = "Default Window");
	void MakeViewport(int offsetX = 0, int offsetY = 0, int width = 800, int height = 600);
	void HandleExit();

	bool ShouldTerminateContext();
	static void ShutDown();
	void SwapBuffers();
};
#endif