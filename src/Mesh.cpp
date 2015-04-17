#include "Mesh.h"

MeshGroup::MeshGroup()
{
	worldTransform = glm::mat4(1);
}

void MeshGroup::Update(float deltaTime)
{
	worldTransform[3] += glm::vec4(0, 0, 1, 1) * deltaTime;

}
void MeshGroup::Draw()
{
	for (unsigned int mesh_index = 0; mesh_index < this->meshData.size(); ++mesh_index)
	{
		glBindVertexArray(this->meshData[mesh_index].m_VAO);
		glDrawElements(GL_TRIANGLES, this->meshData[mesh_index].m_index_count, GL_UNSIGNED_INT, 0);
	}
}

OpenGLData GenerateCube()
{
	// Inverted Cube
	OpenGLData temp;
	temp.m_index_count = 36;

	float cubeVertexData[] = {
		-1, -1, 1, 1,
		1, -1, 1, 1,
		1, -1, -1, 1,
		-1, -1, -1, 1,
		-1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, -1, 1,
		-1, 1, -1, 1,
	};
	unsigned int cubeIndexData[] = {
		4, 5, 0,
		5, 1, 0,
		5, 6, 1,
		6, 2, 1,
		6, 7, 2,
		7, 3, 2,
		7, 4, 3,
		4, 0, 3,
		7, 6, 4,
		6, 5, 4,
		0, 1, 3,
		1, 2, 3
	};

	// Bind Vertex Data
	glGenVertexArrays(1, &temp.m_VAO);
	glBindVertexArray(temp.m_VAO);
	glGenBuffers(1, &temp.m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, temp.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData, GL_STATIC_DRAW);

	// Bind Indices
	glGenBuffers(1, &temp.m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndexData), cubeIndexData, GL_STATIC_DRAW);

	// Enable Shader Code
	glEnableVertexAttribArray(0); // Position  
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)* 4, 0); // Position

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return temp;
}

OpenGLData GeneratePlane()
{
	OpenGLData temp;
	temp.m_index_count = 6;
	float vertexData[] = {
		-5, 0, -1, 1, 0, 0,
		5, 0, -1, 1, 1, 0,
		5, 10, -1, 1, 1, 1,
		-5, 10, -1, 1, 0, 1,
	};
	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	// Bind Vertex Data
	glGenVertexArrays(1, &temp.m_VAO);
	glBindVertexArray(temp.m_VAO);
	glGenBuffers(1, &temp.m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, temp.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* temp.m_index_count * 4, vertexData, GL_STATIC_DRAW);

	// Bind Indices
	glGenBuffers(1, &temp.m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* temp.m_index_count, indexData, GL_STATIC_DRAW);

	// Enable Shader Code
	glEnableVertexAttribArray(0); // Position 
	glEnableVertexAttribArray(1); // UV
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)* temp.m_index_count, 0); // Position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)* temp.m_index_count, ((char*)0) + 16); // UV

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return temp;
}

bool LoadOBJFile(MeshGroup* mesh, std::string filepath )
{
	bool result = true;

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
	else
	{
		mesh->meshData.resize(tiny_shapes.size());

		for (unsigned int shape_index = 0; shape_index < tiny_shapes.size(); ++shape_index)
		{
			std::vector<float> vertex_data;

			unsigned int float_count = tiny_shapes[shape_index].mesh.positions.size();
			float_count += tiny_shapes[shape_index].mesh.normals.size();
			float_count += tiny_shapes[shape_index].mesh.texcoords.size();
			vertex_data.reserve(float_count);

			vertex_data.insert(vertex_data.end(), tiny_shapes[shape_index].mesh.positions.begin(), tiny_shapes[shape_index].mesh.positions.end());
			vertex_data.insert(vertex_data.end(), tiny_shapes[shape_index].mesh.normals.begin(), tiny_shapes[shape_index].mesh.normals.end());
			vertex_data.insert(vertex_data.end(), tiny_shapes[shape_index].mesh.texcoords.begin(), tiny_shapes[shape_index].mesh.texcoords.end());

			mesh->meshData[shape_index].m_index_count = tiny_shapes[shape_index].mesh.indices.size();

			// generate ....
			glGenVertexArrays(1, &mesh->meshData[shape_index].m_VAO);
			glGenBuffers(1, &mesh->meshData[shape_index].m_VBO);
			glGenBuffers(1, &mesh->meshData[shape_index].m_IBO);

			glBindVertexArray(mesh->meshData[shape_index].m_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, mesh->meshData[shape_index].m_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)* float_count, vertex_data.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->meshData[shape_index].m_IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, tiny_shapes[shape_index].mesh.indices.size() * sizeof(unsigned int), tiny_shapes[shape_index].mesh.indices.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0); // Position
			glEnableVertexAttribArray(1); // Normal data
			glEnableVertexAttribArray(2); // UV

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)*tiny_shapes[shape_index].mesh.positions.size()));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)*tiny_shapes[shape_index].mesh.positions.size() + sizeof(float)*tiny_shapes[shape_index].mesh.normals.size()));

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
	return result;
}



//void LoadOBJFile(MeshGroup &mesh, std::vector<tinyobj::shape_t>& a_shapes)
//{
//	mesh.meshData.resize(a_shapes.size());
//
//	for (unsigned int shape_index = 0; shape_index < a_shapes.size(); ++shape_index)
//	{
//		std::vector<float> vertex_data;
//
//		unsigned int float_count = a_shapes[shape_index].mesh.positions.size();
//		float_count += a_shapes[shape_index].mesh.normals.size();
//		vertex_data.reserve(float_count);
//
//		vertex_data.insert(vertex_data.end(), a_shapes[shape_index].mesh.positions.begin(), a_shapes[shape_index].mesh.positions.end());
//		vertex_data.insert(vertex_data.end(), a_shapes[shape_index].mesh.normals.begin(), a_shapes[shape_index].mesh.normals.end());
//
//		mesh.meshData[shape_index].m_index_count = a_shapes[shape_index].mesh.indices.size();
//
//		// generate ....
//		glGenVertexArrays(1, &mesh.meshData[shape_index].m_VAO);
//		glGenBuffers(1, &mesh.meshData[shape_index].m_VBO);
//		glGenBuffers(1, &mesh.meshData[shape_index].m_IBO);
//
//		glBindVertexArray(mesh.meshData[shape_index].m_VAO);
//
//		glBindBuffer(GL_ARRAY_BUFFER, mesh.meshData[shape_index].m_VBO);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* float_count, vertex_data.data(), GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.meshData[shape_index].m_IBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_shapes[shape_index].mesh.indices.size() * sizeof(unsigned int), a_shapes[shape_index].mesh.indices.data(), GL_STATIC_DRAW);
//
//		glEnableVertexAttribArray(0); // Position
//		glEnableVertexAttribArray(1); // Normal data
//
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)*a_shapes[shape_index].mesh.positions.size()));
//
//		glBindVertexArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	}
//}