#pragma once
#include "afxcmn.h"


// CTabChar ��ȭ �����Դϴ�.

class CTabChar : public CDialogEx
{
	DECLARE_DYNAMIC(CTabChar)

public:
	CTabChar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTabChar();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Char };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeTask;
	virtual BOOL OnInitDialog();
	CTreeCtrl m_Treelist;

public:
	static int	m_iCount;
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
