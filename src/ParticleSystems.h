#ifndef _PARTICLE_SYSTEMS_H_
#define _PARTICLE_SYSTEMS_H_

#include "Application.h"

#include "Emitter.h"

class ParticleSystems : public Application
{
private:
	// Base Variables

	// Project Specific

public:
	virtual ~ParticleSystems();

	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 
	Emitter m_emitter;
};

#endif // _PARTICLE_SYSTEMS_H_