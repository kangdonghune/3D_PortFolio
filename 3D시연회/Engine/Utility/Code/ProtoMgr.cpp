#include "ProtoMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CProtoMgr)

Engine::CProtoMgr::CProtoMgr(void)
{

}

Engine::CProtoMgr::~CProtoMgr(void)
{
	Free();
}

HRESULT Engine::CProtoMgr::Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent)
{
	CComponent*		pPrototype = Find_Proto(pProtoTag);

	if (nullptr != pPrototype)
		return E_FAIL;

	m_mapProto.emplace(pProtoTag, pComponent);

	return S_OK;
}

CComponent* Engine::CProtoMgr::Clone_Proto(const _tchar* pProtoTag)
{
	CComponent*		pPrototype = Find_Proto(pProtoTag);

	if (nullptr == pPrototype)
		return nullptr;

	return pPrototype->Clone();
}

CComponent* Engine::CProtoMgr::Find_Proto(const _tchar* pProtoTag)
{
	auto	iter = find_if(m_mapProto.begin(), m_mapProto.end(), CTag_Finder(pProtoTag));

	if (iter == m_mapProto.end())
		return nullptr;

	return iter->second;
}

void Engine::CProtoMgr::Free()
{
	for_each(m_mapProto.begin(), m_mapProto.end(), CDeleteMap());
	m_mapProto.clear();
}

