#ifndef ENDSCENE_H_
#define ENDSCENE_H_

#include <irrlicht.h>
#include "EffScene.h"

using namespace irr;

class GuiAnimation;

class EndScene : public EffScene
{
public:
	EndScene(bool won, int score);

	/**
	 * Creates all the objects, basically sets up the scene visually
	 */
	virtual bool init (void);
	virtual void update (float deltaTime);
	
	virtual void onButtonClick(s32 id);

	~EndScene(void);

	

	enum BUTTONS
	{
		GUI_ID_MAIN_BUTTON = 101,
		GUI_ID_RESTART_BUTTON
	};
	
private:


	/**
	 * The camera in the scene
	 */
	scene::ICameraSceneNode* camera;

	bool won;
	int score;

	gui::IGUIStaticText* scoreText;
	gui::IGUIButton* restartButton;
	GuiAnimation* restartButtonAnim;

	gui::IGUIButton* backButton;
	GuiAnimation* backButtonAnim;

};

#endif