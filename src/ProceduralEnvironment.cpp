#include "ProceduralEnvironment.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
//#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

#include "Building.h"

ProceduralEnvironment::~ProceduralEnvironment() {

}

void ProceduralEnvironment::SetApplicationDefaults() {
	this->applicationName = "ProceduralEnvironment";
	this->screenSize.Width = 1280;
	this->screenSize.Height = 720;
}

bool ProceduralEnvironment::ApplicationStartup() {
	if( !Application::ApplicationStartup() ) {
		return false;
	}
	glClearColor( 0.3f, 0.3f, 0.3f, 1.0f );
	glEnable( GL_DEPTH_TEST );
	// enable transparency
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	

	//glEnable( GL_CULL_FACE );

	// Load Shadow Frame Buffer
	gBuffer.SetWindowSize( 1280, 720 );
	gBuffer.SetPlaneSize( 1280, 720 );
	gBuffer.GenerateTarget();

	gBuffer.GenerateGBuffer();
	gBuffer.GenerateLightBuffer();

	pointLight = GenerateCube();



	gridMesh = BuildGrid(glm::vec2(128, 128), glm::ivec2(128, 128));
	

	tweak_light_direction = glm::vec3(1, -1, 0);
	tweak_light_color = glm::vec3(1, 1, 1);
	tweak_light_intensity = 1.0f;
	tweakBarLighting = TwNewBar("Lighting");
	TwAddVarRW(tweakBarLighting, "Direction", TW_TYPE_DIR3F, &tweak_light_direction, "group=Light1");
	TwAddVarRW(tweakBarLighting, "Color", TW_TYPE_COLOR3F, &tweak_light_color, "group=Light1");
	TwAddVarRW(tweakBarLighting, "Intensity", TW_TYPE_FLOAT, &tweak_light_intensity, "group=Light1");


	tweak_lava_direction = glm::vec3(0, 0, 1);
	tweak_lava_speed = 10.0f;
	tweak_lava_height = 0.0f;
	tweak_perlin_position = glm::vec3(0, 0, 0);
	tweak_perlin_size = 256.0f;
	tweak_perlin_height = 50.0f;
	tweak_perlin_octaves = 8.0f;
	tweak_perlin_persistance = 0.5f;
	tweakBarTerrain = TwNewBar("Terrain");
	TwAddVarRW(tweakBarTerrain, "Position", TW_TYPE_DIR3F, &tweak_perlin_position, "group=Perlin");
	
	TwAddVarRW(tweakBarTerrain, "Size", TW_TYPE_FLOAT, &tweak_perlin_size, "group=Perlin");
	TwAddVarRW(tweakBarTerrain, "Height", TW_TYPE_FLOAT, &tweak_perlin_height, "group=Perlin");
	TwAddVarRW(tweakBarTerrain, "Octaves", TW_TYPE_FLOAT, &tweak_perlin_octaves, "group=Perlin");
	TwAddVarRW(tweakBarTerrain, "Persistance", TW_TYPE_FLOAT, &tweak_perlin_persistance, "group=Perlin");

	TwAddVarRW(tweakBarTerrain, "Direction", TW_TYPE_DIR3F, &tweak_lava_direction, "group=Lava");
	TwAddVarRW(tweakBarTerrain, "Speed", TW_TYPE_FLOAT, &tweak_lava_speed, "group=Lava");
	//TwAddVarRW(tweakBarTerrain, "Lava", TW_TYPE_FLOAT, &tweak_lava_height, "group=Heights precision 2");
	TwAddVarRW(tweakBarTerrain, "LavaHeight", TW_TYPE_FLOAT, &tweak_lava_height, "group=Lava");


	//glm::vec3 tweak_lava_direction;
	//float tweak_lava_speed;
	//float tweak_rock_height;
	//float tweak_grass_height;
	//float tweak_lava_height;
	//float tweak_dirt_height;
	

	//gridMesh = BuildVertexGrid( glm::vec2( 128, 128 ), glm::ivec2( 128, 128 ) );
	//BuildPerlinTexture( &perlin_1_texture, glm::ivec2( 128, 128 ), 16, 0.8f, ( float )0.0f );

	LoadTexture(&dirt_texture,	"Assignment1/dirt512.png");
	LoadTexture(&grass_texture,	"Assignment1/grass512.tga");
	LoadTexture(&lava_texture,	"Assignment1/lava512.tga");
	LoadTexture(&rock_texture,	"Assignment1/rock512.tga");

	// Load Plane
	//glPlane = GeneratePlane();
	// Load Mesh

	buildingBase = new MeshGroup();
	if( !LoadOBJFile( buildingBase, "BuildingBase.obj" ) ) {
		return false;
	}

	buildings.resize( 9 );
	int index = 0;
	for( int row = 0; row < 3; ++row ) {
		for( int col = 0; col < 3; ++col ) {

			buildings[index] = new Building();
			buildings[index]->InitiateBuilding();
			buildings[index]->base->worldTransform[3] = glm::vec4( row * 10, 0, col * 10, 1 );
			index++;
		}
	}
	bunnyObject = new MeshGroup();
	if (!LoadOBJFile(bunnyObject, "stanford/bunny.obj")) {
		return false;
	}
	//CreateOpenGLBuffers(shapes);
	// Load Shaders
	
	Reload();
	ReloadShaders();

	return true;
}

