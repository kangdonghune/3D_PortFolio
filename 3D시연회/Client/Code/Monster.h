#pragma once

#include "GameObject.h"
#include "Define.h"

class CSphere;
class CPlayer;
BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;
class CSphrerMeshs;
class CDynamicCollider;

END
class CMonster : public CGameObject
{
	enum animID {
		Walk_B,                                   //0	frame:[46]
		Walk_F,                                   //1	frame:[45]
		Walk_L,                                   //2	frame:[45]
		Walk_R,                                   //3	frame:[46]
		Idle,                                     //4	frame:[209]
		CallToAction_Roar,                        //5	frame:[106]
		Laying_F,                                 //6	frame:[6]
		Laying_B,                                 //7	frame:[6]
		Land_F_Light_Loop,                        //8	frame:[27]
		KnockDown_B,                              //9	frame:[112]
		GetUp_B,                                  //10	frame:[331]
		Death_B,                                  //11	frame:[248]
		TentacleSwing_Sideways_00,                //12	frame:[110]
		End
	};


private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster(void);

public:	
	virtual HRESULT			Ready_Object(void) override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(void) override;

	void					Set_State(MonsterState::GoblinBlakcSmith eType) { type = eType; }
	CSphere*				Get_Sphere() { return m_pSphere; } //추후 몬스터 하나에 여러 충돌체가 붙을 거 같으니 이부분은 벡터와 인덱스로 변경할 것
	CTransform*				Get_SphereTransform() { return m_pShprerTransCom; }

	void					Damaged() { m_iHp--; }

private:
	void					IdleF();
	void					Walk(_float fTimedelta);
	void					Roar();
	void					Down();
	void					Damage();
	void					Dead();
	void					GetUp();
	void					Attack();
	void					Stop();

	void					Update_TargetDist();

protected:
	void					Key_Input(const _float& fTimeDelta);
	void					StateCheck(_float fTimedelta);
	HRESULT					Add_Component(void);
	HRESULT					Add_Collider(void);
	_float					Check_ObjectCollision();
	HRESULT					Select_ProtoMesh(const _tchar* pObjProtoName);

protected:
	CDynamicMesh*			m_pMeshCom = nullptr;
	CNaviMesh*				m_pNaviCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CSphere*				m_pSphere;
	CTransform*				m_pShprerTransCom = nullptr;
	CPlayer*				m_pTarget = nullptr;
	_float					m_fTargetDist = 99999.f;
	CSphere*				m_pAttackSphere = nullptr;
	CTransform*				m_pAttackSphereTransCom = nullptr;
	CDynamicCollider*		m_pDynamicColliderCom = nullptr;
	_float					m_fDeltaTime;

	_int					m_iHp =30;

	MonsterState::GoblinBlakcSmith  type = MonsterState::GoblinBlakcSmith::IDLE;
public:
	static CMonster*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pObjProtoName);

private:
	virtual void		Free(void);
};


