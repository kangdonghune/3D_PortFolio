#ifndef Loading_h__
#define Loading_h__

#include "Define.h"
#include "Engine_Define.h"
#include "Base.h"

class CLoading : public CBase
{
public:
	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading(void);

public:
	LOADINGID			Get_LoadingID(void) const { return m_eID; }
	CRITICAL_SECTION*	Get_Crt(void) { return &m_Crt; }
	_bool				Get_Finish(void) const { return m_bFinish; }
	const _tchar*		Get_String(void) const { return m_szLoading; }

public:
	HRESULT			Ready_Loading(LOADINGID eID);
	
	_uint			LoadingForStage(void);
	_uint			LoadingForBoss(void);

public:
	static unsigned int CALLBACK Thread_Main(void* pArg);

private:
	HANDLE					m_hThread;
	CRITICAL_SECTION		m_Crt;
	LOADINGID				m_eID;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_bool					m_bFinish;
	_tchar					m_szLoading[256];

public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);
private:
	virtual void		Free(void);

};

#endif // Loading_h__
