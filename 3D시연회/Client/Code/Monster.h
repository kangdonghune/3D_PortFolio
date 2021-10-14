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
	enum animID {
		Goblin_Blacksmith_Idle,                   //0	frame:[120]
		Goblin_Blacksmith_Impact_F_FromL,         //1	frame:[50]
		Goblin_Blacksmith_P_Emote_01,             //2	frame:[152]
		Goblin_Blacksmith_P_Emote_02,             //3	frame:[139]
		Goblin_Blacksmith_P_Emote_03,             //4	frame:[109]
		Goblin_Blacksmith_P_Emote_04,             //5	frame:[164]
		End
	};


private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster(void);

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(void) override;

	void				Set_State(MonsterState::GoblinBlakcSmith eType) { type = eType; }
	CSphere*		Get_Sphere() { return m_pSphere; } //추후 몬스터 하나에 여러 충돌체가 붙을 거 같으니 이부분은 벡터와 인덱스로 변경할 것
	CTransform*	Get_SphereTransform() { return m_pShprerTransCom; }
protected:
	void					Key_Input(const _float& fTimeDelta);
	void					StateCheck();
	HRESULT					Add_Component(void);
	HRESULT					Add_Collider(void);
	HRESULT					Select_ProtoMesh(const _tchar* pObjProtoName);

protected:
	CDynamicMesh*			m_pMeshCom = nullptr;
	CNaviMesh*				m_pNaviCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	_vec3					m_vDir;
	CSphere*				m_pSphere;
	CTransform*				m_pShprerTransCom = nullptr;


	MonsterState::GoblinBlakcSmith  type = MonsterState::GoblinBlakcSmith::IDLE;
public:
	static CMonster*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pObjProtoName);

private:
	virtual void		Free(void);
};


