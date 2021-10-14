#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
}

CLoading::~CLoading(void)
{

}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eID = eID;

	return S_OK;
}

Engine::_uint CLoading::LoadingForStage(void)
{
	lstrcpy(m_szLoading, L"Texture Loading.................");

	//버퍼
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Buffer_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Buffer_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	// 텍스쳐
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Terrain/Terrain.dds", TYPE_NORMAL, 2)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Effect", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", TYPE_NORMAL, 90)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_UI", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/hpbar.png", TYPE_NORMAL, 1)), E_FAIL);

	// 메쉬
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Navi", CNaviMesh::Create(m_pGraphicDev)), E_FAIL);
	//플레이어
	
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Hunter", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Dynamicmesh/Player/", L"Player.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_AK47", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Weapon/", L"Sword.X")), E_FAIL);


	//몬스터
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Goblin", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Dynamicmesh/Monster/Goblin/", L"GoblinBlackSmith.X")), E_FAIL);

	//오브젝트
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Train", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Stuff/Train/", L"Train.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_TrainRightDoor", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Stuff/Train/", L"TrainRightDoor.X")), E_FAIL);

	// 기타 등등
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Optimization", COptimization::Create(m_pGraphicDev, true, VTXCNTX, VTXCNTZ)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/SkyBox/burger0.dds", TYPE_CUBE, 1)), E_FAIL);



	m_bFinish = true;

	lstrcpy(m_szLoading, L"Loading Complete!!!!!!!!!!!!!!!!!!!!!!");


	return 0;
}

Engine::_uint CLoading::LoadingForBoss(void)
{
	// 수업상 정의한 코드

	return 0;
}

unsigned int CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint	iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->LoadingForStage();
		break;

	case LOADING_BOSS:
		iFlag = pLoading->LoadingForBoss();
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	// _endthreadex(0);		리턴값이 0인 경우엔 _endthreadex함수가 자동 호출되어 문제가 없지만 그렇지 않은 경우에는 예외처리로 이 함수를 호출할 수 있도록 설계해야 한다.
	
	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);

}

