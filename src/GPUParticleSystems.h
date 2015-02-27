#ifndef _GPU_PARTICLE_SYSTEMS_H_
#define _GPU_PARTICLE_SYSTEMS_H_

#include "Application.h"

#include "GPUEmitter.h"

class GPUParticleSystems : public Application
{
private:
	// Base Variables

	// Project Specific

public:
	virtual ~GPUParticleSystems();

	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 
	GPUPointEmitter* m_emitter;
};

#endif // _GPU_PARTICLE_SYSTEMS_H_