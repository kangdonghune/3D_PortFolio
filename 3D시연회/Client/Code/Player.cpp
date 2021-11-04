
#include "stdafx.h"
#include "Player.h"
#include "Sphrer.h"
#include "Object.h"
#include "Export_Function.h"
#include "UI.h"
#include "Weapon.h"
#include "SoundMgr.h"
#include "Monster.h"
#include "Terrain.h"
#include "Effect.h"
#include "Trigger.h"
#include "TriggerFunc.h"
#include "DynamicCamera.h"
#include "ObjAnime.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	,m_State(PlayerState::IDLE)
	,m_3State(Player3State::IDLE)
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

	//m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	//m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	
	FAILED_CHECK_RETURN(LateAdd_Component(), E_FAIL);
	m_pMeshCom->Set_AnimationIndex(M4_Idle);
	
	list<CGameObject*> pHPBarlst = *Engine::Get_List(UI_LAYER, L"HPBar");
	list<CGameObject*> pStaminaBarlst = *Engine::Get_List(UI_LAYER, L"StaminaBar");

	m_pHPBar = dynamic_cast<CUI*>(pHPBarlst.front());
	m_pStaminaBar = dynamic_cast<CUI*>(pStaminaBarlst.front());

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	if(m_bDead)
		return 0;
	

	CGameObject::Update_Object(fTimeDelta);

//	SetUp_OnTerrain();

//	Key_Input(fTimeDelta);
	
	Check_State(fTimeDelta);


	if (m_State == PlayerState::FIRE)
	{
	m_pMeshCom->Play_Animation(fTimeDelta);
	}
	else
		m_pMeshCom->Play_Animation(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	if (nullptr == m_pShprer->Get_ParentBoneMartrix())
	{

		const D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName("Hunter_Spine1");

		m_pShprer->Set_ParentBoneMartrix(&pFrame->CombinedTransformMatrix);
		m_pShprer->Set_ParentWorldMartrix(m_pTransformCom->Get_WorldMatrix());
	}


	m_pShprerTransCom->Set_ParentMatrix(&(*m_pShprer->Get_ParentBoneMartrix() * *m_pShprer->Get_ParentWorldMartrix()));
	Collision_Trigger();


	Update_StatBar();
	//list<CGameObject*> pBuildinglst = Engine::Get_List(GAMELOGIC, L"Building");
	//m_pOptimizationCom->Isin_FrustumForOBuilding(pBuildinglst);




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

void CPlayer::Check_State(const _float & fTimeDelta)
{
	switch (m_State)
	{
	case PlayerState::IDLE:
		Idle(fTimeDelta);
		break;
	case PlayerState::WALK:
		Walk(fTimeDelta);
		break;
	case PlayerState::RUN:
		Run(fTimeDelta);
		break;
	case PlayerState::AIMStand:
		AimStand(fTimeDelta);
		break;
	case PlayerState::AIMWALK:
		AimWalk(fTimeDelta);
		break;
	case PlayerState::FIRE:
		Fire(fTimeDelta);
		break;
	case PlayerState::RELOAD:
		Reload(fTimeDelta);
		break;
	case PlayerState::BASH:
		Bash(fTimeDelta);
		break;
	default:
		break;
	}
}

void CPlayer::Update_Stat()
{

}

void CPlayer::Idle(const _float & fTimeDelta)
{
	if (m_fStamina < 200.f)
		m_fStamina += 20.f* fTimeDelta;

	m_pMeshCom->Set_AnimationIndex(M4_Idle);

	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_State = PlayerState::WALK;
		m_3State = Player3State::MOVE;
		return;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_State = PlayerState::WALK;
		m_3State = Player3State::MOVE;
		return;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_State = PlayerState::WALK;
		m_3State = Player3State::MOVE;
		return;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_State = PlayerState::WALK;
		m_3State = Player3State::MOVE;
		return;
	}
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_State = PlayerState::AIMStand;
		m_3State = Player3State::AIM;
		return;
	}
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_State = PlayerState::FIRE;
		m_3State = Player3State::AIM;
		return;
	}
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		m_State = PlayerState::BASH;
		return;
	}
	if (GetAsyncKeyState('R') & 0x8000)
	{
		m_State = PlayerState::RELOAD;
		CSoundMgr::Get_Instance()->PlaySound(L"SA2_Weapon_AR_M4A1_Reload.ogg", CSoundMgr::PLAYER);
		return;
	}
}

