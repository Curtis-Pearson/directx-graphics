#pragma once
#include "DirectXFramework.h"

class DirectXApp : public DirectXFramework
{
public:
	void CreateSceneGraph();
	void UpdateSceneGraph();

private:
	int		_rotationAngle = { 0 };
};

