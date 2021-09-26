// Form.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC.h"
#include "Form.h"

#include"TabChar.h"
#include"TabObject.h"
#include"TabTerrain.h"

// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)
{

}

CForm::~CForm()
{
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabctrl);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CForm::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CForm 진단입니다.

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기입니다.








void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_tabctrl.InsertItem(1, _T("Object"));
	m_tabctrl.InsertItem(2, _T("NaviMesh"));
	m_tabctrl.InsertItem(0, _T("Charictor"));

	m_tabctrl.SetCurSel(0);

	CRect rect;
	m_tabctrl.GetWindowRect(rect);

	m_ptabChar = new CTabChar;
	m_ptabChar->Create(IDD_Char, &m_tabctrl);
	m_ptabChar->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_ptabChar->ShowWindow(SW_SHOW);

	m_ptabObject = new CTabObject;
	m_ptabObject->Create(IDD_Object, &m_tabctrl);
	m_ptabObject->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_ptabObject->ShowWindow(SW_SHOW);

	m_ptabTerrain = new CTabTerrain;
	m_ptabTerrain->Create(IDD_Terrain, &m_tabctrl);
	m_ptabTerrain->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_ptabTerrain->ShowWindow(SW_SHOW);

}


void CForm::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel = m_tabctrl.GetCurSel();

	switch (sel)
	{
	case 0:
		m_ptabChar->ShowWindow(SW_SHOW);
		m_ptabObject->ShowWindow(SW_HIDE);
		m_ptabTerrain->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_ptabChar->ShowWindow(SW_HIDE);
		m_ptabObject->ShowWindow(SW_SHOW);
		m_ptabTerrain->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_ptabChar->ShowWindow(SW_HIDE);
		m_ptabObject->ShowWindow(SW_HIDE);
		m_ptabTerrain->ShowWindow(SW_SHOW);
		break;
	}
}
