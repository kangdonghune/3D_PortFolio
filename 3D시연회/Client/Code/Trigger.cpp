#include "stdafx.h"


#include "Export_Function.h"
#include "Trigger.h"
//#include "../MFC/Include/MainFrm.h"

_int CTrigger::m_iShpereCount = -1;
_int CTrigger::m_iTriggerCount = -1;
CTrigger::CTrigger(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_iTriggerCount++;
	m_iTriggetNum = m_iTriggerCount;
}

CTrigger::CTrigger(const CTrigger& rhs)
	: CGameObject(rhs)
{
	m_iID = rhs.m_iID;
	m_iTriggerCount++;
	m_iTriggetNum = m_iTriggerCount;
}

CTrigger::~CTrigger(void)
{
	m_iTriggerCount--;
}

HRESULT CTrigger::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	////구생성
	LPD3DXMESH            pTempMesh;
	D3DXCreateSphere(m_pGraphicDev, m_fRadius, 10, 10, &pTempMesh, nullptr);
	pTempMesh->CloneMeshFVF(0, FVF_COL, m_pGraphicDev, &m_pSphereMesh);
	pTempMesh->Release();

	m_pSphereMesh->GetVertexBuffer(&m_pVB);
	int iNumVertex = m_pSphereMesh->GetNumVertices();

	VTXCOL* pVert;
	m_pVB->Lock(0, 0, (void**)&pVert, 0);
	for (_int i = 0; i < iNumVertex; i++)
	{
		pVert[i].dwColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	m_pVB->Unlock();
	m_pVB->Release();

	FAILED_CHECK_RETURN(Get_Scene()->Add_GameObject(TRIGGER, L"Trigger", this), E_FAIL);

	return S_OK;
}

Engine::_int CTrigger::Update_Object(const _float& fTimeDelta)
{

	if (m_bDead)
		return 0;

	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);


	//if (nullptr == m_pParentBoneMatrix)
	//{
	//	CDynamicMesh*		pPlayerMeshCom = dynamic_cast<CDynamicMesh*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Mesh", ID_STATIC));
	//	NULL_CHECK_RETURN(pPlayerMeshCom, -1);

	//	//Bip_IK_Hand_R
	//	//R_Hand
	//	const D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("Bip_IK_Hand_R");

	//	m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

	//	CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", ID_DYNAMIC));
	//	NULL_CHECK_RETURN(pPlayerTransCom, -1);

	//	m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();
	//}

	//CGameObject::Update_Object(fTimeDelta);

	////본의 위치 매트릭스랑 플레이어의 월드 매트릭스 가져와서 곱함
	////m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));

	////m_bColl = Collision_ToObject(L"GameLogic", L"Player");


	////navitoolcheck일때만 렌더하자
	//CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	//CForm* pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	////Tab3* pTab = pForm->pTab3;

	//if (pTab->m_CheckNaviTool.GetCheck())
	//{
	//	Add_RenderGroup(RENDER_NONALPHA, this);
	//}

	return 0;
}

void CTrigger::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	if (GetAsyncKeyState(VK_SPACE) & 0x8001)
	{
		////구 출력
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		m_pSphereMesh->DrawSubset(0);

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

HRESULT CTrigger::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

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


	return S_OK;

}

void CTrigger::Set_ID()
{
	m_iShpereCount++;
	m_iID = m_iShpereCount;
}



CTrigger* CTrigger::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fRadius)
{
	CTrigger*	pInstance = new CTrigger(pGraphicDev);
	pInstance->Set_Radius(fRadius);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CTrigger::Free(void)
{
	CGameObject::Free();
}