#pragma once
#include "afxcmn.h"
#include "global_var.h"

// dlg_recongnize 对话框

class dlg_recongnize : public CDialogEx
{
	DECLARE_DYNAMIC(dlg_recongnize)

public:
	dlg_recongnize(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~dlg_recongnize();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECONGNIZE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString eidt_rec_cmd;
	CSliderCtrl thresholdSlider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedShot();
	afx_msg void OnBnClickedReshot();
	afx_msg void OnBnClickedRecongnize();
};
