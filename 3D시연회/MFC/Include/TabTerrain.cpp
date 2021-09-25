// TabTerrain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC.h"
#include "TabTerrain.h"
#include "afxdialogex.h"


// CTabTerrain 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabTerrain, CDialogEx)

CTabTerrain::CTabTerrain(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Terrain, pParent)
{

}

CTabTerrain::~CTabTerrain()
{
}

void CTabTerrain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabTerrain, CDialogEx)
END_MESSAGE_MAP()


// CTabTerrain 메시지 처리기입니다.
