
#include "GameEngine.h"
#include "GameScene.h"
#include "Bullet.h"

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

	// Array with bullets
	Bullet* bullets[10];

	// Add which place in the bullets array to add the new bullet
	int bulletIndex;

	// Counter to keep track of the shoot cooldown
	float shootCooldown;
};

