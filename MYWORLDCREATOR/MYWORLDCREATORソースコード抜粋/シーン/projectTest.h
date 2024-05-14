#pragma once
#include "scene.h"

class ProjectTest : public Scene {

private:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ShowIMGUI();

};