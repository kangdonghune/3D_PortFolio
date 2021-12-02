#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement(void)
	: m_pScene(nullptr)
{

}

Engine::CManagement::~CManagement(void)
{
	Free();
}

list<CGameObject*>* CManagement::Get_List(Layer type, const _tchar * pObjTag)
{
	return m_pScene->Get_List(type, pObjTag);
}

void CManagement::Clear_List(Layer type, const _tchar * pObjTag)
{
	return m_pScene->Clear_List(type, pObjTag);
}

HRESULT CManagement::Add_GameObject(Layer type, const _tchar * pObjTag, CGameObject * pInstance)
{
	return m_pScene->Add_GameObject(type, pObjTag, pInstance);
}

HRESULT Engine::CManagement::Ready_Shader(LPDIRECT3DDEVICE9& pGraphicDev)
{
	D3DVIEWPORT9	ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Albedo", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL); //고유 색
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Nomal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL); //법선 정보를 텍스쳐형태로 추출
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Shade", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);// 노말 타겟이랑 빛의 정보를 저장

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred", L"Target_Albedo"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Albedo", 0.f, 0.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred", L"Target_Nomal"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Nomal", 0.f, 200.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_LightAcc", L"Target_Shade"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Shade", 200.f, 0.f, 200.f, 200.f), E_FAIL);

	CShader*		pShader = nullptr;

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Sample.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Sample", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Terrain.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Terrain", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Mesh.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Mesh", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Effect.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Effect", pShader), E_FAIL);

	return S_OK;
}

HRESULT Engine::CManagement::Set_Scene(CScene* pScene)
{
	Safe_Release(m_pScene);	// 주의합시다!!!!!!!!!!!!
	Clear_RenderGroup();	// 기존에 그리고 있던 오브젝트 그룹을 지운다.

	m_pScene = pScene;

	return S_OK;
}

Engine::_int Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	m_pScene->Update_Scene(fTimeDelta);

	return 0;
}

void Engine::CManagement::Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Render_GameObject(pGraphicDev);

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

void Engine::CManagement::Free(void)
{
	Safe_Release(m_pScene);
}

