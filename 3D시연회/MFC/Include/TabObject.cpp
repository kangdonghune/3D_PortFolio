// TabObject.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC.h"
#include "TabObject.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFCView.h"
#include "Object.h"
#include "Form.h"
#include "TabChar.h"


// CTabObject 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabObject, CDialogEx)

CTabObject::CTabObject(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Object, pParent)
	, X_Counter(0)
	, Y_Counter(0)
	, Z_Counter(0)
	, m_RotateX(0)
	, m_RotateY(0)
	, m_RotateZ(0)
	, m_Scale(0)
{

}

CTabObject::~CTabObject()
{
}

void CTabObject::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ObjectTree);
	DDX_Text(pDX, IDC_EDIT1, X_Counter);
	DDX_Text(pDX, IDC_EDIT2, Y_Counter);
	DDX_Text(pDX, IDC_EDIT3, Z_Counter);
	DDX_Text(pDX, IDC_EDIT5, m_RotateX);
	DDX_Text(pDX, IDC_EDIT6, m_RotateY);
	DDX_Text(pDX, IDC_EDIT7, m_RotateZ);
	DDX_Text(pDX, IDC_EDIT8, m_Scale);
	DDX_Control(pDX, IDC_SLIDER3, m_XCounterSlider);
	DDX_Control(pDX, IDC_SLIDER2, m_YCounterSlider);
	DDX_Control(pDX, IDC_SLIDER4, m_ZCounterSlider);
	DDX_Control(pDX, IDC_SLIDER5, m_RotateXSlider);
	DDX_Control(pDX, IDC_SLIDER6, m_RotateYSlider);
	DDX_Control(pDX, IDC_SLIDER7, m_RotateZSlider);
	DDX_Control(pDX, IDC_SLIDER8, m_ScaleSlider);
}


BEGIN_MESSAGE_MAP(CTabObject, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CTabObject::OnEnChangeXCounter)
	ON_EN_CHANGE(IDC_EDIT2, &CTabObject::OnEnChangeYCounter)
	ON_EN_CHANGE(IDC_EDIT3, &CTabObject::OnEnChangeZCounter)
	ON_EN_CHANGE(IDC_EDIT5, &CTabObject::OnEnChangeRotX)
	ON_EN_CHANGE(IDC_EDIT6, &CTabObject::OnEnChangeRotY)
	ON_EN_CHANGE(IDC_EDIT7, &CTabObject::OnEnChangeRotZ)
	ON_EN_CHANGE(IDC_EDIT8, &CTabObject::OnEnChangeScale)

	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTabObject::OnDeltaposXCount)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CTabObject::OnDeltaposYCount)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CTabObject::OnDeltaposZCount)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CTabObject::OnDeltaposRotX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CTabObject::OnDeltaposRotY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CTabObject::OnDeltaposRotZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CTabObject::OnDeltaposScale)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CTabObject::OnNMDblclkTree1)

	ON_BN_CLICKED(IDC_BUTTON1, &CTabObject::OnBnClickedDeleteObject)
	ON_BN_CLICKED(IDC_BUTTON2, &CTabObject::OnBnClickedBuildingSave)
	ON_BN_CLICKED(IDC_BUTTON3, &CTabObject::OnBnClickedBuildingLoad)
	ON_BN_CLICKED(IDC_BUTTON5, &CTabObject::OnBnClickedStuffSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CTabObject::OnBnClickedStuffLoad)
END_MESSAGE_MAP()


// CTabObject 메시지 처리기입니다.


void CTabObject::Get_Transform()
{
	if (m_pObject == nullptr)
		return;

	UpdateData(true);
	CTransform* pTrans = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 Pos = {};
	pTrans->Get_Info(INFO_POS, &Pos);
	X_Counter = Pos.x;
	Y_Counter = Pos.y;
	Z_Counter = Pos.z;

	m_XCounterSlider.SetPos(X_Counter * 100);
	m_YCounterSlider.SetPos(Y_Counter * 100);
	m_ZCounterSlider.SetPos(Z_Counter * 100);


	UpdateData(false);
}

