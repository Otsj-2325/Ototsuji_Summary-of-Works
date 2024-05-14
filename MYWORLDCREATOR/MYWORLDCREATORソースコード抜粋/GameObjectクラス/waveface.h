#pragma once

#include "gameObject.h"

struct WAVEPOINT
{
	int status;
	D3DXVECTOR3 position;
	float amplitude;
	float time;

};

#define WAVE_PITCH 10
#define WAVE_COUNT 100
#define WAVE_AMPLITUDE 1.0f
#define WAVE_LENGTH 20.0f
#define WAVE_CYCLE 20.0f

class Waveface : public GameObject
{
	private:
		ID3D11VertexShader* m_VertexShader {};
		ID3D11PixelShader* m_PixelShader {};
		ID3D11InputLayout* m_VertexLayout {};
		ID3D11Buffer* m_PositionBuffer{};

		WAVEPOINT m_WavePoints[WAVE_COUNT * WAVE_COUNT];

		VERTEX_3D m_Vertex[WAVE_COUNT][WAVE_COUNT];
		ID3D11Buffer* m_VertexBuffer = NULL;
		ID3D11Buffer* m_IndexBuffer = NULL;

		int	m_numVertex = 0;
		int	m_numIndex = 0;

	public:
		Waveface();
		Waveface(Waveface& copy);
		~Waveface();
		void Init() override;
		void Uninit() override;
		void Update() override;
		void Draw() override;


};