#pragma once

#include"TreeSpatialPartition.h"
#include "TSPObject.h"
#include "obbCollision.h"

#define SP_COLTREEOBJECT sp<ColTreeObject<OBBCollider>>

class TSPManager
{
	private:
		TreeSpatialPartition<OBBCollider>* m_pTSPInstance = nullptr;
		std::vector<SP_COLTREEOBJECT>* m_SpCTOVect;	

		int GetAllColList(std::vector<OBBCollider*>& OutVect)
		{
			int CollisioNum = m_pTSPInstance->GetAllCollisionList(OutVect);
			return CollisioNum;
		}

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		TSPManager()
		{
			m_pTSPInstance = new TreeSpatialPartition<OBBCollider>();
			m_SpCTOVect = new std::vector<SP_COLTREEOBJECT>();

			//���������Ԃ̐ݒ�
			{
				D3DXVECTOR3 SpatialMinPoint = { 500.0f, 500.0f, 500.0f };
				D3DXVECTOR3 SpatialMaxPoint = { -500.0f, -500.0f, -500.0f };
				if (m_pTSPInstance->Init(6, SpatialMinPoint, SpatialMaxPoint))
				{
					//�W���؏Փ˃��X�g�̐����Ɏ��s
					assert(1);
					return;
				}
			}
		}

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~TSPManager()
		{
			delete m_pTSPInstance;
			delete m_SpCTOVect;
		}

		/// <summary>
		/// m_SpCTOVect��OBBCollision��ݒ�
		/// </summary>
		/// <param name="obb"></param>
		/// <returns></returns>
		bool SetColTreeObject(OBBCollider* obb)
		{
			ColTreeObject<OBBCollider>* cto = new ColTreeObject<OBBCollider>;
			cto->m_pObject = obb;
			sp<ColTreeObject<OBBCollider>> p;
			p.SetPtr(cto);

			if (m_SpCTOVect)
			{
				m_SpCTOVect->push_back(p);
				return true;
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// �W���؋�ԕ�����OBB�̏Փ˔���i�I�u�W�F�N�g�̍X�V������ɑ��点��j
		/// </summary>
		void Update()
		{
			std::vector<SP_COLTREEOBJECT>::iterator it;

			//������Ԃ��v�Z�E�o�^
			for (it = m_SpCTOVect->begin(); it < m_SpCTOVect->end(); it++)
			{
				sp<ColTreeObject<OBBCollider>> spObj = *it;
				spObj->Remove();

				D3DXVECTOR3 MinPos = spObj->m_pObject->GetMinPoint();
				D3DXVECTOR3 MaxPos = spObj->m_pObject->GetMaxPoint();
				m_pTSPInstance->Regist(&MinPos, &MaxPos, *it);

			}

			//�Փ˔���̃y�A���擾
			std::vector<OBBCollider*> calcVect;
			int colNum;
			colNum = GetAllColList(calcVect);
			if (colNum > 0){
				colNum /= 2;
			}

			//�擾�����Փ˔���̃y�A���ɏՓ˔��������
			for (int i = 0; i < colNum; i++)
			{
				std::vector<OBBCollider*>::iterator it;
				it = calcVect.begin();
				it = it + i * 2;
				auto ownerObb = *it;
				auto targetObb = *(it + 1);

				ownerObb->GetOwner()->ClearColliionList();

				//�Փ˂��F�߂�ꂽ��
				if (ownerObb->OBBCheckCollision(targetObb))
				{
					//�I�[�i�[�I�u�W�F�N�g��GameObject::CollisiionList�ɓo�^����
					auto ownerObject = ownerObb->GetOwner();
					auto targetObject = targetObb->GetOwner();

					ownerObject->AddCollisionList(targetObject->GetName());
				}
			}

		}
};
