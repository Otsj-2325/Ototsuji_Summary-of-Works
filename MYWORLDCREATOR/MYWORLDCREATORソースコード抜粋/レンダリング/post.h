#pragma once


class Post
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	//�����ɃV�F�[�_�[�֘A�̕ϐ���ǉ�
	ID3D11VertexShader* m_VertexShader[2]; //���_�V�F�[�_�[�I�u�W�F�N�g
	ID3D11PixelShader* m_PixelShader[2]; //�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ID3D11InputLayout* m_VertexLayout; //���_���C�A�E�g�I�u�W�F�N�g

public:
	void Init();
	void Uninit();
	void Update();
	/// <summary>
	/// DeferrdLighting, ForwardRendering
	/// </summary>
	/// <param name="mode">0:DeferrdLighting, 1: ForwardRendering</param>
	void Draw(int mode);
};