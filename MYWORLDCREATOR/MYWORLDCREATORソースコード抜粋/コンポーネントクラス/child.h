#pragma once

#include "gameObject.h"
#include "model.h"

class Child :public GameObject {
private:
	ID3D11VertexShader* m_VertexShader {};
	ID3D11PixelShader* m_PixelShader {};
	ID3D11InputLayout* m_VertexLayout {};
	Model* m_model {};
	class Player* m_Parent{};
	D3DXVECTOR2 rot = {0.0f, 0.0f};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetParent(Player* parent){ m_Parent = parent; }
};