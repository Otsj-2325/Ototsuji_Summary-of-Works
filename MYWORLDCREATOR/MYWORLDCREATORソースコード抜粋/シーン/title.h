#pragma once

#include "scene.h"

class Title : public Scene
{
	private:


	public:
		void Init() override;
		void Uninit() override;
		void Update() override;
		void Draw() override;

};