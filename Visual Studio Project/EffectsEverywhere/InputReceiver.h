#ifndef INPUTRECEIVER_H
#define INPUTRECEIVER_H

#include <irrlicht.h>

using namespace irr;

class EffEngine;

class InputReceiver : public IEventReceiver
{
public:
	InputReceiver(EffEngine* engine);
	core::position2di cursor;

	
	// Called by Irrlicht when a key event happens
	virtual bool OnEvent(const SEvent& event);
	// To check if a key is down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const;

	virtual bool IsMLeftButtonDown(void);

	~InputReceiver(void);
private:

	EffEngine* engine;
    // We use this array to store the current state of each key
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool MLeftButtonDown;
};

#endif
