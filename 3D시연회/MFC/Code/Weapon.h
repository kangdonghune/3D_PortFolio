#ifndef Sword_h__
#define Sword_h__

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

END
class CWeapon : public CGameObject
{
private:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWeapon(const CWeapon& rhs);
	virtual ~CWeapon(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


private:
	HRESULT					Select_ProtoMesh(const _tchar* pObjProtoName);
	void					Key_Input(const _float& fTimeDelta);
	HRESULT					Add_Component(void);
	HRESULT					Add_Object(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);



private:	
	CStaticMesh*			m_pMeshCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;

	CSphere*				m_pShprer;
	CTransform*				m_pShprerTransCom = nullptr;

	CForm*					m_pForm;

	const _matrix*			m_pParentBoneMatrix = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;


public:
	static CWeapon*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pObjProtoName);

private:
	virtual void		Free(void);
};


#endif // Sword_h__
