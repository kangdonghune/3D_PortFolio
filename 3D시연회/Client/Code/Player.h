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
class CCollider;
class CDynamicCollider;

END
class CPlayer : public CGameObject
{
	enum animID {
		Player_Move_Idle_01,                      //0	frame:[180]
		Player_Move_Walk_F,                       //1	frame:[68]
		Player_Move_Walk_B,                       //2	frame:[68]
		Player_Move_Walk_L,                       //3	frame:[68]
		Player_Move_Walk_R,                       //4	frame:[68]
		Player_Atk_Power_01_NoPower,              //5	frame:[73]
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
	
public:
	CSphere*				Get_Shprer() { return m_pShprer; }

private:
	HRESULT					Add_Component(void);
	HRESULT					LateAdd_Component(void);
	HRESULT					Add_Object(void);
	void					Key_Input(const _float& fTimeDelta);
	void					SetUp_OnTerrain(void);
	HRESULT					Select_ProtoMesh(const _tchar* pObjProtoName);
	_float					Check_ObjectCollision();
		

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
public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pObjProtoName);

private:
	virtual void		Free(void);
};


