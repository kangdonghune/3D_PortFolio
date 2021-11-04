#pragma once

class C_Object;
class CObjAnime 
{

	DECLARE_SINGLETON(CObjAnime)
private:
	explicit CObjAnime();
	~CObjAnime(void);

public:
	void	Update_ObjAnime(_float fTimedelta);
	void	Render_ObjAnime();
	void	Classify_Obj(C_Object* pObj);

	void	Set_GraphicDev(LPDIRECT3DDEVICE9 pGraphicDev) { m_pGraphicDev = pGraphicDev; m_pGraphicDev->AddRef(); }
	_int	Get_CableCount() { return m_iCableCount; }

private:
	void	Cable_Anime();
	void	ChurchDoorL_Anmie(_float fTimedelta);
	void	ChurchDoorR_Anime(_float fTimedelta);
	void	Generator_Anime(_float fTimedelta);

	_bool	m_bCableAni = false;
	_bool	m_bChurchDoorLAni = false;
	_bool	m_bChurchDoorRAni = false;
	_bool	m_bGeneratorAni = false;
	C_Object*		m_pCable = nullptr;
	C_Object*		m_pChurchDoorL = nullptr;
	C_Object*		m_pChurchDoorR = nullptr;
	C_Object*		m_pGenerator = nullptr;

	_float			m_fDoorLAngle = 0.f;
	_float			m_fDoorRAngle = 0.f;

	_bool			m_bCable = false;
	_int			m_iCableCount = 0;
	CGameObject*	m_pEffect = nullptr;
	CGameObject*	m_pFinalDoor = nullptr;

	_bool			m_bTouch = false;
	
	
private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
};