void ProceduralEnvironment::ApplicationShutdown() {
	Application::ApplicationShutdown();
}

bool ProceduralEnvironment::Update() {
	CheckInput();
	if( !Application::Update() ) {
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////

	//buildingBase->Update(deltaTime);

	int index = 0;
	for( int row = 0; row < 3; ++row ) {
		for( int col = 0; col < 3; ++col ) {

			buildings[index]->Update( deltaTime );
			index++;
		}
	}

	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void ProceduralEnvironment::Draw() {
	//glCullFace(GL_BACK);

	glEnable( GL_DEPTH_TEST );
	RenderGeometry();
	glDisable( GL_DEPTH_TEST );

	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE );
	RenderLights();
	glDisable( GL_BLEND );

	RenderComposite();

	//////////////////////
	//! End of Draw Code 
	//////////////////////

	Application::Draw();
	glfwSwapBuffers( this->window );
	glfwPollEvents();
}

void ProceduralEnvironment::RenderGeometry() {
	glViewport( 0, 0, gBuffer.m_screen.Width, gBuffer.m_screen.Height );
	glBindFramebuffer( GL_FRAMEBUFFER, gBuffer.m_fbo );
	glClearColor( defaultBackgroundColour.x, defaultBackgroundColour.y, defaultBackgroundColour.z, defaultBackgroundColour.w );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUseProgram( gBufferProgramID );

	unsigned int	uniform_view = glGetUniformLocation( gBufferProgramID, "view" );
	unsigned int	uniform_proj_view = glGetUniformLocation( gBufferProgramID, "projection_view" );
	unsigned int	uniform_mesh_position = glGetUniformLocation( gBufferProgramID, "mesh_offset" );
	glUniformMatrix4fv( uniform_view, 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetView() );
	glUniformMatrix4fv( uniform_proj_view, 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetProjectionView() );
	//glUniform3fv(uniform_mesh_position, 1, (float*)&buildingBase->worldTransform[3]);


	//buildingBase->Draw();
	bunnyObject->Draw();
	for( int i = 0; i < 9; ++i ) {
		buildings[i]->Draw( gBufferProgramID );
	}

	

	//glCullFace(GL_BACK); 
	//glBindVertexArray(pointLight.m_VAO);
	//glDrawElements(GL_TRIANGLES, pointLight.m_index_count, GL_UNSIGNED_INT, 0);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	DrawPerlin();

	DebugDraw();

}

void ProceduralEnvironment::DrawPerlin() {

	/*glUseProgram( perlinUpdateProgramID );

	int uniform_projection_view = glGetUniformLocation( perlinUpdateProgramID, "projection_view" );
	int uniform_world = glGetUniformLocation( perlinUpdateProgramID, "world" );
	int uniform_timer = glGetUniformLocation( perlinUpdateProgramID, "timer" );

	glUniformMatrix4fv( uniform_projection_view, 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetProjectionView() );
	glUniformMatrix4fv( uniform_world, 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetWorld() );
	glUniform1f( uniform_timer, currentGameTime );

	int uniform_perlin_height_texture = glGetUniformLocation( perlinUpdateProgramID, "perlin_1_texture" );
	glUniform1i( uniform_perlin_height_texture, 0 );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, perlin_1_texture );

	glEnable( GL_RASTERIZER_DISCARD );
	glBindVertexArray( vao[active_buffer] );
	unsigned int other_buffer = ( active_buffer + 1 ) % 2;
	glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[other_buffer] );
	glBeginTransformFeedback( GL_POINTS );
	glDrawArrays( GL_POINTS, 0, grid_size );
	glEndTransformFeedback();
	glDisable( GL_RASTERIZER_DISCARD );
	glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0 );*/

	// render pass
	glUseProgram( perlinDrawProgramID );

	int uniform_projection_view = glGetUniformLocation( perlinDrawProgramID, "projection_view" );
	int uniform_world = glGetUniformLocation( perlinDrawProgramID, "world" );
	int uniform_timer = glGetUniformLocation( perlinDrawProgramID, "timer" );
	int uniform_position_offset = glGetUniformLocation(perlinDrawProgramID, "position_offset");
	int uniform_lava_direction = glGetUniformLocation(perlinDrawProgramID, "lava_direction");
	int uniform_lava_speed = glGetUniformLocation(perlinDrawProgramID, "lava_speed");
	int uniform_lava_height = glGetUniformLocation(perlinDrawProgramID, "lava_height");

	glUniformMatrix4fv( uniform_projection_view, 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetProjectionView() );
	glUniformMatrix4fv(uniform_world, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetWorld());
	glUniform1f(uniform_timer, currentGameTime);
	glUniform3fv(uniform_position_offset, 1, (float*)&tweak_perlin_position);
	glUniform3fv(uniform_lava_direction, 1, (float*)&tweak_lava_direction);
	glUniform1f(uniform_lava_speed, tweak_lava_speed);
	glUniform1f(uniform_lava_height, tweak_lava_height);

	int uniform_perlin_height_texture = glGetUniformLocation( perlinDrawProgramID, "perlin_1_texture" );
	glUniform1i( uniform_perlin_height_texture, 0 );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, perlin_1_texture );

	int uniform_dirt_texture = glGetUniformLocation(perlinDrawProgramID, "dirt_texture");
	glUniform1i(uniform_dirt_texture, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dirt_texture);

	int uniform_grass_texture = glGetUniformLocation(perlinDrawProgramID, "grass_texture");
	glUniform1i(uniform_grass_texture, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, grass_texture);

	int uniform_rock_texture = glGetUniformLocation(perlinDrawProgramID, "rock_texture");
	glUniform1i(uniform_rock_texture, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, rock_texture);

	int uniform_lava_texture = glGetUniformLocation(perlinDrawProgramID, "lava_texture");
	glUniform1i(uniform_lava_texture, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, lava_texture);

	//glBindVertexArray(vao[other_buffer]);
	glDrawArrays( GL_POINTS, 0, grid_size );

	glBindVertexArray(gridMesh.m_VAO);
	glDrawElements(GL_TRIANGLES, gridMesh.m_index_count, GL_UNSIGNED_INT, 0);

	//last_draw_time = 
	//active_buffer = other_buffer;
}

