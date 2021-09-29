
#include "stdafx.h"
#include "Player.h"
#include "Sphrer.h"
#include "Export_Function.h"
#include "MainFrm.h"
#include "Form.h"
#include "TabChar.h"
#include "TabObject.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Object(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	
	//m_pNaviCom->Set_CellIndex(1);
	m_pMeshCom->Set_AnimationIndex(0);
	
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	if(m_bDead)
		return 0;
	

	CGameObject::Update_Object(fTimeDelta);

	//SetUp_OnTerrain();

	Key_Input(fTimeDelta);

	//m_pMeshCom->Play_Animation(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);
	
	//m_pShprerTransCom->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pShprerTransCom->Set_Pos(vPos.x, vPos.y+ m_pShprer->Get_Height(), vPos.z);

	//m_pShprer->Update_Object(fTimeDelta);

	return 0;
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	//m_pNaviCom->Render_NaviMesh();

	m_pMeshCom->Render_Meshes();;
}

HRESULT CPlayer::Add_Component(void)
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

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);





	return S_OK;

}

HRESULT CPlayer::Add_Object(void)
{

	m_pShprer = CSphere::Create(m_pGraphicDev, 1.f);
	m_pShprer->Set_Height(1.f);
	m_pShprerTransCom = (CTransform*)m_pShprer->Get_Component(L"Com_Transform", ID_DYNAMIC);
	(CTransform*)m_pShprer->Get_Component(L"Com_Transform", ID_DYNAMIC)->AddRef();
	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	m_pTransformCom->Get_Info(INFO_LOOK, &m_vDir);


	//if (Get_DIKeyState(DIK_UP) & 0x80)
	//{
	//	_vec3	vPos, vDir;
	//	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	//	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	//	D3DXVec3Normalize(&vDir, &vDir);

	//	m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f)));
	//	m_pMeshCom->Set_AnimationIndex(0);
	//}
	//

	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//{
	//	D3DXVec3Normalize(&m_vDir, &m_vDir);
	//	m_pTransformCom->Move_Pos(&m_vDir, -10.f, fTimeDelta);
	//}

	//if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//{
	//	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * -fTimeDelta);
	//}

	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//{
	//	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * fTimeDelta);
	//}	

	if (Get_DIMouseState(DIM_LB) & 0X80)
	{
		if (m_pCalculatorCom->raySphrerIntersection(g_HWnd, m_pShprer->Get_Radius(), m_pShprerTransCom))
		{
			m_pForm->m_ptabChar->Set_Object(this);
			m_pForm->m_ptabObject->Set_Object(nullptr);
			m_pForm->m_ptabChar->Get_Transform();
			m_pForm->m_ptabChar->Get_Rotate();
			m_pForm->m_ptabChar->Get_Scale();
		}
	}

	if (Get_DIMouseState(DIM_RB) & 0X80)
	{
		if (m_pForm->m_ptabChar->m_pObject == nullptr)
			return;

		list<CGameObject*> pTerrainTexlst = Engine::Get_List(L"GameLogic", L"Terrain");
		  
		CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>((*pTerrainTexlst.begin())->Get_Component(L"Com_Buffer", ID_STATIC));
		CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>((*pTerrainTexlst.begin())->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_vec3 MovePos = m_pCalculatorCom->Picking_OnTerrain(g_HWnd, pTerrainBufferCom, pTerrainTransCom);
		if (MovePos.y == -100.f)
			return;
		m_pForm->m_ptabChar->Set_Transform(&MovePos);
	}
		
	//if (Get_DIMouseState(DIM_RB) & 0X80)
	//{
	//	m_pMeshCom->Set_AnimationIndex(30);
	//}

	//if(true == m_pMeshCom->Is_AnimationsetFinish())
	//	m_pMeshCom->Set_AnimationIndex(57);
}

void CPlayer::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);


	list<CGameObject*> pTerrainTexlst = Engine::Get_List(L"GameLogic", L"Terrain");

	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>((*pTerrainTexlst.begin())->Get_Component(L"Com_Buffer", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3*	ptPos = pTerrainBufferCom->Get_VtxPos();


	_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_vec3 CPlayer::PickUp_OnTerrain(void)
{


	list<CGameObject*> pTerrainTexlst = Engine::Get_List(L"GameLogic", L"Terrain");

	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>((*pTerrainTexlst.begin())->Get_Component(L"Com_Buffer", ID_STATIC));
	CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>((*pTerrainTexlst.begin())->Get_Component(L"Com_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());


	return m_pCalculatorCom->Picking_OnTerrain(g_HWnd, pTerrainBufferCom, pTerrainTransCom);
}

HRESULT CPlayer::Select_ProtoMesh(const _tchar * pObjProtoName)
{
	CComponent*			pComponent = nullptr;
	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(pObjProtoName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pObjProtoName)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);
	pInstance->Set_NameTag(pObjProtoName);
	if (FAILED(pInstance->Select_ProtoMesh(pObjProtoName)))
		Safe_Release(pInstance);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free(void)
{
	CGameObject::Free();
	m_pShprer->Set_Dead(true);
}

