#ifndef EFFACTOR_H_
#define EFFACTOR_H_

#include <irrlicht.h>

using namespace irr;

class EffScene;

class EffActor
{
public:
	EffActor(void);
	virtual void update (float deltaTime);
	virtual void setNode (scene::ISceneNode* node);
	virtual void setScene (EffScene* scene);
	~EffActor(void);

	scene::ISceneNode* node;
protected:
	EffScene* scene;
};

#endif