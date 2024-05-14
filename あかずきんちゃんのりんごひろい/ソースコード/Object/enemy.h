#pragma once
#include"texPolygon.h"
#include"randam.h"

class BULLET :public PTEX {
private:
	bool usef = false;
	bool rightf = false;
	float speed = 7.0f;
public:
	void updateData() override;
	void setRight(bool r) { rightf = r; usef = true; }
	Float2 getPos() { return texset.pos; };
};

class ENEMY :public PTEX {
private:
	BULLET blt;
	bool usef = false;
	bool rightf = false;
	float speed = 1.0f;
	bool attackf = true;
	PTEX* target;

public:
	void setTarget(PTEX* t) { target = t; };
	void updateData() override;
	void drawData() override;
	void setRight(bool r) { rightf = r; usef = true; }
	PTEX* getBullet() { return &blt; }
	Float2 getPos() { return PTEX::getPos(); };

};