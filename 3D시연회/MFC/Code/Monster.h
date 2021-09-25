#pragma once

#include "GameObject.h"
#include "Define.h"


class CSphere;
BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;
class CSphrerMeshs;

END
class CMonster : public CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT					Add_Component(void);
	HRESULT					Add_Object(void);
	void					SetUp_OnTerrain(void);
	_vec3					PickUp_OnTerrain(void);
	HRESULT					Select_ProtoMesh(const _tchar* pObjProtoName);

private:
	CDynamicMesh*			m_pMeshCom = nullptr;
	CNaviMesh*				m_pNaviCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	_vec3					m_vDir;
	CSphere*				m_pShprer;
	CTransform*				m_pShprerTransCom = nullptr;

public:
	static CMonster*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pObjProtoName);

private:
	virtual void		Free(void);
};


