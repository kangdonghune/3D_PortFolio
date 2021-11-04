#include "Scene.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene(void)
{

}



list<CGameObject*>* CScene::Get_List(Layer type, const _tchar * pObjTag)
{
	return m_mapLayer[type]->Get_List(pObjTag);
}

void CScene::Clear_List(Layer type, const _tchar * pObjTag)
{
	m_mapLayer[type]->Clear_List(pObjTag);
}

HRESULT CScene::Add_GameObject(Layer type, const _tchar * pObjTag, CGameObject * pInstance)
{
	if (pInstance == nullptr)
		return E_FAIL;
	return m_mapLayer[type]->Add_GameObject(pObjTag, pInstance);
}

HRESULT Engine::CScene::Ready_Scene(void)
{
	return S_OK;
}

Engine::_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	_int iResult = 0;

	for (int i = 0;  i <Layer::LAYER_END; i++)
	{
		iResult = m_mapLayer[i]->Update_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}
		
	return iResult;
}

void Engine::CScene::Render_Scene(void)
{

}

void Engine::CScene::Free(void)
{
	for (CLayer* pLayer : m_mapLayer)
	{
		Safe_Release(pLayer);
	}
	Safe_Release(m_pGraphicDev);
}

