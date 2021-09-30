// TabTerrain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC.h"
#include "TabTerrain.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFCView.h"
#include "Form.h"
#include "TabChar.h"
#include "TabObject.h"

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

CSphere* CTabTerrain::Create_Sphrer(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 * pPos)
{
	if (!IsDlgButtonChecked(IDC_RADIO1))
		return nullptr;

	CSphere* pShpere = nullptr;

	m_pShprer = pShpere = CSphere::Create(pGraphicDev, 1.f);
	CTransform* pTransform = (CTransform*)m_pShprer->Get_Component(L"Com_Transform", ID_DYNAMIC);
	pTransform->Set_Pos(pPos);

	return pShpere;

}


BEGIN_MESSAGE_MAP(CTabTerrain, CDialogEx)

	ON_WM_MOUSEHOVER()
	ON_WM_NCMOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TCARD()
	ON_BN_CLICKED(IDC_RADIO1, &CTabTerrain::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTabTerrain::OnBnClickedRadio2)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTabTerrain 메시지 처리기입니다.








void CTabTerrain::OnBnClickedRadio1()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	pForm->m_ptabObject->Set_Object(nullptr);
	pForm->m_ptabChar->Set_Object(nullptr);
}


void CTabTerrain::OnBnClickedRadio2()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	pForm->m_ptabObject->Set_Object(nullptr);
	pForm->m_ptabChar->Set_Object(nullptr);
}


void CTabTerrain::OnDestroy()
{
	CDialogEx::OnDestroy();
	


	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
