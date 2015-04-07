#ifndef _BUILDING_H_ 
#define _BUILDING_H_

class MeshGroup;

class Building
{
public:

	//glm::mat4 worldTransform;

	MeshGroup* base;
	MeshGroup* northSlot;
	MeshGroup* southSlot;
	MeshGroup* eastSlot;
	MeshGroup* westSlot;

	Building();
	bool InitiateBuilding();

	void Update(float deltaTime);

	void Draw(unsigned int &program);



};


#endif // _BUILDING_H_