#include "EffActor.h"
#include "EffScene.h"
#include <iostream>

EffActor::EffActor(void)
{
	node = nullptr;
}

void EffActor::start ()
{
}

void EffActor::update (float deltaTime)
{
}

void EffActor::setScene (EffScene* scene)
{
	this->scene = scene;
}

void EffActor::setNode (scene::ISceneNode* node)
{
	this->node = node;
}

EffActor::~EffActor(void)
{
	// If this actor has a node, remove it from the scene
	if (node != nullptr)
	{
		node->remove ();
		node = nullptr;
	}
}
