
// FACE_ID_BaseLine1_0Dlg.h : header file
//

#pragma once
#include "global_var.h"
#include "dlg_create.h"
#include "dlg_recongnize.h"


// CFACE_ID_BaseLine1_0Dlg dialog
class CFACE_ID_BaseLine1_0Dlg : public CDialogEx
{
// Construction
public:
	CFACE_ID_BaseLine1_0Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACE_ID_BASELINE1_0_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCamIn();
	CString cmd;
	afx_msg void OnBnClickedCamEx();
	afx_msg void OnBnClickedAbout();

private:
	dlg_create* dlgCreate = new dlg_create();
	dlg_recongnize* dlgRecongnize = new dlg_recongnize();
	

public:
	afx_msg void OnBnClickedRun();




	afx_msg void OnBnClickedEnableStream();
	afx_msg void OnBnClickedRunrec();
};
