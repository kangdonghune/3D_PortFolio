#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Transform.h"
BEGIN(Engine)

class ENGINE_DLL CSphrerMeshs : public CComponent
{
private:
	explicit CSphrerMeshs(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphrerMeshs(const CSphrerMeshs& rhs);
	virtual ~CSphrerMeshs(void);

public:
	HRESULT Ready_Mesh(void);
	void	Render_Mesh();

public:
	void	Set_TransCom(CTransform* pTransCom) { m_pTransCom = pTransCom; }
	void	Set_Radius(_int iRadius) { m_iRadius = iRadius; }
	void	Set_Height(_float fHeight) { m_fHeight = fHeight; }


private:
	LPD3DXMESH						m_pMesh;
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;

	CTransform*						m_pTransCom = nullptr;
	_int							m_iRadius = 100;
	_float							m_fHeight = 0;

public:
	static  CSphrerMeshs*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);
private:
	virtual void Free(void) override;

};

END
