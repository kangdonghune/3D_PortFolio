#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev : public CBase
{

	DECLARE_SINGLETON(CInputDev)

private:
	explicit CInputDev(void);
	virtual ~CInputDev(void);
	
public:

	_byte		Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	_byte		Get_DIMouseState(MOUSEKEYSTATE eID) { return m_MouseState.rgbButtons[eID]; }
	_long		Get_DIMouseMove(MOUSEMOVESTATE eState)	{	return *(((_long*)&m_MouseState) + eState);}

public:
	HRESULT			Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void			Update_InputDev(void);

private:
	LPDIRECTINPUT8				m_pInputSDK;

	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;

	_byte						m_byKeyState[256];
	DIMOUSESTATE				m_MouseState;


public:
	virtual void Free(void);
};


END
#endif // InputDev_h__
