//#include "TemplateApplication.h"	// Project 00
//#include "IntroToOpenGL.h"			// Project 01
//#include "CameraAndProjections.h"	// Project 02
//#include "RenderingGeometry.h"		// Project 03
//#include "BasicTexturing.h"			// Project 04
//#include "BasicLighting.h"			// Project 05

#include "AdvancedTexturing.h"		// Project 06

int main(int argc, char *argv[])
{
	//TemplateApplication app;
	//IntroToOpenGL app;
	//CameraAndProjections app;
	//RenderingGeometry app;
	//BasicTexturing app;
	//BasicLighting app;

	AdvancedTexturing app;

	// sets the name and screen size of the app
	app.setDefaults();

	if (!app.startup())
	{
		return -1;
	}	

	while (app.update())
	{
		app.draw();
	}
	
	app.shutdown();
	return 0;
}