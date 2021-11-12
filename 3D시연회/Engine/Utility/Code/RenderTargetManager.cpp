#include "RenderTargetManager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CRenderTargetManager)

CRenderTargetManager::CRenderTargetManager()
{
}

CRenderTargetManager::~CRenderTargetManager()
{
	Free();
}

HRESULT CRenderTargetManager::Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pTargetTag, const _uint & iWidth, const _uint & iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr != pRenderTarget)
		return E_FAIL;

	pRenderTarget = CRenderTarget::Create(pGraphicDev, iWidth, iHeight, Format, Color);
	NULL_CHECK_RETURN(pRenderTarget, E_FAIL);

	m_mapRenderTarget.emplace(pTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CRenderTargetManager::Ready_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag)
{
	CRenderTarget*	pRenderTarget = Find_RenderTarget(pTargetTag);
	NULL_CHECK_RETURN(pRenderTarget, E_FAIL);

	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (pMRTList == nullptr)
	{
		list<CRenderTarget*>	MRTList;
		MRTList.push_back(pRenderTarget);
		m_mapMRT.emplace(pMRTTag,MRTList);
	}

	else
	{
	pMRTList->push_back(pRenderTarget);
	}

	

	return S_OK;
}

HRESULT CRenderTargetManager::Begin_MRT(const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);

	if (pMRTList == nullptr)
		return E_FAIL;

	for (auto& iter : *pMRTList)
	{
		iter->Clear_RenderTarget();

	}

	_uint iIndex = 0;

	for (auto& iter : *pMRTList)
	{
		iter->Setup_OnGraphicDev(iIndex++);
	}

	return S_OK;
}

HRESULT CRenderTargetManager::End_MRT(const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);

	if (pMRTList == nullptr)
		return E_FAIL;

	_uint iIndex = 0;

	for (auto& iter : *pMRTList)
	{
		iter->Release_OnGraphicDev(iIndex++);
	}

	return S_OK;
}

CRenderTarget * CRenderTargetManager::Find_RenderTarget(const _tchar * pTargetTag)
{
	auto	iter = find_if(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CTag_Finder(pTargetTag));
	
	if (iter == m_mapRenderTarget.end())
		return nullptr;


	return iter->second;
}

list<CRenderTarget*>* CRenderTargetManager::Find_MRT(const _tchar * pMRTTag)
{
	auto&	iter = find_if(m_mapMRT.begin(), m_mapMRT.end(), CTag_Finder(pMRTTag));

	if (iter == m_mapMRT.end())
		return nullptr;

	return &iter->second;
}

void CRenderTargetManager::Free()
{
	for_each(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CDeleteMap());
	m_mapRenderTarget.clear();
	
	for (auto& iter : m_mapMRT)
		iter.second.clear();

	m_mapMRT.clear();
}
