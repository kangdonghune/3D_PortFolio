#include "AniCtrl.h"

USING(Engine)

Engine::CAniCtrl::CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
	, m_fAccTime(0.f)
	, m_iOldIndex(999)
	, m_dPeriod(0.0)

{
	m_pAniCtrl->AddRef();
}

Engine::CAniCtrl::CAniCtrl(const CAniCtrl& rhs)
	: m_iCurrentTrack(rhs.m_iCurrentTrack)
	, m_iNewTrack(rhs.m_iNewTrack)
	, m_fAccTime(rhs.m_fAccTime)
	, m_iOldIndex(rhs.m_iOldIndex)
	, m_dPeriod(0.0)
{
	// 애니메이션 컨트롤러를 복제해주는 함수
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),	// 복제시 원본 객체에서 제공하는 애니메이션의 개수
		rhs.m_pAniCtrl->GetMaxNumAnimationSets(),		// 구동 가능한 애니메이션의 최대 개수(대부분 첫 번째 인자값과 같은 값)
		rhs.m_pAniCtrl->GetMaxNumTracks(),				// 최대 트랙의 개수(우리는 진짜 많이 사용해봐야 두 개쓴다)
		rhs.m_pAniCtrl->GetMaxNumEvents(),				// 애니메이션에 삽입된 이벤트 정보(우리는 사용하지 못한다)
		&m_pAniCtrl);
}

Engine::CAniCtrl::~CAniCtrl(void)
{

}

HRESULT Engine::CAniCtrl::Ready_AniCtrl(void)
{
	return S_OK;
}

void Engine::CAniCtrl::Set_AnimationIndex(const _uint& iIndex)
{
	if (m_iOldIndex == iIndex)
		return;

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET		pAS = NULL;

	// 인덱스 값에 해당하는 애니메이션 정보를 얻어오는 함수
	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);
	// m_pAniCtrl->GetAnimationSetByName(); // 문자열 입력을 통해 원하는 애니메이션 셋을 얻어오는 함수

	// 현재 애니메이션 셋의 전체 재생 시간을 반환하는 함수
	m_dPeriod = pAS->GetPeriod();

	// 지정된 트랙에 애니메이션 셋 정보를 설치하는 함수
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// 애니메이션 셋이 갖고 있는 고유의 이벤트 정보를 종료시켜야함(만약 하지 않을 경우 모션 보간이 실패하게 됨)

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// 지정된 트랙을 활성화 또는 비활성화 할지 결정하고, 어느 시점에 그것을 수행할 지 지정하는 함수
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.25);
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// 트랙을 활성화하도록 설정하는 함수
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime(); // AdvanceTime 함수 호출 시 내부적으로 증가하던 시간 값을 리셋하는 함수
	m_fAccTime = 0.f;

	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldIndex = iIndex;
	m_iCurrentTrack = m_iNewTrack;
}

void CAniCtrl::Set_AnimationIndexRepeat(const _uint & iIndex)
{

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET		pAS = NULL;

	// 인덱스 값에 해당하는 애니메이션 정보를 얻어오는 함수
	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);
	// m_pAniCtrl->GetAnimationSetByName(); // 문자열 입력을 통해 원하는 애니메이션 셋을 얻어오는 함수

	// 현재 애니메이션 셋의 전체 재생 시간을 반환하는 함수
	m_dPeriod = pAS->GetPeriod();

	// 지정된 트랙에 애니메이션 셋 정보를 설치하는 함수
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// 애니메이션 셋이 갖고 있는 고유의 이벤트 정보를 종료시켜야함(만약 하지 않을 경우 모션 보간이 실패하게 됨)

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// 지정된 트랙을 활성화 또는 비활성화 할지 결정하고, 어느 시점에 그것을 수행할 지 지정하는 함수
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.5);
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.5, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.7f, m_fAccTime, 0.5, D3DXTRANSITION_LINEAR);

	// 트랙을 활성화하도록 설정하는 함수
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.5, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.3f, m_fAccTime, 0.5, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime(); // AdvanceTime 함수 호출 시 내부적으로 증가하던 시간 값을 리셋하는 함수
	m_fAccTime = 0.f;

	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldIndex = iIndex;
	m_iCurrentTrack = m_iNewTrack;
}

void Engine::CAniCtrl::Play_Animation(const _float& fTimeDelta)
{
	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);	// 실제 애니메이션을 구동하는 함수(2인자 : 애니메이션이 재생될 때 동작에 따른 사운드나 이펙트에 대한 처리)
	m_fAccTime += fTimeDelta;
}

_bool CAniCtrl::Is_AnimationsetFinish(void)
{
	D3DXTRACK_DESC			TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	// 0.1이란 값은 크게 의미가 없음, 본인의 게임의 특성 또는 취향에 맞춰서 값을 변경 가능
	if (TrackInfo.Position >= m_dPeriod)
		return true;

	return false;
}

CAniCtrl* Engine::CAniCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAniCtrl*	pInstance = new CAniCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

CAniCtrl* Engine::CAniCtrl::Create(const CAniCtrl& rhs)
{
	CAniCtrl*	pInstance = new	CAniCtrl(rhs);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CAniCtrl::Free(void)
{
	Safe_Release(m_pAniCtrl);
}

