#pragma once

#include "Sphrer.h"
#include "Export_Function.h"
// CTabTerrain ��ȭ �����Դϴ�.

class CTabTerrain : public CDialogEx
{
	DECLARE_DYNAMIC(CTabTerrain)

public:
	CTabTerrain(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTabTerrain();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Terrain };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CSphere*			m_pShprer = nullptr;

public:
	CSphere*		Create_Sphrer(LPDIRECT3DDEVICE9 pGraphicDev,_vec3* pPos);

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnDestroy();
};
