// dlg_create.cpp : 实现文件
//

#include "stdafx.h"
#include "FACE_ID_BaseLine1_0.h"
#include "dlg_create.h"
#include "afxdialogex.h"


// dlg_create 对话框

IMPLEMENT_DYNAMIC(dlg_create, CDialog)

dlg_create::dlg_create(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CREATE, pParent)
	, edit_userId(_T(""))
	, edit_userDescription(_T(""))
	, edit_dimOfVector(0)
	, edit_sizeOfCodebook(0)
	, edit_numberOfSample(0)
{

}





void dlg_create::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_userId, edit_userId);
	DDX_Text(pDX, IDC_EDIT_userDescription, edit_userDescription);
	DDX_Text(pDX, IDC_EDIT_dimOfVector, edit_dimOfVector);
	DDV_MinMaxInt(pDX, edit_dimOfVector, 2, 8);
	DDX_Text(pDX, IDC_EDIT_sizeOfCodebook, edit_sizeOfCodebook);
	DDX_Text(pDX, IDC_EDIT_numberOfSample, edit_numberOfSample);
}


BEGIN_MESSAGE_MAP(dlg_create, CDialog)
	ON_BN_CLICKED(IDC_SHOT, &dlg_create::OnBnClickedShot)
	ON_BN_CLICKED(IDC_CONFIG, &dlg_create::OnBnClickedConfig)
	ON_BN_CLICKED(IDC_NEW, &dlg_create::OnBnClickedNew)
	ON_BN_CLICKED(IDC_CUTSTREAM, &dlg_create::OnBnClickedCutstream)
	ON_BN_CLICKED(IDC_RESTART, &dlg_create::OnBnClickedRestart)
	ON_BN_CLICKED(IDC_QUIT, &dlg_create::OnBnClickedQuit)
	ON_BN_CLICKED(IDC_CANCEL, &dlg_create::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_NEXT, &dlg_create::OnBnClickedNext)
	ON_BN_CLICKED(IDC_DONECOLLECT, &dlg_create::OnBnClickedDonecollect)
END_MESSAGE_MAP()


// dlg_create 消息处理程序


void dlg_create::OnBnClickedShot()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!flagInitCreate)
	{
		CRect rect;
		GetDlgItem(IDC_STATIC)->GetClientRect(rect);
		cv::namedWindow("myshowWnd");
		cv::resizeWindow("myshowWnd", rect.Width(), rect.Height());
		HWND hWnd = (HWND)cvGetWindowHandle("myshowWnd");
		HWND hParent = ::GetParent(hWnd);
		::SetParent(hWnd, GetDlgItem(IDC_STATIC)->m_hWnd);
		::ShowWindow(hParent, SW_HIDE);
		flagInitCreate = true;
	}
	flagShot = true;
	cv::waitKey(200);
	cv::Mat img = cv::imread("./tmp/face.png");
	cv::imshow("myshowWnd", background);
	cv::waitKey(5);
	cv::imshow("myshowWnd", img);
	cv::waitKey(5);




}


void dlg_create::OnBnClickedConfig()
{
	if (currentFace != NULL) 
	{	
		UpdateData(true);
		string buf1 = (string)(CStringA)edit_userId;
		string buf2 = (string)(CStringA)edit_userDescription;
		char buf_id[100] = "", buf_dec[500] = "";
		strcat(buf_id, buf1.c_str());
		strcat(buf_dec, buf2.c_str());
		int buf_dimV = edit_dimOfVector;
		int buf_sizeC = edit_sizeOfCodebook;
		currentFace->createHeader(buf_id, buf_dec, buf_sizeC, buf_dimV);
	}
	else 
	{
		AfxMessageBox(_T("You must NEW a user first before update configuration!"), MB_OK, MB_ICONSTOP);
	}
}


void dlg_create::OnBnClickedNew()
{
	if (currentFace == NULL) 
	{
		currentFace = new FACE();
		_cprintf("\n\nNew user instance is created.\n\n");
	}
	else 
	{
		if (AfxMessageBox(_T("Unprocessed profile detected, NEW profile will reset current profile!"), MB_OKCANCEL, MB_ICONEXCLAMATION)) 
		{
			currentFace->reset();
			UpdateData(true);
			edit_userId = _T("");
			edit_userDescription = _T("");
			edit_numberOfSample = 0;
			edit_dimOfVector = 4;
			edit_sizeOfCodebook = 179;
			UpdateData(false);
		}
	}
	
}


void dlg_create::OnBnClickedCutstream()
{
	if (flagStream) 
	{
		flagStream = false;
		_cprintf("\n\nStream is paused.\n\n");
	}
	else 
	{
		flagStream = true;
		_cprintf("\n\nStream is running now.\n\n");
	}
	// TODO: 在此添加控件通知处理程序代码
}


void dlg_create::OnBnClickedRestart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (currentFace != NULL) 
	{
		currentFace->reset();
		_cprintf("\n\nSetting and data of current profile are reset.\n\n");
		UpdateData(true);
		edit_userId = _T("");
		edit_userDescription = _T("");
		edit_numberOfSample = 0;
		edit_dimOfVector = 4;
		edit_sizeOfCodebook = 179;
		UpdateData(false);
	}
	else 
	{
		AfxMessageBox(_T("Click NEW to create a new profile."), MB_OK, MB_ICONINFORMATION);
	}
}


void dlg_create::OnBnClickedQuit()
{
	UpdateData(true);
	edit_userId = _T("");
	edit_userDescription = _T("");
	edit_numberOfSample = 0;
	edit_dimOfVector = 4;
	edit_sizeOfCodebook = 179;
	UpdateData(false);
	FACE* p = currentFace;
	if (p != NULL) 
	{
		free(p);
	}	
	this->ShowWindow(SW_HIDE);
	// TODO: 在此添加控件通知处理程序代码
}


void dlg_create::OnBnClickedCancel()
{
	currentFace->goBack();
	// TODO: 在此添加控件通知处理程序代码
}


void dlg_create::OnBnClickedNext()
{
	UpdateData(true);
	edit_numberOfSample = currentFace->checkNumberOfSamples();
	UpdateData(false);
	// TODO: 在此添加控件通知处理程序代码
}


void dlg_create::OnBnClickedDonecollect()
{
	if (currentFace->checkNumberOfSamples() < 10) 
	{
		AfxMessageBox(_T("Too less data for user, shot at least 10 samples."), MB_OK, MB_ICONWARNING);
	}
	else 
	{
		_cprintf("\n\nTraining... this will take a while.\n\n");
		flagStream = false;
		currentFace->train();
	}
	FACE* p = currentFace;
	free(p);
	currentFace = new FACE();
	UpdateData(true);
	edit_userId = _T("");
	edit_userDescription = _T("");
	edit_numberOfSample = 0;
	edit_dimOfVector = 4;
	edit_sizeOfCodebook = 179;
	UpdateData(false);
	flagStream = true;

	// TODO: 在此添加控件通知处理程序代码
}