void CPlayer::Walk(const _float & fTimeDelta)
{
	if (m_fStamina < 200.f)
		m_fStamina += 20.f* fTimeDelta;

	Direction::Dir Dir = Direction::NONE;
	m_vDir = {};
	_vec3 vPos, vLook, vRight;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	if (GetAsyncKeyState(VK_SHIFT) & 0x8001)
	{
		m_State = PlayerState::RUN;
		m_3State = Player3State::MOVE;
		return;
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
	{
		m_State = PlayerState::FIRE;
		m_3State = Player3State::AIM;
		return;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8001)
	{
		m_State = PlayerState::AIMWALK;
		m_3State = Player3State::AIM;
		return;
	}

	if (GetAsyncKeyState('Q') & 0x8001)
	{
		m_State = PlayerState::BASH;
		m_3State = Player3State::MOVE;
		return;
	}


	if (GetAsyncKeyState('W') & 0x8001)
	{
		Dir = Direction::F;
		if (GetAsyncKeyState('A') & 0x8001)
			Dir = Direction::FL;
		if (GetAsyncKeyState('D') & 0x8001)
			Dir = Direction::FR;
	}

	if (GetAsyncKeyState('S') & 0x8001)
	{
		Dir = Direction::B;
		if (GetAsyncKeyState('A') & 0x8001)
			Dir = Direction::BL;
		if (GetAsyncKeyState('D') & 0x8001)
			Dir = Direction::BR;
	}

	if (GetAsyncKeyState('A') & 0x8001)
	{
		Dir = Direction::L;
		if (GetAsyncKeyState('W') & 0x8001)
			Dir = Direction::FL;
		if (GetAsyncKeyState('S') & 0x8001)
			Dir = Direction::BL;
	}

	if (GetAsyncKeyState('D') & 0x8001)
	{
		Dir = Direction::R;
		if (GetAsyncKeyState('W') & 0x8001)
			Dir = Direction::FR;
		if (GetAsyncKeyState('S') & 0x8001)
			Dir = Direction::BR;
	}

	switch (Dir)
	{
	case Direction::F:
		m_vDir = vLook;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Walk_F);
		break;
	case Direction::B:
		m_vDir = vLook*-1;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Walk_B);
		break;
	case Direction::L:
		m_vDir = vRight*-1;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Walk_L);
		break;
	case Direction::R:
		m_vDir = vRight;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Walk_R);
		break;
	case Direction::FL:
		m_vDir = (vRight*-1)+vLook;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Walk_FL);
		break;
	case Direction::FR:
		m_vDir = vRight + vLook;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Walk_FR);
		break;
	case Direction::BL:
		m_vDir = (vRight*-1) + (vLook*-1);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Walk_BL);
		break;
	case Direction::BR:
		m_vDir = vRight + (vLook*-1);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Walk_BR);
		break;
	case Direction::NONE:
		m_State = PlayerState::IDLE;
		m_3State = Player3State::IDLE;
		return;
	default:
		break;
	}

	m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(m_vDir *fTimeDelta * 5.f)));

	if (Check_ObjectCollision())//0이 아니면 충돌
		m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(m_vDir *fTimeDelta * -5.f)));

}

