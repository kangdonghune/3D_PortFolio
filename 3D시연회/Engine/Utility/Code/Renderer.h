#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Define.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer(void);
	virtual ~CRenderer(void);

public:
	HRESULT			Ready_Renderer(LPDIRECT3DDEVICE9& pGraphicDev);

	void			Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
	void			Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
	void			Clear_RenderGroup(void);

	void			Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void			Render_Nonalpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void			Render_NonSpecular(LPDIRECT3DDEVICE9& pGraphicDev);
	void			Render_Specular(LPDIRECT3DDEVICE9& pGraphicDev);
	void			Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void			Render_UI(LPDIRECT3DDEVICE9& pGraphicDev);
	void			Render_Debug(LPDIRECT3DDEVICE9& pGraphicDev);

	void			Render_Deferred(LPDIRECT3DDEVICE9& pGraphicDev);
	void			Render_LightAcc(LPDIRECT3DDEVICE9& pGraphicDev);
	void			Render_Blend(LPDIRECT3DDEVICE9& pGraphicDev);
	void			Render_BlendSpecular(LPDIRECT3DDEVICE9& pGraphicDev);

private:
	list<CGameObject*>				m_RenderGroup[RENDER_END];


	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;

private:
	virtual CComponent*		Clone() { return nullptr; }
	virtual void Free(void);
};

END
#endif // Renderer_h__
