#include "TimerMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimerMgr)

Engine::CTimerMgr::CTimerMgr(void)
{

}

Engine::CTimerMgr::~CTimerMgr(void)
{
	Free();
}

_float CTimerMgr::Get_TimeDelta(const _tchar * pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}

void CTimerMgr::Update_TimeDelta(const _tchar * pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return;

	pTimer->Update_Timer();
}

// 타이머를 생성하고 맵 컨테이너에 넣어주는 함수
HRESULT Engine::CTimerMgr::Ready_Timer(const _tchar* pTimerTag)
{
	CTimer*		pTimer = Find_Timer(pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	NULL_CHECK_RETURN(pTimer, E_FAIL);

	//m_mapTimer.insert(make_pair(pTimerTag, pTimer));
	m_mapTimer.emplace(pTimerTag, pTimer);

	return S_OK;
}

CTimer* Engine::CTimerMgr::Find_Timer(const _tchar* pTimerTag)
{
	//auto	iter = m_mapTimer.find(pTimerTag);

	auto	iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), CTag_Finder(pTimerTag));

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}

void Engine::CTimerMgr::Free(void)
{
	for_each(m_mapTimer.begin(), m_mapTimer.end(), CDeleteMap());
	m_mapTimer.clear();
}