void CPlayer::Run(const _float & fTimeDelta)
{
	if (m_fStamina < 0)
	{
		m_State = PlayerState::IDLE;
		m_3State = Player3State::IDLE;
		return;
	}
	m_fStamina -= 20.f* fTimeDelta;


	Direction::Dir Dir = Direction::NONE;
	m_vDir = {};
	_vec3 vPos, vLook, vRight;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
	{
		m_State = PlayerState::IDLE;
		m_3State = Player3State::IDLE;
		return;
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
	{
		m_State = PlayerState::FIRE;
		m_3State = Player3State::AIM;
		return;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8001)
	{
		m_State = PlayerState::AIMWALK;
		m_3State = Player3State::AIM;
		return;
	}

	if (GetAsyncKeyState('Q') & 0x8001)
	{
		m_State = PlayerState::BASH;
		m_3State = Player3State::MOVE;
		return;
	}


	if (GetAsyncKeyState('W') & 0x8001)
	{
		Dir = Direction::F;
		if (GetAsyncKeyState('A') & 0x8001)
			Dir = Direction::FL;
		if (GetAsyncKeyState('D') & 0x8001)
			Dir = Direction::FR;
	}

	if (GetAsyncKeyState('S') & 0x8001)
	{
		Dir = Direction::B;
		if (GetAsyncKeyState('A') & 0x8001)
			Dir = Direction::BL;
		if (GetAsyncKeyState('D') & 0x8001)
			Dir = Direction::BR;
	}

	if (GetAsyncKeyState('A') & 0x8001)
	{
		Dir = Direction::L;
		if (GetAsyncKeyState('W') & 0x8001)
			Dir = Direction::FL;
		if (GetAsyncKeyState('S') & 0x8001)
			Dir = Direction::BL;
	}

	if (GetAsyncKeyState('D') & 0x8001)
	{
		Dir = Direction::R;
		if (GetAsyncKeyState('W') & 0x8001)
			Dir = Direction::FR;
		if (GetAsyncKeyState('S') & 0x8001)
			Dir = Direction::BR;
	}

	switch (Dir)
	{
	case Direction::F:
		m_vDir = vLook;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Run_F);
		break;
	case Direction::B:
		m_vDir = vLook*-1;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Run_B);
		break;
	case Direction::L:
		m_vDir = vRight*-1;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Run_L);
		break;
	case Direction::R:
		m_vDir = vRight;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Run_R);
		break;
	case Direction::FL:
		m_vDir = (vRight*-1) + vLook;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Run_FL);
		break;
	case Direction::FR:
		m_vDir = vRight + vLook;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Run_FR);
		break;
	case Direction::BL:
		m_vDir = (vRight*-1) + (vLook*-1);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Run_BL);
		break;
	case Direction::BR:
		m_vDir = vRight + (vLook*-1);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Run_BR);
		break;
	case Direction::NONE:
		m_State = PlayerState::IDLE;
		return;
	default:
		break;
	}

	m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(m_vDir *fTimeDelta * 7.f)));

	//if (Check_ObjectCollision())//0이 아니면 충돌
	//	m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(m_vDir *fTimeDelta * -7.f)));
}

void CPlayer::AimStand(const _float & fTimeDelta)
{
	if (m_fStamina < 200.f)
		m_fStamina += 20.f* fTimeDelta;

	m_pMeshCom->Set_AnimationIndex(M4_Standing_CC);
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_State = PlayerState::FIRE;
		CSoundMgr::Get_Instance()->PlaySound(L"SA2_Styleset_Weapon_AR_M4A1_Fire_3p_Wav.ogg", CSoundMgr::PLAYER);
		m_3State = Player3State::AIM;
		return;
	}

	if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
	{
		m_State = PlayerState::IDLE;
		m_3State = Player3State::IDLE;
		return;
	}

	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_State = PlayerState::AIMWALK;
		return;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_State = PlayerState::AIMWALK;
		return;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_State = PlayerState::AIMWALK;
		return;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_State = PlayerState::AIMWALK;
		return;
	}
}

