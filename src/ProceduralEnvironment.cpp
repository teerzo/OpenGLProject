#include "ProceduralEnvironment.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
//#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

#include "Building.h"

#include"../deps/FBXLoader/FBXFile.h"
ProceduralEnvironment::~ProceduralEnvironment() {

}

void ProceduralEnvironment::SetApplicationDefaults() {
	this->applicationName = "ProceduralEnvironment";
	this->screenSize.Width = 1280;
	this->screenSize.Height = 720;
}

bool ProceduralEnvironment::ApplicationStartup() {
	if (!Application::ApplicationStartup()) {
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	// enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	//glEnable( GL_CULL_FACE );

	// Load Shadow Frame Buffer
	gBuffer.SetWindowSize(1280, 720);
	gBuffer.SetPlaneSize(1280, 720);
	gBuffer.GenerateTarget();

	gBuffer.GenerateGBuffer();
	gBuffer.GenerateLightBuffer();

	pointLight = GenerateCube();




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
	tweak_dirt_height = 1.0f;
	tweak_rock_height = 10000.0f;
	tweak_perlin_position = glm::vec3(0, 0, 0);
	tweak_perlin_texture_size = 128.0f;
	tweak_perlin_mesh_size = 128.0f;
	tweak_perlin_height = 50.0f;
	tweak_perlin_octaves = 8.0f;
	tweak_perlin_persistance = 0.5f;
	tweakBarTerrain = TwNewBar("Terrain");
	TwAddVarRW(tweakBarTerrain, "Position", TW_TYPE_DIR3F, &tweak_perlin_position, "group=Perlin");

	TwAddVarRW(tweakBarTerrain, "Texture Size", TW_TYPE_FLOAT, &tweak_perlin_texture_size, "group=Perlin");
	TwAddVarRW(tweakBarTerrain, "Mesh Size", TW_TYPE_FLOAT, &tweak_perlin_mesh_size, "group=Perlin");
	TwAddVarRW(tweakBarTerrain, "Height", TW_TYPE_FLOAT, &tweak_perlin_height, "group=Perlin");
	TwAddVarRW(tweakBarTerrain, "Octaves", TW_TYPE_FLOAT, &tweak_perlin_octaves, "group=Perlin");
	TwAddVarRW(tweakBarTerrain, "Persistance", TW_TYPE_FLOAT, &tweak_perlin_persistance, "group=Perlin");

	TwAddVarRW(tweakBarTerrain, "Direction", TW_TYPE_DIR3F, &tweak_lava_direction, "group=Lava");
	TwAddVarRW(tweakBarTerrain, "Speed", TW_TYPE_FLOAT, &tweak_lava_speed, "group=Lava");
	//TwAddVarRW(tweakBarTerrain, "Lava", TW_TYPE_FLOAT, &tweak_lava_height, "group=Heights precision 2");
	TwAddVarRW(tweakBarTerrain, "L_Height", TW_TYPE_FLOAT, &tweak_lava_height, "group=Lava");
	TwAddVarRW(tweakBarTerrain, "D_Height", TW_TYPE_FLOAT, &tweak_dirt_height, "group=Dirt");
	TwAddVarRW(tweakBarTerrain, "R_Height", TW_TYPE_FLOAT, &tweak_rock_height, "group=Rock");


	//glm::vec3 tweak_lava_direction;
	//float tweak_lava_speed;
	//float tweak_rock_height;
	//float tweak_grass_height;
	//float tweak_lava_height;
	//float tweak_dirt_height;




	LoadTexture(&dirt_texture, "Assignment1/dirt512.png");
	LoadTexture(&grass_texture, "Assignment1/smiley256.tga");
	LoadTexture(&lava_texture, "Assignment1/lava512.tga");
	LoadTexture(&rock_texture, "Assignment1/rock512.tga");
	if (!LoadTexture(&soulspear_texture, "soulspear/soulspear_diffuse.tga"))
	{
		int i = 0;
	}

	// Load Plane
	//glPlane = GeneratePlane();
	// Load Mesh

	//buildingBase = new MeshGroup();
	//if (!LoadOBJFile(buildingBase, "soulspear/soulspear.obj")) {
	//return false;
	//}

	buildings.resize(9);
	int index = 0;
	for (int row = 0; row < 3; ++row) {
		for (int col = 0; col < 3; ++col) {

			buildings[index] = new Building();
			buildings[index]->InitiateBuilding();
			buildings[index]->base->worldTransform[3] = glm::vec4(row * 10, 0, col * 10, 1);
			index++;
		}
	}
	bunnyObject = new MeshGroup();
	if (!LoadOBJFile(bunnyObject, "stanford/bunnyUV.obj"))
	{
		return false;
	}
	//CreateOpenGLBuffers(shapes);
	// Load Shaders

	m_file = new FBXFile;
	m_file->load( "./models/characters/Pyro/pyro.fbx" );
	//m_file->load("./models/all.fbx");
	m_file->initialiseOpenGLTextures();
	GenerateGLMeshes( m_file, m_pyro );
	//LoadShader("../data/shaders/BasicVertex.glsl", "../data/shaders/BasicFragment.glsl", (GLuint*)&m_ProgramID);

	emitter.Initialise( emitter_type::direction, 100, glm::vec4( 0, 0, 0, 1 ), 100, 0.5, 1, 3, 5, 0.5f, 0.8f, color.Green, color.Green );
	emitter.SetDirection( glm::vec4( 0, 1, 0, 0 ) );

	//BuildPerlinTexture(&perlin_1_texture, glm::ivec2(tweak_perlin_size, tweak_perlin_size), tweak_perlin_octaves, tweak_perlin_persistance, (float)0.0f);
	//gridMesh = BuildGrid(glm::vec2(128, 128), glm::ivec2(128, 128));
	//gridMesh = BuildGridWithNormals(glm::vec2(128, 128), glm::ivec2(256, 256));
	//BuildVertexGrid(glm::vec2(128, 128), glm::ivec2(128, 128));

	Reload();
	ReloadShaders();

	return true;
}

void ProceduralEnvironment::ApplicationShutdown() {
	Application::ApplicationShutdown();
}

bool ProceduralEnvironment::Update()
{
	CheckInput();
	if (!Application::Update()) {
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////

	//buildingBase->Update(deltaTime);

	int index = 0;
	for (int row = 0; row < 3; ++row) {
		for (int col = 0; col < 3; ++col) {

			buildings[index]->Update(deltaTime);
			index++;
		}
	}

	//m_pyro.skeleton = m_file->getSkeletonByIndex( 0 );
	//m_pyro.animation = m_file->getAnimationByIndex( 0 );

	////m_pyro.skeleton->evaluate(m_pyro.animation, m_fTimer);
	//EvaluateSkeleton( m_pyro.animation, m_pyro.skeleton, currentGameTime );

	//for( unsigned int i = 0; i < m_pyro.skeleton->m_boneCount; ++i ) {
	//	m_pyro.skeleton->m_nodes[i]->updateGlobalTransform();
	//	glm::mat4 node_global = m_pyro.skeleton->m_nodes[i]->m_globalTransform;
	//	glm::vec3 node_pos = node_global[3].xyz;
	//	Gizmos::addAABB( node_pos, glm::vec3( 5.0f ), color.Red, &node_global );
	//	if( m_pyro.skeleton->m_nodes[i]->m_parent != nullptr ) {
	//		Gizmos::addLine( node_pos, m_pyro.skeleton->m_nodes[i]->m_parent->m_globalTransform[3].xyz, color.Green );
	//	}
	//}

	if( bunnyObject->worldTransform[3].y > tweak_lava_height )
		bunnyObject->worldTransform[3].y = highest_position.y + tweak_perlin_position.y;

	if( soulspears[0]->worldTransform[3].y > tweak_lava_height )
		soulspears[0]->worldTransform[3].y = highest_position.y + tweak_perlin_position.y;
	if( soulspears[1]->worldTransform[3].y > tweak_lava_height )
		soulspears[1]->worldTransform[3].y = lowest_position.y + tweak_lava_height;

	emitter.Update( deltaTime );
	emitter.UpdateVertexData( cameraVector[currentCamera]->GetPosition(), cameraVector[currentCamera]->GetUp(), bunnyObject->worldTransform[3] );

	//emitter.m_position.x += 2 * deltaTime;
	//emitter.m_position = bunnyObject->worldTransform[3];
	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void ProceduralEnvironment::Draw() {
	//glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	RenderGeometry();
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	RenderLights();
	glDisable(GL_BLEND);

	RenderComposite();

	//////////////////////
	//! End of Draw Code 
	//////////////////////

	Application::Draw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void ProceduralEnvironment::RenderGeometry() {
	glViewport(0, 0, gBuffer.m_screen.Width, gBuffer.m_screen.Height);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.m_fbo);
	glClearColor(defaultBackgroundColour.x, defaultBackgroundColour.y, defaultBackgroundColour.z, defaultBackgroundColour.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gBufferProgramID);

	unsigned int	uniform_view = glGetUniformLocation(gBufferProgramID, "view");
	unsigned int	uniform_proj_view = glGetUniformLocation(gBufferProgramID, "projection_view");
	unsigned int	uniform_mesh_position = glGetUniformLocation(gBufferProgramID, "mesh_offset");
	glUniformMatrix4fv(uniform_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetView());
	glUniformMatrix4fv(uniform_proj_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());
	//glUniform3fv(uniform_mesh_position, 1, (float*)&buildingBase->worldTransform[3]);

	int uniform_timer = glGetUniformLocation(gBufferProgramID, "timer");
	glUniform1f(uniform_timer, currentGameTime);

	int uniform_lava_texture = glGetUniformLocation(gBufferProgramID, "lava_texture");
	glUniform1i(uniform_lava_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lava_texture);

	int uniform_soul_spear_texture = glGetUniformLocation(gBufferProgramID, "soul_spear_texture");
	glUniform1i(uniform_soul_spear_texture, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grass_texture);

	

	//buildingBase->Draw();
	//bunnyObject->worldTransform[3] = glm::vec4(0, 1, 0, 1);
	glUniform3fv(uniform_mesh_position, 1, (float*)&bunnyObject->worldTransform[3]);
	bunnyObject->Draw();

	

	for (int i = 0; i < 9; ++i) {
		//buildings[i]->Draw(gBufferProgramID);
	}

	for (unsigned int i = 0; i < soulspears.size(); i++)
	{
		glUniform3fv(uniform_mesh_position, 1, (float*)&soulspears[i]->worldTransform[3]);
		soulspears[i]->Draw();
	}

	DrawParticles();


	//DrawAnimation();

	DrawPerlin();

	for( unsigned int i = 0; i < DEBUGPOSITIONS.size(); i++ )
	{
		//Gizmos::addLine(DEBUGPOSITIONS[i], DEBUGPOSITIONS[i] + DEBUGNORMALS[i] * 2, color.Green);
	}
	DebugDraw();

}
void ProceduralEnvironment::DrawParticles() {

	emitter.m_position = glm::vec4( 0, 30, 0, 1 );
	//emitter.m_position = bunnyObject->worldTransform[3];

	glUseProgram( particleProgramID );
	unsigned int	uniform_proj_view = glGetUniformLocation( particleProgramID, "projection_view" );
	unsigned int	uniform_view = glGetUniformLocation( particleProgramID, "view" );
	unsigned int	uniform_mesh_position = glGetUniformLocation( particleProgramID, "offset" );
	glUniformMatrix4fv( uniform_proj_view, 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetProjectionView() );
	glUniformMatrix4fv( uniform_view, 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetView() );
	glUniform3fv( uniform_mesh_position, 1, ( float* ) &emitter.m_position );
	emitter.Render();
}

void ProceduralEnvironment::DrawAnimation() {
	//glUseProgram( animatedProgramID );
	//int proj_view_handle = glGetUniformLocation( animatedProgramID, "ProjectionView" );
	//if( proj_view_handle >= 0 ) {
	//	glUniformMatrix4fv( proj_view_handle, 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetProjectionView() );
	//}

	//unsigned int	uniform_view = glGetUniformLocation( animatedProgramID, "view" );
	//glUniformMatrix4fv( uniform_view, 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetView() );

	//UpdateBones( m_pyro.skeleton );

	//int diffuse_uniform = glGetUniformLocation( animatedProgramID, "Diffuse_Tex" );
	//glUniform1i( diffuse_uniform, 0 );
	//int bones_uniform = glGetUniformLocation( animatedProgramID, "bones" );
	//glUniformMatrix4fv( bones_uniform, m_pyro.skeleton->m_boneCount, GL_FALSE, ( float* ) m_pyro.skeleton->m_bones );

	//for( unsigned int i = 0; i < m_pyro.meshes.size(); ++i ) {
	//	FBXMeshNode* curr_mesh = m_file->getMeshByIndex( i );
	//	glm::mat4 world_transform = m_file->getMeshByIndex( i )->m_globalTransform;
	//	FBXMaterial* mesh_material = curr_mesh->m_material;

	//	// Bind Texture from file to opengl
	//	glActiveTexture( GL_TEXTURE0 );
	//	glBindTexture( GL_TEXTURE_2D, mesh_material->textures[FBXMaterial::DiffuseTexture]->handle );

	//	// Get World Transform of the mesh from current mesh
	//	int World_Uniform = glGetUniformLocation( animatedProgramID, "World" );
	//	glUniformMatrix4fv( World_Uniform, 1, GL_FALSE, ( float* ) &world_transform );

	//	glBindVertexArray( m_pyro.meshes[i].m_VAO );
	//	glDrawElements( GL_TRIANGLES, m_pyro.meshes[i].m_index_count, GL_UNSIGNED_INT, 0 );
	//}
}

void ProceduralEnvironment::DrawPerlin() {
	// render pass
	glUseProgram(perlinDrawProgramID);

	int uniform_projection_view = glGetUniformLocation(perlinDrawProgramID, "projection_view");
	int uniform_world = glGetUniformLocation(perlinDrawProgramID, "world");
	unsigned int	uniform_view = glGetUniformLocation(perlinDrawProgramID, "view");
	int uniform_timer = glGetUniformLocation(perlinDrawProgramID, "timer");
	int uniform_position_offset = glGetUniformLocation(perlinDrawProgramID, "position_offset");
	int uniform_lava_direction = glGetUniformLocation(perlinDrawProgramID, "lava_direction");
	int uniform_lava_speed = glGetUniformLocation(perlinDrawProgramID, "lava_speed");
	int uniform_lava_height = glGetUniformLocation(perlinDrawProgramID, "lava_height");
	int uniform_rock_height = glGetUniformLocation(perlinDrawProgramID, "rock_height");
	int uniform_dirt_height = glGetUniformLocation(perlinDrawProgramID, "dirt_height");

	glUniformMatrix4fv(uniform_projection_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());
	glUniformMatrix4fv(uniform_world, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetWorld());
	glUniformMatrix4fv(uniform_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetView());

	glUniform1f(uniform_timer, currentGameTime);
	glUniform3fv(uniform_position_offset, 1, (float*)&tweak_perlin_position);
	glUniform3fv(uniform_lava_direction, 1, (float*)&tweak_lava_direction);
	glUniform1f(uniform_lava_speed, tweak_lava_speed);
	glUniform1f(uniform_lava_height, tweak_lava_height);
	glUniform1f(uniform_rock_height, tweak_rock_height);
	glUniform1f(uniform_dirt_height, tweak_dirt_height);

	int uniform_perlin_height_texture = glGetUniformLocation(perlinDrawProgramID, "perlin_1_texture");
	glUniform1i(uniform_perlin_height_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, perlin_1_texture);

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
	glDrawArrays(GL_POINTS, 0, grid_size);

	glBindVertexArray(gridMesh.m_VAO);
	glDrawElements(GL_TRIANGLES, gridMesh.m_index_count, GL_UNSIGNED_INT, 0);

	//last_draw_time = 
	//active_buffer = other_buffer;
}

void ProceduralEnvironment::RenderLights() {
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.light_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawDirectionLight(tweak_light_direction, tweak_light_color);
	//DrawDirectionLight(cameraVector[currentCamera]->GetForward(), tweak_light_color);

	//DrawPointLight(cameraVector[0]->GetPosition(), 100, color.White.xyz);

}
void ProceduralEnvironment::DrawPointLight(glm::vec3 a_position, float a_radius, glm::vec3 a_color) {
	glUseProgram(pointLightProgramID);
	glm::vec4 view_space_pos = cameraVector[currentCamera]->GetView() * glm::vec4(a_position, 1);

	int uniform_pos = glGetUniformLocation(pointLightProgramID, "light_position");
	int uniform_view_pos = glGetUniformLocation(pointLightProgramID, "light_view_position");
	int uniform_light_color = glGetUniformLocation(pointLightProgramID, "light_color");
	int uniform_light_radius = glGetUniformLocation(pointLightProgramID, "light_radius");
	int uniform_proj_view = glGetUniformLocation(pointLightProgramID, "projection_view");

	glUniform3fv(uniform_pos, 1, (float*)&a_position);
	glUniform3fv(uniform_view_pos, 1, (float*)&view_space_pos);
	glUniform3fv(uniform_light_color, 1, (float*)&a_color);
	glUniform1f(uniform_light_radius, a_radius);
	glUniformMatrix4fv(uniform_proj_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	unsigned int uniform_position_texture = glGetUniformLocation(pointLightProgramID, "position_texture");
	glUniform1i(uniform_position_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.position_texture);
	unsigned int uniform_normals_texture = glGetUniformLocation(pointLightProgramID, "normals_texture");
	glUniform1i(uniform_normals_texture, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.normals_texture);

	glBindVertexArray(pointLight.m_VAO);
	glDrawElements(GL_TRIANGLES, pointLight.m_index_count, GL_UNSIGNED_INT, 0);
}

void ProceduralEnvironment::DrawDirectionLight(const glm::vec3& a_direction, const glm::vec3& a_color) {
	glUseProgram(directionalLightProgramID);

	glm::vec4 view_space_light = cameraVector[currentCamera]->GetView() * glm::vec4(glm::normalize(a_direction), 0);

	int uniform_light_direction = glGetUniformLocation(directionalLightProgramID, "light_dir");
	int uniform_light_color = glGetUniformLocation(directionalLightProgramID, "light_color");
	unsigned int	uniform_view = glGetUniformLocation( directionalLightProgramID, "view" );

	glUniform3fv(uniform_light_direction, 1, (float*)&view_space_light);
	glUniform3fv(uniform_light_color, 1, (float*)&a_color);
	glUniformMatrix4fv( uniform_view, 1, GL_FALSE, ( float* ) &cameraVector[currentCamera]->GetView() );

	unsigned int uniform_position_texture = glGetUniformLocation(directionalLightProgramID, "position_texture");
	glUniform1i(uniform_position_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.position_texture);

	unsigned int uniform_normals_texture = glGetUniformLocation(directionalLightProgramID, "normals_texture");
	glUniform1i(uniform_normals_texture, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.normals_texture);


	glBindVertexArray(gBuffer.m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, gBuffer.m_plane.m_index_count, GL_UNSIGNED_INT, 0);
}

void ProceduralEnvironment::RenderComposite() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClearColor(defaultBackgroundColour.x, defaultBackgroundColour.y, defaultBackgroundColour.z, defaultBackgroundColour.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(compositeProgramID);

	glUniform1i(glGetUniformLocation(compositeProgramID, "albedo_texture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.albedo_texture);

	glUniform1i(glGetUniformLocation(compositeProgramID, "light_texture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.light_texture);

	glUniformMatrix4fv(glGetUniformLocation(compositeProgramID, "projection_view"), 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	glBindVertexArray(gBuffer.m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, gBuffer.m_plane.m_index_count, GL_UNSIGNED_INT, 0);
}

void ProceduralEnvironment::DebugDraw() {
	Application::DebugDraw();
}

void ProceduralEnvironment::CheckInput() {
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		ReloadShaders();
	}
	if( glfwGetKey( window, GLFW_KEY_T ) == GLFW_PRESS ) {
		Reload();
	}
	Application::CheckInput();
}



void ProceduralEnvironment::LoadShaders() {
	LoadShader((GLuint*)&gBufferProgramID, "ass1_gbuffer_vertex.glsl", "ass1_gbuffer_fragment.glsl", nullptr);
	LoadShader((GLuint*)&compositeProgramID, "ass1_composite_vertex.glsl", "ass1_composite_fragment.glsl", nullptr);

	LoadShader( ( GLuint* ) &particleProgramID, "ass1_particle_vertex.glsl", "ass1_particle_fragment.glsl", nullptr );


	LoadShader((GLuint*)&directionalLightProgramID, "ass1_directional_light_vertex.glsl", "ass1_directional_light_fragment.glsl", nullptr);
	LoadShader((GLuint*)&pointLightProgramID, "ass1_point_light_vertex.glsl", "ass1_point_light_fragment.glsl", nullptr);
	LoadShader((GLuint*)&spotLightProgramID, "shadow_map_vertex.glsl", "shadow_map_fragment.glsl", nullptr);

	//CreateUpdateShader();
	LoadShader((GLuint*)&perlinDrawProgramID, "ass1_perlin_vertex.glsl", "ass1_perlin_fragment.glsl", nullptr);
	//LoadShader((GLuint*)&perlinDrawProgramID, "ass1_perlin_vertex.glsl", "ass1_perlin_fragment.glsl", nullptr );
}

void ProceduralEnvironment::Reload()
{
	gridMesh = BuildGridWithNormals(glm::vec2(tweak_perlin_mesh_size, tweak_perlin_mesh_size), glm::ivec2(tweak_perlin_texture_size, tweak_perlin_texture_size));
	//BuildPerlinTexture(&perlin_1_texture, glm::ivec2(tweak_perlin_size, tweak_perlin_size), tweak_perlin_octaves, tweak_perlin_persistance, (float)0.0f);

}

void ProceduralEnvironment::ReloadShaders() {
	printf("reloaded Shaders\n");
	//glfwSetTime( 0.0 );
	//currentGameTime = 0.0f;



	glDeleteProgram(perlinUpdateProgramID);
	glDeleteProgram(perlinDrawProgramID);
	glDeleteProgram(gBufferProgramID);
	glDeleteProgram(directionalLightProgramID);
	glDeleteProgram(pointLightProgramID);
	glDeleteProgram(spotLightProgramID);
	glDeleteProgram(compositeProgramID);
	LoadShaders();
}



OpenGLData ProceduralEnvironment::BuildGrid(glm::vec2 real_dims, glm::ivec2 dims) {
	// Allocate vertex data
	OpenGLData temp;
	unsigned int vertex_count = (dims.x + 1) * (dims.y + 1);
	VertexUV* vertex_data = new VertexUV[vertex_count];

	unsigned int index_count = dims.x * dims.y * 6;
	unsigned int* index_data = new unsigned int[index_count];


	BuildPerlinTexture(&perlin_1_texture, glm::ivec2(tweak_perlin_texture_size, tweak_perlin_texture_size), (int)tweak_perlin_octaves, tweak_perlin_persistance, (float)0.0f);
	//BuildPerlinTexture(&perlin_1_texture, glm::ivec2(128, 128), tweak_perlin_octaves, tweak_perlin_persistance, (float)0.0f);

	//real_dims = vec2(128, 128);
	//dims = vec2(100, 100);

	int index = 0;
	float curr_y = -real_dims.y / 2.0f;
	for (int y = 0; y < dims.y + 1; ++y) {
		float curr_x = -real_dims.x / 2.0f;
		for (int x = 0; x < dims.x + 1; ++x) {
			vertex_data[index++].Position = glm::vec4(curr_x, 0, curr_y, 1);
			vertex_data[y * (dims.x + 1) + x].UV = glm::vec2((float)x / (float)dims.x, (float)y / (float)dims.y);
			curr_x += real_dims.x / (float)dims.x;
		}
		curr_y += real_dims.y / (float)dims.y;
	}

	///// $$$ NORMALS!!!!!!!!
	/*
	for( fucking do all the normals here )
	pos.y += (texture(perlin_1_texture, uv).r * offset_pos) - offset_pos/2;
	*/

	int curr_index = 0;
	for (int y = 0; y < dims.y; ++y) {
		for (int x = 0; x < dims.x; ++x) {
			index_data[curr_index++] = y * (dims.x + 1) + x;
			index_data[curr_index++] = (y + 1) * (dims.x + 1) + x;
			index_data[curr_index++] = (y + 1) * (dims.x + 1) + (x + 1);
			index_data[curr_index++] = (y + 1) * (dims.x + 1) + (x + 1);
			index_data[curr_index++] = y * (dims.x + 1) + (x + 1);
			index_data[curr_index++] = y * (dims.x + 1) + x;
		}
	}

	temp.m_index_count = index_count;
	//create vertexarrayobecject, buffers, etc
	glGenVertexArrays(1, &temp.m_VAO);
	glGenBuffers(1, &temp.m_VBO);
	glGenBuffers(1, &temp.m_IBO);

	//bind and fill buffers	
	glBindVertexArray(temp.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, temp.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUV)* vertex_count, vertex_data, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* index_count, index_data, GL_STATIC_DRAW);

	// tell opengl about our vertex structure
	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // UV
	//glEnableVertexAttribArray(2); // Normals

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexUV), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)sizeof(glm::vec4));
	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)sizeof(glm::vec4) + (void*)sizeof(glm::vec2);

	// unbind stuff
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] index_data;
	delete[] vertex_data;

	return temp;
}

OpenGLData ProceduralEnvironment::BuildGridWithNormals(glm::vec2 real_dims, glm::ivec2 dims) {
	// Allocate vertex data
	OpenGLData temp;			// 128       128 
	unsigned int vertex_count = (dims.x) * (dims.y);
	VertexNormal* vertex_data = new VertexNormal[vertex_count];

	unsigned int index_count = (dims.x) * (dims.y) * 6;
	unsigned int* index_data = new unsigned int[index_count];



	BuildPerlinTexture(&perlin_1_texture, glm::vec2(dims.x, dims.y), (int)tweak_perlin_octaves, tweak_perlin_persistance, (float)0.0f);
	//BuildPerlinTexture(&perlin_1_texture, glm::ivec2(tweak_perlin_size, tweak_perlin_size), tweak_perlin_octaves, tweak_perlin_persistance, (float)0.0f);
	//BuildPerlinTexture(&perlin_1_texture, glm::ivec2(129, 129), tweak_perlin_octaves, tweak_perlin_persistance, (float)0.0f);

	//real_dims = vec2(128, 128);
	//dims = vec2(128, 128);

	int index = 0;
	float Ypos = 0;
	int highestIndex = 0;
	int lowestIndex = 0;
	float curr_y = -real_dims.y / 2.0f;
	for (int y = 0; y < dims.y; y++) {
		float curr_x = -real_dims.x / 2.0f;
		for (int x = 0; x < dims.x; x++, index++) {

			Ypos = (perlin_data[index] * tweak_perlin_height) - (tweak_perlin_height / 2);
			vertex_data[index].Position = glm::vec4(curr_x, Ypos, curr_y, 1);
			vertex_data[index].UV = glm::vec2((float)x / (float)dims.x, (float)y / (float)dims.y);

			if (index > 0 && vertex_data[index].Position.y >= vertex_data[highestIndex].Position.y)
			{
				highestIndex = index;
			}
			if (index > 0 && vertex_data[index].Position.y <= vertex_data[lowestIndex].Position.y)
			{
				lowestIndex = index;
			}

			curr_x += real_dims.x / (float)dims.x;
		}
		curr_y += real_dims.y / (float)dims.y;
	}

	//bunnyObject = new MeshGroup();
	//if (!LoadOBJFile(bunnyObject, "stanford/bunny.obj")) {
	//	return false;
	//}
	//

	MeshGroup* spearHigh = new MeshGroup;
	MeshGroup* spearLow = new MeshGroup;
	soulspears.clear();

	//if (LoadOBJFile(spear, "stanford/bunnyUV.obj"))
	if (LoadOBJFile(spearHigh, "soulspear/soulspear.obj"))
	{
		highest_position = vertex_data[highestIndex].Position;
		spearHigh->worldTransform[3] = highest_position;
		soulspears.push_back(spearHigh);
		int i = 0;
	}
	if (LoadOBJFile(spearLow, "soulspear/soulspear.obj"))
	{
		lowest_position = vertex_data[lowestIndex].Position;
		spearLow->worldTransform[3] = lowest_position; 
		spearLow->worldTransform[3].y = 0;
		soulspears.push_back(spearLow);
		int i = 0;
	}
	bunnyObject->worldTransform[3] = glm::vec4( vertex_data[lowestIndex].Position.x, vertex_data[highestIndex].Position.y, vertex_data[lowestIndex].Position.z, 1 );
	emitter.m_position = vertex_data[lowestIndex].Position;
	emitter.m_position.y = 0;
	

	float texture_size = (float)dims.x;
	float texture_offset = 1 / texture_size;
	float sample_size = 1.0f;
	index = 0;
	curr_y = -real_dims.y / 2.0f;
	for (int y = 0; y < dims.y; ++y) {
		float curr_x = -real_dims.x / 2.0f;
		for (int x = 0; x < dims.x; ++x) {


			//if (x > 0 && x < dims.x - 1 && y > 0 && y < dims.y - 1)
			//{


				glm::vec4 final_normal = glm::vec4(0, 0, 0, 0);

				glm::vec4 thispos = vertex_data[index].Position;

				std::vector<glm::vec4> vertexs;

				int index_up = index - dims.x;
				if (y > 0)
				{
					glm::vec4 pos_up = vertex_data[index_up].Position;
					vertexs.push_back(pos_up);
				}
				int index_up_right = (index - dims.x + 1);
				if (y > 0 && x < dims.x-1)
				{
					glm::vec4 pos_up_right = vertex_data[index_up_right].Position;
					vertexs.push_back(pos_up_right);
				}
				int index_right = index + 1;
				if (x < dims.x-1 )
				{
					glm::vec4 pos_right = vertex_data[index_right].Position;
					vertexs.push_back(pos_right);
				}
				int index_down_right = index + dims.x + 1;
				if (y < dims.y-1  && x < dims.x-1 )
				{
					glm::vec4 pos_down_right = vertex_data[index_down_right].Position;
					vertexs.push_back(pos_down_right);
				}
				int index_down = index + dims.x;
				if (y < dims.y-1 )
				{
					glm::vec4 pos_down = vertex_data[index_down].Position;
					vertexs.push_back(pos_down);
				}
				int index_down_left = index + dims.x - 1;
				if (y < dims.y-1  && x > 0)
				{
					glm::vec4 pos_down_left = vertex_data[index_down_left].Position;
					vertexs.push_back(pos_down_left);
				}
				int index_left = index - 1;
				if (x > 0)
				{
					glm::vec4 pos_left = vertex_data[index_left].Position;
					vertexs.push_back(pos_left);
				}
				int index_up_left = index - dims.x - 1;
					if (y > 0 && x > 0)
				{
					glm::vec4 pos_up_left = vertex_data[index_up_left].Position;
					vertexs.push_back(pos_up_left);
				}

				for (unsigned int i = 0; i < vertexs.size(); i++)
				{
					if (i == vertexs.size() - 1)
					{
						final_normal.xyz += glm::cross(vertexs[i].xyz - thispos.xyz, vertexs[0].xyz - thispos.xyz);
					}
					else
					{
						final_normal.xyz += glm::cross(vertexs[i].xyz - thispos.xyz, vertexs[i + 1].xyz - thispos.xyz);
					}
				}

				final_normal = normalize(final_normal);
				//vertex_data[y * (dims.x + 1) + x].Normal = glm::vec4(curr_x, 0, curr_y, 1);
				vertex_data[index].Normal = final_normal;
				vertex_data[index].Tangent = glm::vec4(curr_x, 0, curr_y, 1);

				DEBUGPOSITIONS.push_back(thispos.xyz);
				DEBUGNORMALS.push_back(final_normal.xyz);
		//	}
			//else
			//{
			//	vertex_data[index].Normal = glm::vec4(0, 1, 0, 1);
			//	vertex_data[index].Tangent = glm::vec4(curr_x, 0, curr_y, 1);
			//
			//
			//}

			index++;

			curr_x += real_dims.x / (float)dims.x;
		}
		curr_y += real_dims.y / (float)dims.y;
	}



	///// $$$ NORMALS!!!!!!!!
	/*
	for( fucking do all the normals here )
	pos.y += (texture(perlin_1_texture, uv).r * offset_pos) - offset_pos/2;
	*/
	index = 0;
	int curr_index = 0;
	for (int y = 0; y < dims.y - 1; y++) {
		for (int x = 0; x < dims.x - 1; x++) {

			index_data[curr_index++] = y * (dims.x) + x;
			index_data[curr_index++] = (y + 1) * (dims.x) + x;
			index_data[curr_index++] = (y + 1) * (dims.x) + (x + 1);

			index_data[curr_index++] = (y + 1) * (dims.x) + (x + 1);
			index_data[curr_index++] = y * (dims.x) + (x + 1);
			index_data[curr_index++] = y * (dims.x) + x;
			index++;
		}
	}

	temp.m_index_count = index_count;
	//create vertexarrayobecject, buffers, etc
	glGenVertexArrays(1, &temp.m_VAO);
	glGenBuffers(1, &temp.m_VBO);
	glGenBuffers(1, &temp.m_IBO);

	//bind and fill buffers	
	glBindVertexArray(temp.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, temp.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormal)* vertex_count, vertex_data, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* index_count, index_data, GL_STATIC_DRAW);

	// tell opengl about our vertex structure
	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Normals
	glEnableVertexAttribArray(2); // Tangents
	glEnableVertexAttribArray(3); // UV

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), (void*)(sizeof(glm::vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), (void*)(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), (void*)(sizeof(glm::vec4) * 3));

	// unbind stuff
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] index_data;
	delete[] vertex_data;

	return temp;
}