void CTabObject::Set_Transform(_vec3 * MovePos)
{
	if (m_pObject == nullptr)
		return;

	UpdateData(true);
	CTransform* pTrans = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	pTrans->Set_Pos(MovePos);
	Get_Transform();

	UpdateData(false);
}

void CTabObject::Get_Rotate()
{
	if (m_pObject == nullptr)
		return;

	UpdateData(true);
	CTransform* pTrans = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);

	m_RotateX = D3DXToDegree(pTrans->Get_Rotate(ROT_X));
	m_RotateY = D3DXToDegree(pTrans->Get_Rotate(ROT_Y));
	m_RotateZ = D3DXToDegree(pTrans->Get_Rotate(ROT_Z));

	m_RotateXSlider.SetPos(m_RotateX * 100);
	m_RotateYSlider.SetPos(m_RotateY * 100);
	m_RotateZSlider.SetPos(m_RotateZ * 100);

	UpdateData(false);
}

void CTabObject::Set_Rotate(_vec3 vecRotate)
{
	if (m_pObject == nullptr)
		return;

	UpdateData(true);
	CTransform* pTrans = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);


	pTrans->Rotation2(ROT_X, D3DXToRadian(vecRotate.x));
	pTrans->Rotation2(ROT_Y, D3DXToRadian(vecRotate.y));
	pTrans->Rotation2(ROT_Z, D3DXToRadian(vecRotate.z));

	Get_Rotate();
	UpdateData(false);
}

void CTabObject::Get_Scale()
{
	if (m_pObject == nullptr)
		return;

	UpdateData(true);
	CTransform* pTrans = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);

	m_Scale = pTrans->Get_Scale(SCALE_X);

	m_ScaleSlider.SetPos(m_Scale * 100);

	UpdateData(false);
}

void CTabObject::Set_Scale(_float fScale)
{
	if (m_pObject == nullptr)
		return;

	if (fScale <= 0)
		fScale = 1;


	UpdateData(true);
	CTransform* pTrans = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	pTrans->Set_Scale(fScale, fScale, fScale);
	Get_Scale();

	UpdateData(false);
}

BOOL CTabObject::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Tree
	CString cst_Root;
	HTREEITEM h_Root;
	HTREEITEM h_entryBuilding, h_entryStuff;


	cst_Root.Format(_T("Root"));
	h_Root = m_ObjectTree.InsertItem(cst_Root, NULL, NULL);

	h_entryBuilding = m_ObjectTree.InsertItem(_T("Building"), h_Root, NULL);
	//m_ObjectTree.InsertItem(_T("Proto_Mesh_Hunter"), h_entryPlayer, NULL);

	h_entryStuff = m_ObjectTree.InsertItem(_T("Stuff"), h_Root, NULL);
	m_ObjectTree.InsertItem(_T("Proto_Mesh_Train"), h_entryStuff, NULL);
	//

	//Slider

	m_XCounterSlider.SetRange(0, VTXCNTX * 100);
	m_XCounterSlider.SetPos(0);
	m_XCounterSlider.SetLineSize(1);


	m_YCounterSlider.SetRange(0, VTXCNTX * 100);
	m_YCounterSlider.SetPos(0);
	m_YCounterSlider.SetLineSize(1);

	m_ZCounterSlider.SetRange(0, VTXCNTZ * 100);
	m_ZCounterSlider.SetPos(0);
	m_ZCounterSlider.SetLineSize(1);


	m_RotateXSlider.SetRange(0, 360 * 100);
	m_RotateXSlider.SetPos(0);
	m_RotateXSlider.SetLineSize(1);

	m_RotateYSlider.SetRange(0, 360 * 100);
	m_RotateYSlider.SetPos(0);
	m_RotateYSlider.SetLineSize(1);

	m_RotateZSlider.SetRange(0, 360 * 100);
	m_RotateZSlider.SetPos(0);
	m_RotateZSlider.SetLineSize(1);

	m_ScaleSlider.SetRange(1, 10 * 100);
	m_ScaleSlider.SetPos(0);
	m_ScaleSlider.SetLineSize(1);



	return TRUE;  // return TRUE unless you set the focus to a control

}


