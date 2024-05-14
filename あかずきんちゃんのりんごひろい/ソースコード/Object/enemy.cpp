#include"enemy.h"
#include<iostream>

void ENEMY::updateData()
{

	if (!usef) {
		return;
	}

	if (texset.pos.x > SCREEN_LEFT - 300 && texset.pos.x < SCREEN_RIGHT + 300) {
		if (rightf) {
			texset.pos.x += speed;
			if (texset.pos.x > SCREEN_RIGHT + 300) {
				usef = false;
			}

		}
		else {
			texset.pos.x -= speed;
			if (texset.pos.x < SCREEN_LEFT - 300) {
				usef = false;
			}
		}

		if (target->getPos().x > texset.pos.x) {
			texset.u = { 0.0f, 1.0f, 0.0f };
		}
		else {
			texset.u = { 1.0f, 0.0f, 0.0f };
		}

		if (attackf) {
			int r = getLimitRand(100);			

			if (r >1) {
				blt.initData((char*)"data/TEXTURE/ball.png", { texset.pos.x, texset.pos.y + 10.0f }, { 32.0f, 32.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
				if (target->getPos().x > texset.pos.x) {
					blt.setRight(true);
				}
				else {
					blt.setRight(false);
				}
				attackf = false;
			}
		}
		else {
			blt.updateData();

			if (blt.getPos().x<SCREEN_LEFT - 100 || blt.getPos().x > SCREEN_RIGHT + 100) {
				blt.unInitData();
				attackf = true;
			}
		}
	}
}

void ENEMY::drawData()
{
	if (!attackf) {
		blt.drawData();
	}

	PTEX::drawData();
}



void BULLET::updateData()
{
	if (!usef) {
		return;
	}

	if (texset.pos.x > SCREEN_LEFT - 100 && texset.pos.x < SCREEN_RIGHT + 100){
		if (rightf) {
			texset.pos.x += speed;
			if (texset.pos.x > SCREEN_RIGHT + 100) {
				usef = false;
			}

		}
		else {
			texset.pos.x -= speed;
			if (texset.pos.x < SCREEN_LEFT - 100) {
				usef = false;
			}
		}

	}

}