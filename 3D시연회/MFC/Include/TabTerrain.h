#pragma once

#include "Sphrer.h"
#include "Trigger.h"
#include "Export_Function.h"
// CTabTerrain 대화 상자입니다.

BEGIN(Engine)


END
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
	CSphere*			m_pShpere = nullptr;
	CTrigger*			m_pTrigger = nullptr;
	MFCCELL*			m_pCell = nullptr;
	CSphere*			m_pPointA = nullptr;
	CSphere*			m_pPointB = nullptr;
	CSphere*			m_pPointC = nullptr;
	_ulong				m_dwIndex = 0;



	float m_fXCount;
	float m_fYCount;
	float m_fZCount;
	TerrainTool::TYPE eType = TerrainTool::TERRAIN_END;

public:
	CSphere*		Create_Sphrer(LPDIRECT3DDEVICE9 pGraphicDev,_vec3* pPos);
	CTrigger*		Create_Triger(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos);

	void			Set_Sphere(CSphere* pShphere);
	void			Set_Trigger(CTrigger* pTrigger);
	void			Get_SphereInfo();
	
	void			InPut_Point(CSphere* pSphrer);
	void			Clear_Points();
	void			Create_MFCCell();

	void			Send_Info(_vec3* pPos);
	void			Update_VecCell();

	void			Delete_Cell();



	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnDestroy();
	afx_msg void OnEnChangeMoveX();
	afx_msg void OnEnChangeMoveY();
	afx_msg void OnEnChangeMoveZ();
	afx_msg void OnDeltaposX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSaveCell();
	afx_msg void OnBnClickedLoadCell();
	afx_msg void OnBnClickedSaveSphere();
	afx_msg void OnBnClickedLoadSphrer();
	afx_msg void OnBnClickedSaveTrigger();
	afx_msg void OnBnClickedLoadTrigger();

	afx_msg void OnBnClickedPointClear();
	int m_iPointANum;
	int m_iPointBNum;
	int m_iPointCNum;

	afx_msg void OnBnClickedDeleteSphere();
	afx_msg void OnBnClickedDeleteCell();
	afx_msg void OnBnClickedRadio4();

	afx_msg void OnBnClickedDeleteTrigger();
};
