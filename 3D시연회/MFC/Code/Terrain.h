#ifndef Terrain_h__
#define Terrain_h__

#include "GameObject.h"
class CForm;
class CSphere;
BEGIN(Engine)

class CTerrainTex;
class CTexture;
class CTransform;
class CRenderer;
class COptimization;
class CShader;
class CNaviMesh;
class CCell;
class CCalculator;

END
class CTerrain : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT						Add_Component(void);
	HRESULT						SetUp_Material(void);
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	HRESULT						SetUp_NaviMesh();

	void						Key_Input(const _float& fTimeDelta);

private:
	CTerrainTex*				m_pBufferCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	COptimization*				m_pOptimizationCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CNaviMesh*					m_pNaviCom = nullptr;
	CCalculator*				m_pCalculatorCom = nullptr;

	INDEX32*					m_pIndex = nullptr;
	_ulong						m_dwTriCnt = 0;

	CForm*						m_pForm;
	vector<CCell*>				m_vecCell;
	vector<CSphere*>			m_vecShpere;
public:
	static CTerrain*			Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	virtual void		Free(void);
};


#endif // Terrain_h__
