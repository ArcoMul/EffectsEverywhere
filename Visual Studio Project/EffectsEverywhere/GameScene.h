#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "GameEngine.h"

class GameScene
{
public:
	GameScene();
	GameScene(GameEngine* engine);
	virtual void start (void);
	virtual void update (void);
	~GameScene(void);

	GameEngine* _engine;
};

#endif