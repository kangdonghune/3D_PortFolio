// Form.cpp : ���� �����Դϴ�.
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


// CForm �����Դϴ�.

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


// CForm �޽��� ó�����Դϴ�.








void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
