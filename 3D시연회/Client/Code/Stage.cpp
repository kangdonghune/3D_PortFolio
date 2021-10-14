#include "stdafx.h"
#include "Stage.h"

#include "Terrain.h"
#include "DynamicCamera.h"

#include "Player.h"
#include "Monster.h"
#include "Object.h"
#include "Weapon.h"
#include "Building.h"
#include "SkyBox.h"
#include "DynamicCamera.h"

#include "Export_Function.h"



CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CStage::~CStage(void)
{

}

HRESULT CStage::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Weapon_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Camera_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	

	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);



	return S_OK;
}

Engine::_int CStage::Update_Scene(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;

	return CScene::Update_Scene(fTimeDelta);
}

void CStage::Render_Scene(void)
{
	// debug 용
	++m_dwRenderCnt;

	if (m_fTime >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
	}
	
	Render_Font(L"Font_Jinji", m_szFPS, &_vec2(400.f, 20.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));


}

HRESULT CStage::Ready_Environment_Layer()
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	m_mapLayer[ENVIRONMENT] = pLayer;

	return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer()
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	m_mapLayer[GAMELOGIC] = pLayer;
	return S_OK;
}

HRESULT CStage::Ready_UI_Layer()
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;


	//// UI
	//pGameObject = CUI::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", pGameObject), E_FAIL);

	m_mapLayer[UI_LAYER] = pLayer;
	return S_OK;
}

HRESULT CStage::Ready_Weapon_Layer()
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	//pGameObject = CWeapon::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Weapon", pGameObject), E_FAIL);

	m_mapLayer[WEAPON] = pLayer;
	return S_OK;
}

HRESULT CStage::Ready_Camera_Layer()
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;
	//// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	m_mapLayer[CAMERA] = pLayer;
	return S_OK;
}

HRESULT CStage::Ready_LightInfo(void)
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CStage::Ready_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{

	return S_OK;
}

HRESULT CStage::Load_Data()
{
	FAILED_CHECK_RETURN(Load_NaviMesh(L"../../Resource/Data/NaviMesh/MFCCell.dat"), E_FAIL);
	FAILED_CHECK_RETURN(Load_Player(L"../../Resource/Data/Unit/Player.dat"),E_FAIL);
	FAILED_CHECK_RETURN(Load_Monster(L"../../Resource/Data/Unit/Monster.dat"), E_FAIL);
	FAILED_CHECK_RETURN(Load_Building(L"../../Resource/Data/Object/Building.dat"), E_FAIL);
	FAILED_CHECK_RETURN(Load_Stuff(L"../../Resource/Data/Object/Stuff.dat"), E_FAIL);
	
	Connect_CameraToPlayer();

	CGameObject*			pGameObject = nullptr;

	pGameObject = CWeapon::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::Get_Scene()->Add_GameObject(WEAPON, L"Weapon", pGameObject);
	return S_OK;
}

HRESULT CStage::Load_Player(const _tchar * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	//리스트 초기화 진행 
	Clear_List(GAMELOGIC, L"Player");
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
		pObj = Create_Unit(GAMELOGIC, L"Player", wstrNametag.c_str());
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

HRESULT CStage::Load_Monster(const _tchar * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	//리스트 초기화 진행 
	Clear_List(GAMELOGIC, L"Monster");
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

HRESULT CStage::Load_Building(const _tchar * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	//리스트 초기화 진행 
	Clear_List(GAMELOGIC, L"Building");
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
		pObj = Create_Object(GAMELOGIC, L"Building", wstrNametag.c_str());
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

HRESULT CStage::Load_Stuff(const _tchar * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	//리스트 초기화 진행 
	Clear_List(GAMELOGIC, L"Object");
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
		pObj = Create_Object(GAMELOGIC, L"Object", wstrNametag.c_str());
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

HRESULT CStage::Load_NaviMesh(const _tchar * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	//리스트 초기화 진행 

	DWORD dwByte = 0;
	DWORD dwStringCount = 0;
	TCHAR* szBuf = nullptr;
	list<CGameObject*> pTerrainlst = Engine::Get_List(GAMELOGIC, L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainlst.front());
	for (int i = 0; i < pTerrain->Get_vecCell().size(); i++)
	{
		pTerrain->Get_vecCell()[i]->Set_Dead(true); // Set_Dead하면서 컴퍼넌트에 저장된 것들도 지워주는 예약.
	}
	pTerrain->Get_vecCell().clear();// 셀 벡터 초기화;
	CNaviMesh* pNavi = (CNaviMesh*)pTerrain->Get_Component(L"Com_Navi", ID_STATIC);
	pNavi->Delete_Cell();// 컴퍼넌트에 저장된 셀 정보 초기화;

	_ulong dwIndex = 0;


	//로드 순서 1.pCell 정보 불러들여온다. 2.cell 생성 후 MFC Cell에 생성 터레인 셀 벡터에 추가, 3. 컴퍼넌트에 cell만 추가 
	while (true)
	{
		CCell* pCell = nullptr;
		_vec3 SavePointAPos = {};
		_vec3 SavePointBPos = {};
		_vec3 SavePointCPos = {};
		_int  PointA = 0;
		_int  PointB = 0;
		_int  PointC = 0;
		ReadFile(hFile, &dwIndex, sizeof(_ulong), &dwByte, nullptr);
		if (0 == dwByte)
			break;//읽어올 게 없으면 종료
		ReadFile(hFile, &SavePointAPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &SavePointBPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &SavePointCPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &PointA, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &PointB, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &PointC, sizeof(_int), &dwByte, nullptr);


		pCell = CCell::Create(m_pGraphicDev, dwIndex, &SavePointAPos, &SavePointBPos, &SavePointCPos);
		pTerrain->Get_vecCell().push_back(pCell);
	}
	pTerrain->SetUp_NaviMesh();// 벡터에 셀 다 추가 되었으면 한번에 추가
	CloseHandle(hFile);
	return S_OK;
}

HRESULT CStage::Connect_CameraToPlayer()
{
	CDynamicCamera* pCamera =  (CDynamicCamera*)Get_List(CAMERA, L"DynamicCamera").front();
	CPlayer* pPlayer = (CPlayer*)Get_List(GAMELOGIC, L"Player").front();
	pCamera->Set_Target(pPlayer);
	return S_OK;
}

HRESULT CStage::DisConnect_CameraToPlayer()
{
	CDynamicCamera* pCamera = (CDynamicCamera*)Get_List(CAMERA, L"DynamicCamera").front();
	pCamera->Release_Target();
	return S_OK;
}

CGameObject* CStage::Create_Unit(Layer type, const _tchar * pParentName, const _tchar * pObjProtoName)
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

CGameObject* CStage::Create_Object(Layer type, const _tchar * pParentName, const _tchar * pObjProtoName)
{
	CGameObject* pGameObject = nullptr;
	if (!lstrcmp(L"Building", pParentName))
	{
		pGameObject = CBuilding::Create(m_pGraphicDev, pObjProtoName);
		Engine::Get_Scene()->Add_GameObject(type, L"Building", pGameObject);
		return pGameObject;
	}

	if (!lstrcmp(L"Object", pParentName))
	{
		pGameObject = C_Object::Create(m_pGraphicDev, pObjProtoName);
		Engine::Get_Scene()->Add_GameObject(type, L"Object", pGameObject);
		return pGameObject;
	}
	if (pGameObject == nullptr)
		return nullptr;
	return nullptr;
}


CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStage::Free(void)
{
	DisConnect_CameraToPlayer();
	CScene::Free();
}
