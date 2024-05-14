#include "main.h"
#include "renderer.h"
#include "model.h"
#include "gameObject.h"
#include "ModelRender.h"


void ModelRender::Init(GameObject* owner)
{
	ownerObject = owner;
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\deferredGeometryVS.cso");
	Renderer::CreatePixelShader(&m_PixcelShader, "shader\\deferredGeometryPS.cso");
}

void ModelRender::Uninit()
{
	if(m_PixcelShader){
		m_PixcelShader->Release();
	}

	if(m_VertexLayout){
		m_VertexLayout->Release();
	}

	if(m_VertexShader){
		m_VertexShader->Release();
	}

	if(m_Model){
		m_Model->Unload();
	}
}

void ModelRender::Update()
{
}

void ModelRender::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixcelShader, NULL, 0);

	D3DXMATRIX world = ownerObject->ObjectWorldMatrix();
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();

}
