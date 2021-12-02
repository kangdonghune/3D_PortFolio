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
	
	void				Setup_OnGraphicDev(const _uint& iIndex);//��ġ���Կ� ���ϴ� ����Ÿ���� ��ũ��Ű�� �Լ�
	
	void				Release_OnGraphicDev(const _uint& iIndex);

	void				Clear_RenderTarget();

	HRESULT				Ready_DebugBuffer(const _float& fx, const _float& fy, const _float& fW, const _float& fH);
	void				Render_DebugBuffer();

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPDIRECT3DTEXTURE9	m_pTargetTexture; 

	LPDIRECT3DSURFACE9	m_pTargetSurface; // �츮�� ����ϰ��� �ϴ� Ÿ�� �ؽ��� ����
	LPDIRECT3DSURFACE9	m_pOldTargetSurface; // ������ ���Կ� ����Ǿ� �ִ� Ÿ�� �ؽ��ĸ� �ӽ������� �����ϱ� ���� �����̽�

	D3DXCOLOR			m_ClearColor;

	LPDIRECT3DVERTEXBUFFER9	 m_pVB;
	LPDIRECT3DINDEXBUFFER9	 m_pIB;

public:
	static  CRenderTarget*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
								   const _uint& iWidth, const _uint& iHeight, //������� ������
								   D3DFORMAT format, //16��Ʈ 32��Ʈ ���� �Ǽ����� ������ �� �ִ� ����
									D3DXCOLOR Color);//������� ����
	virtual void			Free();	

};

END