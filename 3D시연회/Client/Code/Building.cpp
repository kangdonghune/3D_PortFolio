
#include "stdafx.h"
#include "Building.h"
#include "Export_Function.h"
#include "Sphrer.h"
	

CBuilding::CBuilding(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CBuilding::CBuilding(const CBuilding& rhs)
	: CGameObject(rhs)
{

}

CBuilding::~CBuilding(void)
{

}

HRESULT CBuilding::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Object(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	
	//m_pNaviCom->Set_CellIndex(1);

	m_pTransformCom->Get_Info(INFO_POS, &m_vPos);

	return S_OK;
}

Engine::_int CBuilding::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return 0;

	CGameObject::Update_Object(fTimeDelta);

	//SetUp_OnTerrain();


	Add_RenderGroup(RENDER_NONALPHA, this);
	/*m_pShprer->Update_Object(fTimeDelta);

	m_pTransformCom->Get_Info(INFO_POS, &m_vPos);
	m_pShprerTransCom->Set_Pos(m_vPos.x, m_vPos.y + m_pShprer->Get_Height(), m_vPos.z);*/

	return 0;
}

void CBuilding::Render_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	//카메라 at을 가져와서 카메라보다 뒤에 있으면 그리지마라.(문제는 집이 통짜건물이라는 점, 분별하려면 낮개로 다 뜯어와야함,근데 그건 너무 빡시다.)
	//카메라 뒤로 하면 집 안에 있다가 집이 사라져버리는 문제 발생 가능(
	//m_pNaviCom->Render_NaviMesh();

	//m_pMeshCom->Render_Meshes();

	
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iMaxPass = 0;

	pEffect->Begin(&iMaxPass, 0);	// 1. 현재 쉐이더 파일이 가진 최대 pass의 개수 반환 2. 시작하는 방식에 대한 flag 값(default 값)
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);






}



HRESULT CBuilding::Add_Component(void)
{
	CComponent*			pComponent = nullptr;



	// NaviMesh
	pComponent = m_pNaviCom = dynamic_cast<CNaviMesh*>(Clone_Proto(L"Proto_Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Navi", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);



	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;

}

HRESULT CBuilding::Add_Object(void)
{

	m_pShprer = CSphere::Create(m_pGraphicDev, 2.f);
	m_pShprer->Set_Height(1.5f);
	m_pShprerTransCom = (CTransform*)m_pShprer->Get_Component(L"Com_Transform", ID_DYNAMIC);
	return S_OK;	
}




HRESULT CBuilding::Select_ProtoMesh(const _tchar * pObjProtoName)
{
	CComponent*			pComponent = nullptr;
	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Proto(pObjProtoName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	return S_OK;
}

HRESULT CBuilding::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	/*const D3DLIGHT9*	pLight = Get_Light();
	NULL_CHECK_RETURN(pLight, E_FAIL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLight->Direction, 0.f));

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLight->Specular);

	D3DMATERIAL9		tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tMtrl.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrl.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrl.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrl.Specular);
	pEffect->SetFloat("g_fPower", tMtrl.Power);


	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView._41);*/

	return S_OK;
}

CBuilding* CBuilding::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pObjProtoName)
{
	CBuilding*	pInstance = new CBuilding(pGraphicDev);
	pInstance->Set_NameTag(pObjProtoName);
	if (FAILED(pInstance->Select_ProtoMesh(pObjProtoName)))
		Safe_Release(pInstance);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBuilding::Free(void)
{
	CGameObject::Free();
	m_pShprer->Set_Dead(true);
}

