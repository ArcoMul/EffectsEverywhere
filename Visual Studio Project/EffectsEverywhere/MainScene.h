#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "GameEngine.h"
#include "GameScene.h"
#include "Bullet.h"
#include "Enemy.h"

class BackgroundFader;

class MainScene : public GameScene
{
public:
	MainScene(GameEngine* engine);
	void start (void);
	void update (void);
	~MainScene(void);

	ICameraSceneNode* camera;
	
private:
	IMeshSceneNode* robot;

	// Array with bullets
	Bullet* bullets[10];

	// Add which place in the bullets array to add the new bullet
	int bulletIndex;

	// Counter to keep track of the shoot cooldown
	float shootCooldown;

	Enemy* enemy1;
	Enemy* enemy2;
};

#endif
