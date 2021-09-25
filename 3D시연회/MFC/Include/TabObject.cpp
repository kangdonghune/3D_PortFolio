// TabObject.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC.h"
#include "TabObject.h"
#include "afxdialogex.h"


// CTabObject 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabObject, CDialogEx)

CTabObject::CTabObject(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Object, pParent)
{

}

CTabObject::~CTabObject()
{
}

void CTabObject::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabObject, CDialogEx)
END_MESSAGE_MAP()


// CTabObject 메시지 처리기입니다.
