#ifndef Terrain_h__
#define Terrain_h__

#include "GameObject.h"
#include "Sphrer.h"
class CForm;
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

	void						Key_Input(const _float& fTimeDelta);
public:
	HRESULT						SetUp_NaviMesh();
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


	_bool						m_bRBPress = false;
	_bool						m_bLBPress = false;
public:
	vector<MFCCELL>				m_vecCell;
	vector<CSphere*>			m_vecShpere;
	static CTerrain*			Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	virtual void		Free(void);
};


#endif // Terrain_h__
