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
		// すでに逸脱している時は処理終了
		if (!m_pCCell)
			return false;

		// 自分を登録している空間に自身を通知
		if (!m_pCCell->OnRemove(this))
			return false;

		// 逸脱処理
		// 前後のオブジェクトを結びつける
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
	/// 所属空間を登録
	/// </summary>
	void RegistCell(CCell<T>* pCell)
	{
		m_pCCell = pCell;
	}
	
	/// <summary>
	/// 同じ空間に所属する次のオブジェクトを取得
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
	/// オブジェクト間のリンクを解除して、自身を削除する
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
	/// 新たなオブジェクトを空間に追加
	/// </summary>
	bool Push(sp<ColTreeObject<T>>& obj)
	{
		//無効な要素を空間に追加しない
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
			//新たなオブジェクトと今あるオブジェクトの間で双方向リンクを作成する
			obj->m_pNextObject = m_LatestObject;
			m_LatestObject->m_pPreObject = obj;
			m_LatestObject = obj;
		}
		obj->RegistCell(this);
		return true;

	}

	/// <summary>
	/// 登録オブジェクトのリンクの先頭を取得
	/// </summary>
	sp<ColTreeObject<T>>& GetFirstObj()
	{
		return m_LatestObject;
	}

	/// <summary>
	/// 空間から削除する際の処理
	/// </summary>
	bool OnRemove(ColTreeObject<T>* obj)
	{
		//自身が先頭オブジェクトで且つ、NULLじゃなければ自身の次のオブジェクトを先頭として登録しなおす
		if (m_LatestObject.GetPtr() == obj)
		{
			if (m_LatestObject.GetPtr() != NULL)//先頭がないときにNULLを渡された際にエラーしない為のチェック
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