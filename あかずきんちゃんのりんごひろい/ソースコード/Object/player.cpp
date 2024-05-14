#ifdef _DEBUG
#include<iostream>
#endif
#include"player.h"

void PC::updateData()
{
	move();
}

void PC::damage() {
	damagef = true;
	unInitData();

	delete this;
}

bool PC::getDamage()
{
	return damagef;
}

void PC::move() {
	static bool keyFlag[2] = { false };
	
	if (jumpf) {
		if (GetKeyboardPress(DIK_A)) {
			keyFlag[0] = true;
			keyFlag[1] = false;

		}
		else if (GetKeyboardPress(DIK_D)) {
			keyFlag[1] = true;
			keyFlag[0] = false;

		}
		else {
			keyFlag[0] = false;
			keyFlag[1] = false;
		}
		
	}

	if (keyFlag[0]) {
		texset.pos.x -= 7.0f;

		if (texset.pos.x - texset.wh.x / 2 < SCREEN_LEFT) {
			texset.pos.x = SCREEN_LEFT + texset.wh.x / 2;
		}
	}
	if (keyFlag[1]) {
		texset.pos.x += 7.0f;

		if (texset.pos.x + texset.wh.x / 2 > SCREEN_RIGHT) {
			texset.pos.x = SCREEN_RIGHT - texset.wh.x / 2;
		}
	}


	if (!keyFlag[0] && !keyFlag[1]) {
		walkCnt++;
		if (walkCnt % 20 == 0) {
			if (jumpf) {
				//PlaySE(walk);
			}
		}
	}
	else {
		walkCnt = 0;
	}

	if (keyFlag[0]) {
		texset.u = { 0.0f, 1.0f, 0.0f };
	}
	else if(keyFlag[1]) {
		texset.u = { 1.0f, 0.0f, 0.0f };
	}

	if (ground != nullptr) {
		if (texset.pos.y + texset.wh.y / 2 >= *ground->getGroundLine()) {
			//groundはcolliderの生成がかみ合わないのでcheckCollisionできない
			jumpf = true;
			landTimer++;
		}
	}

	if (GetKeyboardPress(DIK_SPACE)) {
		if (jumpf && lastButtonPless == false) {
			if (landTimer > 7) {
				jump();
			}
		}

		lastButtonPless = true;
	}
	else {
		lastButtonPless = false;
	}

	if (jumpf) {
		return;
	}

	texset.pos.y = 0.5 * gravity * time * time - v0 * time;

	time++;
}

void PC::jump() {
	if (jumpf) {
		jumpf = false;
		time = 0;
		landTimer = 0;
		//PlaySE(jumpse);
	}
}
