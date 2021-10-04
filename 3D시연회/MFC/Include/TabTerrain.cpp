// TabTerrain.cpp : ���� �����Դϴ�.
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
#include "Terrain.h"
#include "navimesh.h"

// CTabTerrain ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTabTerrain, CDialogEx)

CTabTerrain::CTabTerrain(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Terrain, pParent)
	, m_fXCount(0)
	, m_fYCount(0)
	, m_fZCount(0)
	, m_iPointANum(0)
	, m_iPointBNum(0)
	, m_iPointCNum(0)
{

}

CTabTerrain::~CTabTerrain()
{
}

void CTabTerrain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, m_fXCount);
	DDX_Text(pDX, IDC_EDIT2, m_fYCount);
	DDX_Text(pDX, IDC_EDIT3, m_fZCount);
	DDX_Text(pDX, IDC_EDIT1, m_iPointANum);
	DDX_Text(pDX, IDC_EDIT9, m_iPointBNum);
	DDX_Text(pDX, IDC_EDIT10, m_iPointCNum);
}

CSphere* CTabTerrain::Create_Sphrer(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 * pPos)
{
	if (!IsDlgButtonChecked(IDC_RADIO1))
		return nullptr;

	CSphere* pShpere = nullptr;

	m_pShpere = pShpere = CSphere::Create(pGraphicDev, 1.f);
	CTransform* pTransform = (CTransform*)m_pShpere->Get_Component(L"Com_Transform", ID_DYNAMIC);
	pTransform->Set_Pos(pPos);

	return pShpere;

}

void CTabTerrain::Set_Sphere(CSphere * pShphere)
{
	m_pShpere = pShphere;

	Get_SphereInfo();
}

void CTabTerrain::Get_SphereInfo()
{
	UpdateData(true);
	if (m_pShpere != nullptr)
	{
		CTransform* pTrans = (CTransform*)m_pShpere->Get_Component(L"Com_Transform", ID_DYNAMIC);
		_vec3	Pos = {};
		pTrans->Get_Info(INFO_POS, &Pos);
		m_fXCount = Pos.x;
		m_fYCount = Pos.y;
		m_fZCount = Pos.z;
	}
	UpdateData(false);
}

void CTabTerrain::InPut_Point(CSphere * pSphrer)
{
	UpdateData(true);
	if (m_pPointA == nullptr)
	{
		m_pPointA = pSphrer;
		m_iPointANum = pSphrer->m_iID;

		UpdateData(false);
		return;
	}
	if (m_pPointB == nullptr)
	{
		m_pPointB = pSphrer;
		m_iPointBNum = pSphrer->m_iID;

		UpdateData(false);
		return;
	}
	if (m_pPointC == nullptr)
	{
		m_pPointC = pSphrer;
		m_iPointCNum = pSphrer->m_iID;
		Create_MFCCell();
		Clear_Points();

		UpdateData(false);
		return;
	}

	UpdateData(false);

}

void CTabTerrain::Clear_Points()
{
	UpdateData(true);
	m_pPointA = nullptr;
	m_pPointB = nullptr;
	m_pPointC = nullptr;
	m_iPointANum = -1;
	m_iPointBNum = -1;
	m_iPointCNum = -1;
	UpdateData(false);
}

void CTabTerrain::Create_MFCCell()
{
	//���� 1. ����Ʈb�� x�� ����Ʈ a�� x��ǥ���� Ŭ ��
	//���� 2. ����Ʈc�� z�� ����Ʈ a�� z��ǥ���� ���� ��
	//���� 3. �� �� �̻� ���� ���� �ƴ� ��
	CTransform* pTransA = (CTransform*)m_pPointA->Get_Component(L"Com_Transform", ID_DYNAMIC);
	CTransform* pTransB = (CTransform*)m_pPointB->Get_Component(L"Com_Transform", ID_DYNAMIC);
	CTransform* pTransC = (CTransform*)m_pPointC->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 PosA = {};
	pTransA->Get_Info(INFO_POS, &PosA);
	_vec3 PosB = {};
	pTransB->Get_Info(INFO_POS, &PosB);
	_vec3 PosC = {};
	pTransC->Get_Info(INFO_POS, &PosC);

	if (PosA.x >= PosB.x) //���� 1
		return;
	if (PosA.z <= PosC.z) //���� 2
		return;

	if (PosA == PosB || PosB == PosC || PosC == PosA)
		return;


	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCView* pView = dynamic_cast<CMFCView*>(pMain->m_MainSplitter.GetPane(0, 1));


	MFCCELL tempMFCCell = {};
	tempMFCCell.pCell = CCell::Create(pView->m_pGraphicDev, m_dwIndex++, &PosA, &PosB, &PosC);
	tempMFCCell.PointA = m_pPointA->m_iID;
	tempMFCCell.PointB = m_pPointB->m_iID;
	tempMFCCell.PointC = m_pPointC->m_iID;

	list<CGameObject*> pTerrainlst = Engine::Get_List(L"GameLogic", L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainlst.front());
	pTerrain->m_vecCell.push_back(tempMFCCell);
	CNaviMesh* pNavi = dynamic_cast<CNaviMesh*>(pTerrain->Get_Component(L"Com_Navi", ID_STATIC));
	pNavi->Add_Cell(tempMFCCell.pCell);
}


