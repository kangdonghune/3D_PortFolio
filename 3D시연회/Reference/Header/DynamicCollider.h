#pragma once

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

	const	_matrix*		Get_ColliderWorld() { return &m_matCollWorld; }

	const	_vec3*			Get_Min() { return &m_vMin; }
	const	_vec3*			Get_Max() { return &m_vMax; }
	const	_vec3*			Get_MaxDir() { return &m_vMaxDir; }
	const	_vec3*			Get_Center() { return &m_vCenter; }


public:
	static CDynamicCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _float fHeight, _float fWidthX, _float fWidthZ);
	virtual CComponent*		Clone(void);

private: //여기 있는 값들은 전부 로컬 기준이기에 추후 사용할 때 월드 변환 계산을 해줘야 한다.
	_float						m_fHeight;
	_float						m_fWidthX;
	_float						m_fWidthY;
	_vec3						m_vMin;
	_vec3						m_vMax;
	_vec3						m_vCenter;
	_vec3						m_vMaxDir;
	_matrix						m_matCollWorld;
	


private:
	virtual void Free(void) override;

};

END
