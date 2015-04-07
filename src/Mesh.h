#ifndef _MESH_H_
#define _MESH_H_

#include "Vertex.h"
#include <vector>

#include "tiny_obj_loader.h"
#include "gl_core_4_4.h"

#include <string>

class Mesh
{
public:
	//OpenGLData meshData;
			
};

class MeshGroup
{
public:


	glm::mat4 worldTransform;

	std::vector<OpenGLData> meshData;

	MeshGroup();
	void Update(float deltaTime);
	void Draw();

};

OpenGLData GeneratePlane();
OpenGLData GenerateCube();

//void LoadOBJFile(MeshGroup &mesh, std::vector<tinyobj::shape_t>& a_shapes);
bool LoadOBJFile(MeshGroup *mesh, std::string filepath );


#endif // _MESH_H_