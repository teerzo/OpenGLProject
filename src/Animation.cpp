#include "Animation.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

#include "Utility.h"

#include "Entity.h"


Animation::~Animation()
{

}

void Animation::setDefaults()
{
	this->AppName = "Animation";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;
}

bool Animation::startup()
{
	if (!Application::startup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();
	glfwSetTime(0.0);

	m_cube = new Entity;

	m_file = new FBXFile;
	m_file->load("./models/characters/Pyro/pyro.fbx");
	//m_file->load("./models/all.fbx");
	m_file->initialiseOpenGLTextures();
	GenerateGLMeshes(m_file, m_pyro);
	LoadShader((GLuint*)&m_ProgramID,"../data/shaders/skinned_vertex.glsl", "../data/shaders/skinned_fragment.glsl", nullptr );
	//LoadShader("../data/shaders/BasicVertex.glsl", "../data/shaders/BasicFragment.glsl", (GLuint*)&m_ProgramID);

	return true;
}

void Animation::shutdown()
{
	m_file->unload();
	delete m_file;
	Application::shutdown();
}

bool Animation::update()
{
	if (!Application::update())
	{
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////

	m_pyro.skeleton = m_file->getSkeletonByIndex(0);
	m_pyro.animation = m_file->getAnimationByIndex(0);

	//m_pyro.skeleton->evaluate(m_pyro.animation, m_fTimer);
	EvaluateSkeleton(m_pyro.animation, m_pyro.skeleton, m_fTimer);

	for (unsigned int i = 0; i < m_pyro.skeleton->m_boneCount; ++i)
	{
		m_pyro.skeleton->m_nodes[i]->updateGlobalTransform();
		glm::mat4 node_global = m_pyro.skeleton->m_nodes[i]->m_globalTransform;
		glm::vec3 node_pos = node_global[3].xyz;
		Gizmos::addAABB(node_pos, glm::vec3(5.0f), color.Red, &node_global);
		if (m_pyro.skeleton->m_nodes[i]->m_parent != nullptr)
		{
			Gizmos::addLine(node_pos, m_pyro.skeleton->m_nodes[i]->m_parent->m_globalTransform[3].xyz, color.Green);
		}
	}

	m_cube->Update();

	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void Animation::draw()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);

	int proj_view_handle = glGetUniformLocation(m_ProgramID, "ProjectionView");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, GL_FALSE, (float*)&m_vListofCameras[ActiveCamera]->getProjectionView());

	}
	///////////////////////////////////
	//! Project Specific Drawcode Here 
	///////////////////////////////////

	//m_pyro.skeleton->updateBones();
	UpdateBones(m_pyro.skeleton);

	// Uniforms
	int diffuse_uniform = glGetUniformLocation(m_ProgramID, "Diffuse_Tex");
	glUniform1i(diffuse_uniform, 0);
	int bones_uniform = glGetUniformLocation(m_ProgramID, "bones");
		glUniformMatrix4fv(bones_uniform, m_pyro.skeleton->m_boneCount, GL_FALSE, (float*)m_pyro.skeleton->m_bones );
	
	for (unsigned int i = 0; i < m_pyro.meshes.size(); ++i)
	{
		FBXMeshNode* curr_mesh = m_file->getMeshByIndex(i);
		glm::mat4 world_transform = m_file->getMeshByIndex(i)->m_globalTransform;
		FBXMaterial* mesh_material = curr_mesh->m_material;

		// Bind Texture from file to opengl
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh_material->textures[FBXMaterial::DiffuseTexture]->handle);

		// Get World Transform of the mesh from current mesh
		int World_Uniform = glGetUniformLocation(m_ProgramID, "World");
		glUniformMatrix4fv(World_Uniform, 1, GL_FALSE, (float*)&world_transform);

		glBindVertexArray(m_pyro.meshes[i].m_VAO);
		glDrawElements(GL_TRIANGLES, m_pyro.meshes[i].m_index_count, GL_UNSIGNED_INT, 0);
	}

	m_cube->Draw();

	//////////////////////
	//! End of Draw Code 
	//////////////////////
	Application::draw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void Animation::GenerateGLMeshes(FBXFile* fbx, TestObject& object)
{
	unsigned int mesh_count = fbx->getMeshCount();

	object.meshes.resize(mesh_count);

	for (unsigned int mesh_index = 0; mesh_index < mesh_count; ++mesh_index)
	{
		FBXMeshNode* curr_mesh = fbx->getMeshByIndex(mesh_index);

		object.meshes[mesh_index].m_index_count = curr_mesh->m_indices.size();

		glGenBuffers(1, &object.meshes[mesh_index].m_VBO);
		glGenBuffers(1, &object.meshes[mesh_index].m_IBO);
		glGenVertexArrays(1, &object.meshes[mesh_index].m_VAO);

		glBindVertexArray(object.meshes[mesh_index].m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, object.meshes[mesh_index].m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(FBXVertex)* curr_mesh->m_vertices.size(), curr_mesh->m_vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.meshes[mesh_index].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* curr_mesh->m_indices.size(), curr_mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // Position
		glEnableVertexAttribArray(1); // UV
		glEnableVertexAttribArray(2); // Bone Indices
		glEnableVertexAttribArray(3); // Bone Weights

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::PositionOffset);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::TexCoord1Offset);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::IndicesOffset);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::WeightsOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Animation::EvaluateSkeleton(FBXAnimation* anim, FBXSkeleton* skeleton, float timer)
{
	float fps = 24.0f;
	int current_frame = (int)(timer * fps);

	// loop through all the tracks
	for (unsigned int track_index = 0; track_index < anim->m_trackCount; ++track_index)
	{
		// wrap back to the start of the track if we've gone too far
		int track_frame_count = anim->m_tracks[track_index].m_keyframeCount;
		int track_frame = current_frame % track_frame_count;
		
		// find what keyframes are currently affecting the bones
		FBXKeyFrame curr_frame = anim->m_tracks[track_index].m_keyframes[track_frame];
		FBXKeyFrame next_frame = anim->m_tracks[track_index].m_keyframes[(track_frame + 1) % track_frame_count];

		float time_since_frame_flip = timer - (current_frame / fps);
		float t = time_since_frame_flip * fps;

		glm::vec3 new_pos = glm::mix(curr_frame.m_translation, next_frame.m_translation, t);
		glm::vec3 new_scale = glm::mix(curr_frame.m_scale, next_frame.m_scale, t);
		glm::quat new_rot = glm::slerp(curr_frame.m_rotation, next_frame.m_rotation, t);

		glm::mat4 local_transform = glm::translate(new_pos) * glm::toMat4(new_rot) * glm::scale(new_scale);

		// get the right bone for the given track 
		int bone_index = anim->m_tracks[track_index].m_boneIndex;
	

		// set the FBXNode's local Transforms to match
		if (bone_index < skeleton->m_boneCount)
		{
			skeleton->m_nodes[bone_index]->m_localTransform = local_transform;
		}

		// interpolate between those keyframes to generate the matrix for the current pose
		
	
	}	
}
void Animation::UpdateBones(FBXSkeleton* skeleton)
{

	// loop through the nodes in the skeleton
	for (unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; ++bone_index)
	{
		// generate their global Transforms
		int parent_index = skeleton->m_parentIndex[bone_index];
		if (parent_index == -1)
		{
			skeleton->m_bones[bone_index] = skeleton->m_nodes[bone_index]->m_localTransform;
		}
		else
		{
			skeleton->m_bones[bone_index] = skeleton->m_bones[parent_index] *skeleton->m_nodes[bone_index]->m_localTransform;
		}

		
	}

	for (unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; ++bone_index)
	{
		// mutiply the global transform by the inverse bind pose
		skeleton->m_bones[bone_index] = skeleton->m_bones[bone_index] * skeleton->m_bindPoses[bone_index];
	}
}