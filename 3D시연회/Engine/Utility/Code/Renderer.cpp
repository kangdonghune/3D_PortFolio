#include "Export_Utility.h"


USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer(void)
{

}

Engine::CRenderer::~CRenderer(void)
{
	Free();
}
// 매 프레임마다 렌더 그룹을 재편성
void Engine::CRenderer::Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	if (RENDER_END <= eID || nullptr == pGameObject)
		return;

	m_RenderGroup[eID].push_back(pGameObject);
	pGameObject->AddRef();
}
// 재편성된 렌더그룹을 출력
void Engine::CRenderer::Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Render_Priority(pGraphicDev);	

	Render_Deferred(pGraphicDev);

	Render_Alpha(pGraphicDev);
	Render_UI(pGraphicDev);

	Render_DebugMRT(L"MRT_Deferred");

	Clear_RenderGroup();
}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_Object();

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::Render_Nonalpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
	{
		if(iter->Get_IsRender())
			iter->Render_Object();
	}
}

_bool	Compare_Z(CGameObject* pDest, CGameObject* pSrc)
{
	return pDest->Get_ViewZ() > pSrc->Get_ViewZ();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_RenderGroup[RENDER_ALPHA].sort(Compare_Z);

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
	{
		if (iter->Get_IsRender())
			iter->Render_Object();
	}

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (auto& iter : m_RenderGroup[RENDER_UI])
	{
		if (iter->Get_IsRender())
			iter->Render_Object();
	}

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

void CRenderer::Render_Deferred(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Begin_MRT(L"MRT_Deferred");
	Render_Nonalpha(pGraphicDev);
	End_MRT(L"MRT_Deferred");
}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();
}

