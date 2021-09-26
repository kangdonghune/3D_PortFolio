
// MFCView.h : CMFCView 클래스의 인터페이스
//

#pragma once

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"


class CMFCDoc;
class CForm;

BEGIN(Engine)


END

class CMFCView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFCView();
	DECLARE_DYNCREATE(CMFCView)

// 특성입니다.
public:
	CMFCDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HRESULT					SetUp_DefaultSetting(LPDIRECT3DDEVICE9 *ppGraphicDev);
	HRESULT					Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagementClass);

public:
	int						Update_MainView(const float& fTimeDelta);
	void					Render_Scene();

public:
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	CGraphicDev*			m_pDeviceClass = nullptr;
	CManagement*			m_pManagementClass = nullptr;
	CForm*					m_pForm = nullptr;

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void			OnInitialUpdate();
	

public:

	CGameObject*			CreateCharictor(const _tchar* pLayerTag, const _tchar* pParentName, const _tchar* pObjProtoName);
	CGameObject*			CreateObject(const _tchar* pLayerTag, const _tchar* pParentName, const _tchar* pObjProtoName);
};

#ifndef _DEBUG  // MFCView.cpp의 디버그 버전
inline CMFCDoc* CMFCView::GetDocument() const
   { return reinterpret_cast<CMFCDoc*>(m_pDocument); }
#endif

