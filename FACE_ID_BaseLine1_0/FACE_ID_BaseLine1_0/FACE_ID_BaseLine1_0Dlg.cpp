
// FACE_ID_BaseLine1_0Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "FACE_ID_BaseLine1_0.h"
#include "FACE_ID_BaseLine1_0Dlg.h"
#include "afxdialogex.h"
#include "face.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif





vector<char*> ListAllFilesInDir(const char *directory)
{
	vector<char*> list;
	WIN32_FIND_DATA file_data;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	int num = MultiByteToWideChar(0, 0, directory, -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, directory, -1, wide, num);
	if ((hFind = FindFirstFile(wide, &file_data)) == INVALID_HANDLE_VALUE)
	{
		_cprintf("\n\n Error: Can not find directory.\n\n");
		return list;
	}
	//cout << file_data.cFileName << endl;		
	while (FindNextFile(hFind, &file_data))
	{

		int num = WideCharToMultiByte(CP_OEMCP, NULL, file_data.cFileName, -1, NULL, 0, NULL, FALSE);
		char *pchar = new char[num];
		WideCharToMultiByte(CP_OEMCP, NULL, file_data.cFileName, -1, pchar, num, NULL, FALSE);
		//std::string s = pchar;
		list.push_back(pchar);
	}
	FindClose(hFind);
	return list;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFACE_ID_BaseLine1_0Dlg dialog



CFACE_ID_BaseLine1_0Dlg::CFACE_ID_BaseLine1_0Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FACE_ID_BASELINE1_0_DIALOG, pParent)
	, cmd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFACE_ID_BaseLine1_0Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, cmd);
}

BEGIN_MESSAGE_MAP(CFACE_ID_BaseLine1_0Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CAM_IN, &CFACE_ID_BaseLine1_0Dlg::OnBnClickedCamIn)
	ON_BN_CLICKED(IDC_CAM_EX, &CFACE_ID_BaseLine1_0Dlg::OnBnClickedCamEx)
	ON_BN_CLICKED(IDC_ABOUT, &CFACE_ID_BaseLine1_0Dlg::OnBnClickedAbout)
	ON_BN_CLICKED(IDC_RUN, &CFACE_ID_BaseLine1_0Dlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_ENABLE_STREAM, &CFACE_ID_BaseLine1_0Dlg::OnBnClickedEnableStream)
	ON_BN_CLICKED(IDC_RUNREC, &CFACE_ID_BaseLine1_0Dlg::OnBnClickedRunrec)
END_MESSAGE_MAP()


// CFACE_ID_BaseLine1_0Dlg message handlers

BOOL CFACE_ID_BaseLine1_0Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	dlgCreate->Create(IDD_CREATE);
	dlgCreate->ShowWindow(SW_HIDE);
	dlgRecongnize->Create(IDD_RECONGNIZE);
	dlgRecongnize->ShowWindow(SW_HIDE);
	vector<char*> list = ListAllFilesInDir("./data/*");
	UpdateData(true);
	if (list.size() <= 1)
	{
		char comment[100] = "";
		strcat(comment, "\n\nDatabase is empty.\n\n");
		_cprintf("%s", comment);
		CString tmp(comment);
		cmd = tmp;
	}
	else
	{
		char comment[10000] = "";
		strcat(comment, "\n\nThere is/are ");
		strcat(comment, to_string(list.size() - 1).c_str());
		strcat(comment, " user(s) in the database.\n\n");
		for (int i = 1; i < list.size(); i++) 
		{
			strcat(comment, list[i]);
			strcat(comment, "\n");
		}
		_cprintf("%s", comment);
		CString tmp(comment);
		cmd = tmp;
	}
	UpdateData(false);	
	stored_users.clear();
	for (int i = 1; i < list.size(); i++) 
	{
		char path[200] = "./data/";
		strcat(path, list[i]);
		FACE face = FACE();
		face.loadDatFromFile(path);
		stored_users.push_back(face);
	}
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFACE_ID_BaseLine1_0Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFACE_ID_BaseLine1_0Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFACE_ID_BaseLine1_0Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFACE_ID_BaseLine1_0Dlg::OnBnClickedCamIn()
{
	// TODO: 在此添加控件通知处理程序代码
	flagCamera = 0;
	CString str = _T("\n Switch to internal camera.\n");
	UpdateData(true);
	cmd = str;
	UpdateData(false);
	
}


void CFACE_ID_BaseLine1_0Dlg::OnBnClickedCamEx()
{
	flagCamera = 1;
	CString str = _T("\n Switch to external camera.\n");
	UpdateData(true);
	cmd = str;
	UpdateData(false);
	// TODO: 在此添加控件通知处理程序代码
}


void CFACE_ID_BaseLine1_0Dlg::OnBnClickedAbout()
{

	/*FACE* demo = new FACE();
	demo->createDemo();
	demo->printDat();
	demo->reset();
	demo->printDat();*/
	if (stored_users.size() > 0) 
	{
		for (int i = 0; i < stored_users.size(); i++) 
		{
			stored_users[i].printDat();
		}
	}
	// TODO: 在此添加控件通知处理程序代码
}

void CFACE_ID_BaseLine1_0Dlg::OnBnClickedRun()
{
	dlgCreate->ShowWindow(SW_SHOWDEFAULT);
	CString str = _T("\n Create Session: Create New User...\n");
	UpdateData(true);
	cmd = str;
	UpdateData(false);
	// TODO: 在此添加控件通知处理程序代码
}


void CFACE_ID_BaseLine1_0Dlg::OnBnClickedEnableStream()
{
	std::thread threadStream(globalEnableCamera);
	threadStream.detach();
	// TODO: 在此添加控件通知处理程序代码
}


void CFACE_ID_BaseLine1_0Dlg::OnBnClickedRunrec()
{
	dlgRecongnize->ShowWindow(SW_SHOWNORMAL);
	CString str = _T("\n Running Reconginize Session...\n");
	UpdateData(true);
	cmd = str;
	UpdateData(false);
	// TODO: 在此添加控件通知处理程序代码
}
