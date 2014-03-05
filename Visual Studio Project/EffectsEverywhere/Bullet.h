#include <irrlicht.h>

using namespace irr;

class Bullet
{
public:
	Bullet(void);
	Bullet(scene::IMeshSceneNode* node, float aliveSince);
	~Bullet(void);
	scene::IMeshSceneNode* node;
	float aliveSince;
};

