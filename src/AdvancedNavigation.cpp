#include "AdvancedNavigation.h"

//#include "gl_core_4_4.h"
#include <glfw3.h>
//#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

#include "Building.h"

AdvancedNavigation::~AdvancedNavigation()
{

}

void AdvancedNavigation::SetApplicationDefaults()
{
	this->applicationName = "AdvancedNavigation";
	this->screenSize.Width = 1280;
	this->screenSize.Height = 720;
}

bool AdvancedNavigation::ApplicationStartup()
{
	if (!Application::ApplicationStartup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	// enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);

	// Load Shadow Frame Buffer
	gBuffer.SetWindowSize(1280, 720);
	gBuffer.SetPlaneSize(1280, 720);
	gBuffer.GenerateTarget();

	gBuffer.GenerateGBuffer();
	gBuffer.GenerateLightBuffer();

	pointLight = GenerateCube();

	// Load Plane
	//glPlane = GeneratePlane();
	// Load Mesh

	buildingBase = new MeshGroup();
	if (!LoadOBJFile(buildingBase, "stanford/SponzaSimple.obj"))
	{
		return false;
	}


	buildingNavMesh = new MeshGroup();
	if (!LoadOBJFile(buildingNavMesh, "stanford/SponzaSimpleNavMesh.obj")) {
		return false;
	}

	BuildNavMesh();

	// Load Shaders
	LoadShaders();


	return true;
}

void AdvancedNavigation::ApplicationShutdown()
{
	Application::ApplicationShutdown();
}

bool AdvancedNavigation::Update()
{
	CheckInput();
	if (!Application::Update())
	{
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////

	//buildingBase->Update(deltaTime);

	for (unsigned int i = 0; i < nav_nodes.size(); ++i)
	{
		//Gizmos::addAABB(nav_nodes[i].vertices[0], glm::vec3(0.1), color.Red);
		//Gizmos::addAABB(nav_nodes[i].vertices[1], glm::vec3(0.1), color.Red);
		//Gizmos::addAABB(nav_nodes[i].vertices[2], glm::vec3(0.1), color.Red);

		Gizmos::addAABB(nav_nodes[i].position, glm::vec3(0.1), color.Green);
	

		Gizmos::addLine(nav_nodes[i].vertices[0], nav_nodes[i].vertices[1], color.Yellow);
		Gizmos::addLine(nav_nodes[i].vertices[1], nav_nodes[i].vertices[2], color.Yellow);
		Gizmos::addLine(nav_nodes[i].vertices[2], nav_nodes[i].vertices[0], color.Yellow);
/*
		Gizmos::addLine(nav_nodes[i].position, nav_nodes[i].edges[0]->position, color.Blue);
		Gizmos::addLine(nav_nodes[i].position, nav_nodes[i].edges[1]->position, color.Blue);
		Gizmos::addLine(nav_nodes[i].position, nav_nodes[i].edges[2]->position, color.Blue);

*/
	}

	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void AdvancedNavigation::Draw()
{
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

void AdvancedNavigation::RenderGeometry()
{
	glViewport(0, 0, gBuffer.m_screen.Width, gBuffer.m_screen.Height);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.m_fbo);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gBufferProgramID);

	unsigned int	uniform_view = glGetUniformLocation(gBufferProgramID, "View");
	unsigned int	uniform_proj_view = glGetUniformLocation(gBufferProgramID, "ProjectionView");
	unsigned int	uniform_mesh_position = glGetUniformLocation(gBufferProgramID, "MeshPosition");
	glUniformMatrix4fv(uniform_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetView());
	glUniformMatrix4fv(uniform_proj_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());
	//glUniform4fv(uniform_mesh_position, 1, (float*)&buildingBase->worldTransform[3]);


	buildingBase->Draw();
	//buildingNavMesh->Draw();


	DebugDraw();

	//glCullFace(GL_BACK); 
	//glBindVertexArray(pointLight.m_VAO);
	//glDrawElements(GL_TRIANGLES, pointLight.m_index_count, GL_UNSIGNED_INT, 0);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AdvancedNavigation::RenderLights()
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.light_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawDirectionLight(cameraVector[0]->GetForward(), color.White.xyz);

	//DrawPointLight(cameraVector[0]->GetPosition(), 10, color.White.xyz);

}
void AdvancedNavigation::DrawPointLight(glm::vec3 a_position, float a_radius, glm::vec3 a_color)
{
	glUseProgram(pointLightProgramID);
	glm::vec4 view_space_pos = cameraVector[currentCamera]->GetView() * glm::vec4(a_position, 1);

	int uniform_pos = glGetUniformLocation(pointLightProgramID, "light_position");
	int uniform_view_pos = glGetUniformLocation(pointLightProgramID, "light_view_position");
	int uniform_light_color = glGetUniformLocation(pointLightProgramID, "light_color");
	int uniform_light_radius = glGetUniformLocation(pointLightProgramID, "light_radius");
	int uniform_proj_view = glGetUniformLocation(pointLightProgramID, "ProjectionView");

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

void AdvancedNavigation::DrawDirectionLight(const glm::vec3& a_direction, const glm::vec3& a_color)
{
	glUseProgram(directionalLightProgramID);

	glm::vec4 view_space_light = cameraVector[currentCamera]->GetView() * glm::vec4(glm::normalize(a_direction), 0);

	int uniform_light_direction = glGetUniformLocation(directionalLightProgramID, "light_dir");
	int uniform_light_color = glGetUniformLocation(directionalLightProgramID, "light_color");

	glUniform3fv(uniform_light_direction, 1, (float*)&view_space_light);
	glUniform3fv(uniform_light_color, 1, (float*)&a_color);

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

void AdvancedNavigation::RenderComposite()
{
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

	glUniformMatrix4fv(glGetUniformLocation(compositeProgramID, "ProjectionView"), 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	glBindVertexArray(gBuffer.m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, gBuffer.m_plane.m_index_count, GL_UNSIGNED_INT, 0);
}

void AdvancedNavigation::DebugDraw()
{
	Application::DebugDraw();
}

void AdvancedNavigation::CheckInput()
{
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		ReloadShaders();
	}
	Application::CheckInput();
}



void AdvancedNavigation::LoadShaders()
{
	LoadShader((GLuint*)&gBufferProgramID, "gbuffer_vertex.glsl", "gbuffer_fragment.glsl", nullptr);
	LoadShader((GLuint*)&compositeProgramID, "composite_vertex.glsl", "composite_fragment.glsl", nullptr);

	LoadShader((GLuint*)&directionalLightProgramID, "directional_light_vertex.glsl", "directional_light_fragment.glsl", nullptr);
	LoadShader((GLuint*)&pointLightProgramID, "point_light_vertex.glsl", "point_light_fragment.glsl", nullptr);
	LoadShader((GLuint*)&spotLightProgramID, "shadow_map_vertex.glsl", "shadow_map_fragment.glsl", nullptr);
}

void AdvancedNavigation::ReloadShaders()
{
	printf("reloaded Shaders\n");
	glfwSetTime(0.0);
	currentGameTime = 0.0f;
	glDeleteProgram(gBufferProgramID);
	glDeleteProgram(directionalLightProgramID);
	glDeleteProgram(pointLightProgramID);
	glDeleteProgram(spotLightProgramID);
	glDeleteProgram(compositeProgramID);
	LoadShaders();
}


bool AdvancedNavigation::BuildNavMesh()
{
	// load our nav mesh from obj file 
	bool result = true;
	std::string filepath = "stanford/SponzaSimpleNavMesh.obj";

	std::vector <tinyobj::shape_t> tiny_shapes;
	std::vector <tinyobj::material_t> tiny_materials;

	std::string fullPath = "../data/models/" + filepath;

	std::string err = tinyobj::LoadObj(tiny_shapes, tiny_materials, fullPath.c_str());
	if (err.size() != 0)
	{
		// Mesh Load Error #1
		printf(" Mesh Load Error #1", err);
		printf(" OBJ file missing or Corrupt data ");
		result = false;
	}
	else if (tiny_shapes.size() != 1)
	{
		result = false;
	}
	else
	{
		tinyobj::mesh_t* mesh = &tiny_shapes[0].mesh;

		// resize our nav nodes vector to hold 
		// the right size of nodes 
		unsigned int index_count = mesh->indices.size();
		nav_nodes.resize(index_count / 3);
		
		// loop through all the indices and Building
		// our nodes - set the centres and corners
		for (unsigned int index = 0; index < index_count; index += 3)
		{
			unsigned int node_index = index / 3;
			
			for (unsigned int corner_index = 0; corner_index < 3; ++corner_index)
			{
				unsigned vertex_index = mesh->indices[index + corner_index];
				nav_nodes[node_index].vertices[corner_index].x = mesh->positions[vertex_index * 3 + 0];
				nav_nodes[node_index].vertices[corner_index].y = mesh->positions[vertex_index * 3 + 1];
				nav_nodes[node_index].vertices[corner_index].z = mesh->positions[vertex_index * 3 + 2];
			}
			nav_nodes[node_index].position = (nav_nodes[node_index].vertices[0] + nav_nodes[node_index].vertices[1] + nav_nodes[node_index].vertices[2]) / 3.0f;
		}

		for (auto& first : nav_nodes)
		{
			unsigned int curr_edge = 0;
			first.edges[0] = 0;
			first.edges[1] = 0;
			first.edges[2] = 0;

			for (auto& second : nav_nodes)
			{
				if (&first == &second)
				{
					continue;
				}

				for (unsigned int edge = 0; edge < 3; ++edge)
				{
					if (first.vertices[edge] == second.vertices[0] && first.vertices[(edge + 1) % 3] == second.vertices[1] ||
						first.vertices[edge] == second.vertices[1] && first.vertices[(edge + 1) % 3] == second.vertices[2] ||
						first.vertices[edge] == second.vertices[2] && first.vertices[(edge + 1) % 3] == second.vertices[0] ||

						first.vertices[edge] == second.vertices[1] && first.vertices[(edge + 1) % 3] == second.vertices[0] ||
						first.vertices[edge] == second.vertices[2] && first.vertices[(edge + 1) % 3] == second.vertices[1] ||
						first.vertices[edge] == second.vertices[0] && first.vertices[(edge + 1) % 3] == second.vertices[2])
					{
						first.edges[curr_edge] = &second;
						first.edgeCosts[curr_edge] = glm::distance(second.position, first.position);
						++curr_edge;
						break;
					}
				}
				if (curr_edge == 3)
				{
					break;
				}
			}
		}


	}
	return result;
}




