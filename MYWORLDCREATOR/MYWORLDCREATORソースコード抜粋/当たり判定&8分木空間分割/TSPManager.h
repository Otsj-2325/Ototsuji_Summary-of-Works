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
		/// コンストラクタ
		/// </summary>
		TSPManager()
		{
			m_pTSPInstance = new TreeSpatialPartition<OBBCollider>();
			m_SpCTOVect = new std::vector<SP_COLTREEOBJECT>();

			//分割する空間の設定
			{
				D3DXVECTOR3 SpatialMinPoint = { 500.0f, 500.0f, 500.0f };
				D3DXVECTOR3 SpatialMaxPoint = { -500.0f, -500.0f, -500.0f };
				if (m_pTSPInstance->Init(6, SpatialMinPoint, SpatialMaxPoint))
				{
					//８分木衝突リストの生成に失敗
					assert(1);
					return;
				}
			}
		}

		/// <summary>
		/// デストラクタ
		/// </summary>
		~TSPManager()
		{
			delete m_pTSPInstance;
			delete m_SpCTOVect;
		}

		/// <summary>
		/// m_SpCTOVectにOBBCollisionを設定
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
		/// ８分木空間分割とOBBの衝突判定（オブジェクトの更新処理後に走らせる）
		/// </summary>
		void Update()
		{
			std::vector<SP_COLTREEOBJECT>::iterator it;

			//所属空間を計算・登録
			for (it = m_SpCTOVect->begin(); it < m_SpCTOVect->end(); it++)
			{
				sp<ColTreeObject<OBBCollider>> spObj = *it;
				spObj->Remove();

				D3DXVECTOR3 MinPos = spObj->m_pObject->GetMinPoint();
				D3DXVECTOR3 MaxPos = spObj->m_pObject->GetMaxPoint();
				m_pTSPInstance->Regist(&MinPos, &MaxPos, *it);

			}

			//衝突判定のペアを取得
			std::vector<OBBCollider*> calcVect;
			int colNum;
			colNum = GetAllColList(calcVect);
			if (colNum > 0){
				colNum /= 2;
			}

			//取得した衝突判定のペア毎に衝突判定をする
			for (int i = 0; i < colNum; i++)
			{
				std::vector<OBBCollider*>::iterator it;
				it = calcVect.begin();
				it = it + i * 2;
				auto ownerObb = *it;
				auto targetObb = *(it + 1);

				ownerObb->GetOwner()->ClearColliionList();

				//衝突が認められたら
				if (ownerObb->OBBCheckCollision(targetObb))
				{
					//オーナーオブジェクトのGameObject::CollisiionListに登録する
					auto ownerObject = ownerObb->GetOwner();
					auto targetObject = targetObb->GetOwner();

					ownerObject->AddCollisionList(targetObject->GetName());
				}
			}

		}
};
