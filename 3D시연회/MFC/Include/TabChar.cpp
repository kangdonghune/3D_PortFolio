// TabChar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC.h"
#include "TabChar.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFCView.h"
#include "Player.h"


// CTabChar 대화 상자입니다.

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


// CTabChar 메시지 처리기입니다.


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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}





void CTabChar::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
