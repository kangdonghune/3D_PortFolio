#pragma once


class CTriggerFunc 
{

	DECLARE_SINGLETON(CTriggerFunc)
private:
	explicit CTriggerFunc();
	virtual ~CTriggerFunc(void);

public:
	void		Set_GraphicDev(LPDIRECT3DDEVICE9 pGraphicDev) { m_pGraphicDev = pGraphicDev; m_pGraphicDev->AddRef(); }
	void		Render_Text();
	void		Classify_Trigger(_int index);
	void		Set_TextNum(_int iNum) { m_iTextNum = iNum; }
public:
	HRESULT		 Spawn_Monster(_int i);
	void		 Move_Camera();
private:
	HRESULT			Load_Monster(const _tchar * pFilePath);
	CGameObject*	Create_Unit(Layer type, const _tchar* pParentName, const _tchar* pObjProtoName);
	_int			m_iTextNum = 0;

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

};


