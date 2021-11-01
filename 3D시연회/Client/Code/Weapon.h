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
	enum animID {
		K_Hunter_Reload_M4Flamer_anim,            //0	frame:[71]
		End
	};

private:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWeapon(const CWeapon& rhs);
	virtual ~CWeapon(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


	_int					Get_LoadedBullet() { return m_iLoadedBullet; }
	_int					Get_ResidueBullet() { return m_iResidueBullet;}
	void					Shot();
	void					ReLoad();

private:
	HRESULT					Add_Component(void);
	HRESULT					Add_Collider(void);

	_bool					Collision_ToObject();

	void					Setting_ColliderPos();
	void					KeyInput();
	void					Setting_Transform();

private:	
	CDynamicMesh*			m_pMeshCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CSphere*				m_pSphere = nullptr;
	CTransform*				m_pSphrerTransformCom = nullptr;

	const _matrix*			m_pParentBoneMatrix = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;

	_bool					m_bIsReload = false;

	_int					m_iLoadedBullet;
	_int					m_iResidueBullet;
public:
	static CWeapon*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Sword_h__
