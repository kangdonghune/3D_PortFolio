#include "stdafx.h"
#include "Terrain.h"
#include "MainFrm.h"
#include "Form.h"
#include "Sphrer.h"

#include "TabTerrain.h"
#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CTerrain::CTerrain(const CTerrain& rhs)
	: CGameObject(rhs)
{

}

CTerrain::~CTerrain(void)
{

}

HRESULT CTerrain::Ready_Object(void)
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_NaviMesh(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	//m_pIndex = new INDEX32[m_pBufferCom->Get_TriCnt()];

	return S_OK;
}

Engine::_int CTerrain::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	/*m_pOptimizationCom->Isin_FrustumForTerrain(m_pBufferCom->Get_VtxPos(),
												m_pBufferCom->Get_VtxCntX(),
												m_pBufferCom->Get_VtxCntZ(), 
												m_pIndex, 
												&m_dwTriCnt);*/

	Add_RenderGroup(RENDER_NONALPHA, this);

	Key_Input(fTimeDelta);

	return 0;
}

void CTerrain::Render_Object(void)
{
	//FAILED_CHECK_RETURN(SetUp_Material(), );

	//m_pBufferCom->Copy_Index(m_pIndex, m_dwTriCnt);

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iMaxPass = 0;

	pEffect->Begin(&iMaxPass, 0);	// 1. 현재 쉐이더 파일이 가진 최대 pass의 개수 반환 2. 시작하는 방식에 대한 flag 값(default 값)
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	m_pNaviCom->Render_NaviMesh();


}

HRESULT CTerrain::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Clone_Proto(L"Proto_Buffer_TerrainTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);
	
	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<COptimization*>(Clone_Proto(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Optimization", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_Shader_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);


	// NaviMesh
	pComponent = m_pNaviCom = dynamic_cast<CNaviMesh*>(Clone_Proto(L"Proto_Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Navi", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);

	return S_OK;
}

HRESULT CTerrain::SetUp_Material(void)
{
	D3DMATERIAL9		tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}

HRESULT CTerrain::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{
	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	const D3DLIGHT9*	pLight = Get_Light();
	NULL_CHECK_RETURN(pLight, E_FAIL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLight->Direction, 0.f));

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Ambient);

	D3DMATERIAL9		tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tMtrl.Power = 0.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrl.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrl.Ambient);


	return S_OK;
}

HRESULT CTerrain::SetUp_NaviMesh()
{
	for (CCell* pCell : m_vecCell)
	{
		m_pNaviCom->Add_Cell(pCell);
	}

	return S_OK;
}

void CTerrain::Key_Input(const _float & fTimeDelta)
{
	if (Get_DIMouseState(DIM_LB) & 0X80)
	{
		_vec3 MovePos = m_pCalculatorCom->Picking_OnTerrain(g_HWnd, m_pBufferCom, m_pTransformCom);
		if (MovePos.y == -100.f)
			return;
		CSphere* pSphere = m_pForm->m_ptabTerrain->Create_Sphrer(m_pGraphicDev, &MovePos);
		if (pSphere == nullptr)
			return;
		m_vecShpere.push_back(pSphere);
	}

	if (Get_DIMouseState(DIM_RB) & 0X80)
	{
		
	}

}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{

	//Safe_Delete_Array(m_pIndex);

	m_vecCell.clear();
	m_vecShpere.clear();
	CGameObject::Free();
}

