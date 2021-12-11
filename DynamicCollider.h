#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicCollider : public CVIBuffer
{
private:
	explicit CDynamicCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicCollider(const CDynamicCollider& rhs);
	virtual ~CDynamicCollider(void);

public:
	virtual HRESULT Ready_Buffer(_vec3* pPos, _float fHeight, _float fWidthX, _float fWidthZ);
	virtual void Render_Buffer(const _matrix* matWorld);

public:
	static CDynamicCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _float fHeight, _float fWidthX, _float fWidthZ);
	virtual CComponent*		Clone(void);

private:
	_float						m_fHeight;
	_float						m_fWidthX;
	_float						m_fWidthY;

	_vec3						m_vMin;
	_vec3						m_vMax;

private:
	virtual void Free(void) override;

};

END
#endif // CubeTex_h__
