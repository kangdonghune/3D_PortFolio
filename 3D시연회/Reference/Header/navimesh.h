#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Cell.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNaviMesh : public CComponent
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public:
	void				Set_CellIndex(const _ulong& dwIndex) { m_dwIndex = dwIndex; }

public:
	HRESULT				Ready_NaviMesh(void);
	void				Render_NaviMesh(void);


	HRESULT				Add_Cell(CCell* pCell);
	_vec3				Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);

private:
	HRESULT				Link_Cell(void);

private:
	vector<CCell*>			m_vecCell; 
	_ulong					m_dwIndex;

public:
	static CNaviMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);
};


END
#endif // NaviMesh_h__
