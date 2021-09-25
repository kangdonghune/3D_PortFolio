#ifndef Font_h__
#define Font_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFont : public CBase
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFont(void);

public:
	HRESULT		Ready_Font(const _tchar* pFontType, 
							const _uint& iWidth, 
							const _uint& iHeight,
							const _uint& iWeight);

	void		Render_Font(const _tchar* pString,		// 출력하고자 하는 문자열
							const _vec2* pPos,			// 출력할 위치(윈도우 좌표 그대로)
							D3DXCOLOR Color);			// 출력할 폰트 색상

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPD3DXFONT				m_pFont;
	LPD3DXSPRITE			m_pSprite;

public:
	static CFont*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
								const _tchar* pFontType,		// 글씨체
								const _uint& iWidth,			// 가로 길이
								const _uint& iHeight,			// 세로 길이
								const _uint& iWeight);			// 굵기

private:
	virtual void Free(void);

};

END
#endif // Font_h__
