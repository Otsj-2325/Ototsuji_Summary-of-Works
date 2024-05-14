#pragma once

#include "main.h"
#include"TSPObject.h"

#include <vector>
#include <list>
#include <map>


#define TSP_MAX_LEVEL 7 //���

template <class T>
class TreeSpatialPartition
{
	protected:
		int m_Dim;
		CCell<T>** m_pCellAry;
		unsigned int m_Pow[TSP_MAX_LEVEL + 1];
		D3DXVECTOR3 m_AreaW;
		D3DXVECTOR3 m_AreaMin;
		D3DXVECTOR3 m_AreaMax;
		D3DXVECTOR3 m_Unit;
		DWORD m_CellNum;
		unsigned int m_MinLevel;

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		TreeSpatialPartition()
		{
			m_AreaW = {1.0f, 1.0f, 1.0f};
			m_AreaMin = {0.0f, 0.0f, 0.0f};
			m_AreaMax = {1.0f, 1.0f, 1.0f};
			m_Unit = {1.0f, 1.0f, 1.0f};
			m_CellNum = 0;
			m_MinLevel = 0;
			
		};
		
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~TreeSpatialPartition()
		{
			DWORD i;
			
			//�؍\���̒��̗v�f�̂��镔����delete������
			for (i = 0; i < m_CellNum; i++){
				if (m_pCellAry[i] != nullptr)
				{
					delete m_pCellAry[i];
				}
			}
			delete[] m_pCellAry;

		};

		/// <summary>
		/// ���`�W���ؔz��̍\�z
		/// </summary>
		/// <param name="Level"></param>
		/// <param name="min"></param>
		/// <param name="max"></param>
		/// <returns></returns>
		bool Init(unsigned int Level, D3DXVECTOR3 &min, D3DXVECTOR3 &max)
		{
			//��ԃ��x���̏���𒴂��邽��Ԃ͍쐬�ł��Ȃ�
			if (Level >= TSP_MAX_LEVEL)
			{
				return false;

			}

			//�e��ԃ��x���ɂ�����Ԃ����邩
			m_Pow[0] = 1;
			for (int i = 1; i < TSP_MAX_LEVEL + 1; i++)
			{
				m_Pow[i] = m_Pow[i - 1] * 8;
			}

			//Level�̔z����쐬
			m_CellNum = (m_Pow[Level + 1] - 1) / 7;
			m_pCellAry = new CCell<T>*[m_CellNum];
			ZeroMemory(m_pCellAry, sizeof(CCell<T>*) * m_CellNum);

			//��ԓ�����̃T�C�Y�����
			m_AreaMin = min;
			m_AreaMax = max;
			m_AreaW = max - min;
			m_Unit = m_AreaW / ((float)(1 << Level));
			m_MinLevel = Level;

			return true;

		};
		
		/// <summary>
		/// �I�u�W�F�N�g��o�^����
		/// </summary>
		/// <param name="min"></param>
		/// <param name="max"></param>
		/// <param name="spOFT"></param>
		/// <returns></returns>
		bool Regist(D3DXVECTOR3 *min, D3DXVECTOR3 *max, sp<ColTreeObject<T>> &spOFT)
		{
			//���W���烂�[�g���ԍ����v�Z
			DWORD elem = GetMortonNumber(min, max);
			//���݂����Ԃł���΁A
			if (elem < m_CellNum)
			{
				//�܂���������Ă��Ȃ��Ȃ琶�����āA
				if (!m_pCellAry[elem])
				{
					CreateNewCell(elem);

				}

				//�I�u�W�F�N�g����Ԃɓo�^
				return m_pCellAry[elem]->Push(spOFT);
			}

			//�I�u�W�F�N�g�𐳏�ɓo�^�ł����Ɏ��s
			return false;
		};

		/// <summary>
		/// �Փ˔��胊�X�g�̍쐬
		/// </summary>
		/// <param name="colVect"></param>
		/// <returns></returns>
		DWORD GetAllCollisionList(std::vector<T*> &colVect)
		{
			colVect.clear();

			if (m_pCellAry[0] == NULL)
			{
				return 0;
			}

			std::list<T*> colStac;
			GetCollisionList(0, colVect, colStac);

			return (DWORD)colVect.size();

		};

