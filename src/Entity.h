#ifndef _ENTITY_H_
#define _ENTITY_H_


#include "Object.h"


struct OpenGLData;

class Entity : public Object
{
private:
	OpenGLData m_cube;

public:
	Entity();

	virtual void Update();
	virtual void Draw();

	void GenerateCube();
	

};


#endif // _ENTITY_H_