void ProceduralEnvironment::RenderLights() {
	glBindFramebuffer( GL_FRAMEBUFFER, gBuffer.light_fbo );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	DrawDirectionLight(tweak_light_direction, tweak_light_color);

	//DrawPointLight(cameraVector[0]->GetPosition(), 10, color.White.xyz);

}
void ProceduralEnvironment::DrawPointLight( glm::vec3 a_position, float a_radius, glm::vec3 a_color ) {
	glUseProgram( pointLightProgramID );
	glm::vec4 view_space_pos = cameraVector[currentCamera]->GetView() * glm::vec4( a_position, 1 );

	int uniform_pos = glGetUniformLocation( pointLightProgramID, "light_position" );
	int uniform_view_pos = glGetUniformLocation( pointLightProgramID, "light_view_position" );
	int uniform_light_color = glGetUniformLocation( pointLightProgramID, "light_color" );
	int uniform_light_radius = glGetUniformLocation( pointLightProgramID, "light_radius" );
	int uniform_proj_view = glGetUniformLocation( pointLightProgramID, "projection_view" );

	glUniform3fv( uniform_pos, 1, ( float* ) &a_position );
	glUniform3fv( uniform_view_pos, 1, ( float* ) &view_space_pos );
	glUniform3fv( uniform_light_color, 1, ( float* ) &a_color );
	glUniform1f( uniform_light_radius, a_radius );
	glUniformMatrix4fv( uniform_proj_view, 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetProjectionView() );

	unsigned int uniform_position_texture = glGetUniformLocation( pointLightProgramID, "position_texture" );
	glUniform1i( uniform_position_texture, 0 );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, gBuffer.position_texture );
	unsigned int uniform_normals_texture = glGetUniformLocation( pointLightProgramID, "normals_texture" );
	glUniform1i( uniform_normals_texture, 1 );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, gBuffer.normals_texture );

	glBindVertexArray( pointLight.m_VAO );
	glDrawElements( GL_TRIANGLES, pointLight.m_index_count, GL_UNSIGNED_INT, 0 );
}

