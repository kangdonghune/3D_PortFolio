#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "RenderTarget.h"

BEGIN(Engine)

class ENGINE_DLL  CRenderTargetManager : public CBase
{

DECLARE_SINGLETON(CRenderTargetManager)

private:
	explicit CRenderTargetManager();
	virtual  ~CRenderTargetManager();


public:
	HRESULT				Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, 
											const _tchar* pTargetTag,
											const _uint& iWidth, const _uint& iHeight,
											D3DFORMAT Format, D3DXCOLOR Color);


	HRESULT				Ready_MRT(const _tchar*	pMRTTag, const _tchar* pTargetTag);

	HRESULT				Begin_MRT(const _tchar* pMRTTag);
	
	HRESULT				End_MRT(const _tchar* pMRTTag);

	HRESULT				Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fx, const _float& fy, const _float& fW, const _float& fH);
	void				Render_DebugMRT(const _tchar* pMRTTag);

	void				SetUp_Shader(LPD3DXEFFECT & pEffect, const _tchar* pTargetTag, const char * pContantTable);


private:
	CRenderTarget*			Find_RenderTarget(const _tchar* pTargetTag);
	list<CRenderTarget*>*	Find_MRT(const _tchar* pMRTTag);

public:
	virtual		void	Free();

private:
	map<const _tchar*, CRenderTarget*>	m_mapRenderTarget;
	map<const _tchar*, list<CRenderTarget*>> m_mapMRT;



};

END