/*==============================================================================

   [PolygonDrawer.h] 
   
	 Author :H.Ototsuji
 	 Date   :2022/10/28
==============================================================================*/

#pragma once

#include<cmath>
#include<string>
#include"main.h"

//�ʒu�֌W��\�����߂̗񋓑�
enum class DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	MATCH,
	FAILED,
};

//�|���S���̃f�[�^�ێ��p�\����
struct POLYGONDATA {
	Float2 polyPos = { 0.0f };//�|���S���̒��S���W{X�l, V�l}
	Float2 polyWh = { 0.0f };//�|���S���̕�����{��, ����}
	Float3 texU = { 0.0f };//�|���S����U�l{�n�_�l, �I�_�l, �����l}
	Float3 texV = { 0.0f };//�|���S����V�l{�n�_�l, �I�_�l, �����l}
};

struct COLLISIONDATA {
	DIRECTION direction;
	std::string* tag;
};

struct TEXTUREDATA {
	ID3D11ShaderResourceView* texData;
	Float3 texU;
	Float3 texV;
};

//���_�̃f�[�^�ێ��p�\����
struct POLYGONVERTEXDATA {
	D3DXVECTOR3 position = { 0.0f, 0.0f, 0.0f };//���_�̍��W{X�l, Y�l, Z�l(0.0f)}
	D3DXCOLOR diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };//���_�J���[{R�l, G�l, B�l, A�l}
	D3DXVECTOR2 texcoord = { 0.0f, 0.0f };//���_UV�l{U�l, V�l}
};

class POLYVERTEX {
protected:
	POLYGONDATA polyData;//�|���S���̃f�[�^
	POLYGONVERTEXDATA verData[4];//�e���_�̃f�[�^(0:����, 1:�E��, 2:����, 3:�E��)
	std::string* tag;//�^�O�f�[�^(������)

private:
	ID3D11Buffer* g_VertexBuffer = 0;//polygon�e�_�̍��W�o�b�t�@
	TEXTUREDATA textures[10];//�ǂݍ��񂾃e�N�X�`���f�[�^�̃|�C���^
	int texNum = 0;
	int nowTexNum = 0;
	int fCount = 0;//������t���[�����J�E���g(draw�ŉ��Z)

	void SetVertex();//�`��p���_�o�b�t�@��verData[4]���ړ�
	void setOrigin();//�|���S���̒��S�����_�Ɉړ�
	void resetPosition();//�|���S���̒��S��"polyData.polyPos"�Ɉړ�

protected:
	virtual void initData(std::string* t, const char* tp, Float2 p, Float2 w, Float3 u, Float3 v);
	virtual void unInitData();
	virtual void updateData();
	virtual void drawData();

public:
	void polyTranslation(Float2 incremental);//�|���S���̕��s�ړ�{x�̑���, y�̑���}
	void polyTransPoint(Float2 Point);//�|���S���̕��s�ړ�{�ړ���̍��W(Float2)}
	void polyTransXPoint(float Point);//�|���S����X�����s�ړ�{�ړ����X���W(float)}
	void polyTransYPoint(float Point);//�|���S����Y�����s�ړ�{�ړ����Y���W(float)}
	void polyScalling(float magnification);//�|���S���̊g��k��{�{��}
	void polyRotation(float degree);//�|���S���̉�]{��]�p�x(�x)}
	
	void texAnimetionU(int updateTime);//�e�N�X�`���A�j���[�V������U�l����{�A�j���[�V�����̍X�V�t���[����}
	void texAnimetionV(int uNum);//{�e�N�X�`���A�j���[�V������V�l����{�e�N�X�`���̏ォ�炢���߂̃A�j���[�V������}

	int SetTexture(char* tpc, Float3 u, Float3 v);

	void ChangeTexture(int n);

	std::string getTag();

	void resetVerData();//���_���𐶐����̃f�[�^�ōX�V

	bool polyCollisionCheck(POLYVERTEX& target);//�|���S�����m�̏Փ˔���{�^�[�Q�b�g�|���S��}
	DIRECTION polyDirectionCheck(POLYVERTEX& target);//�|���S�����m�̈ʒu�֌W{�^�[�Q�b�g�|���S��}
	COLLISIONDATA polyCollisionDataCheck(POLYVERTEX& target);


	const POLYGONDATA getPolyData() { return polyData; };
};

class SEAL :public POLYVERTEX {
public:
	void updateData()override {};
};