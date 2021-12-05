
#include "stdafx.h"
#include "Monster.h"
#include "Player.h"
#include "Export_Function.h"
#include "Sphrer.h"
#include "Object.h"
#include "Terrain.h"


CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CMonster::CMonster(const CMonster& rhs)
	: CGameObject(rhs)
{

}

CMonster::~CMonster(void)
{

}

HRESULT CMonster::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Collider(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);

	m_pMeshCom->Set_AnimationIndex(Idle);

	m_pTarget = dynamic_cast<CPlayer*>(Get_List(PLAYER, L"Player")->front());


	return S_OK;
}

Engine::_int CMonster::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return 0;


	if (m_iHp <= 0 && type != MonsterState::STOP)
	{
		type = MonsterState::DEAD;
	}

	StateCheck(fTimeDelta);
	CGameObject::Update_Object(fTimeDelta);
	m_fDeltaTime = fTimeDelta;
	Update_TargetDist();
	

	Add_RenderGroup(RENDER_NONALPHA, this);

	if (nullptr == m_pSphere->Get_ParentBoneMartrix())
	{

		const D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName("Goblin_Spine");

		m_pSphere->Set_ParentBoneMartrix(&pFrame->CombinedTransformMatrix);
		m_pSphere->Set_ParentWorldMartrix(m_pTransformCom->Get_WorldMatrix());
	}

	if (nullptr == m_pAttackSphere->Get_ParentBoneMartrix())
	{

		const D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName("Goblin_JOChain11Tip");

		m_pAttackSphere->Set_ParentBoneMartrix(&pFrame->CombinedTransformMatrix);
		m_pAttackSphere->Set_ParentWorldMartrix(m_pTransformCom->Get_WorldMatrix());
	}

	m_pAttackSphereTransCom->Set_WorldMatrix(&(*m_pAttackSphere->Get_ParentBoneMartrix() * *m_pAttackSphere->Get_ParentWorldMartrix()));
	m_pShprerTransCom->Set_WorldMatrix(&(*m_pSphere->Get_ParentBoneMartrix() * *m_pSphere->Get_ParentWorldMartrix()));

	return 0;
}

void CMonster::Render_Object(void)
{
	if (type == MonsterState::ROAR)
		m_pMeshCom->Play_Animation(2 * m_fDeltaTime);
	else if (type == MonsterState::STOP)
	{

	}
	else
		m_pMeshCom->Play_Animation(m_fDeltaTime);

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

void CMonster::Key_Input(const _float& fTimeDelta)
{

}

	
void CMonster::StateCheck(_float fTimedelta)
{
	switch (type)
	{
	case MonsterState::IDLE:
		IdleF();
		break;
	case MonsterState::ROAR:
		Roar();
		break;
	case MonsterState::WALK:
		Walk(fTimedelta);
		break;
	case MonsterState::ATTACK:
		Attack();
		break;
	case MonsterState::DOWN:
		Down();
		break;
	case MonsterState::GETUP:
		GetUp();
		break;
	case MonsterState::DEAD:
		Dead();
		break;
	case MonsterState::DAMAGED:
		Damage();
		break;
	case MonsterState::STOP:
		Stop();
		break;
	case MonsterState::Enum_END:
		break;
	default:
		break;
	}
}

HRESULT CMonster::Add_Component(void)
{
	CComponent*			pComponent = nullptr;



	// NaviMesh
	CTerrain* pTerrain = (CTerrain*)Engine::Get_List(ENVIRONMENT, L"Terrain")->front();
	NULL_CHECK_RETURN(pTerrain, E_FAIL);
	m_pNaviCom = (CNaviMesh*)pTerrain->Get_Component(L"Com_Navi", ID_STATIC);


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

	//_vec3 Pos = {};
	//m_pTransformCom->Get_Info(INFO_POS, &Pos); //플레이어 월드 좌표
	//_matrix matWorld, matInverseWorld;
	//m_pTransformCom->Get_WorldMatrix(&matWorld); //플레이어 월드행렬
	//D3DXMatrixInverse(&matInverseWorld, NULL, &matWorld); //플레이어 월드 행렬 역행렬로
	//D3DXVec3TransformCoord(&Pos, &Pos, &matInverseWorld); //플레이어 월드 좌표를 로컬 좌표로
	//pComponent = m_pDynamicColliderCom = CDynamicCollider::Create(m_pGraphicDev, &Pos, 185.f, 50.f, 50.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);


	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);



	return S_OK;

}

HRESULT CMonster::Add_Collider(void)
{
	m_pSphere = CSphere::Create(m_pGraphicDev, 50.2f);
	m_pSphere->Set_Height(0.f);
	m_pShprerTransCom = (CTransform*)m_pSphere->Get_Component(L"Com_Transform", ID_DYNAMIC);
	m_pShprerTransCom->Set_Pos(&_vec3{ 0.f, 0.f, 0.f });


	m_pAttackSphere = CSphere::Create(m_pGraphicDev, 30.2f);
	m_pAttackSphere->Set_Height(0.f);
	m_pAttackSphereTransCom = (CTransform*)m_pAttackSphere->Get_Component(L"Com_Transform", ID_DYNAMIC);
	m_pAttackSphereTransCom->Set_Pos(&_vec3{ 0.f, 0.f, 0.f });
	return S_OK;
}

