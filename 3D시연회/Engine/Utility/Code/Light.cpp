#include "Light.h"
USING(Engine)

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_iIndex(0)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
{
	ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
	m_pGraphicDev->AddRef();
}

Engine::CLight::~CLight(void)
{

}

HRESULT Engine::CLight::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	memcpy(&m_tLightInfo, pLightInfo, sizeof(D3DLIGHT9));

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,	// 그리고자 하는 도형의 정점의 용량(삼각형이니 정점 세개 크기만큼의 용량)
		0,					// 정적버퍼를 사용함(정해진 배열 공간을 사용), D3DUSAGE_DYNAMIC 옵션을 주면 동적버퍼(파티클 용도)
		FVF_SCREEN,			// 정점의 속성 정보
		D3DPOOL_MANAGED,	// 정적 버퍼 사용시, MANAGED 동적 버퍼 사용 시, DEFAULT
		&m_pVB,
		NULL)))				// 공유할 핸들이지만 기본값 NULL				
		return E_FAIL;


	// 버텍스 버퍼 생성
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,	// 그리고자 하는 도형의 인덱스 개수에 비례한 메모리의 용량
		0,					// 정적버퍼를 사용함(정해진 배열 공간을 사용), D3DUSAGE_DYNAMIC 옵션을 주면 동적버퍼(파티클 용도)
		D3DFMT_INDEX16,			// 인덱스의 정보	
		D3DPOOL_MANAGED,	// 정적 버퍼 사용시, MANAGED 동적 버퍼 사용 시, DEFAULT
		&m_pIB,
		NULL)))				// 공유할 핸들이지만 기본값 NULL				
		return E_FAIL;


	D3DVIEWPORT9	ViewPort;
	ZeroMemory(&ViewPort, sizeof(&ViewPort));

	m_pGraphicDev->GetViewport(&ViewPort);

	VTXSCREEN*			pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 오른쪽 위
	pVertex[0].vPos = _vec4(0.f, 0.f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(ViewPort.Width, 0.f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(ViewPort.Width, ViewPort.Height, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(0.f, ViewPort.Height, 0.f, 1.f);
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
	//m_pGraphicDev->LightEnable(iIndex, TRUE);

	return S_OK;
}

HRESULT CLight::Render_Light(LPD3DXEFFECT& pEffect)
{
	pEffect->SetVector("g_vLightDir", &_vec4(m_tLightInfo.Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&m_tLightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&m_tLightInfo.Ambient);

	_matrix	matView;
	_matrix	matProj;


	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	pEffect->SetVector("g_vCamPos", (_vec4*)&matView._41);

	pEffect->CommitChanges();

	pEffect->BeginPass(0);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphicDev->SetFVF(FVF_SCREEN);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	
	pEffect->EndPass();
	return	S_OK;
}

CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLight*	pInstance = new CLight(pGraphicDev);

	if (FAILED(pInstance->Ready_Light(pLightInfo, iIndex)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLight::Free(void)
{
//	m_pGraphicDev->LightEnable(m_iIndex, FALSE);
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	
}

