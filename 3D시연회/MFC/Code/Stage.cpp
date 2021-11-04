#include "stdafx.h"
#include "Stage.h"

#include "Terrain.h"
#include "DynamicCamera.h"

#include "Player.h"


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
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Player_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Weapon_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Camera_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Sphere_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Effect_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Trigger_Layer(), E_FAIL);
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

	////// DynamicCamera
	//pGameObject = CDynamicCamera::Create(m_pGraphicDev, 
	//									&_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), 
	//									D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Add_GameObject(ENVIRONMENT,L"DynamicCamera", pGameObject), E_FAIL);

	

	//// SkyBox
	//pGameObject = CSkyBox::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	m_mapLayer[ENVIRONMENT] = pLayer;

	return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer()
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;



	m_mapLayer[GAMELOGIC] = pLayer;

	return S_OK;
}

HRESULT CStage::Ready_Player_Layer()
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	m_mapLayer[PLAYER] = pLayer;

	return S_OK;
}


HRESULT CStage::Ready_Weapon_Layer()
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	m_mapLayer[WEAPON] = pLayer;

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

HRESULT CStage::Ready_Camera_Layer()
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	CGameObject*			pGameObject = nullptr;
	//// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(30.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	m_mapLayer[CAMERA] = pLayer;

	return S_OK;
}

HRESULT CStage::Ready_Sphere_Layer()
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	m_mapLayer[SPHERE] = pLayer;

	return S_OK;
}

HRESULT CStage::Ready_Effect_Layer()
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	m_mapLayer[EFFECT] = pLayer;

	return S_OK;
}

HRESULT CStage::Ready_Trigger_Layer()
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	m_mapLayer[TRIGGER] = pLayer;

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
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Buffer_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Buffer_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	// 텍스쳐
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Terrain/Terrain.dds", TYPE_NORMAL, 2)), E_FAIL);

	// 메쉬
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Sphrer", CSphrerMeshs::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Navi", CNaviMesh::Create(m_pGraphicDev)), E_FAIL);
	//플레이어
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Hunter", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Dynamicmesh/Player/Hunter/", L"Hunter.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_AK47", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Weapon/", L"Revolverx.X")), E_FAIL);
	
	//몬스터
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Goblin", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Dynamicmesh/Monster/Goblin/", L"Goblin.X")), E_FAIL);

	//오브젝트 빌딩
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Brown", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Building/House/HouseBrown/", L"HouseBrown.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Burn", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Building/House/HouseBurn/", L"HouseBurn.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Church", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Building/Church/", L"Church.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Factory", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Building/Factory/", L"factory.X")), E_FAIL);wddsssssssssssssssssssssssssssssssssssssssssx
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_GeneratorBody", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Stuff/Generator/", L"Generator_Body.X")), E_FAIL);


	//스터프
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Sedan", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Stuff/Vehicles/", L"Sedan.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_ChurchDoorL", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Stuff/Door/", L"Church_DoorL.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_ChurchDoorR", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Stuff/Door/", L"Church_DoorR.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_FinalDoor", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Stuff/Door/", L"FinalDoor.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_GeneratorAxle", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Stuff/Generator/", L"Generator_Axle.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Cable", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Stuff/Cable/", L"Cable.X")), E_FAIL);



	////벽
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Wall", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Staticmesh/Stuff/Wall/", L"Wall.X")), E_FAIL);




	// 기타 등등
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Optimization", COptimization::Create(m_pGraphicDev, true, VTXCNTX, VTXCNTZ)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);

	return S_OK;
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
	CScene::Free();
}
