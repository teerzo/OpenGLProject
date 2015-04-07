#ifndef _KM_GUI_MANAGER_H_
#define _KM_GUI_MANAGER_H_

#include "Vertex.h"
#include <vector>

#include "Camera.h"

class kmGUIObject
{
public:
	kmGUIObject() {}
	OpenGLData vertexdata;
	//Rectangle guiRectangle;

	bool active;
	bool visible;

	virtual void Update(float deltaTime);
	virtual void Draw();

};

class kmGUIObject2D : kmGUIObject
{
public:
	glm::vec3 position;

	virtual void Update();
	virtual void Draw();
};

class kmGUIObject3D : kmGUIObject
{
public:
	glm::mat4 worldTransform;

	virtual void Update(float deltaTime);
	virtual void Draw();
};


class kmGUIManager
{
public:

	Camera* cameraTarget;

	std::vector<kmGUIObject*> guiObjects;


	kmGUIObject CreateNewGUIObject(Rectangle a_rectangle);
	bool AddNewGUIObject(kmGUIObject* a_element);



	void Update(float deltaTime);
	// Draw all elements
	void Draw();
	// Render to target Camera;
	void Render();

	void SetTarget(Camera* camera);


};



#endif // _KM_GUI_MANAGER_H_