#ifndef Management_h__
#define Management_h__


#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement(void);
	virtual ~CManagement(void);

public:
	list<CGameObject*>		Get_List(const _tchar* pLayerTag, const _tchar* pObjTag);
	void					Clear_List(const _tchar* pLayerTag, const _tchar* pObjTag);

public:
	CScene*			Get_Scene() { return m_pScene; }
	HRESULT			Set_Scene(CScene* pScene);
	HRESULT			Ready_Shader(LPDIRECT3DDEVICE9& pGraphicDev);
	_int			Update_Scene(const _float& fTimeDelta);
	void			Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev);

private:
	CScene*			m_pScene;

public:
	virtual void Free(void);

};

END
#endif // Management_h__
