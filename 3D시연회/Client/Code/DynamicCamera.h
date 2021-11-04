#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"
#include "Define.h"

class CDynamicCamera : public CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicCamera(const CDynamicCamera& rhs);
	virtual ~CDynamicCamera(void);

public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& fTimeDelta) override;

	void			Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; m_pTargetTransCom = (CTransform*)m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC); }
	void			Release_Target() { m_pTarget = nullptr; m_pTargetTransCom = nullptr; }
	_vec3			Get_At() { return m_vAt; }
	_vec3			Get_Eye() { return m_vEye; }
	_matrix			Get_View() { return m_matView; }

private:
	void			Key_Input(const _float& fTimeDelta, const _float& fSpeed);
	void			Mouse_Move(const _float& fTimeDelta);
	void			Mouse_Fix(void);
	void			Check_TargetState();
	void			Fallow_Target();
	void			Move_Camera(const _float& fTimeDelta);
	

private:
	_bool			m_bClick	= false;
	_bool			m_bFix		= true;
	CGameObject*	m_pTarget = nullptr;
	CTransform*		m_pTargetTransCom = nullptr;
	_float			m_fMouseRotation = 0.f;
	Player3State::State	m_TargetState;
	_vec4			m_CameraSetting;

public:
	static CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, 
										const _float& fFov, const _float& fAspect, 
										const _float& fNear, const _float& fFar);

public:
	void				CCTV_Mod(_float fTimeDelta);

public:
	_bool				m_bMode = false;
	_float				m_fCount = 0.f;
private:
	virtual void		Free(void);
};


#endif // DynamicCamera_h__
