#ifndef _THREADING_H_
#define _THREADING_H_

#include "Application.h"

#include <thread>

class Threading : public Application
{
private:
	// Base Variables

	// Project Specific

public:
	virtual ~Threading();

	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	void print();
	
	// Project Specific 
	//std::thread myThread(print);
	


};

#endif // _THREADING_H_