void CPlayer::AimWalk(const _float & fTimeDelta)
{
	if (m_fStamina < 200.f)
		m_fStamina += 20.f* fTimeDelta;

	Direction::Dir Dir = Direction::NONE;
	m_vDir = {};
	_vec3 vPos, vLook, vRight;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_State = PlayerState::FIRE;
		m_3State = Player3State::AIM;
		CSoundMgr::Get_Instance()->PlaySound(L"SA2_Styleset_Weapon_AR_M4A1_Fire_3p_Wav.ogg", CSoundMgr::PLAYER);
		return;
	}

	if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
	{
		m_State = PlayerState::IDLE;
		m_3State = Player3State::IDLE;
		return;
	}


	if (GetAsyncKeyState('Q') & 0x8001)
	{
		m_State - PlayerState::BASH;
		m_3State = Player3State::MOVE;
		return;
	}


	if (GetAsyncKeyState('W') & 0x8001)
	{
		Dir = Direction::F;
		if (GetAsyncKeyState('A') & 0x8001)
			Dir = Direction::FL;
		if (GetAsyncKeyState('D') & 0x8001)
			Dir = Direction::FR;
	}

	if (GetAsyncKeyState('S') & 0x8001)
	{
		Dir = Direction::B;
		if (GetAsyncKeyState('A') & 0x8001)
			Dir = Direction::BL;
		if (GetAsyncKeyState('D') & 0x8001)
			Dir = Direction::BR;
	}

	if (GetAsyncKeyState('A') & 0x8001)
	{
		Dir = Direction::L;
		if (GetAsyncKeyState('W') & 0x8001)
			Dir = Direction::FL;
		if (GetAsyncKeyState('S') & 0x8001)
			Dir = Direction::BL;
	}

	if (GetAsyncKeyState('D') & 0x8001)
	{
		Dir = Direction::R;
		if (GetAsyncKeyState('W') & 0x8001)
			Dir = Direction::FR;
		if (GetAsyncKeyState('S') & 0x8001)
			Dir = Direction::BR;
	}

	switch (Dir)
	{
	case Direction::F:
		m_vDir = vLook;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Standing_CC);
		break;
	case Direction::B:
		m_vDir = vLook*-1;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Standing_CC);
		break;
	case Direction::L:
		m_vDir = vRight*-1;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Standing_CC);
		break;
	case Direction::R:
		m_vDir = vRight;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Standing_CC);
		break;
	case Direction::FL:
		m_vDir = (vRight*-1) + vLook;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Standing_CC);
		break;
	case Direction::FR:
		m_vDir = vRight + vLook;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Standing_CC);
		break;
	case Direction::BL:
		m_vDir = (vRight*-1) + (vLook*-1);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Standing_CC);
		break;
	case Direction::BR:
		m_vDir = vRight + (vLook*-1);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pMeshCom->Set_AnimationIndex(M4_Standing_CC);
		break;
	case Direction::NONE:
		m_State = PlayerState::AIMStand;
		m_3State = Player3State::AIM;
		return;
	default:
		break;
	}

	m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(m_vDir *fTimeDelta * 3.f)));

	if (Check_ObjectCollision())//0이 아니면 충돌
		m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(m_vDir *fTimeDelta * -3.f)));
}


