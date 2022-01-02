#ifndef LightMgr_h__
#define LightMgr_h__

#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)

private:
	explicit CLightMgr(void);
	virtual ~CLightMgr(void);

public:
	const D3DLIGHT9*		Get_Light(const _ulong& iIndex = 0);
	CLight*					Get_LightClass(const _ulong& iIndex = 0);

public:
	HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	void		Render_Light(LPD3DXEFFECT& pEffect);
	void		Delete_Light(CLight* pLight);
	void		Delete_Light(const _uint& iIndex);

private:
	CLight*		m_LightList[LIGHT_END] = {};

private:
	virtual void	Free(void);
};

END
#endif // LightMgr_h__
