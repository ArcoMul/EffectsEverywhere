#ifndef BULLET_H_
#define BULLET_H_

#include <irrlicht.h>
#include "EffActor.h"

using namespace irr;

class Bullet : public EffActor
{
public:
	Bullet(void);
	Bullet(float aliveSince);
	virtual void update (float deltaTime);
	~Bullet(void);
	float aliveSince;
};

#endif
