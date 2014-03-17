#ifndef EFFACTOR_H_
#define EFFACTOR_H_

#include <irrlicht.h>

using namespace irr;

class EffScene;

class EffActor
{
public:
	EffActor(void);
	virtual void start (void);
	virtual void update (float deltaTime);
	void setScene (EffScene* scene);
	void setNode (scene::ISceneNode* node);
	~EffActor(void);

	scene::ISceneNode* node;

protected:
	EffScene* scene;

private:
};

#endif