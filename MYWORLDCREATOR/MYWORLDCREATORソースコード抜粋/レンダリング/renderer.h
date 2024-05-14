#pragma once

struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};

struct MATERIAL
{
	D3DXCOLOR	Ambient;//����
	D3DXCOLOR	Diffuse;//�g�U��
	D3DXCOLOR	Specular;//���ʔ��˗�
	D3DXCOLOR	Emission;//���Ȕ����F
	float		Shininess;//���ˌ��̋��x
	BOOL		TextureEnable;
	float		Dummy[2];
};

struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;

	D3DXMATRIX View;
	D3DXMATRIX Projection;
};

struct PARAM
{
	D3DXVECTOR4 Param;
};

class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;
	static D3D11_DEPTH_STENCIL_DESC* m_DepthStencilDesc;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*            m_ParamBuffer;

	static ID3D11BlendState*		m_BlendState;
	static ID3D11BlendState*		m_BlendStateATC;

	//GBuffer, �f�t�@�[�h�����_�����O�p
	static ID3D11RenderTargetView* m_DRRenderTargetView[3];
	static ID3D11ShaderResourceView* m_DRShaderResourceView[3];
	
	//�f�t�@�[�h���C�e�B���O�p
	static ID3D11RenderTargetView* m_DLRenderTargetView;
	static ID3D11ShaderResourceView* m_DLShaderResourceView;

	//�t���[���o�b�t�@�p
	static ID3D11RenderTargetView* m_FBRenderTargetView;

	//DepthStencilView�i�f�t�@�[�h�����_�����O�j
	static ID3D11DepthStencilView* m_DFDepthStencilView;
	//DepthStencilView�i�ꖇ�G�p�j
	static ID3D11DepthStencilView* m_DepthStencilView;

	static D3DXMATRIX* m_ViewMatrix;
	static D3DXMATRIX* m_ProjectionMatrix;

public:
	static void Init();
	static void Uninit();

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetParam(PARAM p);

	static ID3D11Device* GetDevice( void ){ return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }
	static IDXGISwapChain* GetSwapChain( void ){ return m_SwapChain; }
	static ID3D11DepthStencilView* GetDepthStencilView( void ){ return m_DepthStencilView; }

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);
		
	static void BeginDR();
	static ID3D11ShaderResourceView** GetDRTexture();

	static void BeginDL();
	static void BeginFR();

	static ID3D11ShaderResourceView** GetDLTexture();

	static void Begin();
	static void End();

	static void SetViewportDefault();
	static void SetViewportDepth();

	static D3DXMATRIX* GetViewMatrix(){ return m_ViewMatrix; }
	static D3DXMATRIX* GetProjectionMatrix(){ return m_ProjectionMatrix; }

	static HRESULT CreateComputeShader(ID3D11ComputeShader** ComputeShader, const char* FileName);

	static void CreateStructuredBuffer(UINT elementSize, UINT count, ID3D11Buffer** pBuffer);
	static void CreateUnorderedAccessBuffer(UINT elementSize, UINT count, ID3D11Buffer** pBuffer);

	static void CreateShaderResourceView(ID3D11Buffer* pBuffer, UINT elementSize, UINT count, ID3D11ShaderResourceView** pSrv);
	static void CreateUnorderedAccessView(ID3D11Buffer* pBuffer, UINT count, ID3D11UnorderedAccessView** pUav);

	static void CreateAndCopyBuffer(UINT elementSize, UINT count, ID3D11Buffer* sourceBuffer, ID3D11Buffer** destBuffer);

};
