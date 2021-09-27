#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "GameObject.h"

// CTabChar 대화 상자입니다.

class CTabChar : public CDialogEx
{
	DECLARE_DYNAMIC(CTabChar)

public:
	CTabChar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabChar();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Char };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeTask;
	virtual BOOL OnInitDialog();
	CGameObject* m_pObject = nullptr;

public:
	void	OnUpdate();
	void	Get_Transform();
	void	Set_Object(CGameObject* pObj) { m_pObject = pObj; }
	void	Set_Transform(_vec3* MovePos);

	void	Get_Rotate();
	void	Set_Rotate(_vec3 vecRotate);

	void	Get_Scale();
	void	Set_Scale(_float fScale);


	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	float X_Counter;
	float Y_Counter;
	float Z_Counter;


	float m_RotateX;
	float m_RotateY;
	float m_RotateZ;
	float m_Scale;

	CSliderCtrl m_XCountSlider;
	CSliderCtrl m_YCountSlider;
	CSliderCtrl m_ZCountSlider;
	CSliderCtrl m_RotateXSlider;
	CSliderCtrl m_RotateYSlider;
	CSliderCtrl m_RotateZSlider;
	CSliderCtrl m_ScaleSlider;

	afx_msg void OnEnChangeEdit1();//PosX
	afx_msg void OnEnChangeEdit2();//PosY
	afx_msg void OnEnChangeEdit3();//PosZ

	afx_msg void OnEnChangeRotX();
	afx_msg void OnEnChangeRotY();
	afx_msg void OnEnChangeRotZ();
	afx_msg void OnEnChangeScale();

	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin8(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	

	afx_msg void OnBnClickedCharDelete();

};
