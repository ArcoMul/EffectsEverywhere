#ifndef MAINSCENE_H_
#define MAINSCENE_H_

#include <irrlicht.h>
#include <ParticleManager.h>
#include <ParticleModel.h>
#include <ParticleParser.h>
#include "EffScene.h"
#include "Enemy.h"

using namespace irr;
using namespace std::placeholders;

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
	void startPlaying(void);
	void createHUD(void);

	void onEnemyDie(Enemy* enemy);

	void onPlayerHit(void);

	void showWaveText(core::stringw text);
	void hideWaveText();

	core::vector2df spawnPoint1;
	core::vector2df spawnPoint2;
	core::vector2df spawnPoint3;

	int enemiesAlive;
	void setLevelWon(bool levelWon);
	
private:
	void AddWaves (void);
	scene::ISceneNodeAnimatorCollisionResponse* collisionLevel;
	/**
	 * Level start robot is landed
	 */
	bool levelstart;
	bool levelWon;
	bool hudActive;
	float gravity;
	/**
	 * The player
	 */
	Robot* robot;

	EffActor* ufo;

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
	GuiAnimation* xpAnim;

	gui::IGUIStaticText* healthText;
	GuiAnimation* healthAnim;

	gui::IGUIStaticText* waveText;
};

#endif
