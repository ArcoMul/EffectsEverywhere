#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "GameEngine.h"
#include "GameScene.h"

class BackgroundFader;

class MainScene : public GameScene
{
public:
	MainScene(GameEngine* engine);
	void start (void);
	void update (void);
	~MainScene(void);

	GameEngine* _engine;
	ICameraSceneNode* camera;
	BackgroundFader* backgroundFader;
private:
	IMeshSceneNode* robot;
};

#endif