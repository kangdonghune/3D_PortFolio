#include "stdafx.h"
#include "Weapon.h"
#include "Player.h"
#include "Monster.h"
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
	FAILED_CHECK_RETURN(Add_Collider(), E_FAIL);
	//m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

Engine::_int CWeapon::Update_Object(const _float& fTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix)
	{
		CPlayer*		pPlayer = dynamic_cast<CPlayer*>(Get_List(GAMELOGIC, L"Player").front());
		
		CDynamicMesh*		pPlayerMeshCom = dynamic_cast<CDynamicMesh*>(pPlayer->Get_Component(L"Com_Mesh", ID_STATIC));
		NULL_CHECK_RETURN(pPlayerMeshCom, -1);

		const D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("Bone_M_Weapon_Sword");

		m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

		CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayerTransCom, -1);

		m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();
	}
		

	CGameObject::Update_Object(fTimeDelta);



	m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));

	m_pSphere->Update_Object(fTimeDelta);
	m_pSphrerTransformCom->Set_ParentMatrix(m_pTransformCom->Get_WorldMatrix());


	if (Collision_ToObject())
	{
		MSG_BOX("Ãæµ¹");
	}

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CWeapon::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pMeshCom->Render_Meshes();
	
	//m_pColliderCom->Render_Collider(COLLTYPE(false), m_pTransformCom->Get_WorldMatrix());
	//m_pColliderCom->Render_Collider(COLLTYPE(false), m_pTransformCom->Get_WorldMatrix());

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
}

HRESULT CWeapon::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Proto(L"Proto_Mesh_AK47"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

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

HRESULT CWeapon::Add_Collider(void)
{

	m_pSphere = CSphere::Create(m_pGraphicDev, 15.2f);
	m_pSphere->Set_Height(0.f);
	m_pSphrerTransformCom = (CTransform*)m_pSphere->Get_Component(L"Com_Transform", ID_DYNAMIC);
	m_pSphrerTransformCom->Set_Pos(&_vec3{ 0.0f, 0.0f, 75.f });
	return S_OK;
}

_bool CWeapon::Collision_ToObject()
{
	list<CGameObject*> pMonlist = Engine::Get_List(GAMELOGIC, L"Monster");
	for (CGameObject* pobj : pMonlist)
	{
		CMonster* pMon = dynamic_cast<CMonster*>(pobj);
		if (m_pCalculatorCom->Collision_Sphere(m_pSphrerTransformCom, m_pSphere->Get_Radius() / 100.f, pMon->Get_SphereTransform(), pMon->Get_Sphere()->Get_Radius() / 100.f))
			return true;
	}


	return false;
}


void CWeapon::Setting_ColliderPos()
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pSphrerTransformCom->Set_Pos(vPos.x, vPos.y+ m_pSphere->Get_Height(), vPos.z);
}


CWeapon* CWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWeapon*	pInstance = new CWeapon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CWeapon::Free(void)
{
	CGameObject::Free();
}

