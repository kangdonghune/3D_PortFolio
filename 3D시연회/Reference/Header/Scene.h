#ifndef Scene_h__
#define Scene_h__

#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene(void);

public:
	list<CGameObject*>*		Get_List(Layer type, const _tchar* pObjTag);
	void					Clear_List(Layer type, const _tchar* pObjTag);
	HRESULT					Add_GameObject(Layer type, const _tchar* pObjTag, CGameObject* pInstance);

public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void) = 0;

protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	CLayer*							m_mapLayer[Layer::LAYER_END];

public:
	virtual void Free(void);
};

END
#endif // Scene_h__
