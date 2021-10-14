
#include "stdafx.h"
#include "Player.h"
#include "Sphrer.h"
#include "Object.h"
#include "Export_Function.h"

#include "Terrain.h"

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
//	FAILED_CHECK_RETURN(Add_Object(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	//m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	//m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	
	FAILED_CHECK_RETURN(LateAdd_Component(), E_FAIL);
	m_pMeshCom->Set_AnimationIndex(Player_Move_Idle_01);
	
	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	if(m_bDead)
		return 0;
	

	CGameObject::Update_Object(fTimeDelta);

//	SetUp_OnTerrain();

	Key_Input(fTimeDelta);

	m_pMeshCom->Play_Animation(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);



	//_vec3	vPos;
	//m_pTransformCom->Get_Info(INFO_POS, &vPos);
	//m_pShprerTransCom->Set_Pos(vPos.x, vPos.y+ m_pShprer->Get_Height(), vPos.z);


	return 0;
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pMeshCom->Render_Meshes();
	_matrix	matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pDynamicColliderCom->Render_Buffer(&matWorld);
}

HRESULT CPlayer::Add_Component(void)
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

	//NaviMap
	CTerrain* pTerrain = (CTerrain*)Engine::Get_List(GAMELOGIC, L"Terrain").front();
	m_pNaviCom = (CNaviMesh*)pTerrain->Get_Component(L"Com_Navi", ID_STATIC);





	return S_OK;

}

HRESULT CPlayer::LateAdd_Component(void)
{
	CComponent*			pComponent = nullptr;

	_vec3 PlayerPos = {};
	m_pTransformCom->Get_Info(INFO_POS, &PlayerPos); //플레이어 월드 좌표
	_matrix matWorld, matInverseWorld; 
	m_pTransformCom->Get_WorldMatrix(&matWorld); //플레이어 월드행렬
	D3DXMatrixInverse(&matInverseWorld, NULL, &matWorld); //플레이어 월드 행렬 역행렬로
	D3DXVec3TransformCoord(&PlayerPos, &PlayerPos, &matInverseWorld); //플레이어 월드 좌표를 로컬 좌표로
	pComponent = m_pDynamicColliderCom = CDynamicCollider::Create(m_pGraphicDev, &PlayerPos, 175.f, 20.f, 20.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	return S_OK;
}

HRESULT CPlayer::Add_Object(void)
{

	m_pShprer = CSphere::Create(m_pGraphicDev, 1.f);
	m_pShprer->Set_Height(1.f);
	m_pShprerTransCom = (CTransform*)m_pShprer->Get_Component(L"Com_Transform", ID_DYNAMIC);
	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	m_pTransformCom->Get_Info(INFO_LOOK, &m_vDir);


	if (Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3	vPos, vPushPos, vDir;
		_float  fPushDist;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		
		
		m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f)));
		if (fPushDist = Check_ObjectCollision())//0이 아니면 충돌
		{																				 
			vPushPos = vPos - vDir *fTimeDelta *10.f;
			m_pTransformCom->Set_Pos(&vPushPos);
		}
			
		m_pMeshCom->Set_AnimationIndex(Player_Move_Walk_F);

	}
	
	if (Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3	vPos, vPushPos, vDir;
		_float  fPushDist;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);


		m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * -5.f)));
		if (fPushDist = Check_ObjectCollision())//0이 아니면 충돌
		{
			vPushPos = vPos + vDir *fTimeDelta *10.f;
			m_pTransformCom->Set_Pos(&vPushPos);
		}
		m_pMeshCom->Set_AnimationIndex(Player_Move_Walk_B);
	}

	if (Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3	vPos, vPushPos, vDir;
		_float  fPushDist;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * -5.f)));
		if (fPushDist = Check_ObjectCollision())//0이 아니면 충돌
		{
			vPushPos = vPos + vDir *fTimeDelta *10.f;
			m_pTransformCom->Set_Pos(&vPushPos);
		}
		m_pMeshCom->Set_AnimationIndex(Player_Move_Walk_L);
	}

	if (Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3	vPos, vPushPos, vDir;
		_float  fPushDist;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);


		m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f)));
		if (fPushDist = Check_ObjectCollision())//0이 아니면 충돌
		{
			vPushPos = vPos - vDir *fTimeDelta *10.f;
			m_pTransformCom->Set_Pos(&vPushPos);
		}
		m_pMeshCom->Set_AnimationIndex(Player_Move_Walk_R);
	}	


		
	if (Get_DIMouseState(DIM_LB) & 0X80)
	{
		m_pMeshCom->Set_AnimationIndex(Player_Atk_Power_01_NoPower);
	}

	//if(true == m_pMeshCom->Is_AnimationsetFinish())
	//	m_pMeshCom->Set_AnimationIndex(57);\
		
	if (true == m_pMeshCom->Is_AnimationsetFinish())
		m_pMeshCom->Set_AnimationIndex(Player_Move_Idle_01);
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

_float CPlayer::Check_ObjectCollision()
{
	list<CGameObject*> Objlist = Get_List(GAMELOGIC, L"Object");

	_float fPushDist = 0.f;

	for (CGameObject* pGameObj : Objlist)
	{
		C_Object* pObj = dynamic_cast<C_Object*>(pGameObj);
		NULL_CHECK_RETURN(pObj, false);
		CCollider* pCollider = (CCollider*)pObj->Get_Component(L"Com_Collider", ID_STATIC);

		if (m_pCalculatorCom->Collision_OBB(m_pDynamicColliderCom->Get_Min(), m_pDynamicColliderCom->Get_Max(),m_pDynamicColliderCom->Get_Center(), m_pDynamicColliderCom->Get_MaxDir(),
											m_pDynamicColliderCom->Get_ColliderWorld(),
											pCollider->Get_Min(), pCollider->Get_Max(), pCollider->Get_Center() ,pCollider->Get_MaxDir(),
											pCollider->Get_CollWorldMatrix(),
											&fPushDist
											))
			return fPushDist;
	}
	return 0;
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
	//m_pShprer->Set_Dead(true);
}

