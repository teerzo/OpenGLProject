//#include "TemplateApplication.h"	// Project 00
//#include "IntroToOpenGL.h"		// Project 01
//#include "CameraAndProjections.h"	// Project 02
//#include "RenderingGeometry.h"	// Project 03
//#include "BasicTexturing.h"		// Project 04
//#include "BasicLighting.h"		// Project 05
//#include "AdvancedTexturing.h"	// Project 06
//#include "Animation.h"			// Project 07
//#include "Quaternions.h"			// Project 07.5
//#include "ParticleSystems.h"		// Project 08
#include "GPUParticleSystems.h"	// Project 09
//#include "AdamUlbricht.h"


int main(int argc, char *argv[])
{
	//TemplateApplication app;
	//IntroToOpenGL app;
	//CameraAndProjections app;
	//RenderingGeometry app;
	//BasicTexturing app;
	//BasicLighting app;
	//AdvancedTexturing app;
	
	//Animation app;
	//Quaternions app;

	//ParticleSystems app;
	GPUParticleSystems app;

	// Artist Specific 
	//AdamUlbricht app;

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