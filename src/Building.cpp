#include "Building.h"

#include "Mesh.h"

Building::Building()
{
	
}

bool Building::InitiateBuilding()
{
	bool result = true;



	base = new MeshGroup;
	northSlot = new MeshGroup;
	southSlot = new MeshGroup;
	eastSlot = new MeshGroup;
	westSlot = new MeshGroup;

	LoadOBJFile(base, "BuildingBase.obj");
	LoadOBJFile(northSlot, "BuildingDoor.obj");
	LoadOBJFile(southSlot, "BuildingWindow.obj");
	LoadOBJFile(eastSlot, "BuildingWindow.obj");
	LoadOBJFile(westSlot, "BuildingWindow.obj");
	
	northSlot->worldTransform[3] += glm::vec4(2, 0, 0, 1);
	southSlot->worldTransform[3] += glm::vec4(-2, 0, 0, 1);
	eastSlot->worldTransform[3] += glm::vec4(0, 0, 2, 1);
	westSlot->worldTransform[3] += glm::vec4(0, 0, -2, 1);

	return result;
}

void Building::Update(float deltaTime)
{
	
	northSlot->worldTransform[3] = base->worldTransform[3] + glm::vec4(2, 0, 0, 1);
	southSlot->worldTransform[3] = base->worldTransform[3] + glm::vec4(-2, 0, 0, 1);
	eastSlot->worldTransform[3] = base->worldTransform[3] + glm::vec4(0, 0, 2, 1);
	westSlot->worldTransform[3] = base->worldTransform[3] + glm::vec4(0, 0, -2, 1);
}

void Building::Draw(unsigned int &program)
{
	unsigned int	uniform_mesh_position = glGetUniformLocation(program, "MeshPosition");
	glUniform4fv(uniform_mesh_position, 1, (float*)&base->worldTransform[3]);
	base->Draw();
	glUniform4fv(uniform_mesh_position, 1, (float*)&northSlot->worldTransform[3]);
	northSlot->Draw();
	glUniform4fv(uniform_mesh_position, 1, (float*)&southSlot->worldTransform[3]);
	southSlot->Draw();
	glUniform4fv(uniform_mesh_position, 1, (float*)&eastSlot->worldTransform[3]);
	eastSlot->Draw();
	glUniform4fv(uniform_mesh_position, 1, (float*)&westSlot->worldTransform[3]);
	westSlot->Draw();
}
