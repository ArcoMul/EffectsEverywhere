#include "FadeOutActor.h"
#include "EffScene.h"

FadeOutActor::FadeOutActor(float delay, float speed)
{
	this->image = nullptr;
	this->text = nullptr;
	this->time = 0;
	this->delay = delay;
	this->speed = speed;
	this->alpha = 255;
}

void FadeOutActor::update(float deltaTime)
{
	EffActor::update(deltaTime);

	time += deltaTime;

	if (time < delay) return;

	bool didSomething = false;

	if (image != nullptr && alpha > 0)
	{
		alpha -= this->speed;
		image->setColor(video::SColor(alpha, 255, 255, 255));
		didSomething = true;
	}

	if (text != nullptr && alpha > 0)
	{
		alpha -= this->speed;
		text->setOverrideColor(video::SColor(alpha, 255, 255, 255));
		didSomething = true;
	}

	if (!didSomething) {
		this->scene->removeActor(this);
	}
}

void FadeOutActor::setGuiImage(gui::IGUIImage* image)
{
	this->image = image;
	this->image->setColor(video::SColor(255,255,255,255));
}

void FadeOutActor::setGuiText(gui::IGUIStaticText* text)
{
	this->text = text;
}

FadeOutActor::~FadeOutActor(void)
{
}
