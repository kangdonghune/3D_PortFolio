#pragma once

#include "Sphrer.h"
#include "Export_Function.h"
// CTabTerrain 대화 상자입니다.

class CTabTerrain : public CDialogEx
{
	DECLARE_DYNAMIC(CTabTerrain)

public:
	CTabTerrain(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabTerrain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Terrain };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CSphere*			m_pShprer = nullptr;

public:
	CSphere*		Create_Sphrer(LPDIRECT3DDEVICE9 pGraphicDev,_vec3* pPos);

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnDestroy();
};
