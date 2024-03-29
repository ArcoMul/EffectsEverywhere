#include "InputReceiver.h"
#include "EffEngine.h"

InputReceiver::InputReceiver(EffEngine* engine)
{
	this->engine = engine;
	// Create a bool array with all keys
	for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}

bool InputReceiver::OnEvent(const SEvent& event)
{
	// Remember whether each key is down or up
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	if(event.EventType == EET_MOUSE_INPUT_EVENT)
    {
		switch(event.MouseInput.Event)
            {
            case EMIE_LMOUSE_PRESSED_DOWN:
                MLeftButtonDown = true;
                break;

            case EMIE_LMOUSE_LEFT_UP:
                MLeftButtonDown = false;
                break;

            case EMIE_MOUSE_MOVED:
				// store mouse X and Y coords
                cursor = core::position2di(event.MouseInput.X, event.MouseInput.Y);
                break;

            default:
                // We won't use the wheel
                break;
            }
    }
	// Mouse click on button
	if (event.EventType == EET_GUI_EVENT)
    {
        s32 id = event.GUIEvent.Caller->getID();
        switch(event.GUIEvent.EventType)
        {
		case EGET_BUTTON_CLICKED:
        engine->onButtonClick(id);
		default:
			return false;
        }  
    }
	return false;
}

bool InputReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
	// Return if the given key is down
	return KeyIsDown[keyCode];
}

bool InputReceiver::IsMLeftButtonDown(void)
{
	// Return if the given key is down
	return MLeftButtonDown;
}

InputReceiver::~InputReceiver(void)
{	
}