void ProceduralEnvironment::BuildVertexGrid(glm::vec2 real_dims, glm::ivec2 dims) {

	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	unsigned int vertex_count = (dims.x + 1) * (dims.y + 1);
	VertexUV* vertex_data = new VertexUV[vertex_count];

	unsigned int index_count = dims.x * dims.y;
	unsigned int* index_data = new unsigned int[index_count];

	float curr_y = -real_dims.y / 2.0f;
	for (int y = 0; y < dims.y + 1; ++y) {
		float curr_x = -real_dims.x / 2.0f;
		for (int x = 0; x < dims.x + 1; ++x) {
			vertex_data[y * (dims.x + 1) + x].Position = glm::vec4(curr_x, 0, curr_y, 1);
			vertex_data[y * (dims.x + 1) + x].UV = glm::vec2((float)x / (float)dims.x, (float)y / (float)dims.y);
			curr_x += real_dims.x / (float)dims.x;
		}
		curr_y += real_dims.y / (float)dims.y;
	}

	grid_size = vertex_count;

	for (unsigned int buffer_index = 0; buffer_index < 2; ++buffer_index) {


		glBindVertexArray(vao[buffer_index]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[buffer_index]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUV)* vertex_count, vertex_data, GL_STREAM_DRAW);

		glEnableVertexAttribArray(0); // Position
		glEnableVertexAttribArray(1); // UV

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexUV), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)sizeof(glm::vec4));

	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool ProceduralEnvironment::BuildPerlinTexture(unsigned int* a_texture, const glm::ivec2 dims, const int octaves, const float persistance, const float offset) {
	bool result = true;
	// set scale
	float scale = (1.0f / dims.x) * 3.0f;

	// Allocate memory for perlin data
	perlin_data = new float[dims.x * dims.y];

	for (int y = 0; y < dims.y; ++y) {
		for (int x = 0; x < dims.x; ++x) {
			float amplitude = 1.0f;
			float frequency = 1.0f;
			perlin_data[y * dims.x + x] = 0;

			for (int i = 0; i < octaves; ++i) {
				float perlin_sample = glm::perlin(glm::vec2(x + offset, y + offset) * scale * frequency + currentGameTime) * 0.5f + 0.5f;
				perlin_sample *= amplitude;
				perlin_data[y * dims.x + x] += perlin_sample;

				amplitude *= persistance;
				frequency *= 2.0f;
			}
		}
	}

	// generate opengl texture handles
	glGenTextures(1, a_texture);
	glBindTexture(GL_TEXTURE_2D, *a_texture);

	// pass the perlin data to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, dims.x, dims.y, 0, GL_RED, GL_FLOAT, perlin_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return result;
}

