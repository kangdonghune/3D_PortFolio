#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera(void);

public:
	virtual HRESULT			Ready_Object(void);
	virtual _int			Update_Object(const _float& fTimeDelta);

protected:
	_vec3				m_vEye, m_vAt, m_vUp;			// 뷰스페이스 변환 행렬 전용 변수
	_float				m_fAspect, m_fFov, m_fNear, m_fFar;	// 원근 투영 행렬 전용 변수
	_matrix				m_matView, m_matProj;

protected:
	virtual void	Free(void);
};

END
#endif // Camera_h__
