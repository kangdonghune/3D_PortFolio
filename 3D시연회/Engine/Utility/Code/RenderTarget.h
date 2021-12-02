#pragma once 

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget : public CBase
{
private:
	explicit CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual  ~CRenderTarget();

public:
	HRESULT				Ready_RenderTarget(const _uint& iWidth, const _uint& iHeight, D3DFORMAT format, D3DXCOLOR Color);
	
	void				Setup_OnGraphicDev(const _uint& iIndex);//장치슬롯에 원하는 랜더타겟을 링크시키는 함수
	
	void				Release_OnGraphicDev(const _uint& iIndex);

	void				Clear_RenderTarget();

	HRESULT				Ready_DebugBuffer(const _float& fx, const _float& fy, const _float& fW, const _float& fH);
	void				Render_DebugBuffer();

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPDIRECT3DTEXTURE9	m_pTargetTexture; 

	LPDIRECT3DSURFACE9	m_pTargetSurface; // 우리가 출력하고자 하는 타겟 텍스쳐 정보
	LPDIRECT3DSURFACE9	m_pOldTargetSurface; // 기존에 슬롯에 연결되어 있는 타겟 텍스쳐를 임시적으로 보관하기 위한 서페이스

	D3DXCOLOR			m_ClearColor;

	LPDIRECT3DVERTEXBUFFER9	 m_pVB;
	LPDIRECT3DINDEXBUFFER9	 m_pIB;

public:
	static  CRenderTarget*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
								   const _uint& iWidth, const _uint& iHeight, //백버퍼의 사이즈
								   D3DFORMAT format, //16비트 32비트 선택 실수값을 저장할 수 있는 범위
									D3DXCOLOR Color);//백버퍼의 색상
	virtual void			Free();	

};

END