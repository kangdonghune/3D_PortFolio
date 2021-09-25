#pragma once
#include "afxcmn.h"


// CTabChar 대화 상자입니다.

class CTabChar : public CDialogEx
{
	DECLARE_DYNAMIC(CTabChar)

public:
	CTabChar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabChar();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Char };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeTask;
	virtual BOOL OnInitDialog();
	CTreeCtrl m_Treelist;

public:
	static int	m_iCount;
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
