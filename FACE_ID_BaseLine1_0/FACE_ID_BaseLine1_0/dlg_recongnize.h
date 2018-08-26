#pragma once
#include "afxcmn.h"
#include "global_var.h"

// dlg_recongnize �Ի���

class dlg_recongnize : public CDialogEx
{
	DECLARE_DYNAMIC(dlg_recongnize)

public:
	dlg_recongnize(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~dlg_recongnize();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECONGNIZE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