void CTabTerrain::Send_Info(_vec3* pPos)
{
	if (m_pShpere == nullptr)
		return;

	CTransform* pTrans = (CTransform*)m_pShpere->Get_Component(L"Com_Transform", ID_DYNAMIC);
	pTrans->Set_Pos(pPos);
	Get_SphereInfo();
}




BEGIN_MESSAGE_MAP(CTabTerrain, CDialogEx)

	ON_WM_MOUSEHOVER()
	ON_WM_NCMOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TCARD()
	ON_BN_CLICKED(IDC_RADIO1, &CTabTerrain::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTabTerrain::OnBnClickedRadio2)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT4, &CTabTerrain::OnEnChangeMoveX)
	ON_EN_CHANGE(IDC_EDIT2, &CTabTerrain::OnEnChangeMoveY)
	ON_EN_CHANGE(IDC_EDIT3, &CTabTerrain::OnEnChangeMoveZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CTabTerrain::OnDeltaposX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTabTerrain::OnDeltaposY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CTabTerrain::OnDeltaposZ)
	ON_BN_CLICKED(IDC_BUTTON1, &CTabTerrain::OnBnClickedSaveCell)
	ON_BN_CLICKED(IDC_BUTTON2, &CTabTerrain::OnBnClickedLoadCell)
	ON_BN_CLICKED(IDC_BUTTON3, &CTabTerrain::OnBnClickedSaveSphere)
	ON_BN_CLICKED(IDC_BUTTON7, &CTabTerrain::OnBnClickedLoadSphrer)
	ON_BN_CLICKED(IDC_BUTTON9, &CTabTerrain::OnBnClickedPointClear)
END_MESSAGE_MAP()


// CTabTerrain �޽��� ó�����Դϴ�.








void CTabTerrain::OnBnClickedRadio1()
{
	eType = TerrainTool::SPHERE;
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	pForm->m_ptabObject->Set_Object(nullptr);
	pForm->m_ptabChar->Set_Object(nullptr);
}


void CTabTerrain::OnBnClickedRadio2()
{
	eType = TerrainTool::CELL;
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	pForm->m_ptabObject->Set_Object(nullptr);
	pForm->m_ptabChar->Set_Object(nullptr);
}


void CTabTerrain::OnDestroy()
{
	CDialogEx::OnDestroy();
	


	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CTabTerrain::OnEnChangeMoveX()
{
	if (m_pShpere == nullptr)
		return;

	UpdateData(true);
	CTransform* pTrans = (CTransform*)m_pShpere->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 InputPos = { m_fXCount, m_fYCount, m_fZCount };
	pTrans->Set_Pos(&InputPos);
	UpdateData(false);
}


void CTabTerrain::OnEnChangeMoveY()
{
	if (m_pShpere == nullptr)
		return;

	UpdateData(true);
	CTransform* pTrans = (CTransform*)m_pShpere->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 InputPos = { m_fXCount, m_fYCount, m_fZCount };
	pTrans->Set_Pos(&InputPos);
	UpdateData(false);
}


void CTabTerrain::OnEnChangeMoveZ()
{
	if (m_pShpere == nullptr)
		return;

	UpdateData(true);
	CTransform* pTrans = (CTransform*)m_pShpere->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 InputPos = { m_fXCount, m_fYCount, m_fZCount };
	pTrans->Set_Pos(&InputPos);
	UpdateData(false);
}


void CTabTerrain::OnDeltaposX(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(true);
	if (m_pShpere == nullptr)
		return;


	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_vec3 tempVec = { m_fXCount, m_fYCount, m_fZCount };
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (pNMUpDown->iDelta < 0)
	{
		tempVec.x += 0.1f;
		Send_Info(&tempVec);
	}
	else
	{
		tempVec.x -= 0.1f;
		Send_Info(&tempVec);
	}


	UpdateData(false);

}


void CTabTerrain::OnDeltaposY(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(true);
	if (m_pShpere == nullptr)
		return;


	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_vec3 tempVec = { m_fXCount, m_fYCount, m_fZCount };
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (pNMUpDown->iDelta < 0)
	{
		tempVec.y += 0.1f;
		Send_Info(&tempVec);
	}
	else
	{
		tempVec.y -= 0.1f;
		Send_Info(&tempVec);
	}


	UpdateData(false);
}


void CTabTerrain::OnDeltaposZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(true);
	if (m_pShpere == nullptr)
		return;


	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_vec3 tempVec = { m_fXCount, m_fYCount, m_fZCount };
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (pNMUpDown->iDelta < 0)
	{
		tempVec.z += 0.1f;
		Send_Info(&tempVec);
	}
	else
	{
		tempVec.z -= 0.1f;
		Send_Info(&tempVec);
	}


	UpdateData(false);
}


void CTabTerrain::OnBnClickedSaveCell()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CTabTerrain::OnBnClickedLoadCell()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CTabTerrain::OnBnClickedSaveSphere()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CTabTerrain::OnBnClickedLoadSphrer()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CTabTerrain::OnBnClickedPointClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Clear_Points();
}
