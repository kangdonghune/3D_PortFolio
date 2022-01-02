#include "LightMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightMgr)


Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{
	Free();
}

HRESULT Engine::CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLight*	pInstance = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	NULL_CHECK_RETURN(pInstance, E_FAIL);

	m_LightList[iIndex] = pInstance;

	return S_OK;
}

void CLightMgr::Render_Light(LPD3DXEFFECT & pEffect)
{
	for (int i = 0; i < LIGHT_END; i++)
	{
		if(m_LightList[i] != nullptr)
			m_LightList[i]->Render_Light(pEffect);
	}
}

void CLightMgr::Delete_Light(CLight * pLight)
{
	for (int i = 0; i < LIGHT_END; i++)
	{
		if (m_LightList[i] == pLight)
		{
			Safe_Release(pLight);
			m_LightList[i] = nullptr;
		}
	}
}

void CLightMgr::Delete_Light(const _uint & iIndex)
{
	if(m_LightList[iIndex] != nullptr)
		Safe_Release(m_LightList[iIndex]);
}

void Engine::CLightMgr::Free(void)
{
	for (int i = 0; i < LIGHT_END; i++)
	{
		if (m_LightList[i] != nullptr)
			Safe_Release(m_LightList[i]);
	}
}

const D3DLIGHT9* Engine::CLightMgr::Get_Light(const _ulong& iIndex /*= 0*/)
{
	return m_LightList[iIndex]->Get_Light();
}

CLight * CLightMgr::Get_LightClass(const _ulong & iIndex)
{
	return m_LightList[iIndex];
}

