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
private:
	IMeshSceneNode* enemy1;
	IMeshSceneNode* enemy2;

	ISceneNodeAnimatorCollisionResponse* collision;
	ISceneNodeAnimatorCollisionResponse* collision2;

	ITriangleSelector* selectorEnemy1;
	ITriangleSelector* selectorEnemy2;

	
	
};

