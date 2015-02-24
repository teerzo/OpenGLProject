#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "Application.h"

#include "../deps/FBXLoader/FBXFile.h"
#include "Vertex.h"
//#include "FBXFile.h"

class Entity;

struct TestObject
{
	std::vector<OpenGLData> meshes;
	FBXSkeleton* skeleton;
	FBXAnimation* animation;

};

class Animation : public Application
{
private:
	// Base Variables

	// Project Specific
	FBXFile* m_file;
	TestObject m_pyro; 
	Entity* m_cube;

public:
	virtual ~Animation();

	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 
	void GenerateGLMeshes(FBXFile* fbx, TestObject& object);
	void EvaluateSkeleton(FBXAnimation* anim, FBXSkeleton* skeleton, float timer);
	void UpdateBones(FBXSkeleton* skeleton);
};

#endif // _ANIMATION_H_