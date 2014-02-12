#include <iostream>
#include "MainScene.h"
#include "GameEngine.h"
#include "BackgroundFader.h"

MainScene::MainScene(GameEngine* engine) :
	_engine(engine)
{
	this->backgroundFader = new BackgroundFader(engine);
}

void MainScene::start(void)
{
	// The the mesh from the system
	IMesh* mesh = _engine->smgr->getMesh("../../Media/robot.obj");

	// Add a new Irrlicht Node with the loaded mesh as mesh
	IMeshSceneNode* node = _engine->smgr->addMeshSceneNode(mesh);

	// Make sure the node is loaded and set what kind of matarial it is
	if (node) {
		node->setMaterialFlag(EMF_LIGHTING, false);
	}

	// Add the camera node to the scene
	_engine->smgr->addCameraSceneNode(0, vector3df(0, 30, -40), vector3df(0, 5, 0));
}

void MainScene::update(void)
{
	// Calculate the new colors for the background fader
	backgroundFader->fade();

	// Set the background fader color to the engine background color
	_engine->backgroundColor = backgroundFader->getColor();
}

MainScene::~MainScene(void)
{
	delete backgroundFader;
}
