#pragma once

#include "Component.h"
#include "Base.h"
BEGIN(Engine)

class ENGINE_DLL CSphrer_Mesh : public CComponent
{
private:
	explicit CSphrer_Mesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphrer_Mesh(const CSphrer_Mesh& rhs);
	virtual ~CSphrer_Mesh(void);

public:
	HRESULT Ready_Mesh(void);
	void Render_Mesh(void);

private:
	LPD3DXMESH				m_pMesh;

public:
	static  CSphrer_Mesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);
private:
	virtual void Free(void) override;

};

END
