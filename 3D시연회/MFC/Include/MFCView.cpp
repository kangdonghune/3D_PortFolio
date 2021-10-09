
// MFCView.cpp : CMFCView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFC.h"
#endif

#include "MFCDoc.h"
#include "MFCView.h"
#include "Stage.h"
#include "Define.h"
#include "MainFrm.h"
#include "Player.h"
#include "Monster.h"
#include "Object.h"
#include "Building.h"
#include "Form.h"
#include "TabChar.h"
#include "TabObject.h"
#include "TabTerrain.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMFCView

HWND g_HWnd;
HINSTANCE g_HInst;

IMPLEMENT_DYNCREATE(CMFCView, CView)

BEGIN_MESSAGE_MAP(CMFCView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCView ����/�Ҹ�

CMFCView::CMFCView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.


}

CMFCView::~CMFCView()
{
	Release_Utility();
	Release_System();
}

BOOL CMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFCView �׸���

void CMFCView::OnDraw(CDC* /*pDC*/)
{
	CMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMFCView �μ�

BOOL CMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFCView ����

#ifdef _DEBUG
void CMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

HRESULT CMFCView::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	g_HWnd = m_hWnd;
	g_HInst = AfxGetInstanceHandle();

	Ready_GraphicDev(g_HWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass);

	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_Device();
	(*ppGraphicDev)->AddRef();



	// ��Ʈ

	Ready_Font((*ppGraphicDev), L"Font_Default", L"����", 15, 20, FW_NORMAL);
	Ready_Font((*ppGraphicDev), L"Font_Jinji", L"�ü�", 30, 30, FW_HEAVY);

	// InputDev

	Ready_InputDev(g_HInst, g_HWnd);


	return S_OK;
}

HRESULT CMFCView::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, CManagement ** ppManagementClass)
{
	CScene*		pScene = nullptr;

	Create_Management(pGraphicDev, ppManagementClass);

	(*ppManagementClass)->AddRef();

	pScene = CStage::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	if (FAILED((*ppManagementClass)->Set_Scene(pScene)))
		return E_FAIL;

	return S_OK;
}

int CMFCView::Update_MainView(const float & fTimeDelta)
{
	if (nullptr == m_pManagementClass)
		return -1;

	Update_InputDev();



	/*_long			MouseMove = 0;

	if (MouseMove = Get_DIMouseMove(DIMS_Z))
	{
	int a = 0;
	}*/


	m_pManagementClass->Update_Scene(fTimeDelta);

	return 0;
}

void CMFCView::Render_Scene()
{

	if (nullptr == m_pManagementClass)
		return;

	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	m_pDeviceClass->Render_End();

	Invalidate(false);
}

CMFCDoc* CMFCView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDoc)));
	return (CMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCView �޽��� ó����


void CMFCView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	SetUp_DefaultSetting(&m_pGraphicDev);
	Ready_Scene(m_pGraphicDev, &m_pManagementClass);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	RECT rcMain{};
	pMain->GetWindowRect(&rcMain);
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);
	RECT rcView{};
	GetClientRect(&rcView);
	int iGapX = rcMain.right - rcView.right; //1440 - 1420 = 20
	int iGapY = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, WINCX + iGapX + 1, WINCY + iGapY + 1, SWP_NOMOVE);
	m_pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//CGameObject* pGameObject = nullptr;
	//pGameObject = CPlayer::Create(m_pGraphicDev, L"Proto_Mesh_Hunter");
	//m_pManagementClass->Get_Scene()->Add_GameObject(L"GameLogic", L"Proto_Mesh_Hunter", pGameObject);
}

CGameObject* CMFCView::CreateCharictor(Layer type, const _tchar * pParentName, const _tchar * pObjProtoName)
{
	CGameObject* pGameObject = nullptr;
	if (!lstrcmp(L"Player", pParentName))
	{
		pGameObject = CPlayer::Create(m_pGraphicDev, pObjProtoName);
		m_pManagementClass->Get_Scene()->Add_GameObject(type, L"Player", pGameObject);
		m_pForm->m_ptabObject->Set_Object(nullptr);
		return pGameObject;
	}

	if (!lstrcmp(L"Monster", pParentName))
	{
		pGameObject = CMonster::Create(m_pGraphicDev, pObjProtoName);
		m_pManagementClass->Get_Scene()->Add_GameObject(type, L"Monster", pGameObject);
		m_pForm->m_ptabObject->Set_Object(nullptr);
		return pGameObject;
	}



	if (pGameObject == nullptr)
		return nullptr;

	
}

CGameObject * CMFCView::CreateObject(Layer type, const _tchar * pParentName, const _tchar * pObjProtoName)
{
	CGameObject* pGameObject = nullptr;
	if (!lstrcmp(L"Building", pParentName))
	{
		pGameObject = CBuilding::Create(m_pGraphicDev, pObjProtoName);
		m_pManagementClass->Get_Scene()->Add_GameObject(type, L"Building", pGameObject);
		m_pForm->m_ptabObject->Set_Object(nullptr);
		return pGameObject;
	}

	if (!lstrcmp(L"Object", pParentName))
	{
		pGameObject = C_Object::Create(m_pGraphicDev, pObjProtoName);
		m_pManagementClass->Get_Scene()->Add_GameObject(type, L"Object", pGameObject);
		m_pForm->m_ptabObject->Set_Object(nullptr);
		return pGameObject;
	}
	if (pGameObject == nullptr)
		return nullptr;

	m_pManagementClass->Get_Scene()->Add_GameObject(type, pObjProtoName, pGameObject);
	m_pForm->m_ptabChar->Set_Object(nullptr);
	return pGameObject;
}


