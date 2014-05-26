#ifndef FADEOUTACTOR_H_
#define FADEOUTACTOR_H_

#include <irrlicht.h>
#include "EffActor.h"

using namespace irr;

class FadeOutActor : public EffActor
{
public:

	FadeOutActor(float delay, float speed = 0.3);
	virtual void update (float deltaTime);
	void setGuiImage(gui::IGUIImage* image);
	void setGuiText(gui::IGUIStaticText* text);
	~FadeOutActor(void);

private:
	float time;
	float delay;
	float speed;
	float alpha;
	gui::IGUIImage* image;
	gui::IGUIStaticText* text;
};

#endif