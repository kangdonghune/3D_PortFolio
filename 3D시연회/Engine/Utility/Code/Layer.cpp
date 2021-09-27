#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer(void)
{

}

Engine::CLayer::~CLayer(void)
{

}

HRESULT Engine::CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pInstance)
{
	if (nullptr == pInstance)
		return E_FAIL;
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
	{
		list<CGameObject*> list;
		m_mapObject.emplace(pObjTag, list);
		m_mapObject.find(pObjTag)->second.emplace_back(pInstance);
	}
	else
	{
		iter->second.emplace_back(pInstance);
	}
	return S_OK;
}

HRESULT Engine::CLayer::Ready_Layer(void)
{
	return S_OK;
}

int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	iResult = 0;

	for (auto& iter = m_mapObject.begin(); iter != m_mapObject.end(); iter++)
	{
		for (auto& iter2 = iter->second.begin(); iter2 != iter->second.end(); )
		{
			if ((*iter2)->Get_Dead() == true)
			{
				Safe_Release((*iter2));
				iter2 = iter->second.erase(iter2); //iter = pair 의 이터레이터 iter->second 리스트 , iter2 = 리스트의 이터레이터
			}
			else
			{
				iResult = (*iter2)->Update_Object(fTimeDelta);
				if (iter2 != iter->second.end())
					iter2++;
			}
	
		}
	}
	return iResult;
}

CLayer* Engine::CLayer::Create(void)
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for (auto& iter = m_mapObject.begin(); iter != m_mapObject.end(); iter++)
	{
		for (auto& iter2 = iter->second.begin(); iter2 != iter->second.end(); )
		{
			if ((*iter2)->Get_Dead() == true)
			{
				Safe_Release((*iter2));
				iter2 = iter->second.erase(iter2);
			}
		}
	}
	m_mapObject.clear();
}

CComponent* Engine::CLayer::Get_Component(const _tchar* pObjTag, CGameObject* pObj, const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	for (auto& iter2 = iter->second.begin(); iter2 != iter->second.end(); )
	{
		if ((*iter2) == pObj)
		{
			return (*iter2)->Get_Component(pComponentTag, eID);
		}
	}

	return nullptr;
}

list<CGameObject*> CLayer::Get_List(const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	return iter->second;
	
}



