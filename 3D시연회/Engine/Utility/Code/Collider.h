#ifndef Collider_h__
#define Collider_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider(void);

public:
	const _matrix*			Get_CollWorldMatrix(void) { return &m_matCollWorld; }
	const _vec3*			Get_Min(void) { return &m_vMin; }
	const _vec3*			Get_Max(void) { return &m_vMax; }
	const _vec3*			Get_Center(void) { return &m_vCenter; }
	const _vec3*			Get_MaxDir(void) { return &m_vMaxDir; }




public:
	HRESULT			Ready_Collider(const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwVtxSize);
	void			Render_Collider(COLLTYPE eType, const _matrix* pColliderMatrix);

private:
	_vec3						m_vMin, m_vMax;
	_vec3						m_vCenter;
	_vec3						m_vMaxDir;

#ifdef _DEBUG
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];
#endif

	_matrix						m_matCollWorld;

public:
	static CCollider*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
		const _vec3* pPos,
		const _ulong& dwNumVtx,
		const _ulong& dwVtxSize);

	virtual CComponent* Clone(void);
	virtual void Free(void);
};

END
#endif // Collider_h__
