// TabChar.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC.h"
#include "TabChar.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFCView.h"
#include "Player.h"


// CTabChar ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTabChar, CDialogEx)

int CTabChar::m_iCount = 0;

CTabChar::CTabChar(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Char, pParent)
{

}

CTabChar::~CTabChar()
{
}

void CTabChar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeTask);
}


BEGIN_MESSAGE_MAP(CTabChar, CDialogEx)

	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CTabChar::OnNMDblclkTree1)
END_MESSAGE_MAP()


// CTabChar �޽��� ó�����Դϴ�.


BOOL CTabChar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString cst_Root;



	HTREEITEM h_Root;
	HTREEITEM h_entryPlayer, h_entryMonster;


	cst_Root.Format(_T("Root"));
	h_Root = m_TreeTask.InsertItem(cst_Root, NULL, NULL);

	h_entryPlayer = m_TreeTask.InsertItem(_T("Player"), h_Root, NULL);
	m_TreeTask.InsertItem(_T("Proto_Mesh_Hunter"), h_entryPlayer, NULL);

	h_entryMonster = m_TreeTask.InsertItem(_T("Monster"), h_Root, NULL);
	m_TreeTask.InsertItem(_T("Proto_Mesh_Goblin"), h_entryMonster, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}





void CTabChar::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CPoint p;
	GetCursorPos(&p);
	UINT flag;
	m_TreeTask.ScreenToClient(&p);
	HTREEITEM hItem_dc = m_TreeTask.HitTest(p, &flag);
	HTREEITEM hItem_parent = m_TreeTask.GetParentItem(hItem_dc);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCView* pView = dynamic_cast<CMFCView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pView->CreateCharictor(L"Environment", m_TreeTask.GetItemText(hItem_parent), m_TreeTask.GetItemText(hItem_dc));
	
	*pResult = 0;
}