void CTabObject::OnEnChangeXCounter()
{
	UpdateData(true);
	//Xcount
	CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 InputPos = { X_Counter, Y_Counter, Z_Counter };
	pTransCom->Set_Pos(&InputPos);

	m_XCounterSlider.SetPos(X_Counter * 100);
	m_YCounterSlider.SetPos(Y_Counter * 100);
	m_ZCounterSlider.SetPos(Z_Counter * 100);
	UpdateData(false);
}


void CTabObject::OnEnChangeYCounter()
{
	UpdateData(true);
	//Ycount
	CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 InputPos = { X_Counter, Y_Counter, Z_Counter };
	pTransCom->Set_Pos(&InputPos);

	m_XCounterSlider.SetPos(X_Counter * 100);
	m_YCounterSlider.SetPos(Y_Counter * 100);
	m_ZCounterSlider.SetPos(Z_Counter * 100);
	UpdateData(false);
}


void CTabObject::OnEnChangeZCounter()
{
	UpdateData(true);
	//Zcount
	CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 InputPos = { X_Counter, Y_Counter, Z_Counter };
	pTransCom->Set_Pos(&InputPos);


	m_XCounterSlider.SetPos(X_Counter * 100);
	m_YCounterSlider.SetPos(Y_Counter * 100);
	m_ZCounterSlider.SetPos(Z_Counter * 100);
	UpdateData(false);
}


void CTabObject::OnEnChangeRotX()
{
	UpdateData(true);
	//Rotx
	CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	if (m_RotateX >= 361.f)
	{
		int OverCountX = m_RotateX / 360;
		m_RotateX -= 360 * OverCountX;
	}
	pTransCom->Rotation(ROT_X, m_RotateX);
	UpdateData(false);
}


void CTabObject::OnEnChangeRotY()
{
	UpdateData(true);
	//Rotx
	CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	if (m_RotateY >= 361.f)
	{
		int OverCountY = m_RotateY / 360;
		m_RotateY -= 360 * OverCountY;
	}
	pTransCom->Rotation(ROT_Y, m_RotateY);
	UpdateData(false);
}


void CTabObject::OnEnChangeRotZ()
{
	UpdateData(true);
	//Rotx
	CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	if (m_RotateY >= 361.f)
	{
		int OverCountZ = m_RotateZ / 360;
		m_RotateZ -= 360 * OverCountZ;
	}
	pTransCom->Rotation(ROT_Z, m_RotateZ);
	UpdateData(false);
}


void CTabObject::OnEnChangeScale()
{
	UpdateData(true);
	//Rotx
	CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	pTransCom->Set_Scale(m_Scale, m_Scale, m_Scale);

	m_ScaleSlider.SetPos(m_Scale * 100);

	UpdateData(false);
}





void CTabObject::OnDeltaposXCount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_vec3 tempVec = { X_Counter, Y_Counter, Z_Counter };
	if (pNMUpDown->iDelta < 0)
	{
		tempVec.x += 0.1f;
		Set_Transform(&tempVec);
	}
	else
	{
		tempVec.x -= 0.1f;
		Set_Transform(&tempVec);
	}
	*pResult = 0;
}


void CTabObject::OnDeltaposYCount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_vec3 tempVec = { X_Counter, Y_Counter, Z_Counter };
	if (pNMUpDown->iDelta < 0)
	{
		tempVec.y += 0.1f;
		Set_Transform(&tempVec);
	}
	else
	{
		tempVec.y -= 0.1f;
		Set_Transform(&tempVec);
	}
	*pResult = 0;
}


void CTabObject::OnDeltaposZCount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_vec3 tempVec = { X_Counter, Y_Counter, Z_Counter };
	if (pNMUpDown->iDelta < 0)
	{
		tempVec.z += 0.1f;
		Set_Transform(&tempVec);
	}
	else
	{
		tempVec.z -= 0.1f;
		Set_Transform(&tempVec);
	}
	*pResult = 0;
}


