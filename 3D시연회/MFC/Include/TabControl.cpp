// TabControl.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC.h"
#include "TabControl.h"
#include "afxdialogex.h"


// CTabControl ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTabControl, CDialogEx)

CTabControl::CTabControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FORM, pParent)
{

}

CTabControl::~CTabControl()
{
}

void CTabControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabControl, CDialogEx)
END_MESSAGE_MAP()


// CTabControl �޽��� ó�����Դϴ�.
