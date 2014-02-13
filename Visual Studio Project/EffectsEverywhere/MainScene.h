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
	BackgroundFader* backgroundFader;
private:
	IMeshSceneNode* robot;
};

