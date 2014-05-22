#ifndef MAINSCENE_H_
#define MAINSCENE_H_

#include <irrlicht.h>
#include <ParticleManager.h>
#include <ParticleModel.h>
#include <ParticleParser.h>
#include "EffScene.h"
#include "Enemy.h"

using namespace irr;

class EffEngine;
class Robot;
class WaveSystem;
class GuiAnimation;

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

	void spawnEnemy (core::vector2df position, Enemy::TYPES type);

	void createHUD(void);

	void onEnemyDie(void);

	void onPlayerHit(void);

	void showWaveText(core::stringw text);
	void hideWaveText();

	core::vector2df spawnPoint1;
	core::vector2df spawnPoint2;
	core::vector2df spawnPoint3;

	int enemiesAlive;
	
private:

	void AddWaves (void);

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

	/**
	 * Controller which keeps track of the waves, starts and stops them
	 */
	WaveSystem* waveSystem;

	GuiAnimation* hudBarAnim;

	gui::IGUIStaticText* scoreText;
	int score;
	GuiAnimation* scoreAnim;

	gui::IGUIStaticText* xpText;
	int xp;
	GuiAnimation* xpAnim;

	gui::IGUIStaticText* healthText;
	GuiAnimation* healthAnim;

	gui::IGUIStaticText* waveText;
};

#endif
