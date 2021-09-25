#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase 
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	explicit CGraphicDev(void);
	virtual ~CGraphicDev(void);

public:
	LPDIRECT3DDEVICE9		Get_Device() { return m_pGraphicDev; }
public:
	HRESULT			Ready_GraphicDev(HWND hWnd, WINMODE eMode, 
									const _uint& iSizeX, 
									const _uint& iSizeY, 
									CGraphicDev** ppGraphicDev);

	void			Render_Begin(D3DXCOLOR Color);
	void			Render_End(void);

private:
	LPDIRECT3D9				m_pSDK;				// 장치의 조사하는 역할(실제적인 근본 객체)
	LPDIRECT3DDEVICE9		m_pGraphicDev;		// 그리기 장치를 컨트롤하는 객체(m_pSDK에 의해서만 생성 가능)



public:
	virtual		void		Free(void);
};

END
#endif // GraphicDev_h__

