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
//#include "GPUParticleSystems.h"	// Project 09
//#include "SceneManagement.h"	//Project 10
//#include "RenderTargets.h" // Project 11
//#include "PostProcessing.h" // Project 12
//#include "ShadowsApp.h" // Project 13
//#include "DeferredRendering.h" // Project 14
//#include "ProceduralGeneration.h" // project 15
#include "AdvancedNavigation.h" 

//#include "AdamUlbricht.h"

//#include "ProceduralEnvironment.h"

// Threading 
#include<thread>
#include<mutex>

void print(int i ) 
{
	static std::mutex myMutex;
	std::lock_guard<std::mutex> guard(myMutex);

	printf("Hello Thread %i\n", i);
	printf("I'm here...\n");
	printf("...not there.\n");

}

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
	//GPUParticleSystems app;
	//SceneManagement app;
	//RenderTargets app;
	//PostProcessing app;
	//ShadowsApp app;
	//DeferredRendering app;
	//ProceduralGeneration app; 
	//ProceduralEnvironment app;

	AdvancedNavigation app;

	// Artist Specific 
	//AdamUlbricht app;

	// Threading
	//std::vector<std::thread> threads;
	//vec4 myVectors[50000];

	//int chunklength = 50000 / 10;


	//for (int i = 0; i < 10; i++)
	//{
	//	threads.push_back(std::thread(
	//	//Lambda
	//	[&](int low, int high)
	//	{
	//		for (int j = low; j < high; j++)
	//		{
	//			myVectors[i] = normalize(myVectors[i]); 
	//			printf("Thread Num: %i\n", j);
	//		}
	//	}, i * chunklength, (i + 1)* chunklength ));
	//}
	//
	//for (auto& thread : threads)
	//{
	//	thread.join();
	//}
	

	// sets the name and screen size of the app
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