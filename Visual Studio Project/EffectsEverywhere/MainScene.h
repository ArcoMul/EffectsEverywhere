#ifndef MAINSCENE_H_
#define MAINSCENE_H_

#include <irrlicht.h>
#include <ParticleManager.h>
#include <ParticleModel.h>
#include "EffScene.h"
#include <ParticleManager.h>
#include <ParticleModel.h>
#include <ParticleParser.h>

using namespace irr;

class EffEngine;
class Enemy;
class Robot;

class MainScene : public EffScene
{
public:
	MainScene();

	/**
	 * Creates all the objects, basically sets up the scene visually
	 */
	virtual bool init (void);
	
	virtual void update (float deltaTime);

	~MainScene(void);

	void spawnEnemy(void);

	void createHUD(void);

	void onEnemyDies(void);

	void onPlayerHit(void);
	
private:

	/**
	 * The player
	 */
	Robot* robot;

	/**
	 * List of all the enemies in the scene
	 */
	core::list<Enemy*> enemies;

	/**
	 * The camera in the scene
	 */
	scene::ICameraSceneNode* camera;

	gui::IGUIStaticText* scoreText;
	int score;

	gui::IGUIStaticText* xpText;
	int xp;

	gui::IGUIStaticText* healthText;
	int health;
};

#endif
