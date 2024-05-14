#pragma once
#include "component.h"

class Model;

class ModelRender : public Component
{
	private:
		Model* m_Model {};
		ID3D11VertexShader* m_VertexShader {};
		ID3D11PixelShader* m_PixcelShader {};
		ID3D11InputLayout* m_VertexLayout {};

	public:		
		void Init(GameObject* owner) override;
		void Uninit() override;
		void Update() override;
		void Draw() override;

		void SetModel(Model* model){ m_Model = model; }
};