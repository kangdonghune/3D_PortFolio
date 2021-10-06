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
	pShpere->Set_ID();

	return pShpere;

}

void CTabTerrain::Set_Sphere(CSphere * pShphere)
{
	m_pShpere = pShphere;

	Get_SphereInfo();
}

void CTabTerrain::Get_SphereInfo()
{
	if (m_pShpere == nullptr)
		return;
	UpdateData(true);

	CTransform* pTrans = (CTransform*)m_pShpere->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3	Pos = {};
	pTrans->Get_Info(INFO_POS, &Pos);
	m_fXCount = Pos.x;
	m_fYCount = Pos.y;
	m_fZCount = Pos.z;


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
	//���� 1. A�� B�� �׾����� �� C�� B���� ���ʿ� �־�� �Ѵ�.
	//���� 2. ����Ʈc�� z�� ����Ʈ a�� z��ǥ���� ���� ��
	//���� 3. �� �� �̻� ���� ���� �ƴ� ��
	CTransform* pTransA = (CTransform*)m_pPointA->Get_Component(L"Com_Transform", ID_DYNAMIC);
	CTransform* pTransB = (CTransform*)m_pPointB->Get_Component(L"Com_Transform", ID_DYNAMIC);
	CTransform* pTransC = (CTransform*)m_pPointC->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3* PosA = nullptr;
	PosA = (_vec3*)pTransA->Get_InfoRef(INFO_POS);
	_vec3* PosB = nullptr;
	PosB = (_vec3*)pTransB->Get_InfoRef(INFO_POS);
	_vec3* PosC = nullptr;
	PosC = (_vec3*)pTransC->Get_InfoRef(INFO_POS);

	//if (PosA->x >= PosB->x) //���� 1
	//	return;
	//if (PosA->z <= PosC->z) //���� 2
	//	return;

	if (m_pPointA->m_iID == m_pPointB->m_iID || m_pPointB->m_iID == m_pPointC->m_iID || m_pPointC->m_iID == m_pPointA->m_iID)
		return;


	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCView* pView = dynamic_cast<CMFCView*>(pMain->m_MainSplitter.GetPane(0, 1));


	MFCCELL tempMFCCell = {};
	tempMFCCell.pCell = CCell::Create(pView->m_pGraphicDev, m_dwIndex++, PosA, PosB, PosC);
	tempMFCCell.PointA = m_pPointA->m_iID;
	tempMFCCell.PointB = m_pPointB->m_iID;
	tempMFCCell.PointC = m_pPointC->m_iID;

	list<CGameObject*> pTerrainlst = Engine::Get_List(GAMELOGIC, L"Terrain");
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

void CTabTerrain::Update_VecCell()
{
	UpdateData(true);
	list<CGameObject*> pTerrainlst = Engine::Get_List(GAMELOGIC, L"Terrain");
	CTerrain*	pTerrain = (CTerrain*)pTerrainlst.front();
	for (int i = 0; i < pTerrain->m_vecCell.size(); i++)
	{
		if (pTerrain->m_vecCell[i].PointA == m_pShpere->m_iID)
		{
			CTransform* pTransMove = (CTransform*)m_pShpere->Get_Component(L"Com_Transform", ID_DYNAMIC);
			pTerrain->m_vecCell[i].pCell->Set_Point(CCell::POINT_A, (_vec3*)pTransMove->Get_InfoRef(INFO_POS));
		}
		if (pTerrain->m_vecCell[i].PointB == m_pShpere->m_iID)
		{
			CTransform* pTransMove = (CTransform*)m_pShpere->Get_Component(L"Com_Transform", ID_DYNAMIC);
			pTerrain->m_vecCell[i].pCell->Set_Point(CCell::POINT_B, (_vec3*)pTransMove->Get_InfoRef(INFO_POS));
		}
		if (pTerrain->m_vecCell[i].PointC == m_pShpere->m_iID)
		{
			CTransform* pTransMove = (CTransform*)m_pShpere->Get_Component(L"Com_Transform", ID_DYNAMIC);
			pTerrain->m_vecCell[i].pCell->Set_Point(CCell::POINT_C, (_vec3*)pTransMove->Get_InfoRef(INFO_POS));
		}

	}
	UpdateData(false);
}

void CTabTerrain::Delete_Cell()
{
	if (m_pShpere == nullptr)
		return;

	list<CGameObject*> pTerrainlst = Engine::Get_List(GAMELOGIC, L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainlst.front());
	for (int i = 0; i < pTerrain->m_vecCell.size(); i++)
	{
		if (pTerrain->m_vecCell[i].PointA == m_pShpere->m_iID)
		{
			Safe_Release(pTerrain->m_vecCell[i].pCell);
			pTerrain->m_vecCell.erase(pTerrain->m_vecCell.begin()+i);
			i--;
			continue;
		}
		if (pTerrain->m_vecCell[i].PointB == m_pShpere->m_iID)
		{
			Safe_Release(pTerrain->m_vecCell[i].pCell);
			pTerrain->m_vecCell.erase(pTerrain->m_vecCell.begin() + i);
			i--;
			continue;
		}
		if (pTerrain->m_vecCell[i].PointC == m_pShpere->m_iID)
		{
			Safe_Release(pTerrain->m_vecCell[i].pCell);
			pTerrain->m_vecCell.erase(pTerrain->m_vecCell.begin() + i);
			i--;
			continue;
		}
	}
	//CNaviMesh* pNavi = (CNaviMesh*)pTerrain->Get_Component(L"Com_Navi", ID_STATIC);
	//pNavi->Delete_Cell();
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
	ON_BN_CLICKED(IDC_RADIO3, &CTabTerrain::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_BUTTON10, &CTabTerrain::OnBnClickedDeleteSphere)
	ON_BN_CLICKED(IDC_BUTTON11, &CTabTerrain::OnBnClickedDeleteCell)
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
	Update_VecCell();
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
		Update_VecCell();
	}
	else
	{
		tempVec.y -= 0.1f;
		Send_Info(&tempVec);
		Update_VecCell();
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
	CFileDialog Dlg(FALSE, // �ٸ��̸����� ����. ���� TRUE ���� ����. 
		L"dat",// ����Ʈ Ȯ���� 
		L"MFCCell.dat",// ����Ʈ ���� �̸� 
		OFN_OVERWRITEPROMPT);// ����� ��� �޽��� ����ְڴ�. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir); //���� ���丮 ��ȯ. ..../3D_�ÿ�ȸ//MFC//Include
	PathRemoveFileSpec(szCurDir); //���� ��� ����. //Include ����
	PathRemoveFileSpec(szCurDir); //���� ��� ����. //MFC ����
	lstrcat(szCurDir, L"\\ReSource");
	lstrcat(szCurDir, L"\\Data");
	lstrcat(szCurDir, L"\\NaviMesh"); //
	Dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == Dlg.DoModal()) //���� �� ���� ���
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		DWORD dwByte = 0;
		DWORD dwStringCount = 0;
		//��ȸ�ϸ鼭 Ű�� ����Ʈ �����۵� ����
		list<CGameObject*> pTerrainlst = Engine::Get_List(GAMELOGIC, L"Terrain");
		CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainlst.front());
		vector<MFCCELL> vecMFCCell = pTerrain->m_vecCell;
		//������. pCell(index,����Ʈ 3�� ��ǥ), PointABC ��ȣ
		for (MFCCELL MCell : vecMFCCell)
		{
			_ulong dwIndex = MCell.pCell->Get_Index();
			const _vec3* SavePointAPos = MCell.pCell->Get_Point(CCell::POINT_A);
			const _vec3* SavePointBPos = MCell.pCell->Get_Point(CCell::POINT_B);
			const _vec3* SavePointCPos = MCell.pCell->Get_Point(CCell::POINT_C);
			WriteFile(hFile, &dwIndex, sizeof(_ulong), &dwByte, nullptr);
			WriteFile(hFile, SavePointAPos, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, SavePointBPos, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, SavePointCPos, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &MCell.PointA, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &MCell.PointB, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &MCell.PointC, sizeof(_int), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}

}


