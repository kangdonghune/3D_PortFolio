#include "stdafx.h"

#include "TriggerFunc.h"
#include "DynamicCamera.h"
#include "ObjAnime.h"
#include "Player.h"
#include "Monster.h"
#include "Export_Function.h"


IMPLEMENT_SINGLETON(CTriggerFunc)
CTriggerFunc::CTriggerFunc()
{

}

CTriggerFunc::~CTriggerFunc(void)
{
	Safe_Release(m_pGraphicDev);
}


void CTriggerFunc::Render_Text()
{
	switch (m_iTextNum)
	{
	case 0:
		Render_Font(L"Font_UI_M", L"미션: 공장으로 이동하라", &_vec2(WINCX/2 -100.f, 75.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		break;
	case 1:
		_tchar szCount[256];
		wsprintf(szCount, L"미션: 코일 수집 %d / 3", CObjAnime::GetInstance()->Get_CableCount());
		Render_Font(L"Font_UI_M", szCount, &_vec2(WINCX / 2 - 100.f, 75.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		break;
	case 2:
		Render_Font(L"Font_UI_L", L"탈 출 성 공", &_vec2(WINCX / 2 - 150.f, 300.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		break;
	default:
		break;
	}

	//	
	//	
}

void CTriggerFunc::Classify_Trigger(_int index)
{
	switch (index)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		Spawn_Monster(index);
		break;
	case 4:
		Move_Camera();
		break;
	default:
		break;
	}
}

HRESULT CTriggerFunc::Spawn_Monster(_int i)
{
	_tchar szPath[256];
	wsprintf(szPath, L"../../Resource/Data/Unit/MonsterTrigger%d.dat", i);
	FAILED_CHECK_RETURN(Load_Monster(szPath), E_FAIL);
}

void CTriggerFunc::Move_Camera()
{
	m_iTextNum = 1;
	CDynamicCamera* pCamera =dynamic_cast<CDynamicCamera*>(Get_List(CAMERA, L"DynamicCamera")->front());
	pCamera->m_bMode = true;

}


HRESULT CTriggerFunc::Load_Monster(const _tchar * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	//리스트 초기화 진행 

	DWORD dwByte = 0;
	DWORD dwStringCount = 0;
	TCHAR* szBuf = nullptr;


	//로드 순서 1. 네임태그 2. pos 3. rotate 4. scale
	while (true)
	{
		CGameObject* pObj = nullptr;
		wstring wstrNametag;
		_vec3   LoadPos = {};
		_vec3	LoadRot = {};
		_vec3	LoadScale = {};
		ReadFile(hFile, &dwStringCount, sizeof(DWORD), &dwByte, nullptr);
		if (0 == dwByte)
			break;//읽어올 게 없으면 종료
		szBuf = new TCHAR[dwStringCount];
		ReadFile(hFile, szBuf, dwStringCount, &dwByte, nullptr);
		wstrNametag = szBuf;
		Safe_Delete_Array(szBuf);
		ReadFile(hFile, &LoadPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &LoadRot, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &LoadScale, sizeof(_vec3), &dwByte, nullptr);
		pObj = Create_Unit(GAMELOGIC, L"Monster", wstrNametag.c_str());
		CTransform* pTransCom = (CTransform*)pObj->Get_Component(L"Com_Transform", ID_DYNAMIC);
		pTransCom->Set_Pos(&LoadPos);
		pTransCom->Rotation2(ROT_X, LoadRot.x);
		pTransCom->Rotation2(ROT_Y, LoadRot.y);
		pTransCom->Rotation2(ROT_Z, LoadRot.z);
		pTransCom->Set_Scale(LoadScale.x, LoadScale.y, LoadScale.z);
	}
	CloseHandle(hFile);
	return S_OK;
}


CGameObject* CTriggerFunc::Create_Unit(Layer type, const _tchar * pParentName, const _tchar * pObjProtoName)
{
	CGameObject* pGameObject = nullptr;
	if (!lstrcmp(L"Player", pParentName))
	{
		pGameObject = CPlayer::Create(m_pGraphicDev, pObjProtoName);
		Engine::Get_Scene()->Add_GameObject(type, L"Player", pGameObject);
		return pGameObject;
	}

	if (!lstrcmp(L"Monster", pParentName))
	{
		pGameObject = CMonster::Create(m_pGraphicDev, pObjProtoName);
		Engine::Get_Scene()->Add_GameObject(type, L"Monster", pGameObject);
		return pGameObject;
	}



	if (pGameObject == nullptr)
		return nullptr;
	return nullptr;
}


