#ifndef GUINANIMATION_H_
#define GUINANIMATION_H_

#include <irrlicht.h>

using namespace irr;

class GuiAnimation
{
public:
	GuiAnimation(gui::IGUIElement* element, float x, float y, float speed, float delay);
	void update (float deltaTime);
	~GuiAnimation(void);
private:
	gui::IGUIElement* element;
	float x;
	float y;
	float speed;
	float delay;
	float running;
	core::vector2df position;
};

#endif