void CTabObject::OnDeltaposRotX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_vec3 tempVec = { m_RotateX, m_RotateY, m_RotateZ };
	if (pNMUpDown->iDelta < 0)
	{
		tempVec.x += 0.01f;
		Set_Rotate(tempVec);
	}
	else
	{
		tempVec.x -= 0.01f;
		Set_Rotate(tempVec);
	}
	*pResult = 0;
}


void CTabObject::OnDeltaposRotY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_vec3 tempVec = { m_RotateX, m_RotateY, m_RotateZ };
	if (pNMUpDown->iDelta < 0)
	{
		tempVec.y += 0.01f;
		Set_Rotate(tempVec);
	}
	else
	{
		tempVec.y -= 0.01f;
		Set_Rotate(tempVec);
	}
	*pResult = 0;
}


void CTabObject::OnDeltaposRotZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_vec3 tempVec = { m_RotateX, m_RotateY, m_RotateZ };
	if (pNMUpDown->iDelta < 0)
	{
		tempVec.z += 0.01f;
		Set_Rotate(tempVec);
	}
	else
	{
		tempVec.z -= 0.01f;
		Set_Rotate(tempVec);
	}
	*pResult = 0;
}


void CTabObject::OnDeltaposScale(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_float tempScale = m_Scale;
	if (pNMUpDown->iDelta < 0)
	{
		tempScale += 0.01f;
		Set_Scale(tempScale);
	}
	else
	{
		tempScale -= 0.01f;
		Set_Scale(tempScale);
	}
	*pResult = 0;
}


void CTabObject::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	_int iXPos = m_XCounterSlider.GetPos();
	_int iYPos = m_YCounterSlider.GetPos();
	_int iZPos = m_ZCounterSlider.GetPos();

	_int iXRot = m_RotateXSlider.GetPos();
	_int iYRot = m_RotateYSlider.GetPos();
	_int iZRot = m_RotateZSlider.GetPos();

	_int iScale = m_ScaleSlider.GetPos();

	_vec3 tempPosVec = {};
	tempPosVec.x = (iXPos / 100.f);
	tempPosVec.y = (iYPos / 100.f);
	tempPosVec.z = (iZPos / 100.f);

	Set_Transform(&tempPosVec);

	_vec3 tempRotVec = {};
	tempRotVec.x = (iXRot / 100.f);
	tempRotVec.y = (iYRot / 100.f);
	tempRotVec.z = (iZRot / 100.f);

	Set_Rotate(tempRotVec);

	_float tempScale = iScale / 100.f;
	Set_Scale(tempScale);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}





void CTabObject::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CPoint p;
	GetCursorPos(&p);
	UINT flag;
	m_ObjectTree.ScreenToClient(&p);
	HTREEITEM hItem_dc = m_ObjectTree.HitTest(p, &flag);
	HTREEITEM hItem_parent = m_ObjectTree.GetParentItem(hItem_dc);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCView* pView = dynamic_cast<CMFCView*>(pMain->m_MainSplitter.GetPane(0, 1));
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	pForm->m_ptabChar->Set_Object(nullptr);
	m_pObject = pView->CreateObject(L"GameLogic", m_ObjectTree.GetItemText(hItem_parent), m_ObjectTree.GetItemText(hItem_dc));
	Get_Transform();
	Get_Rotate();
	Get_Scale();

	*pResult = 0;
}





void CTabObject::OnBnClickedDeleteObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pObject != nullptr)
	{
		m_pObject->Set_Dead(true);
	}
}