_float CMonster::Check_ObjectCollision()
{
	list<CGameObject*>* Objlist = Engine::Get_List(GAMELOGIC, L"Object");
	if (Objlist == nullptr)
		return 0.f;
	_float fPushDist = 0.f;

	for (CGameObject* pGameObj : *Objlist)
	{
		C_Object* pObj = dynamic_cast<C_Object*>(pGameObj);
		NULL_CHECK_RETURN(pObj, false);
		CCollider* pCollider = (CCollider*)pObj->Get_Component(L"Com_Collider", ID_STATIC);

		if (m_pCalculatorCom->Collision_OBB(m_pDynamicColliderCom->Get_Min(), m_pDynamicColliderCom->Get_Max(), m_pDynamicColliderCom->Get_ColliderWorld(),
			pCollider->Get_Min(), pCollider->Get_Max(),pCollider->Get_CollWorldMatrix()
		))
			return fPushDist;
	}
	return 0;
}



HRESULT CMonster::Select_ProtoMesh(const _tchar * pObjProtoName)
{
	CComponent*			pComponent = nullptr;
	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(pObjProtoName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	return S_OK;
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pObjProtoName)
{
	CMonster*	pInstance = new CMonster(pGraphicDev);
	pInstance->Set_NameTag(pObjProtoName);
	if (FAILED(pInstance->Select_ProtoMesh(pObjProtoName)))
		Safe_Release(pInstance);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMonster::Free(void)
{
	CGameObject::Free();
	m_pSphere->Set_Dead(true);
	m_pAttackSphere->Set_Dead(true);
}

void CMonster::IdleF()
{
	if (m_iHp < 30 || m_fTargetDist < 50.f)
		type = MonsterState::ROAR;
	m_pMeshCom->Set_AnimationIndex(Idle);
}

void CMonster::Walk(_float fTimedelta)
{
	m_pMeshCom->Set_AnimationIndex(Walk_F);
	_vec3 TargetPos;
	CTransform* TargetTransform = (CTransform*)m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC);
	TargetTransform->Get_Info(INFO_POS, &TargetPos);
	m_pTransformCom->Compute_LookAtTarget_Set(&TargetPos);
	if (m_fTargetDist < 10.f)
		type = MonsterState::ATTACK;
	
	_vec3 vDir = {};
	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	D3DXVec3Normalize(&vDir, &vDir);
	_vec3 vPos = {};
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimedelta * 3.f)));

	//if (Check_ObjectCollision())//0이 아니면 충돌
	//	m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimedelta * -3.f)));
}

void CMonster::Roar()
{
	m_pMeshCom->Set_AnimationIndex(CallToAction_Roar);
	_vec3 TargetPos;
	CTransform* TargetTransform = (CTransform*)m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC);
	TargetTransform->Get_Info(INFO_POS, &TargetPos);
	m_pTransformCom->Compute_LookAtTarget_Set(&TargetPos);;
	if (m_pMeshCom->Is_AnimationsetFinish())
	{
		type = MonsterState::WALK;
		return;
	}
}

void CMonster::Down()
{
	m_pMeshCom->Set_AnimationIndex(KnockDown_B);
	if (m_pMeshCom->Is_AnimationsetFinish())
	{
		type = MonsterState::GETUP;
		return;
	}
}

void CMonster::Damage()
{
	m_pMeshCom->Set_AnimationIndex(CallToAction_Roar);
	if (m_pMeshCom->Is_AnimationsetFinish())
	{
		type = MonsterState::WALK;
		return;
	}
}

void CMonster::Dead()
{
	m_pMeshCom->Set_AnimationIndex(Death_B);
	if (m_pMeshCom->Is_AnimationsetFinish())
	{
		Set_Dead(true);
		return;
	}
}

void CMonster::GetUp()
{
	m_pMeshCom->Set_AnimationIndex(GetUp_B);
	if (m_pMeshCom->Is_AnimationsetFinish())
	{
		type = MonsterState::WALK;
		return;
	}
}

void CMonster::Attack()
{
	m_pMeshCom->Set_AnimationIndex(TentacleSwing_Sideways_00);
	if (m_pMeshCom->Is_AnimationsetFinish())
	{
		type = MonsterState::WALK;
		return;
	}
}

void CMonster::Stop()
{
	//m_pMeshCom->Set_AnimationIndex(GetUp_B);
}

void CMonster::Update_TargetDist()
{
	_vec3 TargetPos, MonsPos;

	CTransform* TargetTransform = (CTransform*)m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC);
	TargetTransform->Get_Info(INFO_POS, &TargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &MonsPos);
	_vec3 TargetDist = TargetPos - MonsPos;
	m_fTargetDist = D3DXVec3Dot(&TargetDist, &TargetDist);
}

HRESULT CMonster::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	//const D3DLIGHT9*	pLight = Get_Light();
	//NULL_CHECK_RETURN(pLight, E_FAIL);

	//pEffect->SetVector("g_vLightDir", &_vec4(pLight->Direction, 0.f));

	//pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Diffuse);
	//pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Ambient);
	//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLight->Specular);

	//D3DMATERIAL9		tMtrl;
	//ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	//tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	//tMtrl.Power = 20.f;

	//pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrl.Diffuse);
	//pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrl.Ambient);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrl.Specular);
	//pEffect->SetFloat("g_fPower", tMtrl.Power);


	//D3DXMatrixInverse(&matView, NULL, &matView);
	//pEffect->SetVector("g_vCameraPos", (_vec4*)&matView._41);

	return S_OK;
}
