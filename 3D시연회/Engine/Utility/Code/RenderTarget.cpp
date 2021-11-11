#include "RenderTarget.h"
USING(Engine)
CRenderTarget::CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
	,m_pTargetTexture(nullptr)
	,m_pTargetSurface(nullptr)
	,m_pOldTargetSurface(nullptr)
{
	m_pGraphicDev->AddRef();
}

CRenderTarget::~CRenderTarget()
{
}

HRESULT CRenderTarget::Ready_RenderTarget(const _uint & iWidth, const _uint & iHeight, D3DFORMAT format, D3DXCOLOR Color)
{
	if (FAILED(D3DXCreateTexture(m_pGraphicDev, iWidth, iHeight, 1, D3DUSAGE_RENDERTARGET, format, D3DPOOL_DEFAULT, &m_pTargetTexture)))
		return E_FAIL;
	//텍스쳐 컴객체로 부터 랜더타겟 용도의 정보,서페이스를 추출
	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);

	return S_OK;
}

void CRenderTarget::Setup_OnGraphicDev(const _uint & iIndex)
{
	m_pGraphicDev->GetRenderTarget(iIndex, &m_pOldTargetSurface);

	m_pGraphicDev->SetRenderTarget(iIndex, m_pTargetSurface);



}

CRenderTarget * CRenderTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint & iWidth, const _uint & iHeight, D3DFORMAT format, D3DXCOLOR Color)
{
	CRenderTarget* pInstant = new CRenderTarget(pGraphicDev);

	if (FAILED(pInstant->Ready_RenderTarget(iWidth, iHeight, format, Color)))
		Safe_Release(pInstant);
	return pInstant;
}

void CRenderTarget::Free()
{

	Safe_Release(m_pTargetTexture);
	Safe_Release(m_pGraphicDev);

}
