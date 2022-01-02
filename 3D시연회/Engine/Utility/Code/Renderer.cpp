#include "Export_Utility.h"


USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer(void)
	:m_pVB(nullptr)
	,m_pIB(nullptr)
{

}

Engine::CRenderer::~CRenderer(void)
{
	Free();
}
HRESULT CRenderer::Ready_Renderer(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	if (FAILED(pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,	// 그리고자 하는 도형의 정점의 용량(삼각형이니 정점 세개 크기만큼의 용량)
		0,					// 정적버퍼를 사용함(정해진 배열 공간을 사용), D3DUSAGE_DYNAMIC 옵션을 주면 동적버퍼(파티클 용도)
		FVF_SCREEN,			// 정점의 속성 정보
		D3DPOOL_MANAGED,	// 정적 버퍼 사용시, MANAGED 동적 버퍼 사용 시, DEFAULT
		&m_pVB,
		NULL)))				// 공유할 핸들이지만 기본값 NULL				
		return E_FAIL;


	// 버텍스 버퍼 생성
	if (FAILED(pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,	// 그리고자 하는 도형의 인덱스 개수에 비례한 메모리의 용량
		0,					// 정적버퍼를 사용함(정해진 배열 공간을 사용), D3DUSAGE_DYNAMIC 옵션을 주면 동적버퍼(파티클 용도)
		D3DFMT_INDEX16,			// 인덱스의 정보	
		D3DPOOL_MANAGED,	// 정적 버퍼 사용시, MANAGED 동적 버퍼 사용 시, DEFAULT
		&m_pIB,
		NULL)))				// 공유할 핸들이지만 기본값 NULL				
		return E_FAIL;


	D3DVIEWPORT9	ViewPort;
	ZeroMemory(&ViewPort, sizeof(&ViewPort));

	pGraphicDev->GetViewport(&ViewPort);

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

	return S_OK;
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
	Render_Debug(pGraphicDev);

	Render_Deferred(pGraphicDev);
	Render_LightAcc(pGraphicDev);
	Render_Blend(pGraphicDev);
	//Render_BlendSpecular(pGraphicDev);
	Render_Alpha(pGraphicDev);
	Render_UI(pGraphicDev);

	if (GetAsyncKeyState(VK_TAB) & 0x8000)
	{
		Render_DebugMRT(L"MRT_Deferred");
		Render_DebugMRT(L"MRT_LightAcc");
	}

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

void CRenderer::Render_NonSpecular(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_NONSPECULAR])
	{
		if (iter->Get_IsRender())
			iter->Render_Object();
	}
}

void CRenderer::Render_Specular(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_SPECULAR])
	{
		if (iter->Get_IsRender())
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

void CRenderer::Render_Debug(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_DEBUG])
	{
		if (iter->Get_IsRender())
			iter->Render_Object();
	}
}

void CRenderer::Render_Deferred(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Begin_MRT(L"MRT_Deferred");
	Render_Nonalpha(pGraphicDev);
	Render_NonSpecular(pGraphicDev);
	Render_Specular(pGraphicDev);
	End_MRT(L"MRT_Deferred");
}

void CRenderer::Render_LightAcc(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Begin_MRT(L"MRT_LightAcc");

	CShader* pShader = dynamic_cast<CShader*>(Clone_Proto(L"Proto_Shader_Shade"));
	NULL_CHECK_RETURN(pShader, );

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect,);

	SetUp_Shader(pEffect, L"Target_Nomal", "g_NormalTexture");
	SetUp_Shader(pEffect, L"Target_Depth", "g_DepthTexture");
	pEffect->AddRef();

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	Render_Light(pEffect); 
	pEffect->End();

	End_MRT(L"MRT_LightAcc");

	Safe_Release(pEffect);
}

void CRenderer::Render_Blend(LPDIRECT3DDEVICE9 & pGraphicDev)
{

	CShader* pShader = dynamic_cast<CShader*>(Clone_Proto(L"Proto_Shader_Blend"));
	NULL_CHECK_RETURN(pShader, );

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, );
	pEffect->AddRef();

	SetUp_Shader(pEffect, L"Target_Albedo", "g_AlbedoTexture");
	SetUp_Shader(pEffect, L"Target_Shade", "g_ShadeTexture");
	SetUp_Shader(pEffect, L"Target_Specular", "g_SpecularTexture");


	pEffect->CommitChanges();

	pEffect->Begin(NULL,0);
	pEffect->BeginPass(0); 

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void CRenderer::Render_BlendSpecular(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_SPECULAR])
	{
		CShader* pShader = dynamic_cast<CShader*>(Clone_Proto(L"Proto_Shader_Blend"));
		NULL_CHECK_RETURN(pShader, );

		LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
		NULL_CHECK_RETURN(pEffect, );
		pEffect->AddRef();

		SetUp_Shader(pEffect, L"Target_Albedo", "g_AlbedoTexture");
		SetUp_Shader(pEffect, L"Target_Shade", "g_ShadeTexture");
		SetUp_Shader(pEffect, L"Target_Specular", "g_SpecularTexture");


		pEffect->CommitChanges();

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(1);

		pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
		pGraphicDev->SetFVF(FVF_SCREEN);
		pGraphicDev->SetIndices(m_pIB);
		pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		pEffect->EndPass();
		pEffect->End();

		Safe_Release(pEffect);
	}

}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

}

