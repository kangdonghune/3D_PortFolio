#ifndef MainApp_h__
#define MainApp_h__

// 객체의 생성 및 소멸을 아무 곳에서 할 수 있도록 허용하지 않겠다 ; 방어적인 코드
#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"


BEGIN(Engine)

class CGraphicDev;
class CManagement;

END

class CMainApp : public CBase
{
private:						// 생성자, 소멸자
	explicit CMainApp(void);	// 묵시적 형변환 불가
	virtual ~CMainApp(void);

	// public 함수, 변수
	// prorected 함수, 변수
	// private 함수, 변수
public:
	HRESULT				Ready_MainApp(void);
	int					Update_MainApp(const float& fTimeDelta);
	void				Render_MainApp(void);

private:
	HRESULT					SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT					Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagementClass);

private:
	CGraphicDev*			m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	CManagement*			m_pManagementClass = nullptr;

	CTriCol*				m_pBuffer = nullptr;

public:
	static CMainApp*		Create(void);		// 생성 함수
	virtual void			Free(void);			// 소멸 함수

};

#endif // MainApp_h__


