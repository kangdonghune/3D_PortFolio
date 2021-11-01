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
	//Setting_Transform();

	m_iLoadedBullet = 30;
	m_iResidueBullet = 999;
	return S_OK;
}

Engine::_int CWeapon::Update_Object(const _float& fTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix)
	{
		CPlayer*		pPlayer = dynamic_cast<CPlayer*>(Get_List(GAMELOGIC, L"Player")->front());
		
		CDynamicMesh*		pPlayerMeshCom = dynamic_cast<CDynamicMesh*>(pPlayer->Get_Component(L"Com_Mesh", ID_STATIC));
		NULL_CHECK_RETURN(pPlayerMeshCom, -1);

		const D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("Hunter_RightHand_Weap");

		m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

		CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayerTransCom, -1);

		m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();
	}
		

	CGameObject::Update_Object(fTimeDelta);



	m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));

//	m_pSphere->Update_Object(fTimeDelta);
	m_pSphrerTransformCom->Set_ParentMatrix(m_pTransformCom->Get_WorldMatrix());


	KeyInput();

	if(m_bIsReload)
		m_pMeshCom->Play_Animation(fTimeDelta);

	Collision_ToObject();


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

void CWeapon::Shot()
{
	if (m_iLoadedBullet == 0)
	{
		return;
	}
	m_iLoadedBullet--;
}

void CWeapon::ReLoad()
{
	if (m_iResidueBullet == 0)
		return;

	m_iLoadedBullet = 30;
	m_iResidueBullet -= 30;
	if (m_iResidueBullet < 0)
	{
		m_iLoadedBullet = 30 - abs(m_iResidueBullet);
		m_iResidueBullet = 0;
	}
}

HRESULT CWeapon::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// 
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_AK47"));
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
	m_pSphrerTransformCom->Set_Pos(&_vec3{ 0.0f, 0.0f, 0.f });


	return S_OK;
}

_bool CWeapon::Collision_ToObject()
{
	list<CGameObject*> pMonlist = *(Engine::Get_List(GAMELOGIC, L"Monster"));
	for (CGameObject* pobj : pMonlist)
	{
		CMonster* pMon = dynamic_cast<CMonster*>(pobj);
		if (m_pCalculatorCom->Collision_Sphere(m_pSphrerTransformCom, m_pSphere->Get_Radius() / 100.f, pMon->Get_SphereTransform(), pMon->Get_Sphere()->Get_Radius() / 100.f))
		{

			pMon->Set_State(MonsterState::IMPACT);
			return true;
		}
	}


	return false;
}


void CWeapon::Setting_ColliderPos()
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pSphrerTransformCom->Set_Pos(vPos.x, vPos.y+ m_pSphere->Get_Height(), vPos.z);
}

void CWeapon::KeyInput()
{
	if (m_pMeshCom->Is_AnimationsetFinish())
	{
		m_bIsReload = false;
	}
	if (GetAsyncKeyState('R') & 0x8001)
	{
		m_pMeshCom->Set_AnimationIndex(K_Hunter_Reload_M4Flamer_anim);

		m_bIsReload = true;
	}


}

void CWeapon::Setting_Transform()
{
	//일단 로컬로 내린다음에 뭘 해줘야지 월드 기준으로 돌려버리면 문제가 되는 듯?
	_vec3 GunPos,GunRotate;
	m_pTransformCom->Get_Info(INFO_POS, &GunPos);
	GunRotate = m_pTransformCom->Get_RotationVec();
	//총 위치하고 회전 벡터를 구해옴
	_matrix matWorld,matInverseWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matInverseWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&GunRotate, &GunRotate, &matInverseWorld);
	D3DXVec3TransformCoord(&GunRotate, &GunRotate, &matInverseWorld);
	//로컬 단계의 각도로 전환 후 회전
	GunRotate.x += 100.f;
	GunRotate.y += 90.f;
	GunRotate.z += 90.f;
	GunPos.z += 9.f;
	GunPos.x -= 10.f;
	GunPos.y -= 8.f;

	D3DXVec3TransformCoord(&GunPos, &GunPos, &matWorld);
	D3DXVec3TransformCoord(&GunRotate, &GunRotate, &matWorld);
	m_pTransformCom->Rotation2(ROT_X, GunRotate.x);
	m_pTransformCom->Rotation2(ROT_Y, GunRotate.y);
	m_pTransformCom->Rotation2(ROT_Z, GunRotate.z);
	//m_pTransformCom->Set_Pos(&GunPos);

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

