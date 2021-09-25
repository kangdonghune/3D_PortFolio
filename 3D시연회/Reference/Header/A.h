#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL A : public CComponent
{
private:
	explicit A(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit A(const A& rhs);
	virtual ~A(void);

public:

public:
	HRESULT				Ready_NaviMesh(void);
	void				Render_NaviMesh(void);

	_vec3				Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);

private:
	HRESULT				Link_Cell(void);

private:


public:
	static  A*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);
};


END

