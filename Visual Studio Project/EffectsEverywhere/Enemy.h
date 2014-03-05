#ifndef ENEMY_H
#define ENEMY_H

#include "GameEngine.h"
#include "GameScene.h"

class Enemy
{
public:
	Enemy(GameEngine* engine, ICameraSceneNode* _camera, IMeshSceneNode* robot);

	GameEngine* _engine;
private:
	
};

#endif
