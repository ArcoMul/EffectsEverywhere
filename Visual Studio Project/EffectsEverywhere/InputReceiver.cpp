#include "InputReceiver.h"

InputReceiver::InputReceiver(void)
{
	// Create a bool array with all keys
	for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}

bool InputReceiver::OnEvent(const SEvent& event)
{
	// Remember whether each key is down or up
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	return false;
}

bool InputReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
	// Return if the given key is down
	return KeyIsDown[keyCode];
}


InputReceiver::~InputReceiver(void)
{
	
}
