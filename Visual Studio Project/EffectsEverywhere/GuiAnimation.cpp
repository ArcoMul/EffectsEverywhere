#include "GuiAnimation.h"


GuiAnimation::GuiAnimation(gui::IGUIElement* element, float x, float y, float speed, float delay, bool loop)
{
	this->speed = speed;
	this->delay = delay;
	this->element = element;
	this->loop = loop;
	running = 0;
	position = core::vector2df(element->getRelativePosition().UpperLeftCorner.X, element->getRelativePosition().UpperLeftCorner.Y);
	goalPosition = core::vector2df(x, y);
	startPosition = position;
}

/**
 * TODO: remove the animation or something when the animation is done
 */
void GuiAnimation::update (float deltaTime)
{
	// Count up so that we know when to start
	running += deltaTime;

	// If we waited long enough, start animating
	if (delay > running) return;

	if (position.X < goalPosition.X - (speed * deltaTime)) {
		position.X += speed * deltaTime;
	} else if (position.X > goalPosition.X + (speed * deltaTime)) {
		position.X -= speed * deltaTime;
	} else {
		position.X = goalPosition.X;
	}

	if (position.Y < goalPosition.Y - (speed * deltaTime)) {
		position.Y += speed * deltaTime;
	} else if (position.Y > goalPosition.Y + (speed * deltaTime)) {
		position.Y -= speed * deltaTime;
	} else {
		position.Y = goalPosition.Y;
	}

	element->setRelativePosition(core::position2di(position.X, position.Y));

	if (loop && position.Y == goalPosition.Y && position.X == goalPosition.X) {
		goalPosition = startPosition;
		startPosition = position;
	}
}

GuiAnimation::~GuiAnimation(void)
{
}
