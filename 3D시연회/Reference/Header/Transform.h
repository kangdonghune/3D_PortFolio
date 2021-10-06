#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform(void);

public:
	void					Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	const	void*			Get_InfoRef(INFO eType);
	void					Set_Pos(_vec3* pPos);
	void					Set_Look(_vec3* pLook);
	const	_matrix*		Get_WorldMatrix(void);
	const	 _matrix*		Get_NRotWorldMatrix(void);
	void					Get_WorldMatrix(_matrix* pWorld) const;
	void					Get_Info(INFO eType, _vec3* pInfo);
	void					Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta);
	void					Move_Pos(const _vec3* pDir, const _float& fDist);
	void					Move_PickingPos(const _vec3* pPos, const _float& fSpeed, const _float& fTimeDelta);
	void					Rotation(ROTATION eType, const _float& fAngle);
	void					Rotation2(ROTATION eType, const _float& fAngle);
	const	_float			Get_Rotate(ROTATION eType);

	void					Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	const	_matrix*		Compute_LookAtTarget(const _vec3* pTargetPos);
	const	_float			Get_Scale(SCALE eType);
	void					Set_Scale(const _float& fX, const _float& fY, const _float& fZ);


	void					Set_ParentMatrix(const _matrix* pParentMatrix);
	void					Set_WorldMatrix(const _matrix* pWorld);

public:
	HRESULT					Ready_Transform(void);
	virtual _int			Update_Component(const _float& fTimeDelta);

public:
	_vec3		m_vInfo[INFO_END];
	_vec3		m_vScale;
	_vec3		m_vAngle;
	_matrix		m_matWorld;
	_matrix		m_matNRotWorld;

public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);

private:
	virtual void Free(void);

};

END
#endif // Transform_h__