void ProceduralEnvironment::CreateUpdateShader() {

	unsigned int vertex_shader;
	LoadShaderType("../data/shaders/ass1_perlin_update_vertex.glsl", GL_VERTEX_SHADER, &vertex_shader);



	perlinUpdateProgramID = glCreateProgram();
	glAttachShader(perlinUpdateProgramID, vertex_shader);

	const char* outputs[2] = { "updated_position"
		"updated_uv" };

	glTransformFeedbackVaryings(perlinUpdateProgramID, 2, outputs, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(perlinUpdateProgramID);
	int success = GL_FALSE;
	glGetProgramiv(perlinUpdateProgramID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int log_length = 0;
		glGetProgramiv(perlinUpdateProgramID, GL_INFO_LOG_LENGTH, &log_length);
		char* log = new char[log_length];
		glGetProgramInfoLog(perlinUpdateProgramID, log_length, 0, log);
		printf("Error: Shaders SHIT FAILED \n ");
		printf("%s", log);

		delete[] log;
	}
	glDeleteShader(vertex_shader);

}

void ProceduralEnvironment::GenerateGLMeshes( FBXFile* fbx, TestObject& object ) {
	unsigned int mesh_count = fbx->getMeshCount();

	object.meshes.resize( mesh_count );

	for( unsigned int mesh_index = 0; mesh_index < mesh_count; ++mesh_index ) {
		FBXMeshNode* curr_mesh = fbx->getMeshByIndex( mesh_index );

		object.meshes[mesh_index].m_index_count = curr_mesh->m_indices.size();

		glGenBuffers( 1, &object.meshes[mesh_index].m_VBO );
		glGenBuffers( 1, &object.meshes[mesh_index].m_IBO );
		glGenVertexArrays( 1, &object.meshes[mesh_index].m_VAO );

		glBindVertexArray( object.meshes[mesh_index].m_VAO );

		glBindBuffer( GL_ARRAY_BUFFER, object.meshes[mesh_index].m_VBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof( FBXVertex )* curr_mesh->m_vertices.size(), curr_mesh->m_vertices.data(), GL_STATIC_DRAW );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, object.meshes[mesh_index].m_IBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int )* curr_mesh->m_indices.size(), curr_mesh->m_indices.data(), GL_STATIC_DRAW );

		glEnableVertexAttribArray( 0 ); // Position
		glEnableVertexAttribArray( 1 ); // UV
		glEnableVertexAttribArray( 2 ); // Bone Indices
		glEnableVertexAttribArray( 3 ); // Bone Weights

		glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( FBXVertex ), ( void* ) FBXVertex::PositionOffset );
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( FBXVertex ), ( void* ) FBXVertex::TexCoord1Offset );
		glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( FBXVertex ), ( void* ) FBXVertex::IndicesOffset );
		glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, sizeof( FBXVertex ), ( void* ) FBXVertex::WeightsOffset );

		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
}

