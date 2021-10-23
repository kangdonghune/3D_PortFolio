
#include "stdafx.h"
#include "Monster.h"
#include "Export_Function.h"
#include "Sphrer.h"


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
	
	m_pMeshCom->Set_AnimationIndex(0);



	return S_OK;
}

Engine::_int CMonster::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return 0;

	CGameObject::Update_Object(fTimeDelta);

	StateCheck();
	m_pMeshCom->Play_Animation(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);
	m_pSphere->Update_Object(fTimeDelta);

	if (nullptr == m_pSphere->Get_ParentBoneMartrix())
	{

		const D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName("Goblin_Belly");

		m_pSphere->Set_ParentBoneMartrix(&pFrame->CombinedTransformMatrix);
		m_pSphere->Set_ParentWorldMartrix(m_pTransformCom->Get_WorldMatrix());
	}


	CGameObject::Update_Object(fTimeDelta);

	m_pSphere->Update_Object(fTimeDelta);

	m_pShprerTransCom->Set_ParentMatrix(&(*m_pSphere->Get_ParentBoneMartrix() * *m_pSphere->Get_ParentWorldMartrix()));

	return 0;
}

void CMonster::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	//m_pNaviCom->Render_NaviMesh();

	m_pMeshCom->Render_Meshes();

}

void CMonster::Key_Input(const _float& fTimeDelta)
{

}

void CMonster::StateCheck()
{
	//switch (type)
	//{
	//case MonsterState::IDLE:
	//	if (true == m_pMeshCom->Is_AnimationsetFinish())
	//		m_pMeshCom->Set_AnimationIndex(Goblin_Blacksmith_Idle);
	//	break;
	//case MonsterState::IMPACT:
	//	m_pMeshCom->Set_AnimationIndex(Goblin_Blacksmith_Impact_F_FromL);
	//	if (true == m_pMeshCom->Is_AnimationsetFinish())
	//		m_pMeshCom->Set_AnimationIndex(Goblin_Blacksmith_Idle);
	//	break;
	//case MonsterState::Enum_END:
	//	if (true == m_pMeshCom->Is_AnimationsetFinish())
	//		m_pMeshCom->Set_AnimationIndex(Goblin_Blacksmith_Idle);
	//	break;
	//default:
	//	if (true == m_pMeshCom->Is_AnimationsetFinish())
	//		m_pMeshCom->Set_AnimationIndex(Goblin_Blacksmith_Idle);
	//	break;
	//}
}

HRESULT CMonster::Add_Component(void)
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




	return S_OK;

}

HRESULT CMonster::Add_Collider(void)
{
	m_pSphere = CSphere::Create(m_pGraphicDev, 30.2f);
	m_pSphere->Set_Height(0.f);
	m_pShprerTransCom = (CTransform*)m_pSphere->Get_Component(L"Com_Transform", ID_DYNAMIC);
	m_pShprerTransCom->Set_Pos(&_vec3{ 0.f, 0.f, 0.f });
	return S_OK;
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
}

