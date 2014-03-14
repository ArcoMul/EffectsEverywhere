#include "EffActor.h"
#include "EffScene.h"
#include <iostream>

EffActor::EffActor(void)
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
}
