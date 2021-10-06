#pragma once

#include "GameObject.h"
#include "Define.h"



BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;

END

class CSphere : public CGameObject
{
private:
	explicit CSphere(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphere(const CSphere& rhs);
	virtual ~CSphere(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT					Add_Component(void);

public:
	const _float					Get_Radius() { return m_fRadius; }
	const _float					Get_Height() { return m_fHeight; }

	void					Set_ID();
	void					Set_Height(_float fHeight) { m_fHeight = fHeight; }
	void					Set_Radius(_float fRadius) { m_fRadius = fRadius; }

private:
	CStaticMesh*			m_pMeshCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;

	_float					m_fHeight = 0;
	_float					m_fRadius = 1;
	//구만들기
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;	// 정점들을 보관하는 컴 객체
	LPD3DXMESH						m_pSphereMesh;
	_ulong							m_dwFVF;	// FVF의종류

public:
	static	_int			m_iShpereCount;
	_int					m_iID = -2;

public:
	static CSphere*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fRadius = 1);

private:
	virtual void		Free(void);
};

