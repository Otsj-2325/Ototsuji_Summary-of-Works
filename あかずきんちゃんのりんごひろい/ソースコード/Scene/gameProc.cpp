#include "gameProc.h"

bool GMANAGER::requestSceneChange()
{
	if (goal.getGoalf() || player->getDamage() || gameClearf) {
		return true;
	}
	return false;
}

void GMANAGER::gameInit() {
	ndsystem.initNumDips();
	background.initData((char*)"data/TEXTURE/sky.png", { 0.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	ground.initData((char*)"data/TEXTURE/ground.png", { 0.0f, SCREEN_HEIGHT / 3 }, { SCREEN_WIDTH, SCREEN_HEIGHT / 3 }, { 0.0f, 1.0f,0.0f }, { 0.0f, 1.0f, 0.0f });
	groundLine = ground.getGroundLine();
	
	for (int i = 0; i < 5; i++) {
		trees[i] = new PTEX();
		trees[i]->initData((char*)"data/TEXTURE/tree_tall.png", { -1 * (SCREEN_WIDTH / 2) + SCREEN_WIDTH / 5 * i + 100, -150 }, { SCREEN_WIDTH / 1.5f, SCREEN_HEIGHT }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	}

	player = new PC();
	player->initData((char*)"data/TEXTURE/akazukin.png", { 0.0f, 0.0f }, { 64.0f, 64.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	player->setGround(&ground);
	useFlags[0] = true;
	goal.initData((char*)"data/TEXTURE/house2.png",{SCREEN_RIGHT-64.0f, *groundLine - 50.0f}, {128.0f, 150.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f});

	for (int i = 0; i < NUN_ITEMS; i++) {
		items[i].initData((char*)"data/TEXTURE/apple.png", { -1 * (SCREEN_WIDTH / 2) + SCREEN_WIDTH / 10 * i + 50, -1000.0f }, { 64.0f, 64.0f }, { 0.0f,1.0f,0.0f }, { 0.0f,1.0f,0.0f });

	}

	UI1.initData((char*)"data/TEXTURE/score2.png", { 115.0f, SCREEN_UP + 50.0f }, { 256.0f, 70.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

	goal.getTarget(player);
}

void GMANAGER::gameUpdate() {

	for (int i = 0; i < NUN_ITEMS; i++) {
		items[i].updateData();

	}
	if (flameCnt > 300) {
		if (newEmyf) {
			int r = getLimitRand(100);
			if (r > 10) {
				emy = new ENEMY();
				emy->setTarget(player);
				useFlags[1] = true;

				if (getLimitRand(100) > 50) {
					emy->initData((char*)"data/TEXTURE/okami.png", { SCREEN_RIGHT, *groundLine - 55.0f }, { 128.0f, 128.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
					emy->setRight(false);
				}
				else {
					emy->initData((char*)"data/TEXTURE/okami.png", { SCREEN_LEFT, *groundLine - 55.0f }, { 128.0f, 128.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
					emy->setRight(true);
				}

				newEmyf = false;
			}
		}
		else {

			emy->updateData();

			if (emy->getPos().x <= SCREEN_LEFT - 300 || emy->getPos().x >= SCREEN_RIGHT + 300) {
				emy->unInitData();
				delete emy;
				useFlags[1] = false;
				newEmyf = true;
			}

			/*�G�l�~�[�Ƃ̐ڐG����*/
			if (player->checkCollision(*emy)) {
				//PlaySE(damage);
				player->damage();
				useFlags[0] = false;
				gameClearf=false;
			}
			/*�G�l�~�[�̒e�Ƃ̐ڐG����*/
			if (player->checkCollision(*emy->getBullet())) {
				//PlaySE(damage);
				player->damage();
				useFlags[0] = false;
				gameClearf=false;
			}
		}
	}


	//player����E�X�V
	/*�A�C�e���̓����*/
	for (int i = 0; i < NUN_ITEMS; i++) {
		if (player->checkCollision(items[i])) {
			items[i].usef = false;
			//score�v�Z
			score += SCORE_ITEM;
			//PlaySE(get);
		}
	}

	if (score >= 9999) {
		score = 9999;
		gameClearf = true;
	}

	/*�ڐG����𓥂܂���pc�������Ă���Ȃ�X�V�E�`�揈�����s��*/
	if (useFlags[0]) {
		player->updateData();
	}

	flameCnt++;
	if (flameCnt == 300) {
		newEmyf = true;

	}

	goal.updateData();

	if(goal.getGoalf()){
		gameClearf=true;
	}

	std::cout << "SCORE : " << score << std::endl;
#ifdef _DEBUG
	if (useFlags[1]) {
		std::cout << "emy check collision " << player->checkCollision(*emy) << std::endl;
	}
#endif
}

void GMANAGER::gameDraw() {

	background.drawData();

	for (int i = 0; i < 5; i++) {
		trees[i]->drawData();
	}
	
	for (int i = 0; i < NUN_ITEMS; i++) {
		items[i].drawData();
	}

	ground.drawData();

	goal.drawData();

	if (useFlags[0]) {
		player->drawData();
	}
	
	if (useFlags[1]) {
		emy->drawData();
	}

	

	UI1.drawData();
	ndsystem.dispInt(score, 4, { 270.0f, SCREEN_UP + 50.0f });
}

void GMANAGER::gameUninit() {
	if (useFlags[1]) {
		emy->unInitData();
	}
	for (int i = 0; i < NUN_ITEMS; i++) {
		items[i].unInitData();
	}

	for (int i = 0; i < 5; i++) {
		trees[i]->unInitData();
	}

	ground.unInitData();

}

