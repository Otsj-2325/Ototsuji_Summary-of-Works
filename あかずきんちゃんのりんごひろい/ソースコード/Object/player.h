#pragma once
#include"texPolygon.h"
#include"randam.h"
#include"ground.h"

class PC :public PTEX {
private:
	float v0 = 18.0f;
	int time = 0;
	float gravity = 1.0f;
	bool jumpf = false;
	int landTimer = 0;
	bool lastButtonPless = false;
	bool damagef = false;
	int walkCnt = 0;
	GROUND* ground = nullptr;

	void jump();
	void move();


public:
	void setGround(GROUND* g) { ground = g; }
	void updateData()override;
	void damage();
	bool getDamage();
};