void CTabTerrain::OnBnClickedLoadCell()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE, // �ٸ��̸����� ����. ���� TRUE ���� ����. 
		L"dat",// ����Ʈ Ȯ���� 
		L"MFCCell.dat",// ����Ʈ ���� �̸� 
		OFN_OVERWRITEPROMPT);// ����� ��� �޽��� ����ְڴ�. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	PathRemoveFileSpec(szCurDir); //���� ��� ����. //Include ����
	PathRemoveFileSpec(szCurDir); //���� ��� ����. //MFC ����
	lstrcat(szCurDir, L"\\ReSource");
	lstrcat(szCurDir, L"\\Data");
	lstrcat(szCurDir, L"\\NaviMesh");
	Dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		//����Ʈ �ʱ�ȭ ���� 
		
		DWORD dwByte = 0;
		DWORD dwStringCount = 0;
		TCHAR* szBuf = nullptr;
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMFCView* pView = dynamic_cast<CMFCView*>(pMain->m_MainSplitter.GetPane(0, 1));
		list<CGameObject*> pTerrainlst = Engine::Get_List(GAMELOGIC, L"Terrain");
		CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainlst.front());
		for (int i = 0; i < pTerrain->m_vecCell.size(); i++)
		{
			pTerrain->m_vecCell[i].pCell->Set_Dead(true); // Set_Dead�ϸ鼭 ���۳�Ʈ�� ����� �͵鵵 �����ִ� ����.
		}
		pTerrain->m_vecCell.clear();// �� ���� �ʱ�ȭ;
		CNaviMesh* pNavi = (CNaviMesh*)pTerrain->Get_Component(L"Com_Navi", ID_STATIC);
		pNavi->Delete_Cell();// ���۳�Ʈ�� ����� �� ���� �ʱ�ȭ;

		_ulong dwIndex = 0;


		//�ε� ���� 1.pCell ���� �ҷ��鿩�´�. 2.cell ���� �� MFC Cell�� ���� �ͷ��� �� ���Ϳ� �߰�, 3. ���۳�Ʈ�� cell�� �߰� 
		while (true)
		{
			_vec3 SavePointAPos = {};
			_vec3 SavePointBPos = {};
			_vec3 SavePointCPos = {};
			_int  PointA = 0;
			_int  PointB = 0;
			_int  PointC = 0;
			ReadFile(hFile, &dwIndex, sizeof(_ulong), &dwByte, nullptr);
			if (0 == dwByte)
				break;//�о�� �� ������ ����
			ReadFile(hFile, &SavePointAPos, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &SavePointBPos, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &SavePointCPos, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &PointA, sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &PointB, sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &PointC, sizeof(_int), &dwByte, nullptr);
	

			MFCCELL tempMFCCell = {};
			tempMFCCell.pCell = CCell::Create(pView->m_pGraphicDev, dwIndex, &SavePointAPos, &SavePointBPos, &SavePointCPos);
			tempMFCCell.PointA = PointA;
			tempMFCCell.PointB = PointB;
			tempMFCCell.PointC = PointC;
			pTerrain->m_vecCell.push_back(tempMFCCell);
		}
		m_dwIndex = dwIndex + 1;
		pTerrain->SetUp_NaviMesh();// ���Ϳ� �� �� �߰� �Ǿ����� ���������������߰�
		CloseHandle(hFile);
	}

}


