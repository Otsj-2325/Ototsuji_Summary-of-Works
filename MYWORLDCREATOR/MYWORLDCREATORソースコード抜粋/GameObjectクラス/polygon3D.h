#pragma once

#include "gameObject.h"

class Polygon3D : public GameObject{
private:
	ID3D11Buffer* m_Vertexbuffer{};
	ID3D11Buffer* m_Indexbuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VSBuffer{};
	ID3D11PixelShader* m_PSBuffer{};
	ID3D11InputLayout* m_InputLayout{};

public:
	Polygon3D();
	Polygon3D(Polygon3D& copy);
	~Polygon3D();
	void Init();
	void Uninit();
	void Update();
	void Draw();
};