// TabChar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC.h"
#include "TabChar.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFCView.h"
#include "Player.h"


#include "Export_Function.h"


// CTabChar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabChar, CDialogEx)


CTabChar::CTabChar(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Char, pParent)
	, X_Counter(0)
	, Y_Counter(0)
	, Z_Counter(0)
	, m_RotateX(0)
	, m_RotateY(0)
	, m_RotateZ(0)
	, m_Scale(0)
{

}

CTabChar::~CTabChar()
{
}

void CTabChar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeTask);

	DDX_Text(pDX, IDC_EDIT1, X_Counter);
	DDX_Text(pDX, IDC_EDIT2, Y_Counter);
	DDX_Text(pDX, IDC_EDIT3, Z_Counter);

	DDX_Control(pDX, IDC_SLIDER3, m_XCountSlider);
	DDX_Control(pDX, IDC_SLIDER2, m_YCountSlider);
	DDX_Control(pDX, IDC_SLIDER4, m_ZCountSlider);
	DDX_Text(pDX, IDC_EDIT5, m_RotateX);
	DDX_Text(pDX, IDC_EDIT6, m_RotateY);
	DDX_Text(pDX, IDC_EDIT7, m_RotateZ);
	DDX_Text(pDX, IDC_EDIT8, m_Scale);
	DDX_Control(pDX, IDC_SLIDER5, m_RotateXSlider);
	DDX_Control(pDX, IDC_SLIDER6, m_RotateYSlider);
	DDX_Control(pDX, IDC_SLIDER7, m_RotateZSlider);
	DDX_Control(pDX, IDC_SLIDER8, m_ScaleSlider);
}


BEGIN_MESSAGE_MAP(CTabChar, CDialogEx)

	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CTabChar::OnNMDblclkTree1)
	ON_EN_CHANGE(IDC_EDIT1, &CTabChar::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CTabChar::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CTabChar::OnEnChangeEdit3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTabChar::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTabChar::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTabChar::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CTabChar::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CTabChar::OnDeltaposSpin3)

	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT5, &CTabChar::OnEnChangeRotX)
	ON_EN_CHANGE(IDC_EDIT6, &CTabChar::OnEnChangeRotY)
	ON_EN_CHANGE(IDC_EDIT7, &CTabChar::OnEnChangeRotZ)
	ON_EN_CHANGE(IDC_EDIT8, &CTabChar::OnEnChangeScale)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CTabChar::OnDeltaposSpin5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CTabChar::OnDeltaposSpin6)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CTabChar::OnDeltaposSpin7)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CTabChar::OnDeltaposSpin8)
				ON_BN_CLICKED(IDC_BUTTON1, &CTabChar::OnBnClickedCharDelete)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON3, &CTabChar::OnBnClickedUnitSave)

	ON_BN_CLICKED(IDC_BUTTON4, &CTabChar::OnBnClickedMonsterSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CTabChar::OnBnClickedLoad)
END_MESSAGE_MAP()


// CTabChar 메시지 처리기입니다.


BOOL CTabChar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Tree
	CString cst_Root;
	HTREEITEM h_Root;
	HTREEITEM h_entryPlayer, h_entryMonster;


	cst_Root.Format(_T("Root"));
	h_Root = m_TreeTask.InsertItem(cst_Root, NULL, NULL);

	h_entryPlayer = m_TreeTask.InsertItem(_T("Player"), h_Root, NULL);
	m_TreeTask.InsertItem(_T("Proto_Mesh_Hunter"), h_entryPlayer, NULL);

	h_entryMonster = m_TreeTask.InsertItem(_T("Monster"), h_Root, NULL);
	m_TreeTask.InsertItem(_T("Proto_Mesh_Goblin"), h_entryMonster, NULL);
	//

	//Slider

	m_XCountSlider.SetRange(0, VTXCNTX * 100);
	m_XCountSlider.SetPos(0);
	m_XCountSlider.SetLineSize(1);


	m_YCountSlider.SetRange(0, VTXCNTX * 100);
	m_YCountSlider.SetPos(0);
	m_YCountSlider.SetLineSize(1);

	m_ZCountSlider.SetRange(0, VTXCNTZ * 100);
	m_ZCountSlider.SetPos(0);
	m_ZCountSlider.SetLineSize(1);


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


void CTabChar::OnUpdate()
{

}

void CTabChar::Get_Transform()
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

	m_XCountSlider.SetPos(X_Counter * 100);
	m_YCountSlider.SetPos(Y_Counter * 100);
	m_ZCountSlider.SetPos(Z_Counter * 100);


	UpdateData(false);
}

void CTabChar::Set_Transform(_vec3* MovePos)
{
	if (m_pObject == nullptr)
		return;

	UpdateData(true);
	CTransform* pTrans = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	pTrans->Set_Pos(MovePos);
	Get_Transform();

	UpdateData(false);
}



void CTabChar::Get_Rotate()
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

void CTabChar::Set_Rotate(_vec3 vecRotate)
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

void CTabChar::Get_Scale()
{
	if (m_pObject == nullptr)
		return;

	UpdateData(true);
	CTransform* pTrans = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);

	m_Scale = pTrans->Get_Scale(SCALE_X);

	m_ScaleSlider.SetPos(m_Scale * 100);

	UpdateData(false);
}

void CTabChar::Set_Scale(_float fScale)
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

