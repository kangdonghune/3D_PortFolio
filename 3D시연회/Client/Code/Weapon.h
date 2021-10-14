#ifndef Sword_h__
#define Sword_h__

#include "GameObject.h"
#include "Define.h"
#include "Sphrer.h"

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
	HRESULT					Add_Component(void);
	HRESULT					Add_Collider(void);

	_bool					Collision_ToObject();

	void					Setting_ColliderPos();

private:	
	CStaticMesh*			m_pMeshCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CSphere*				m_pSphere = nullptr;
	CTransform*				m_pSphrerTransformCom = nullptr;

	const _matrix*			m_pParentBoneMatrix = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;


public:
	static CWeapon*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Sword_h__
