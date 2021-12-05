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

	m_LightList.push_back(pInstance);

	return S_OK;
}

void CLightMgr::Render_Light(LPD3DXEFFECT & pEffect)
{
	for (auto& iter : m_LightList)
		iter->Render_Light(pEffect);
}

void Engine::CLightMgr::Free(void)
{
	for_each(m_LightList.begin(), m_LightList.end(), CDeleteObj());
	m_LightList.clear();
}

const D3DLIGHT9* Engine::CLightMgr::Get_Light(const _ulong& iIndex /*= 0*/)
{
	auto	iter = m_LightList.begin();

	for (_ulong i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Light();
}

