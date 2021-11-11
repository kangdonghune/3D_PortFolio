#pragma once

#include "GameObject.h"
#include "Define.h"

class CSphere;
class CUI;
class CWeapon;
BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CShader;
class CCollider;
class CNaviMesh;
class CCollider;
class CDynamicCollider;
class COptimization;

END
class CPlayer : public CGameObject
{
	enum animID {
		M4_Walk_B,                                //0	frame:[37]
		M4_Walk_BL,                               //1	frame:[34]
		M4_Walk_BR,                               //2	frame:[39]
		M4_Walk_F,                                //3	frame:[38]
		M4_Walk_FL,                               //4	frame:[35]
		M4_Walk_FR,                               //5	frame:[36]
		M4_Walk_L,                                //6	frame:[36]
		M4_Walk_R,                                //7	frame:[35]
		M4Flamer_Walk_B_Aim,                      //8	frame:[37]
		M4Flamer_Walk_BL_Aim,                     //9	frame:[34]
		M4Flamer_Walk_FR_Aim,                     //10	frame:[36]
		M4_Run_B,                                 //11	frame:[19]
		M4_Run_BL,                                //12	frame:[25]
		M4_Run_BR,                                //13	frame:[22]
		M4_Run_F,                                 //14	frame:[44]
		M4_Run_FL,                                //15	frame:[26]
		M4_Run_FR,                                //16	frame:[26]
		M4_Run_L,                                 //17	frame:[21]
		M4_Run_R,                                 //18	frame:[22]
		M4_Fire,                                  //19	frame:[21]
		M4_Standing_CC,                           //20	frame:[2]
		M4_Standing_Idle,                         //21	frame:[567]
		M4_Standing_CD,                           //22	frame:[2]
		M4_Standing_CU,                           //23	frame:[2]
		M4_Bash,                                  //24	frame:[41]
		M4_Reload,                                //25	frame:[70]
		M4_Idle,                                  //26	frame:[460]
		M4_KnockDown_B,                           //27	frame:[58]
		End
	};

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void	Check_State(const _float& fTimeDelta);
	void	Update_Stat();
	PlayerState::STATE	Get_State() { return m_State; }

private: // state func
	void	Idle(const _float& fTimeDelta);
	void	Walk(const _float& fTimeDelta);
	void	Run(const _float& fTimeDelta);
	void	AimStand(const _float& fTimeDelta);
	void	AimWalk(const _float& fTimeDelta);
	void	Fire(const _float& fTimeDelta);
	void	Reload(const _float& fTimeDelta);
	void	Bash(const _float& fTimeDelta);

private:
	void	RotatePlayer(_float fDegree);
	_bool	HitCheck();

public:
	void					Set_State(PlayerState::STATE State) { m_State = State; }

	void					Set_3State(Player3State::State State) { m_3State = State; }
	Player3State::State		Get_3State() { return m_3State; }

public:
	CSphere*				Get_Shprer() { return m_pShprer; }
	_int					Get_HP() { return m_iHP; }
	_float					Get_Stamina() { return m_fStamina; }

private:
	HRESULT					Add_Component(void);
	HRESULT					LateAdd_Component(void);
	HRESULT					Add_Object(void);
	void					SetUp_OnTerrain(void);
	HRESULT					Select_ProtoMesh(const _tchar* pObjProtoName);
	_float					Check_ObjectCollision();
	_int					Collision_Trigger();
	void					Update_StatBar();
		

private:
	CDynamicMesh*			m_pMeshCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CNaviMesh*				m_pNaviCom = nullptr;
	_vec3					m_vDir;
	CSphere*				m_pShprer;
	CTransform*				m_pShprerTransCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CDynamicCollider*		m_pDynamicColliderCom = nullptr;
	COptimization*			m_pOptimizationCom = nullptr;
	_float					m_fFireTime = 0.f;
	CUI*					m_pHPBar;
	CUI*					m_pStaminaBar;
	CWeapon*				m_pM4 = nullptr;


	_int					m_iHP = 100;
	_float					m_fStamina = 200;

private:
	PlayerState::STATE		m_State;
	Player3State::State		m_3State;
public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pObjProtoName);

private:
	virtual void		Free(void);
};


