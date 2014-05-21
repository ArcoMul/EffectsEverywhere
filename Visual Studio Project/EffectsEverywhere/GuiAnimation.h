#ifndef GUINANIMATION_H_
#define GUINANIMATION_H_

#include <irrlicht.h>

using namespace irr;

class GuiAnimation
{
public:
	/**
	 * element: the element which gets animated
	 * x: goal of the x position
	 * y: goal of the y position
	 * speed: how fast the element should move
	 * delay: after how many miliseconds the animation should start
	 */
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
