#include <iostream>
#include "EndScene.h"
#include "EffEngine.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "EffTimer.h"
#include "StartScene.h"
#include "MainScene.h"
#include "GuiAnimation.h"

EndScene::EndScene(bool won, int score)
{
	this->won = won;
	this->score = score;
}

bool EndScene::init(void)
{
	EffScene::init ();

	// Show mouse in this scene
	setMouseVisible(true);

	this->getSoundEngine()->stopAllSounds();
	this->getSoundEngine()->play2D("../../Media/sounds/ipgamesopenloop.wav",true);
	
	// Background
	gui->addImage(this->getTexture(EffEngine::getPath("Media/menu/background.png").c_str()), core::position2d<int>(0, 0));

	// Won/Lost
	if(won){
		gui->addImage(this->getTexture(EffEngine::getPath("Media/menu/you-won.png").c_str()), core::position2d<int>(200, 35));
	}else{
		gui->addImage(this->getTexture(EffEngine::getPath("Media/menu/you-lost.png").c_str()), core::position2d<int>(200, 35));
	}

	// Score
	IGUISkin* skin = gui->getSkin();
	IGUIFont* font = gui->getFont((char*) EffEngine::getPath("Media/fonts/Score.xml").c_str());
	if (font) {
		skin->setFont(font);
	}
	scoreText = gui->addStaticText(core::stringw(score).c_str() , rect<s32>(440, 220, 800, 400), false);
	scoreText->setOverrideColor(video::SColor(255,193,0,28));
	scoreText->setTextAlignment(gui::EGUI_ALIGNMENT::EGUIA_LOWERRIGHT, gui::EGUI_ALIGNMENT::EGUIA_UPPERLEFT);
	gui->addImage(this->getTexture(EffEngine::getPath("Media/menu/label-points.png").c_str()), core::position2d<int>(722, 370));

	// Back button
	restartButton = gui->addButton(rect<s32>(509,770,857,838), 0, GUI_ID_RESTART_BUTTON);
	restartButton->setDrawBorder(false);
	restartButton->setImage(getTexture(EffEngine::getPath("Media/menu/button-play-again.png").c_str()));
	restartButton->setPressedImage(getTexture(EffEngine::getPath("Media/menu/button-play-again.png").c_str()));
	restartButtonAnim = new GuiAnimation(restartButton, 509, 550, 1.50, 400);

	backButton = gui->addButton(rect<s32>(509,868,857,936), 0, GUI_ID_MAIN_BUTTON);
	backButton->setDrawBorder(false);
	backButton->setImage(getTexture(EffEngine::getPath("Media/menu/button-back-to-menu.png").c_str()));
	backButton->setPressedImage(getTexture(EffEngine::getPath("Media/menu/button-back-to-menu.png").c_str()));
	backButtonAnim = new GuiAnimation(backButton, 509, 648, 1.50, 400);
	
	// Add the camera node to the scene
	camera = manager->addCameraSceneNode();

	return true;
}

void EndScene::update(float deltaTime)
{
	restartButtonAnim->update(deltaTime);
	backButtonAnim->update(deltaTime);
}

void EndScene::onButtonClick(s32 id)
{
	switch(id)
    {
	case BUTTONS::GUI_ID_MAIN_BUTTON:
		switchScene (new StartScene());
		break;
	case BUTTONS::GUI_ID_RESTART_BUTTON:
		switchScene (new MainScene());
		break;
	default:
		break;
	}
}

EndScene::~EndScene(void)
{
}