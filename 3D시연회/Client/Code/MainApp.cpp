#include "stdafx.h"
#include "MainApp.h"
#include "Logo.h"
#include "SoundMgr.h"
#include "TriggerFunc.h"
#include "ObjAnime.h"
CMainApp::CMainApp(void)
{
	ShowCursor(false);
}

CMainApp::~CMainApp(void)
{

}

HRESULT CMainApp::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);
	CTriggerFunc::GetInstance()->Set_GraphicDev(m_pGraphicDev);
	CObjAnime::GetInstance()->Set_GraphicDev(m_pGraphicDev);
	CSoundMgr::Get_Instance()->Initialize();
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{
	if (nullptr == m_pManagementClass)
		return -1;

	Update_InputDev();


	/*_long			MouseMove = 0;

	if (MouseMove = Get_DIMouseMove(DIMS_Z))
	{
		int a = 0;
	}*/
	CObjAnime::GetInstance()->Update_ObjAnime(fTimeDelta);

	m_pManagementClass->Update_Scene(fTimeDelta);

	return 0;
}

void CMainApp::Render_MainApp(void)
{
	if (nullptr == m_pManagementClass)
		return;

	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
		
	m_pManagementClass->Render_Scene(m_pGraphicDev);


	m_pDeviceClass->Render_End();
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 *ppGraphicDev)
{
	FAILED_CHECK_RETURN(Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);

	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_Device();
	(*ppGraphicDev)->AddRef();

	Ready_Renderer(*ppGraphicDev);
	// 폰트

	FAILED_CHECK_RETURN(Ready_Font((*ppGraphicDev), L"Font_Default", L"바탕", 15, 20, FW_NORMAL), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Font((*ppGraphicDev), L"Font_Jinji", L"궁서", 15, 15, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Font((*ppGraphicDev), L"Font_UI", L"비트로 코어 OTF", 15, 15, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Font((*ppGraphicDev), L"Font_UI_M", L"비트로 코어 OTF", 20, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Font((*ppGraphicDev), L"Font_UI_L", L"비트로 코어 OTF", 50, 50, FW_HEAVY), E_FAIL);


	// InputDev

	FAILED_CHECK_RETURN(Ready_InputDev(g_hInst, g_hWnd), E_FAIL);



	return S_OK;
}



HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, CManagement ** ppManagementClass)
{
	CScene*		pScene = nullptr;

	FAILED_CHECK_RETURN(Create_Management(pGraphicDev, ppManagementClass), E_FAIL);

	(*ppManagementClass)->AddRef();
		
	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	if (FAILED((*ppManagementClass)->Set_Scene(pScene)))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		delete pInstance;
		pInstance = NULL;
	}

	return pInstance;
}

void CMainApp::Free(void)
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pManagementClass);
	
	Release_Utility();
	Release_System();

	CTriggerFunc::DestroyInstance();
}






