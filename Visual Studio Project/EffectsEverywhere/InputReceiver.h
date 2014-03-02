#include <irrlicht.h>

using namespace irr;

class InputReceiver : public IEventReceiver
{
public:
	core::position2di cursor; 
	

	InputReceiver(void);

	// Called by Irrlicht when a key event happens
	virtual bool OnEvent(const SEvent& event);

	

	// To check if a key is down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const;

	
        
 

	~InputReceiver(void);
private:
    // We use this array to store the current state of each key
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
