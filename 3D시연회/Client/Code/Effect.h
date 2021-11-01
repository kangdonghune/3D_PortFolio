#ifndef Effect_h__
#define Effect_h__

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CEffect : public CGameObject
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect(void);

public:
	virtual HRESULT Ready_Object(_vec3* pPos);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT					Add_Component(void);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	_float					m_fFrame = 0.f;

public:
	static CEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos);

private:
	virtual void		Free(void);
};


#endif // Effect_h__