void CTabChar::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CPoint p;
	GetCursorPos(&p);
	UINT flag;
	m_TreeTask.ScreenToClient(&p);
	HTREEITEM hItem_dc = m_TreeTask.HitTest(p, &flag);
	HTREEITEM hItem_parent = m_TreeTask.GetParentItem(hItem_dc);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCView* pView = dynamic_cast<CMFCView*>(pMain->m_MainSplitter.GetPane(0, 1));

	m_pObject = pView->CreateCharictor(L"GameLogic", m_TreeTask.GetItemText(hItem_parent), m_TreeTask.GetItemText(hItem_dc));
	Get_Transform();
	Get_Rotate();
	Get_Scale();

	OnUpdate();
	*pResult = 0;
}


void CTabChar::OnEnChangeEdit1()
{
	if (m_pObject == nullptr)
		return;

	UpdateData(true);
	//Xcount
	CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 InputPos = { X_Counter, Y_Counter, Z_Counter };
	pTransCom->Set_Pos(&InputPos);

	m_XCountSlider.SetPos(X_Counter * 100);
	m_YCountSlider.SetPos(Y_Counter * 100);
	m_ZCountSlider.SetPos(Z_Counter * 100);
	UpdateData(false);
}


void CTabChar::OnEnChangeEdit2()
{
	if (m_pObject == nullptr)
		return;

	UpdateData(true);
	//Ycount
	CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 InputPos = { X_Counter, Y_Counter, Z_Counter };
	pTransCom->Set_Pos(&InputPos);

	m_XCountSlider.SetPos(X_Counter * 100);
	m_YCountSlider.SetPos(Y_Counter * 100);
	m_ZCountSlider.SetPos(Z_Counter * 100);
	UpdateData(false);
}


void CTabChar::OnEnChangeEdit3()
{
	if (m_pObject == nullptr)
		return;

	UpdateData(true);
	//Zcount
	CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 InputPos = { X_Counter, Y_Counter, Z_Counter };
	pTransCom->Set_Pos(&InputPos);


	m_XCountSlider.SetPos(X_Counter * 100);
	m_YCountSlider.SetPos(Y_Counter * 100);
	m_ZCountSlider.SetPos(Z_Counter * 100);
	UpdateData(false);
}








void CTabChar::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pObject == nullptr)
		return;

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


void CTabChar::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pObject == nullptr)
		return;

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


void CTabChar::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pObject == nullptr)
		return;

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





void CTabChar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_pObject == nullptr)
		return;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	_int iXPos = m_XCountSlider.GetPos();
	_int iYPos = m_YCountSlider.GetPos();
	_int iZPos = m_ZCountSlider.GetPos();
	
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


void CTabChar::OnEnChangeRotX()
{
	if (m_pObject == nullptr)
		return;
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


void CTabChar::OnEnChangeRotY()
{
	if (m_pObject == nullptr)
		return;

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


void CTabChar::OnEnChangeRotZ()
{
	if (m_pObject == nullptr)
		return;

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


void CTabChar::OnEnChangeScale()
{
	if (m_pObject == nullptr)
		return;

	UpdateData(true);
	//Rotx
	CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
	pTransCom->Set_Scale(m_Scale, m_Scale, m_Scale);

	m_ScaleSlider.SetPos(m_Scale * 100);

	UpdateData(false);
}


void CTabChar::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult) //rot x
{
	if (m_pObject == nullptr)
		return;

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


void CTabChar::OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pObject == nullptr)
		return;

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


void CTabChar::OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pObject == nullptr)
		return;
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


void CTabChar::OnDeltaposSpin8(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pObject == nullptr)
		return;

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


void CTabChar::OnBnClickedCharDelete()
{	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pObject != nullptr)
	{
		m_pObject->Set_Dead(true);
	}
}	




void CTabChar::OnBnClickedUnitSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"Player.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir); //현재 디렉토리 반환. ..../3D_시연회//MFC//Include
	PathRemoveFileSpec(szCurDir); //끝에 경로 제거. //Include 제거
	PathRemoveFileSpec(szCurDir); //끝에 경로 제거. //MFC 제거
	lstrcat(szCurDir, L"\\ReSource");
	lstrcat(szCurDir, L"\\Data");
	lstrcat(szCurDir, L"\\Unit"); //
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
		list<CGameObject*> pPlayerlst = Engine::Get_List(L"GameLogic", L"Player");
	
		//저장요소. 오브젝트 태그명(proto type name), pos, rotate, scale 
		for (CGameObject* pObj : pPlayerlst)
		{
			CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
			_vec3 SavePos = {};
			_vec3 SaveRot = { pTransCom->Get_Rotate(ROT_X),pTransCom->Get_Rotate(ROT_Y),pTransCom->Get_Rotate(ROT_Z)};
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




void CTabChar::OnBnClickedMonsterSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"Monster.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir); //현재 디렉토리 반환. ..../3D_시연회//MFC//Include
	PathRemoveFileSpec(szCurDir); //끝에 경로 제거. //Include 제거
	PathRemoveFileSpec(szCurDir); //끝에 경로 제거. //MFC 제거
	lstrcat(szCurDir, L"\\ReSource");
	lstrcat(szCurDir, L"\\Data");
	lstrcat(szCurDir, L"\\Unit"); //
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
		list<CGameObject*> pPlayerlst = Engine::Get_List(L"GameLogic", L"Monster");

		//저장요소. 오브젝트 태그명(proto type name), pos, rotate, scale 
		for (CGameObject* pObj : pPlayerlst)
		{
			CTransform* pTransCom = (CTransform*)m_pObject->Get_Component(L"Com_Transform", ID_DYNAMIC);
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


void CTabChar::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
