#pragma once


class Post
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	//ここにシェーダー関連の変数を追加
	ID3D11VertexShader* m_VertexShader[2]; //頂点シェーダーオブジェクト
	ID3D11PixelShader* m_PixelShader[2]; //ピクセルシェーダーオブジェクト
	ID3D11InputLayout* m_VertexLayout; //頂点レイアウトオブジェクト

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