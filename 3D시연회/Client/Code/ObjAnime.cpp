#include "stdafx.h"
#include "UI.h"
#include "ObjAnime.h"
#include "TriggerFunc.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "SoundMgr.h"
#include "Export_Function.h"


IMPLEMENT_SINGLETON(CObjAnime)
CObjAnime::CObjAnime()
{

}

CObjAnime::~CObjAnime(void)
{
	
}

void CObjAnime::Update_ObjAnime(_float fTimedelta)
{

	Cable_Anime();
	ChurchDoorL_Anmie(fTimedelta);
	ChurchDoorR_Anime(fTimedelta);
	Generator_Anime(fTimedelta);
}

void CObjAnime::Render_ObjAnime()
{

}

void CObjAnime::Classify_Obj(C_Object * pObj)
{
	if (pObj->Get_NameTag() == L"Proto_Mesh_Cable")
	{
		m_bCableAni = true;
		m_pCable = pObj;
	}
	if (pObj->Get_NameTag() == L"Proto_Mesh_ChurchDoorL")
	{
		m_bChurchDoorLAni = true;
		m_pChurchDoorL = pObj;
	}
	if (pObj->Get_NameTag() == L"Proto_Mesh_ChurchDoorR")
	{
		m_bChurchDoorRAni = true;
		m_pChurchDoorR = pObj;
	}
	if (pObj->Get_NameTag() == L"Proto_Mesh_GeneratorAxle")
	{
		m_bGeneratorAni = true;
		m_pGenerator = pObj;
		m_bTouch = true;
	}
}



void CObjAnime::Cable_Anime()
{
	if (!m_bCableAni)
		return;
	if (m_pCable == nullptr)
		return;
	if (m_bCable)
		return;

	CGameObject*			pGameObject = nullptr;

	m_pEffect = pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_CableIcon", 100, 40, 45, 45);
	Engine::Get_Scene()->Add_GameObject(EFFECT,L"CableIcon", pGameObject);
	m_pCable->Set_Dead(true);
	m_bCableAni = false;
	CSoundMgr::Get_Instance()->PlaySound(L"FoleyBody_HandSwipe03.ogg", CSoundMgr::EFFECT);
	m_bCable = true;
	
}

void CObjAnime::ChurchDoorL_Anmie(_float fTimedelta)
{
	if (!m_bChurchDoorLAni)
		return;
	if (m_pChurchDoorL == nullptr)
		return;

	CTransform* pTransform = (CTransform*)m_pChurchDoorL->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_float fAngle = 50.f * fTimedelta;
	m_fDoorLAngle += fAngle;
	pTransform->Rotation(ROT_Y, -D3DXToRadian(fAngle));
	if (m_fDoorLAngle >= 90.f)
	{
		m_fDoorLAngle = 0.f;
		m_bChurchDoorLAni = false;
		return;
	}
}

void CObjAnime::ChurchDoorR_Anime(_float fTimedelta)
{
	if (!m_bChurchDoorRAni)
		return;

	if (m_pChurchDoorR == nullptr)
		return;

	CTransform* pTransform = (CTransform*)m_pChurchDoorR->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_float fAngle = 50.f * fTimedelta;
	m_fDoorRAngle += fAngle;
	pTransform->Rotation(ROT_Y, D3DXToRadian(fAngle));
	if (m_fDoorRAngle >= 90.f)
	{
		m_fDoorRAngle = 0.f;
		m_bChurchDoorRAni = false;
		return;
	}
}

void CObjAnime::Generator_Anime(_float fTimedelta)
{

	if (m_pGenerator == nullptr)
		return;
	if (m_bCable && m_bTouch)
	{
		m_iCableCount++;
		m_bCable = false;
		m_pEffect->Set_Dead(true);
		m_bTouch = false;
	}

	_float fAngle = 70.f * fTimedelta;
	CTransform* pTransform = dynamic_cast<CTransform*>(m_pGenerator->Get_Component(L"Com_Transform", ID_DYNAMIC));
	pTransform->Rotation(ROT_Z, D3DXToRadian(fAngle*m_iCableCount));

	if (m_iCableCount >= 3)
	{
		if (m_pFinalDoor == nullptr)
		{
			list<CGameObject*> pObjlst = *Get_List(GAMELOGIC, L"Object");
			for (auto& pObj : pObjlst)
			{
				if (pObj->Get_NameTag() == L"Proto_Mesh_FinalDoor")
				{
					m_pFinalDoor = pObj;
				}
			}
		}

		CTriggerFunc::GetInstance()->Set_TextNum(2);
		CTransform* pTrans = dynamic_cast<CTransform*>(m_pFinalDoor->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_float fPush = 3.f * fTimedelta;
		_vec3 DoorPos = {};
		pTrans->Get_Info(INFO_POS, &DoorPos);
		DoorPos.y += fPush;
		pTrans->Set_Pos(&DoorPos);

	}


}



