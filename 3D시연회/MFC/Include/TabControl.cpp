// TabControl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC.h"
#include "TabControl.h"
#include "afxdialogex.h"


// CTabControl 대화 상자입니다.

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


// CTabControl 메시지 처리기입니다.
