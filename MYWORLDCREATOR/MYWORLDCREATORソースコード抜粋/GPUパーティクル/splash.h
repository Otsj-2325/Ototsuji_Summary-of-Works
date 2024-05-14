#pragma once

#include "gameObject.h"

struct VERTEX_3D_PARTICLE
{
	D3DXVECTOR3 Position;
	D3DXVECTOR2 TexCoord;
};

struct ParticleCompute
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 vel;
	float life;
	D3DXCOLOR color;
};

class Splash : public GameObject
{
private:
	ID3D11InputLayout* m_InputLayout{};
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11ComputeShader* m_ComputeShader{};
	ParticleCompute* m_Particle{};

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_ParticleBuffer{};
	ID3D11Buffer* m_ResultBuffer{};
	ID3D11Buffer* m_PositionBuffer{};
	ID3D11Buffer* m_ColorBuffer{};
	ID3D11Buffer* m_ResetIDBuffer{};

	static ID3D11ShaderResourceView* m_ParticleSRV;
	static ID3D11ShaderResourceView* m_PositionSRV;
	static ID3D11ShaderResourceView* m_ColorSRV;

	static ID3D11UnorderedAccessView* m_ResultUAV;


public:
	Splash();
	Splash(Splash& copy);
	~Splash();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	ID3D11Buffer* GetPositionBuffer() { return m_PositionBuffer; }

};