void ProceduralEnvironment::EvaluateSkeleton( FBXAnimation* anim, FBXSkeleton* skeleton, float timer ) {
	float fps = 24.0f;
	int current_frame = ( int ) ( timer * fps );

	// loop through all the tracks
	for( unsigned int track_index = 0; track_index < anim->m_trackCount; ++track_index ) {
		// wrap back to the start of the track if we've gone too far
		int track_frame_count = anim->m_tracks[track_index].m_keyframeCount;
		int track_frame = current_frame % track_frame_count;

		// find what keyframes are currently affecting the bones
		FBXKeyFrame curr_frame = anim->m_tracks[track_index].m_keyframes[track_frame];
		FBXKeyFrame next_frame = anim->m_tracks[track_index].m_keyframes[( track_frame + 1 ) % track_frame_count];

		float time_since_frame_flip = timer - ( current_frame / fps );
		float t = time_since_frame_flip * fps;

		glm::vec3 new_pos = glm::mix( curr_frame.m_translation, next_frame.m_translation, t );
		glm::vec3 new_scale = glm::mix( curr_frame.m_scale, next_frame.m_scale, t );
		glm::quat new_rot = glm::slerp( curr_frame.m_rotation, next_frame.m_rotation, t );

		glm::mat4 local_transform = glm::translate( new_pos ) * glm::toMat4( new_rot ) * glm::scale( new_scale );

		// get the right bone for the given track 
		int bone_index = anim->m_tracks[track_index].m_boneIndex;


		// set the FBXNode's local Transforms to match
		if( bone_index < skeleton->m_boneCount ) {
			skeleton->m_nodes[bone_index]->m_localTransform = local_transform;
		}

		// interpolate between those keyframes to generate the matrix for the current pose


	}
}
void ProceduralEnvironment::UpdateBones( FBXSkeleton* skeleton ) {

	// loop through the nodes in the skeleton
	for( unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; ++bone_index ) {
		// generate their global Transforms
		int parent_index = skeleton->m_parentIndex[bone_index];
		if( parent_index == -1 ) {
			skeleton->m_bones[bone_index] = skeleton->m_nodes[bone_index]->m_localTransform;
		} else {
			skeleton->m_bones[bone_index] = skeleton->m_bones[parent_index] * skeleton->m_nodes[bone_index]->m_localTransform;
		}


	}

	for( unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; ++bone_index ) {
		// mutiply the global transform by the inverse bind pose
		skeleton->m_bones[bone_index] = skeleton->m_bones[bone_index] * skeleton->m_bindPoses[bone_index];
	}
}

