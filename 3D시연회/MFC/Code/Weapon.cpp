#include "stdafx.h"
#include "Weapon.h"
#include "Player.h"
#include "Sphrer.h"
#include "MainFrm.h"
#include "Form.h"
#include "TabChar.h"
#include "TabObject.h"
#include "Export_Function.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CWeapon::CWeapon(const CWeapon& rhs)
	: CGameObject(rhs)
{

}

CWeapon::~CWeapon(void)
{

}

HRESULT CWeapon::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

//	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	
	
	return S_OK;
}

Engine::_int CWeapon::Update_Object(const _float& fTimeDelta)
{

	CGameObject::Update_Object(fTimeDelta);

	Key_Input(fTimeDelta);
	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");

	if (nullptr == m_pParentBoneMatrix)
	{
		CPlayer*		pPlayer = dynamic_cast<CPlayer*>(Engine::Get_List(GAMELOGIC, L"Player")->front());

		CDynamicMesh*		pPlayerMeshCom = dynamic_cast<CDynamicMesh*>(pPlayer->Get_Component(L"Com_Mesh", ID_STATIC));
		NULL_CHECK_RETURN(pPlayerMeshCom, -1);

		const D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("Hunter_RightHand");

		m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

		CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayerTransCom, -1);

		m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();
	}



	//Setting_Pos();
	m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pShprerTransCom->Set_Pos(vPos.x, vPos.y + m_pShprer->Get_Height(), vPos.z);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CWeapon::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pMeshCom->Render_Meshes();
	
	//m_pColliderCom->Render_Collider(COLLTYPE(false), m_pTransformCom->Get_WorldMatrix());

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
}

HRESULT CWeapon::Select_ProtoMesh(const _tchar * pObjProtoName)
{
	CComponent*			pComponent = nullptr;
	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Proto(pObjProtoName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);
}

void CWeapon::Key_Input(const _float & fTimeDelta)
{



	if (Get_DIKeyState(DIK_1) & 0X80)
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

	if (Get_DIKeyState(DIK_2) & 0X80)
	{
		if (m_pForm->m_ptabChar->m_pObject == nullptr)
			return;


		list<CGameObject*> pTerrainTexlst = *Engine::Get_List(GAMELOGIC, L"Terrain");

		CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>((*pTerrainTexlst.begin())->Get_Component(L"Com_Buffer", ID_STATIC));
		CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>((*pTerrainTexlst.begin())->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_vec3 MovePos = m_pCalculatorCom->Picking_OnTerrain(g_HWnd, pTerrainBufferCom, pTerrainTransCom);
		if (MovePos.y == -100.f)
			return;
		m_pForm->m_ptabObject->Set_Transform(&MovePos);
	}

}

HRESULT CWeapon::Add_Component(void)
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

	// Collider
	pComponent = m_pColliderCom = CCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVtx(), m_pMeshCom->Get_VtxSize());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;

}

HRESULT CWeapon::Add_Object(void)
{
	m_pShprer = CSphere::Create(m_pGraphicDev, 2.f);
	m_pShprer->Set_Height(3.5f);
	m_pShprerTransCom = (CTransform*)m_pShprer->Get_Component(L"Com_Transform", ID_DYNAMIC);
	return S_OK;
}



Engine::_bool CWeapon::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	//CCollider*		pPlayerColliderCom = dynamic_cast<CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", ID_STATIC));
	//NULL_CHECK_RETURN(pPlayerColliderCom, false);

	///*return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
	//										m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());*/

	//return m_pCalculatorCom->Collision_OBB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
	//	m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());
	return false;
}





CWeapon* CWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pObjProtoName)
{
	CWeapon*	pInstance = new CWeapon(pGraphicDev);
	pInstance->Set_NameTag(pObjProtoName);
	if (FAILED(pInstance->Select_ProtoMesh(pObjProtoName)))
		Safe_Release(pInstance);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CWeapon::Free(void)
{
	CGameObject::Free();
	m_pShprer->Set_Dead(true);
}