void CPlayer::Fire(const _float & fTimeDelta)
{
	if (m_fStamina < 200.f)
		m_fStamina += 20.f* fTimeDelta;

	if (m_pM4->Get_LoadedBullet() == 0)
	{
		m_State = PlayerState::RELOAD;
		m_3State = Player3State::IDLE;
		CSoundMgr::Get_Instance()->PlaySound(L"SA2_Weapon_AR_M4A1_Reload.ogg", CSoundMgr::PLAYER);
		return;
	}


	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_fFireTime += fTimeDelta;
		if (m_fFireTime > 0.06f)
		{
			m_pM4->Shot();
			HitCheck();
			m_fFireTime = 0.f;
			CSoundMgr::Get_Instance()->PlaySound(L"SA2_Styleset_Weapon_AR_M4A1_Fire_3p_Wav.ogg", CSoundMgr::PLAYER);
			m_pMeshCom->Set_AnimationIndexRepeat(M4_Fire);
			return;
		}
	}

	else
		m_fFireTime = 0.f;

	if(!(GetAsyncKeyState(VK_LBUTTON) & 0x8000) && (GetAsyncKeyState(VK_RBUTTON) & 0x8000) )
	{
		m_State = PlayerState::AIMStand;
		m_3State = Player3State::AIM;
		return;
	}

	m_pMeshCom->Set_AnimationIndex(M4_Fire);

	Direction::Dir Dir = Direction::NONE;
	m_vDir = {0.f,0.f,0.f};
	_vec3 vPos, vLook, vRight;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);


	if (GetAsyncKeyState('Q') & 0x8001)
	{
		m_State - PlayerState::BASH;
		return;
	}


	if (GetAsyncKeyState('W') & 0x8001)
	{
		Dir = Direction::F;
		if (GetAsyncKeyState('A') & 0x8001)
			Dir = Direction::FL;
		if (GetAsyncKeyState('D') & 0x8001)
			Dir = Direction::FR;
	}

	if (GetAsyncKeyState('S') & 0x8001)
	{
		Dir = Direction::B;
		if (GetAsyncKeyState('A') & 0x8001)
			Dir = Direction::BL;
		if (GetAsyncKeyState('D') & 0x8001)
			Dir = Direction::BR;
	}

	if (GetAsyncKeyState('A') & 0x8001)
	{
		Dir = Direction::L;
		if (GetAsyncKeyState('W') & 0x8001)
			Dir = Direction::FL;
		if (GetAsyncKeyState('S') & 0x8001)
			Dir = Direction::BL;
	}

	if (GetAsyncKeyState('D') & 0x8001)
	{
		Dir = Direction::R;
		if (GetAsyncKeyState('W') & 0x8001)
			Dir = Direction::FR;
		if (GetAsyncKeyState('S') & 0x8001)
			Dir = Direction::BR;
	}

	switch (Dir)
	{
	case Direction::F:
		m_vDir = vLook;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		break;
	case Direction::B:
		m_vDir = vLook*-1;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		break;
	case Direction::L:
		m_vDir = vRight*-1;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		break;
	case Direction::R:
		m_vDir = vRight;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		break;
	case Direction::FL:
		m_vDir = (vRight*-1) + vLook;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		break;
	case Direction::FR:
		m_vDir = vRight + vLook;
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		break;
	case Direction::BL:
		m_vDir = (vRight*-1) + (vLook*-1);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		break;
	case Direction::BR:
		m_vDir = vRight + (vLook*-1);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		break;
	case Direction::NONE:
		break;
	default:
		break;
	}

	m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(m_vDir *fTimeDelta * 3.f)));

	if (Check_ObjectCollision())//0이 아니면 충돌
		m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(m_vDir *fTimeDelta * -3.f)));


	if (m_pMeshCom->Is_AnimationsetFinish())
	{
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			m_pM4->Shot();
			HitCheck();
			m_State = PlayerState::AIMStand;
			m_3State = Player3State::AIM;
			return;
		}
		m_pM4->Shot();
		HitCheck();
		m_State = PlayerState::IDLE;
		m_3State = Player3State::IDLE;
	}

}


void CPlayer::Reload(const _float & fTimeDelta)
{
	if(m_fStamina < 200.f)
		m_fStamina += 20.f* fTimeDelta;
	

	m_pMeshCom->Set_AnimationIndex(M4_Reload);

	if (true == m_pMeshCom->Is_AnimationsetFinish())
	{
		m_pM4->ReLoad();
		m_State = PlayerState::IDLE;
		m_3State = Player3State::IDLE;
	}
}

void CPlayer::Bash(const _float & fTimeDelta)
{
	m_pMeshCom->Set_AnimationIndex(M4_Bash);

	if (true == m_pMeshCom->Is_AnimationsetFinish())
	{
		m_State = PlayerState::IDLE;
		m_3State = Player3State::IDLE;
	}
}

void CPlayer::RotatePlayer(_float fDegree)
{
	_vec3 vRight;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

}

_bool CPlayer::HitCheck()
{
	list<CGameObject*>* pMonsterlst = Get_List(GAMELOGIC, L"Monster");
	if (pMonsterlst == nullptr)
		return false;
	list<CMonster*> Monsterlst = *((list<CMonster*>*)pMonsterlst);
	for (CMonster* pMon : Monsterlst)
	{
		CSphere* pSphere = pMon->Get_Sphere();
		//CTransform* pTransform = dynamic_cast<CTransform*>(pSphere->Get_Component(L"Com_Transform", ID_DYNAMIC));
		if (m_pCalculatorCom->raySphrerIntersection(g_hWnd, pSphere->Get_Radius()/100.f , pMon->Get_SphereTransform())) //충돌체크 
		{
			_vec3 temp = {};
			pMon->Get_SphereTransform()->Get_Info(INFO_POS, &temp);
			_vec3 CameraAt = dynamic_cast<CDynamicCamera*>(Get_List(CAMERA, L"DynamicCamera")->front())->Get_At();
			_vec3 CameraEye = dynamic_cast<CDynamicCamera*>(Get_List(CAMERA, L"DynamicCamera")->front())->Get_Eye();
			//_vec3 tempDir = CameraEye - temp;
			_vec3 tempDir = CameraAt - CameraEye;
			D3DXVec3Normalize(&tempDir, &tempDir);
			_float fDist = sqrtf(powf(temp.x - CameraEye.x, 2) + powf(temp.y - CameraEye.y, 2) + powf(temp.z - CameraEye.z, 2));

			_vec3 EffectPos = CameraEye + fDist*tempDir - (pSphere->Get_Radius() / 100.f)*tempDir;
			CEffect::Create(m_pGraphicDev, &EffectPos);
			pMon->Damaged();
			return true;
		}
	}

	return false;
}



