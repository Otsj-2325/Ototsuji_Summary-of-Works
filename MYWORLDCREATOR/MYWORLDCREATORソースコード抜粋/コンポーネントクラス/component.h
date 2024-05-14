#pragma once

class GameObject;

class Component
{
protected:
	GameObject* ownerObject;

public:
	virtual void Init(GameObject* Owner) {} // âºëzä÷êî
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
	GameObject* GetOwner() {return ownerObject;}

};