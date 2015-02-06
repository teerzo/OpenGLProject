#include "CameraAndProjections.h"

int main(int argc, char *argv[])
{
	CameraAndProjections app;
		
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