#include "RenderTarget.h"
USING(Engine)
CRenderTarget::CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
	, m_pTargetTexture(nullptr)
	, m_pTargetSurface(nullptr)
	, m_pOldTargetSurface(nullptr)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
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

	m_ClearColor = Color;

	//�ؽ��� �İ�ü�� ���� ����Ÿ�� �뵵�� ����,�����̽��� ����
	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);


	return S_OK;
}

void CRenderTarget::Setup_OnGraphicDev(const _uint & iIndex)
{
	m_pGraphicDev->GetRenderTarget(iIndex, &m_pOldTargetSurface);

	m_pGraphicDev->SetRenderTarget(iIndex, m_pTargetSurface);



}

void CRenderTarget::Release_OnGraphicDev(const _uint & iIndex)
{
	m_pGraphicDev->SetRenderTarget(iIndex, m_pOldTargetSurface);
	Safe_Release(m_pOldTargetSurface);
}

void CRenderTarget::Clear_RenderTarget()
{
	m_pGraphicDev->GetRenderTarget(0, &m_pOldTargetSurface);

	m_pGraphicDev->SetRenderTarget(0, m_pTargetSurface);

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0);

	m_pGraphicDev->SetRenderTarget(0, m_pOldTargetSurface);

	Safe_Release(m_pOldTargetSurface);
}

HRESULT CRenderTarget::Ready_DebugBuffer(const _float & fx, const _float & fy, const _float & fW, const _float & fH)
{
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,	// �׸����� �ϴ� ������ ������ �뷮(�ﰢ���̴� ���� ���� ũ�⸸ŭ�� �뷮)
		0,					// �������۸� �����(������ �迭 ������ ���), D3DUSAGE_DYNAMIC �ɼ��� �ָ� ��������(��ƼŬ �뵵)
		FVF_SCREEN,			// ������ �Ӽ� ����
		D3DPOOL_MANAGED,	// ���� ���� ����, MANAGED ���� ���� ��� ��, DEFAULT
		&m_pVB,
		NULL)))				// ������ �ڵ������� �⺻�� NULL				
		return E_FAIL;


	// ���ؽ� ���� ����
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,	// �׸����� �ϴ� ������ �ε��� ������ ����� �޸��� �뷮
		0,					// �������۸� �����(������ �迭 ������ ���), D3DUSAGE_DYNAMIC �ɼ��� �ָ� ��������(��ƼŬ �뵵)
		D3DFMT_INDEX16,			// �ε����� ����	
		D3DPOOL_MANAGED,	// ���� ���� ����, MANAGED ���� ���� ��� ��, DEFAULT
		&m_pIB,
		NULL)))				// ������ �ڵ������� �⺻�� NULL				
		return E_FAIL;

	VTXSCREEN*			pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// ������ ��
	pVertex[0].vPos = _vec4(fx - 0.5f, fy - 0.5f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(fx + fW - 0.5f, fy - 0.5f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(fx + fW - 0.5f, fy + fH - 0.5f, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(fx - 0.5f, fy + fH - 0.5f, 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();


	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();


	return S_OK;
}

void CRenderTarget::Render_DebugBuffer()
{
	m_pGraphicDev->SetTexture(0, m_pTargetTexture);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphicDev->SetFVF(FVF_SCREEN);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

}

void CRenderTarget::SetUp_Shader(LPD3DXEFFECT & pEffect, const char * pContantTable)
{
	pEffect->SetTexture(pContantTable, m_pTargetTexture);
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
	Safe_Release(m_pTargetSurface);
	Safe_Release(m_pTargetTexture);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pGraphicDev);

}
