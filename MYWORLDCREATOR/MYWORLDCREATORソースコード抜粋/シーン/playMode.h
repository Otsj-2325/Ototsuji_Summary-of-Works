#pragma once
#pragma once
#include "scene.h"

#include "TreeSpatialPartition.h"

class PlayMode : public Scene {

private:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};