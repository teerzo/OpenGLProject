#include "kmAppCheckers.h"

int main(int argc, char *argv[])
{
	kmAppCheckers app;
	app.SetApplicationDefaults();
	if (!app.ApplicationStartup())
	{
		return -1;
	}
	while (app.Update())
	{
		app.Draw();
	}
	app.ApplicationShutdown();
	return 0;
}