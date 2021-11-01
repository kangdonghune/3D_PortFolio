#ifndef UI_h__
#define UI_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CUI : public CGameObject
{
private:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI(const CUI& rhs);
	virtual ~CUI(void);

public:
	HRESULT Ready_Object(_float fX, _float fY, _float fSizeX, _float fSizeY);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public:
	void					Set_X(_float fX) { m_fX = fX; }
	void					Set_Y(_float fY) { m_fY = fY; }
	void					Set_XSize(_float fXSize) { m_fSizeX = fXSize; }
	void					Set_YSize(_float fYSize) { m_fSizeY = fYSize; }

	_float					Get_XSize() { return m_fSizeX; }

private:
	HRESULT					Add_Component(void);
	HRESULT					Select_ProtoMesh(const _tchar* pUiProtoname);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	_matrix					m_matProj;

	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;

public:
	static CUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pUiProtoname ,_float fX, _float fY, _float fSizeX, _float fSizeY);

private:
	virtual void		Free(void);
};


#endif // UI_h__