void CTabTerrain::OnBnClickedSaveSphere()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(FALSE, // �ٸ��̸����� ����. ���� TRUE ���� ����. 
		L"dat",// ����Ʈ Ȯ���� 
		L"MFCSphere.dat",// ����Ʈ ���� �̸� 
		OFN_OVERWRITEPROMPT);// ����� ��� �޽��� ����ְڴ�. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir); //���� ���丮 ��ȯ. ..../3D_�ÿ�ȸ//MFC//Include
	PathRemoveFileSpec(szCurDir); //���� ��� ����. //Include ����
	PathRemoveFileSpec(szCurDir); //���� ��� ����. //MFC ����
	lstrcat(szCurDir, L"\\ReSource");
	lstrcat(szCurDir, L"\\Data");
	lstrcat(szCurDir, L"\\NaviMesh"); //
	Dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == Dlg.DoModal()) //���� �� ���� ���
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		DWORD dwByte = 0;
		DWORD dwStringCount = 0;
		//��ȸ�ϸ鼭 Ű�� ����Ʈ �����۵� ����
		list<CGameObject*> pTerrainlst = Engine::Get_List(GAMELOGIC, L"Terrain");
		CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainlst.front());
		vector<CSphere*> vecSphere = pTerrain->m_vecShpere;
		//������. //1. id. 2. Ʈ������. 3. ���콺
		for (CSphere* pSphere : vecSphere)
		{
			CTransform*	pTrans = (CTransform*)pSphere->Get_Component(L"Com_Transform", ID_DYNAMIC);

			_int dwId = pSphere->m_iID;
			_float fRadius = pSphere->Get_Radius();
			_vec3	tempPos = {};
			pTrans->Get_Info(INFO_POS, &tempPos);
			WriteFile(hFile, &dwId, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &fRadius, sizeof(_float), &dwByte, nullptr);
			WriteFile(hFile, &tempPos, sizeof(_vec3), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
}


void CTabTerrain::OnBnClickedLoadSphrer()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE, // �ٸ��̸����� ����. ���� TRUE ���� ����. 
		L"dat",// ����Ʈ Ȯ���� 
		L"MFCSphere.dat",// ����Ʈ ���� �̸� 
		OFN_OVERWRITEPROMPT);// ����� ��� �޽��� ����ְڴ�. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	PathRemoveFileSpec(szCurDir); //���� ��� ����. //Include ����
	PathRemoveFileSpec(szCurDir); //���� ��� ����. //MFC ����
	lstrcat(szCurDir, L"\\ReSource");
	lstrcat(szCurDir, L"\\Data");
	lstrcat(szCurDir, L"\\NaviMesh");
	Dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		//����Ʈ �ʱ�ȭ ���� 

		DWORD dwByte = 0;
		DWORD dwStringCount = 0;
		TCHAR* szBuf = nullptr;
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMFCView* pView = dynamic_cast<CMFCView*>(pMain->m_MainSplitter.GetPane(0, 1));
		list<CGameObject*> pTerrainlst = Engine::Get_List(GAMELOGIC, L"Terrain");
		CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainlst.front());
		for (int i = 0; i < pTerrain->m_vecShpere.size(); i++)
		{
			pTerrain->m_vecShpere[i]->Set_Dead(true); // Set_Dead�ϸ鼭 ���۳�Ʈ�� ����� �͵鵵 �����ִ� ����.
		}
		pTerrain->m_vecShpere.clear();// �� ���� �ʱ�ȭ;

		_int iId = -1;


		//�ε� ���� 1.���̵� 2. ������ 3. ��ǥ
		while (true)
		{
			
			_float fRadius = 0.f;
			_vec3  tempPos = {};
			CSphere* pSphere;
			ReadFile(hFile, &iId, sizeof(_int), &dwByte, nullptr);
			if (0 == dwByte)
				break;//�о�� �� ������ ����
			ReadFile(hFile, &fRadius, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &tempPos, sizeof(_vec3), &dwByte, nullptr);

			m_pShpere = pSphere = CSphere::Create(pView->m_pGraphicDev, fRadius);
			CTransform* pTransform = (CTransform*)m_pShpere->Get_Component(L"Com_Transform", ID_DYNAMIC);
			pTransform->Set_Pos(&tempPos);
			pSphere->m_iID = iId;
			pTerrain->m_vecShpere.push_back(pSphere);
		}
		CSphere::m_iShpereCount = iId;
		CloseHandle(hFile);
	}
}


void CTabTerrain::OnBnClickedPointClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Clear_Points();
}


void CTabTerrain::OnBnClickedRadio3()
{
	eType = TerrainTool::TERRAIN_END;
	m_pShpere = nullptr;
}


void CTabTerrain::OnBnClickedDeleteSphere()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_pShpere != nullptr)
	{
		Delete_Cell();
		list<CGameObject*> pTerrainlst = Engine::Get_List(GAMELOGIC, L"Terrain");
		CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainlst.front());
		for (int i = 0; i < pTerrain->m_vecShpere.size(); i++)
		{
			if (pTerrain->m_vecShpere[i]->m_iID == m_pShpere->m_iID)
			{
				pTerrain->m_vecShpere[i]->Set_Dead(true);
				pTerrain->m_vecShpere.erase(pTerrain->m_vecShpere.begin() + i);
			}
		}
	}
}

	
void CTabTerrain::OnBnClickedDeleteCell()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	list<CGameObject*> pTerrainlst = Engine::Get_List(GAMELOGIC, L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainlst.front());
	pTerrain->m_vecCell.back().pCell->Set_Dead(true);
	pTerrain->m_vecCell.pop_back();
}
