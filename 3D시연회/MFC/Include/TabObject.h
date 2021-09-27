#pragma once
#include "afxcmn.h"
#include "GameObject.h"

// CTabObject 대화 상자입니다.

class CTabObject : public CDialogEx
{
	DECLARE_DYNAMIC(CTabObject)

public:
	CTabObject(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabObject();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Object };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	CTreeCtrl m_ObjectTree;
	CGameObject* m_pObject = nullptr;

	float X_Counter;
	float Y_Counter;
	float Z_Counter;

	float m_RotateX;
	float m_RotateY;
	float m_RotateZ;

	float m_Scale;

	CSliderCtrl m_XCounterSlider;
	CSliderCtrl m_YCounterSlider;
	CSliderCtrl m_ZCounterSlider;
	CSliderCtrl m_RotateXSlider;
	CSliderCtrl m_RotateYSlider;
	CSliderCtrl m_RotateZSlider;
	CSliderCtrl m_ScaleSlider;

public:

	void	Get_Transform();
	void	Set_Object(CGameObject* pObj) { m_pObject = pObj; }
	void	Set_Transform(_vec3* MovePos);

	void	Get_Rotate();
	void	Set_Rotate(_vec3 vecRotate);

	void	Get_Scale();
	void	Set_Scale(_float fScale);
	virtual BOOL OnInitDialog();
	



	afx_msg void OnEnChangeXCounter();
	afx_msg void OnEnChangeYCounter();
	afx_msg void OnEnChangeZCounter();
	afx_msg void OnEnChangeRotX();
	afx_msg void OnEnChangeRotY();
	afx_msg void OnEnChangeRotZ();
	afx_msg void OnEnChangeScale();

	afx_msg void OnDeltaposXCount(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposYCount(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposZCount(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRotX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRotY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRotZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposScale(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedDeleteObject();
};
