#pragma once
#include "common_header.h"
#include "FACE.h"

// dlg_create 对话框

class dlg_create : public CDialog
{
	DECLARE_DYNAMIC(dlg_create)

public:
	dlg_create(CWnd* pParent = NULL);   // 标准构造函数
	//virtual ~dlg_create();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString edit_userId;
	CString edit_userDescription;
	int edit_dimOfVector;
	int edit_sizeOfCodebook;
	int edit_numberOfSample;

private: 
	FACE* consNewUser;

public:
	afx_msg void OnBnClickedShot();
	afx_msg void OnBnClickedConfig();
	afx_msg void OnBnClickedNew();
	afx_msg void OnBnClickedCutstream();
	afx_msg void OnBnClickedRestart();
	afx_msg void OnBnClickedQuit();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedDonecollect();
};