void CTabObject::OnBnClickedBuildingSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"Building.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir); //현재 디렉토리 반환. ..../3D_시연회//MFC//Include
	PathRemoveFileSpec(szCurDir); //끝에 경로 제거. //Include 제거
	PathRemoveFileSpec(szCurDir); //끝에 경로 제거. //MFC 제거
	lstrcat(szCurDir, L"\\ReSource");
	lstrcat(szCurDir, L"\\Data");
	lstrcat(szCurDir, L"\\Object"); //
	Dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == Dlg.DoModal()) //저장 시 시작 경로
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		DWORD dwByte = 0;
		DWORD dwStringCount = 0;
		//순회하면서 키와 리스트 아이템들 저장
		list<CGameObject*> pPlayerlst = Engine::Get_List(L"GameLogic", L"Building");

		//저장요소. 오브젝트 태그명(proto type name), pos, rotate, scale 
		for (CGameObject* pObj : pPlayerlst)
		{
			CTransform* pTransCom = (CTransform*)pObj->Get_Component(L"Com_Transform", ID_DYNAMIC);
			_vec3 SavePos = {};
			_vec3 SaveRot = { pTransCom->Get_Rotate(ROT_X),pTransCom->Get_Rotate(ROT_Y),pTransCom->Get_Rotate(ROT_Z) };
			_vec3 SaveScale = { pTransCom->Get_Scale(SCALE_X),pTransCom->Get_Scale(SCALE_Y),pTransCom->Get_Scale(SCALE_Z) };
			pTransCom->Get_Info(INFO_POS, &SavePos);
			dwStringCount = (pObj->Get_NameTag().length() + 1) * sizeof(_tchar);
			WriteFile(hFile, &dwStringCount, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, pObj->Get_NameTag().c_str(), dwStringCount, &dwByte, nullptr);
			WriteFile(hFile, &SavePos, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &SaveRot, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &SaveScale, sizeof(_vec3), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
}


void CTabObject::OnBnClickedBuildingLoad()
{
	CFileDialog Dlg(TRUE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"Building.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	PathRemoveFileSpec(szCurDir); //끝에 경로 제거. //Include 제거
	PathRemoveFileSpec(szCurDir); //끝에 경로 제거. //MFC 제거
	lstrcat(szCurDir, L"\\ReSource");
	lstrcat(szCurDir, L"\\Data");
	lstrcat(szCurDir, L"\\Object");
	Dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		//리스트 초기화 진행 
		Clear_List(L"GameLogic", L"Building");
		DWORD dwByte = 0;
		DWORD dwStringCount = 0;
		TCHAR* szBuf = nullptr;
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMFCView* pView = dynamic_cast<CMFCView*>(pMain->m_MainSplitter.GetPane(0, 1));

		//로드 순서 1. 네임태그 2. pos 3. rotate 4. scale
		while (true)
		{
			CGameObject* pObj = nullptr;
			wstring wstrNametag;
			_vec3   LoadPos = {};
			_vec3	LoadRot = {};
			_vec3	LoadScale = {};
			ReadFile(hFile, &dwStringCount, sizeof(DWORD), &dwByte, nullptr);
			if (0 == dwByte)
				break;//읽어올 게 없으면 종료
			szBuf = new TCHAR[dwStringCount];
			ReadFile(hFile, szBuf, dwStringCount, &dwByte, nullptr);
			wstrNametag = szBuf;
			Safe_Delete_Array(szBuf);
			ReadFile(hFile, &LoadPos, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &LoadRot, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &LoadScale, sizeof(_vec3), &dwByte, nullptr);
			pObj = pView->CreateObject(L"GameLogic", L"Building", wstrNametag.c_str());
			CTransform* pTransCom = (CTransform*)pObj->Get_Component(L"Com_Transform", ID_DYNAMIC);
			pTransCom->Set_Pos(&LoadPos);
			pTransCom->Rotation2(ROT_X, LoadRot.x);
			pTransCom->Rotation2(ROT_Y, LoadRot.y);
			pTransCom->Rotation2(ROT_Z, LoadRot.z);
			pTransCom->Set_Scale(LoadScale.x, LoadScale.y, LoadScale.z);
		}
		CloseHandle(hFile);
	}
}


void CTabObject::OnBnClickedStuffSave()
{
	CFileDialog Dlg(FALSE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"Stuff.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir); //현재 디렉토리 반환. ..../3D_시연회//MFC//Include
	PathRemoveFileSpec(szCurDir); //끝에 경로 제거. //Include 제거
	PathRemoveFileSpec(szCurDir); //끝에 경로 제거. //MFC 제거
	lstrcat(szCurDir, L"\\ReSource");
	lstrcat(szCurDir, L"\\Data");
	lstrcat(szCurDir, L"\\Object"); //
	Dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == Dlg.DoModal()) //저장 시 시작 경로
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		DWORD dwByte = 0;
		DWORD dwStringCount = 0;
		//순회하면서 키와 리스트 아이템들 저장
		list<CGameObject*> pPlayerlst = Engine::Get_List(L"GameLogic", L"Stuff");

		//저장요소. 오브젝트 태그명(proto type name), pos, rotate, scale 
		for (CGameObject* pObj : pPlayerlst)
		{
			CTransform* pTransCom = (CTransform*)pObj->Get_Component(L"Com_Transform", ID_DYNAMIC);
			_vec3 SavePos = {};
			_vec3 SaveRot = { pTransCom->Get_Rotate(ROT_X),pTransCom->Get_Rotate(ROT_Y),pTransCom->Get_Rotate(ROT_Z) };
			_vec3 SaveScale = { pTransCom->Get_Scale(SCALE_X),pTransCom->Get_Scale(SCALE_Y),pTransCom->Get_Scale(SCALE_Z) };
			pTransCom->Get_Info(INFO_POS, &SavePos);
			dwStringCount = (pObj->Get_NameTag().length() + 1) * sizeof(_tchar);
			WriteFile(hFile, &dwStringCount, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, pObj->Get_NameTag().c_str(), dwStringCount, &dwByte, nullptr);
			WriteFile(hFile, &SavePos, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &SaveRot, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &SaveScale, sizeof(_vec3), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
}


void CTabObject::OnBnClickedStuffLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"Stuff.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	PathRemoveFileSpec(szCurDir); //끝에 경로 제거. //Include 제거
	PathRemoveFileSpec(szCurDir); //끝에 경로 제거. //MFC 제거
	lstrcat(szCurDir, L"\\ReSource");
	lstrcat(szCurDir, L"\\Data");
	lstrcat(szCurDir, L"\\Object");
	Dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		//리스트 초기화 진행 
		Clear_List(L"GameLogic", L"Stuff");
		DWORD dwByte = 0;
		DWORD dwStringCount = 0;
		TCHAR* szBuf = nullptr;
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMFCView* pView = dynamic_cast<CMFCView*>(pMain->m_MainSplitter.GetPane(0, 1));

		//로드 순서 1. 네임태그 2. pos 3. rotate 4. scale
		while (true)
		{
			CGameObject* pObj = nullptr;
			wstring wstrNametag;
			_vec3   LoadPos = {};
			_vec3	LoadRot = {};
			_vec3	LoadScale = {};
			ReadFile(hFile, &dwStringCount, sizeof(DWORD), &dwByte, nullptr);
			if (0 == dwByte)
				break;//읽어올 게 없으면 종료
			szBuf = new TCHAR[dwStringCount];
			ReadFile(hFile, szBuf, dwStringCount, &dwByte, nullptr);
			wstrNametag = szBuf;
			Safe_Delete_Array(szBuf);
			ReadFile(hFile, &LoadPos, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &LoadRot, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &LoadScale, sizeof(_vec3), &dwByte, nullptr);
			pObj = pView->CreateObject(L"GameLogic", L"Stuff", wstrNametag.c_str());
			CTransform* pTransCom = (CTransform*)pObj->Get_Component(L"Com_Transform", ID_DYNAMIC);
			pTransCom->Set_Pos(&LoadPos);
			pTransCom->Rotation2(ROT_X, LoadRot.x);
			pTransCom->Rotation2(ROT_Y, LoadRot.y);
			pTransCom->Rotation2(ROT_Z, LoadRot.z);
			pTransCom->Set_Scale(LoadScale.x, LoadScale.y, LoadScale.z);
		}
		CloseHandle(hFile);
	}
}