	protected:
		/// <summary>
		/// ��ԓ��ŏՓ˃��X�g���쐬����
		/// </summary>
		/// <param name="elem"></param>
		/// <param name="colVect"></param>
		/// <param name="colStac"></param>
		/// <returns></returns>
		bool GetCollisionList(DWORD elem, std::vector<T*> &colVect, std::list<T*> &colStac)
		{
			std::list<T*>::template iterator it;
			sp<ColTreeObject<T>> spObj1 = m_pCellAry[elem]->GetFirstObj();

			while (spObj1.GetPtr() != NULL)
			{
				//�����ԓ��̃I�u�W�F�N�g�Ƃ̏Փ˃��X�g���쐬
				sp<ColTreeObject<T>> spObj2 = spObj1->m_pNextObject;
				while (spObj2 != NULL)
				{
					colVect.push_back(spObj1->m_pObject);
					colVect.push_back(spObj2->m_pObject);
					spObj2 = spObj2->m_pNextObject;
				}

				//�Փ˃X�^�b�N�Ƃ̏Փ˃��X�g�쐬
				for (it = colStac.begin(); it != colStac.end(); it++)
				{
					colVect.push_back(spObj1->m_pObject);
					colVect.push_back(*it);
				}
				spObj1 = spObj1->m_pNextObject;
			}

			//�q��ԂɈړ����ď������J��Ԃ�
			bool childFlag = false;
			DWORD objNum = 0;
			for (DWORD i = 0; i < 8; i++)
			{
				DWORD nextElem = elem * 8 + 1 + i;
				if(nextElem < m_CellNum && m_pCellAry[elem * 8 + 1 + i])
				{
					if (!childFlag)
					{
						spObj1 = m_pCellAry[elem]->GetFirstObj();
						while (spObj1.GetPtr())
						{
							colStac.push_back(spObj1->m_pObject);
							objNum++;
							spObj1 = spObj1->m_pNextObject;
						}
					}

					childFlag = true;
					GetCollisionList(elem * 8 + 1 + i, colVect, colStac);
				}
			}

			if (childFlag)
			{
				for (DWORD i = 0; i < objNum; i++)
				{
					colStac.pop_back();
				}
			}

			return true;
		};

		/// <summary>
		/// ��Ԃ𐶐�
		/// </summary>
		/// <returns></returns>
		bool CreateNewCell(DWORD elem)
		{
			while (!m_pCellAry[elem])
			{
				m_pCellAry[elem] = new CCell<T>;
				elem = (elem - 1) >> 3;
				if(elem >= m_CellNum) break;
			}

			return true;
		};

		/// <summary>
		/// ���W�����Ԕԍ����Z�o
		/// </summary>
		/// <param name="min"></param>
		/// <param name="max"></param>
		/// <returns></returns>
		DWORD GetMortonNumber(D3DXVECTOR3 *min, D3DXVECTOR3 *max)
		{
			DWORD LT = GetPointElem(*min);
			DWORD RB = GetPointElem(*max);

			DWORD Def = RB ^ LT;
			unsigned int HiLevel = 1;
			unsigned int i;
			for (i = 0; i < m_MinLevel; i++)
			{
				DWORD Check = (Def >> (i * 3)) & 0x7;
				if (Check != 0)
					HiLevel = i + 1;
			}
			DWORD SpaceNum = RB >> (HiLevel * 3);
			DWORD AddNum = (m_Pow[m_MinLevel - HiLevel] - 1) / 7;
			SpaceNum += AddNum;

			if (SpaceNum > m_CellNum)
				return 0xffffffff;

			return SpaceNum;

		};

		/// <summary>
		/// �r�b�g�����֐�
		/// </summary>
		/// <param name="n"></param>
		/// <returns></returns>
		DWORD BitSeparateFor3D(BYTE n)
		{
			DWORD s = n;
			s = (s | s << 8) & 0x0000f00f;
			s = (s | s << 4) & 0x000c30c3;
			s = (s | s << 2) & 0x00249249;
			return s;
		};

		/// <summary>
		/// ���[�g���ԍ����Z�o
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="z"></param>
		/// <returns></returns>
		DWORD Get3DMortonNumber(BYTE x, BYTE y, BYTE z)
		{
			return BitSeparateFor3D(x) | BitSeparateFor3D(y) << 1 | BitSeparateFor3D(z) << 2;

		};

		/// <summary>
		/// ���W������`�W���ؗv�f�ԍ��֕ϊ�����
		/// </summary>
		/// <param name="p"></param>
		/// <returns></returns>
		DWORD GetPointElem(D3DXVECTOR3 &p)
		{
			return Get3DMortonNumber(
				(BYTE)((p.x - m_AreaMin.x) / m_Unit.x),
				(BYTE)((p.y - m_AreaMin.y) / m_Unit.y),
				(BYTE)((p.z - m_AreaMin.z) / m_Unit.z)
			);
		};
	
};

