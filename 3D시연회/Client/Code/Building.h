#pragma once

#include "GameObject.h"
#include "Define.h"


class CSphere;
class CForm;
BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;
class CSphrerMeshs;

END
class CBuilding : public CGameObject
{
private:
	explicit CBuilding(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBuilding(const CBuilding& rhs);
	virtual ~CBuilding(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT					Add_Component(void);
	HRESULT					Add_Object(void);

	HRESULT					Select_ProtoMesh(const _tchar* pObjProtoName);

private:
	CStaticMesh*			m_pMeshCom = nullptr;
	CNaviMesh*				m_pNaviCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	_vec3					m_vDir;
	CSphere*				m_pShprer;
	CTransform*				m_pShprerTransCom = nullptr;

public:
	static CBuilding*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pObjProtoName);

private:
	virtual void		Free(void);
};


