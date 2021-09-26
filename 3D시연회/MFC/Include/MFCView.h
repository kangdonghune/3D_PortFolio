
// MFCView.h : CMFCView Ŭ������ �������̽�
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
protected: // serialization������ ��������ϴ�.
	CMFCView();
	DECLARE_DYNCREATE(CMFCView)

// Ư���Դϴ�.
public:
	CMFCDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
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

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void			OnInitialUpdate();
	

public:

	CGameObject*			CreateCharictor(const _tchar* pLayerTag, const _tchar* pParentName, const _tchar* pObjProtoName);
	CGameObject*			CreateObject(const _tchar* pLayerTag, const _tchar* pParentName, const _tchar* pObjProtoName);
};

#ifndef _DEBUG  // MFCView.cpp�� ����� ����
inline CMFCDoc* CMFCView::GetDocument() const
   { return reinterpret_cast<CMFCDoc*>(m_pDocument); }
#endif