void ProceduralEnvironment::DrawDirectionLight( const glm::vec3& a_direction, const glm::vec3& a_color ) {
	glUseProgram( directionalLightProgramID );

	glm::vec4 view_space_light = cameraVector[currentCamera]->GetView() * glm::vec4( glm::normalize( a_direction ), 0 );

	int uniform_light_direction = glGetUniformLocation( directionalLightProgramID, "light_dir" );
	int uniform_light_color = glGetUniformLocation( directionalLightProgramID, "light_color" );

	glUniform3fv( uniform_light_direction, 1, ( float* ) &view_space_light );
	glUniform3fv( uniform_light_color, 1, ( float* ) &a_color );

	unsigned int uniform_position_texture = glGetUniformLocation( directionalLightProgramID, "position_texture" );
	glUniform1i( uniform_position_texture, 0 );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, gBuffer.position_texture );

	unsigned int uniform_normals_texture = glGetUniformLocation( directionalLightProgramID, "normals_texture" );
	glUniform1i( uniform_normals_texture, 1 );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, gBuffer.normals_texture );

	
	glBindVertexArray( gBuffer.m_plane.m_VAO );
	glDrawElements( GL_TRIANGLES, gBuffer.m_plane.m_index_count, GL_UNSIGNED_INT, 0 );
}

void ProceduralEnvironment::RenderComposite() {
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	//glClearColor(defaultBackgroundColour.x, defaultBackgroundColour.y, defaultBackgroundColour.z, defaultBackgroundColour.w);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUseProgram( compositeProgramID );

	glUniform1i( glGetUniformLocation( compositeProgramID, "albedo_texture" ), 0 );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, gBuffer.albedo_texture );

	glUniform1i( glGetUniformLocation( compositeProgramID, "light_texture" ), 1 );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, gBuffer.light_texture );

	glUniformMatrix4fv( glGetUniformLocation( compositeProgramID, "projection_view" ), 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetProjectionView() );

	glBindVertexArray( gBuffer.m_plane.m_VAO );
	glDrawElements( GL_TRIANGLES, gBuffer.m_plane.m_index_count, GL_UNSIGNED_INT, 0 );
}

void ProceduralEnvironment::DebugDraw() {
	Application::DebugDraw();
}

void ProceduralEnvironment::CheckInput() {
	if( glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS ) {
		Reload();		
		ReloadShaders();		
	}
	Application::CheckInput();
}



void ProceduralEnvironment::LoadShaders() {
	LoadShader( ( GLuint* ) &gBufferProgramID, "ass1_gbuffer_vertex.glsl", "ass1_gbuffer_fragment.glsl", nullptr );
	LoadShader( ( GLuint* ) &compositeProgramID, "ass1_composite_vertex.glsl", "ass1_composite_fragment.glsl", nullptr );


	LoadShader( ( GLuint* ) &directionalLightProgramID, "ass1_directional_light_vertex.glsl", "ass1_directional_light_fragment.glsl", nullptr );
	LoadShader( ( GLuint* ) &pointLightProgramID, "ass1_point_light_vertex.glsl", "ass1_point_light_fragment.glsl", nullptr );
	LoadShader( ( GLuint* ) &spotLightProgramID, "shadow_map_vertex.glsl", "shadow_map_fragment.glsl", nullptr );

	LoadShader((GLuint*)&perlinDrawProgramID, "ass1_perlin_vertex.glsl", "ass1_perlin_fragment.glsl", "ass1_perlin_geometry.glsl");
	//LoadShader((GLuint*)&perlinDrawProgramID, "ass1_perlin_vertex.glsl", "ass1_perlin_fragment.glsl", nullptr );
	//CreateUpdateShader();
}

