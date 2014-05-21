#include "GuiAnimation.h"


GuiAnimation::GuiAnimation(gui::IGUIElement* element, float x, float y, float speed, float delay)
{
	this->x = x;
	this->y = y;
	this->speed = speed;
	this->delay = delay;
	this->element = element;
	running = 0;
	position = core::vector2df(element->getRelativePosition().UpperLeftCorner.X, element->getRelativePosition().UpperLeftCorner.Y);
}

void GuiAnimation::update (float deltaTime)
{
	running += deltaTime;

	if (delay > running) return;

	if (position.X < x) {
		position.X += speed * deltaTime;
	} else {
		position.X = x;
	}

	if (position.Y < y) {
		position.Y += speed * deltaTime;
	} else {
		position.Y = y;
	}

	element->setRelativePosition(core::position2di(position.X, position.Y));
}

GuiAnimation::~GuiAnimation(void)
{
}
