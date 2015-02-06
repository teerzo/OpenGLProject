#ifndef _APPLICATION_H_
#define _APPLICATION_H_

struct Screen
{	
	int Width;
	int Height;
};

struct GLFWwindow;

class Application
{
private:

protected:
	char* AppName;
	Screen ScreenSize;
	GLFWwindow* window;
public:
	Application();
	virtual ~Application();

	virtual void setDefaults();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();

	GLFWwindow* getWindow();
};


#endif // _APPLICATION_H_