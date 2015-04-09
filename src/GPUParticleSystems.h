#ifndef _GPU_PARTICLE_SYSTEMS_H_
#define _GPU_PARTICLE_SYSTEMS_H_

#include "Application.h"

#include "GPUEmitter.h"

class GPUParticleSystems : public Application
{
public:
	// Project Specific 
	GPUPointEmitter* m_emitter;
	unsigned int GPUProgramID;
	unsigned int GPUUpdateProgramID;


	virtual ~GPUParticleSystems();

	virtual void SetApplicationDefaults();
	virtual bool ApplicationStartup();
	virtual void ApplicationShutdown();
	virtual void CheckInput();
	virtual bool Update();
	virtual void Draw();
	virtual void DebugDraw();
	void ReloadShaders();
	void LoadShaders();

	
};

#endif // _GPU_PARTICLE_SYSTEMS_H_