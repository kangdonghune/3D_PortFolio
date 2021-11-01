HRESULT			Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	FAILED_CHECK_RETURN(pManagement->Ready_Shader(pGraphicDev), E_FAIL);

	*ppManagement = pManagement;

	return S_OK;
}

HRESULT			Set_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}
inline CScene* const	Get_Scene()
{
	return CManagement::GetInstance()->Get_Scene();
}

_int				Update_Scene(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}
void				Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
}

list<CGameObject*>*		Get_List(Layer type, const _tchar* pObjTag)
{
	return CManagement::GetInstance()->Get_List(type, pObjTag);
}

void					Clear_List(Layer type, const _tchar* pObjTag)
{
	return CManagement::GetInstance()->Clear_List(type, pObjTag);
}

HRESULT					Add_GameObject(Layer type, const _tchar* pObjTag, CGameObject* pInstance)
{
	return CManagement::GetInstance()->Add_GameObject(type, pObjTag, pInstance);
}

HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent)
{
	return CProtoMgr::GetInstance()->Ready_Proto(pProtoTag, pComponent);
}
CComponent*		Clone_Proto(const _tchar* pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Proto(pProtoTag);
}

CRenderer*	Get_Renderer(void)
{
	return CRenderer::GetInstance();
}
void			Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eID, pGameObject);
}
void			Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void			Clear_RenderGroup(void)
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}
HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

const D3DLIGHT9*		Get_Light(const _ulong& iIndex)
{
	return CLightMgr::GetInstance()->Get_Light(iIndex);
}

void				Release_Utility(void)
{
	CLightMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}