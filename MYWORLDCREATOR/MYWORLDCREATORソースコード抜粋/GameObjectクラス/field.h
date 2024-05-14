#pragma once

#include "gameObject.h"


class Field : public GameObject
{
private:
	D3DXVECTOR3	m_Position;
	D3DXVECTOR3	m_Rotation;
	D3DXVECTOR3	m_Scale;

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;

	int	m_numVertex = 0;
	int	m_numIndex = 0;
	
	ID3D11ShaderResourceView* m_Texture = NULL;
	ID3D11ShaderResourceView* m_bTexture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

public:
	Field();
	Field(Field& copy);
	~Field();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ShowProperty() override;

};