void ProceduralEnvironment::Reload()
{
	BuildPerlinTexture(&perlin_1_texture, glm::ivec2(tweak_perlin_size, tweak_perlin_size), tweak_perlin_octaves, tweak_perlin_persistance, (float)0.0f);

}

void ProceduralEnvironment::ReloadShaders() {
	printf( "reloaded Shaders\n" );
	//glfwSetTime( 0.0 );
	//currentGameTime = 0.0f;

	

	glDeleteProgram( perlinUpdateProgramID );
	glDeleteProgram( perlinDrawProgramID );
	glDeleteProgram( gBufferProgramID );
	glDeleteProgram( directionalLightProgramID );
	glDeleteProgram( pointLightProgramID );
	glDeleteProgram( spotLightProgramID );
	glDeleteProgram( compositeProgramID );
	LoadShaders();
}



OpenGLData ProceduralEnvironment::BuildGrid( glm::vec2 real_dims, glm::ivec2 dims ) {
	// Allocate vertex data
	OpenGLData temp;
	unsigned int vertex_count = ( dims.x + 1 ) * ( dims.y + 1 );
	VertexUV* vertex_data = new VertexUV[vertex_count];

	unsigned int index_count = dims.x * dims.y * 6;
	unsigned int* index_data = new unsigned int[index_count];

	float curr_y = -real_dims.y / 2.0f;
	for( int y = 0; y < dims.y + 1; ++y ) {
		float curr_x = -real_dims.x / 2.0f;
		for( int x = 0; x < dims.x + 1; ++x ) {
			vertex_data[y * ( dims.x + 1 ) + x].Position = glm::vec4( curr_x, 0, curr_y, 1 );
			vertex_data[y * ( dims.x + 1 ) + x].UV = glm::vec2( ( float ) x / ( float ) dims.x, ( float ) y / ( float ) dims.y );
			curr_x += real_dims.x / ( float ) dims.x;
		}
		curr_y += real_dims.y / ( float ) dims.y;
	}

	int curr_index = 0;
	for( int y = 0; y < dims.y; ++y ) {
		for( int x = 0; x < dims.x; ++x ) {
			index_data[curr_index++] = y * ( dims.x + 1 ) + x;
			index_data[curr_index++] = ( y + 1 ) * ( dims.x + 1 ) + x;
			index_data[curr_index++] = ( y + 1 ) * ( dims.x + 1 ) + ( x + 1 );
			index_data[curr_index++] = ( y + 1 ) * ( dims.x + 1 ) + ( x + 1 );
			index_data[curr_index++] = y * ( dims.x + 1 ) + ( x + 1 );
			index_data[curr_index++] = y * ( dims.x + 1 ) + x;
		}
	}

	temp.m_index_count = index_count;
	//create vertexarrayobecject, buffers, etc
	glGenVertexArrays( 1, &temp.m_VAO );
	glGenBuffers( 1, &temp.m_VBO );
	glGenBuffers( 1, &temp.m_IBO );

	//bind and fill buffers	
	glBindVertexArray( temp.m_VAO );
	glBindBuffer( GL_ARRAY_BUFFER, temp.m_VBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, temp.m_IBO );

	glBufferData( GL_ARRAY_BUFFER, sizeof( VertexUV )* vertex_count, vertex_data, GL_STATIC_DRAW );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int )* index_count, index_data, GL_STATIC_DRAW );

	// tell opengl about our vertex structure
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( VertexUV ), 0 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( VertexUV ), ( void* )sizeof( glm::vec4 ) );

	// unbind stuff
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	delete[] index_data;
	delete[] vertex_data;

	return temp;
}

