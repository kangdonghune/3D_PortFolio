#ifndef Timer_h__
#define Timer_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer(void);
	virtual ~CTimer(void);

public:
	_float			Get_TimeDelta(void) { return m_fTimeDelta; }
	
public:
	HRESULT			Ready_Timer(void);
	void			Update_Timer(void);

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;

	LARGE_INTEGER		m_CpuTick;

	_float						m_fTimeDelta;

public:
	static CTimer*		Create(void);
	virtual void		Free(void);
};

END

#endif // Timer_h__

// GetTickCount()				: 윈도우 구동 시점 내부 증가하는 숫자 이용하는 방식, 대략 1000이란 숫자가 카운트 되면 1초라 판단
// QueryPerformanceCounter()	: 하드웨어 구동 시점부터 하드웨어 장치 내부적으로 증가하는 숫자 이용하는 방식, 대략 10,000,000란 숫자가 카운트 되면 1초라 판단