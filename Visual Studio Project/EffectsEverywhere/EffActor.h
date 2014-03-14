#ifndef EFFACTOR_H_
#define EFFACTOR_H_

#include <irrlicht.h>

using namespace irr;

class EffActor
{
public:
	EffActor(void);
	virtual void update (float deltaTime);
	~EffActor(void);
private:
	scene::ISceneNode* node;
};

#endif