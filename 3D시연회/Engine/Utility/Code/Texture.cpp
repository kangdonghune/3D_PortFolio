#include "Texture.h"

USING(Engine)


Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{

}

Engine::CTexture::CTexture(const CTexture& rhs)
	:CComponent(rhs)
	,m_vecTexture(rhs.m_vecTexture)
{
	_uint iContainerSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iContainerSize);

	for (_ulong i = 0; i < iContainerSize; ++i)
		m_vecTexture[i]->AddRef();

}

Engine::CTexture::~CTexture(void)
{

}

HRESULT Engine::CTexture::Ready_Texture(const _tchar* pPath, TEXTUREID eType, const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9*		pTexture = NULL;

	for (_uint i = 0; i < iCnt; ++i)
	{
		_tchar	szFileName[256] = L"";

		wsprintf(szFileName, pPath, i);

		switch (eType)
		{
		case TYPE_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			 
			break;

		case TYPE_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		}

		m_vecTexture.push_back(pTexture);
	}	
	return S_OK;
}

void Engine::CTexture::Render_Texture(const _uint& iIndex /*= 0*/)
{
	if (m_vecTexture.size() < iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

CComponent* Engine::CTexture::Clone(void)
{
	return new CTexture(*this);
}

CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTUREID eType, const _uint& iCnt /*= 1*/)
{

	CTexture*	pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture(pPath, eType, iCnt)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CTexture::Free(void)
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
	m_vecTexture.clear();

	CComponent::Free();
}

void Engine::CTexture::Set_Texture(LPD3DXEFFECT pEffect, const char* pConstantName, const _uint& iIndex /*= 0*/)
{
	if (m_vecTexture.size() < iIndex)
		return;

	pEffect->SetTexture(pConstantName, m_vecTexture[iIndex]);
}

