// TabObject.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC.h"
#include "TabObject.h"
#include "afxdialogex.h"


// CTabObject ��ȭ �����Դϴ�.

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


// CTabObject �޽��� ó�����Դϴ�.
