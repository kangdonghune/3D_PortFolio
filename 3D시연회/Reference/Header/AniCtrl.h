#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAniCtrl(const CAniCtrl& rhs);
	virtual ~CAniCtrl(void);

public:
	HRESULT			Ready_AniCtrl(void);
	void			Set_AnimationIndex(const _uint& iIndex);		// 내가 원하는 애니메이션으로 교체하기 위한 함수
	void			Set_AnimationIndexRepeat(const _uint& iIndex);  // 같은 애니메이션을 도중에 끊고 반복하기 위한 목적 함수
	void			Play_Animation(const _float& fTimeDelta);		// 실제적으로 애니메이션 재생을 명령하는 함수
	_bool			Is_AnimationsetFinish(void);
private:
	LPD3DXANIMATIONCONTROLLER			m_pAniCtrl;

	// 트랙은 최대 2개 이상만 생성(이 둘은 서로 같은 값이면 안된다)
	_uint								m_iCurrentTrack;
	_uint								m_iNewTrack;

	// 애니메이션이 재생되는 시간을 누적시키기 위한 변수
	_float								m_fAccTime;

	// 기존에 세팅되어있던 애니메이션 셋의 인덱스를 보관하기 위해 있는 변수
	_uint								m_iOldIndex;

	_double								m_dPeriod;

public:
	static CAniCtrl*		Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAniCtrl*		Create(const CAniCtrl& rhs);
	virtual void			Free(void);
};

END
#endif // AniCtrl_h__
