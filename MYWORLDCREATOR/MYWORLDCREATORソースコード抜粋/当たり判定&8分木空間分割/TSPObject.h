#pragma once

#include "SmartPtr.h"

template<class T>
class CCell;

template<class T>
class ColTreeObject
{
	public:
	CCell<T> *m_pCCell;
	T* m_pObject;
	sp<ColTreeObject<T>> m_pPreObject;
	sp<ColTreeObject<T>> m_pNextObject;


	ColTreeObject()
	{
		m_pCCell = NULL;
		m_pObject = NULL;
	}
	
	virtual ~ColTreeObject(){};

	bool Remove()
	{
		// ���łɈ�E���Ă��鎞�͏����I��
		if (!m_pCCell)
			return false;

		// ������o�^���Ă����ԂɎ��g��ʒm
		if (!m_pCCell->OnRemove(this))
			return false;

		// ��E����
		// �O��̃I�u�W�F�N�g�����т���
		if (m_pPreObject.GetPtr() != NULL)
		{
			m_pPreObject->m_pNextObject = m_pNextObject;
			m_pPreObject.SetPtr(NULL);
		}
		if (m_pNextObject.GetPtr() != NULL)
		{
			m_pNextObject->m_pPreObject = m_pPreObject;
			m_pNextObject.SetPtr(NULL);
		}
		m_pCCell = NULL;
		return true;
	}

	/// <summary>
	/// ������Ԃ�o�^
	/// </summary>
	void RegistCell(CCell<T>* pCell)
	{
		m_pCCell = pCell;
	}
	
	/// <summary>
	/// ������Ԃɏ������鎟�̃I�u�W�F�N�g���擾
	/// </summary>
	sp<ColTreeObject<T>>& GetNextObj()
	{
		return m_pNextObject;
	}


};


template<class T>
class CCell
{
	protected:
	sp<ColTreeObject<T>> m_LatestObject;

	public:
	CCell(){}
	~CCell()
	{
		if (m_LatestObject.GetPtr() != NULL)
		{
			ResetLink(m_LatestObject);
		}
	}

	/// <summary>
	/// �I�u�W�F�N�g�Ԃ̃����N���������āA���g���폜����
	/// </summary>
	void ResetLink(sp<ColTreeObject<T>>& obj)
	{
		if (obj->m_pNextObject.GetPtr() != NULL)
		{
			ResetLink(obj->m_pNextObject);
		}

		obj.SetPtr(NULL);
	}

	/// <summary>
	/// �V���ȃI�u�W�F�N�g����Ԃɒǉ�
	/// </summary>
	bool Push(sp<ColTreeObject<T>>& obj)
	{
		//�����ȗv�f����Ԃɒǉ����Ȃ�
		if(obj.GetPtr() == NULL)
		{
			return false;
		}

		if (obj->m_pCCell == this)
		{
			return false;
		}

		if (m_LatestObject.GetPtr() == NULL)
		{
			m_LatestObject = obj;
		}
		else
		{
			//�V���ȃI�u�W�F�N�g�ƍ�����I�u�W�F�N�g�̊Ԃőo���������N���쐬����
			obj->m_pNextObject = m_LatestObject;
			m_LatestObject->m_pPreObject = obj;
			m_LatestObject = obj;
		}
		obj->RegistCell(this);
		return true;

	}

	/// <summary>
	/// �o�^�I�u�W�F�N�g�̃����N�̐擪���擾
	/// </summary>
	sp<ColTreeObject<T>>& GetFirstObj()
	{
		return m_LatestObject;
	}

	/// <summary>
	/// ��Ԃ���폜����ۂ̏���
	/// </summary>
	bool OnRemove(ColTreeObject<T>* obj)
	{
		//���g���擪�I�u�W�F�N�g�Ŋ��ANULL����Ȃ���Ύ��g�̎��̃I�u�W�F�N�g��擪�Ƃ��ēo�^���Ȃ���
		if (m_LatestObject.GetPtr() == obj)
		{
			if (m_LatestObject.GetPtr() != NULL)//�擪���Ȃ��Ƃ���NULL��n���ꂽ�ۂɃG���[���Ȃ��ׂ̃`�F�b�N
			{
				m_LatestObject = m_LatestObject->GetNextObj();
			}
			return true;
		}
		else
		{
			return false;
		}

	}

};