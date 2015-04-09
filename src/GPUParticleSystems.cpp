#include "GPUParticleSystems.h"
#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"
#include "Utility.h"

GPUParticleSystems::~GPUParticleSystems() {

}

void GPUParticleSystems::SetApplicationDefaults() {
	this->applicationName = "GPUParticleSystems";
	this->screenSize.Width = 1280;
	this->screenSize.Height = 720;
}

bool GPUParticleSystems::ApplicationStartup() {
	if( !Application::ApplicationStartup() ) {
		return false;
	}
	glClearColor( 0.3f, 0.3f, 0.3f, 1.0f );
	glEnable( GL_DEPTH_TEST );
	// enable transparency
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	Gizmos::create();
	glfwSetTime( 0.0 );
	

	LoadShaders();

	//LoadShader( "particle", ( GLuint* ) &m_ProgramID );

	m_emitter = new GPUPointEmitter();
	m_emitter->Initialise( 100000, glm::vec3( 0, 0, 0 ), 20, 20, 30, 10, 15, 0.5f, 0.8f, color.Green, color.Red );

	return true;
}

void GPUParticleSystems::ApplicationShutdown() {
	Application::ApplicationShutdown();
}

bool GPUParticleSystems::Update() {
	CheckInput();
	if( !Application::Update() ) {
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////


	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void GPUParticleSystems::Draw() {
	glClearColor( defaultBackgroundColour.x, defaultBackgroundColour.y, defaultBackgroundColour.z, defaultBackgroundColour.w );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glUseProgram( basicProgram );

	int proj_view_handle = glGetUniformLocation( basicProgram, "ProjectionView" );
	if( proj_view_handle >= 0 ) {
		glUniformMatrix4fv( proj_view_handle, 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetProjectionView() );

	}
	///////////////////////////////////
	//! Project Specific Drawcode Here 
	///////////////////////////////////
	m_emitter->Draw( currentGameTime, cameraVector[currentCamera]->GetWorld(), cameraVector[currentCamera]->GetProjectionView() );

	//////////////////////
	//! End of Draw Code 
	//////////////////////
	Application::Draw();
	glfwSwapBuffers( this->window );
	glfwPollEvents();
}

void GPUParticleSystems::DebugDraw() {
	Application::DebugDraw();
}

void GPUParticleSystems::CheckInput() {
	if( glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS ) {
		ReloadShaders();
	}
	Application::CheckInput();
}



void GPUParticleSystems::LoadShaders() {
	LoadShader( ( GLuint* ) &GPUProgramID, "gpu_particle_vertex.glsl", "gpu_particle_fragment.glsl", "gpu_particle_geometry.glsl" );
	LoadShader( ( GLuint* ) &GPUUpdateProgramID, "particle_update_vertex.glsl", "basic_fragment.glsl", nullptr );
}

void GPUParticleSystems::ReloadShaders() {
	printf( "reloaded Shaders\n" );
	glfwSetTime( 0.0 );
	currentGameTime = 0.0f;
	glDeleteProgram( GPUProgramID );
	glDeleteProgram( GPUUpdateProgramID );
	LoadShaders();
}