HRESULT CPlayer::Add_Component(void)
{
	CComponent*			pComponent = nullptr;




	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);

	//NaviMap
	CTerrain* pTerrain = (CTerrain*)Engine::Get_List(ENVIRONMENT, L"Terrain")->front();
	m_pNaviCom = (CNaviMesh*)pTerrain->Get_Component(L"Com_Navi", ID_STATIC);


	// Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<COptimization*>(Engine::Clone_Proto(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Optimization", pComponent);



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

	m_pShprer = CSphere::Create(m_pGraphicDev, 30.f);
	m_pShprer->Set_Height(1.f);
	m_pShprerTransCom = (CTransform*)m_pShprer->Get_Component(L"Com_Transform", ID_DYNAMIC);


	m_pM4 = CWeapon::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pM4, E_FAIL);
	Engine::Get_Scene()->Add_GameObject(WEAPON, L"M4", m_pM4);
	
	return S_OK;
}



HRESULT CPlayer::Select_ProtoMesh(const _tchar * pObjProtoName)
{
	CComponent*			pComponent = nullptr;
	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Engine::Clone_Proto(pObjProtoName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);



	return S_OK;
}

_float CPlayer::Check_ObjectCollision()
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

		if (m_pCalculatorCom->Collision_OBB(m_pDynamicColliderCom->Get_Min(), m_pDynamicColliderCom->Get_Max(), m_pDynamicColliderCom->Get_Center(), m_pDynamicColliderCom->Get_MaxDir(),
			m_pDynamicColliderCom->Get_ColliderWorld(),
			pCollider->Get_Min(), pCollider->Get_Max(), pCollider->Get_Center(), pCollider->Get_MaxDir(),
			pCollider->Get_CollWorldMatrix(),
			&fPushDist
		))
		{
			if (GetAsyncKeyState('E') & 0x0001)
			{
				CObjAnime::GetInstance()->Classify_Obj(pObj);
			}
			return fPushDist;
		}
	}
	return 0;
}

_int CPlayer::Collision_Trigger()
{
	list<CGameObject*> pTerrainlst = *Engine::Get_List(ENVIRONMENT, L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainlst.front());
	vector<CTrigger*> pTriggerlst = pTerrain->Get_vecTrigger();
	for (auto& pTrigger : pTriggerlst)
	{
		if (!pTrigger->Get_TriggerOn())
			continue;
		if (m_pCalculatorCom->Collision_Sphere(m_pShprerTransCom, m_pShprer->Get_Radius() / 100.f, (CTransform*)pTrigger->Get_Component(L"Com_Transform", ID_DYNAMIC), pTrigger->Get_Radius()))
		{
			CTriggerFunc::GetInstance()->Classify_Trigger(pTrigger->Get_TriggerNum());
			pTrigger->Set_TriggetOn(false);
			return pTrigger->Get_TriggerNum();
		}

	}
	return -1;
}

void CPlayer::Update_StatBar()
{
	_float fHPPer = m_iHP / 100.f;
	_float fStaminaPer = m_fStamina / 200.f;

	m_pHPBar->Set_XSize(151 * fHPPer);
	m_pHPBar->Set_X(275 - (m_pHPBar->Get_XSize()-151)/2);

	m_pStaminaBar->Set_XSize(151 * fStaminaPer);
	m_pStaminaBar->Set_X(525 + (m_pStaminaBar->Get_XSize() - 151)/2);
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