void ProceduralEnvironment::BuildVertexGrid( glm::vec2 real_dims, glm::ivec2 dims ) {

	glGenVertexArrays( 2, vao );
	glGenBuffers( 2, vbo );

	unsigned int vertex_count = ( dims.x + 1 ) * ( dims.y + 1 );
	VertexUV* vertex_data = new VertexUV[vertex_count];

	unsigned int index_count = dims.x * dims.y;
	unsigned int* index_data = new unsigned int[index_count];

	float curr_y = -real_dims.y / 2.0f;
	for( int y = 0; y < dims.y + 1; ++y ) {
		float curr_x = -real_dims.x / 2.0f;
		for( int x = 0; x < dims.x + 1; ++x ) {
			vertex_data[y * ( dims.x + 1 ) + x].Position = glm::vec4( curr_x, 0, curr_y, 1 );
			vertex_data[y * ( dims.x + 1 ) + x].UV = glm::vec2( ( float ) x / ( float ) dims.x , ( float ) y / ( float ) dims.y );
			curr_x += real_dims.x / ( float ) dims.x;
		}
		curr_y += real_dims.y / ( float ) dims.y;
	}

	grid_size = vertex_count;

	for( unsigned int buffer_index = 0; buffer_index < 2; ++buffer_index ) {


		glBindVertexArray( vao[buffer_index] );
		glBindBuffer( GL_ARRAY_BUFFER, vbo[buffer_index] );
		glBufferData( GL_ARRAY_BUFFER, sizeof( VertexUV ) * vertex_count, vertex_data, GL_STREAM_DRAW );

		glEnableVertexAttribArray( 0 ); // Position
		glEnableVertexAttribArray( 1 ); // UV

		glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( VertexUV ), 0 );
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( VertexUV ), ( void* )sizeof( glm::vec4 ) );

	}

	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

bool ProceduralEnvironment::BuildPerlinTexture( unsigned int* a_texture, const glm::ivec2 dims, const int octaves, const float persistance, const float offset ) {
	bool result = true;
	// set scale
	float scale = ( 1.0f / dims.x ) * 3.0f;

	// Allocate memory for perlin data
	perlin_data = new float[dims.x * dims.y];

	for( int y = 0; y < dims.y; ++y ) {
		for( int x = 0; x < dims.x; ++x ) {
			float amplitude = 1.0f;
			float frequency = 1.0f;
			perlin_data[y * dims.x + x] = 0;

			for( int i = 0; i < octaves; ++i ) {
				float perlin_sample = glm::perlin( glm::vec2( x + offset, y + offset ) * scale * frequency + currentGameTime ) * 0.5f + 0.5f;
				perlin_sample *= amplitude;
				perlin_data[y * dims.x + x] += perlin_sample;

				amplitude *= persistance;
				frequency *= 2.0f;
			}
		}
	}

	// generate opengl texture handles
	glGenTextures( 1, a_texture );
	glBindTexture( GL_TEXTURE_2D, *a_texture );

	// pass the perlin data to the texture
	glTexImage2D( GL_TEXTURE_2D, 0, GL_R32F, dims.x, dims.y, 0, GL_RED, GL_FLOAT, perlin_data );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	return result;
}

void ProceduralEnvironment::CreateUpdateShader() {

	/*unsigned int vertex_shader;
	LoadShaderType( "../data/shaders/ass1_perlin_update_vertex.glsl", GL_VERTEX_SHADER, &vertex_shader );



	perlinUpdateProgramID = glCreateProgram();
	glAttachShader( perlinUpdateProgramID, vertex_shader );

	const char* outputs[2] = { "updated_position"
		"updated_uv" };

	glTransformFeedbackVaryings( perlinUpdateProgramID, 2, outputs, GL_INTERLEAVED_ATTRIBS );
	glLinkProgram( perlinUpdateProgramID );
	int success = GL_FALSE;
	glGetProgramiv( perlinUpdateProgramID, GL_LINK_STATUS, &success );
	if( success == GL_FALSE ) {
		int log_length = 0;
		glGetProgramiv( perlinUpdateProgramID, GL_INFO_LOG_LENGTH, &log_length );
		char* log = new char[log_length];
		glGetProgramInfoLog( perlinUpdateProgramID, log_length, 0, log );
		printf( "Error: Shaders SHIT FAILED \n " );
		printf( "%s", log );

		delete[] log;
	}
	glDeleteShader( vertex_shader );*/

}