// dlg_recongnize.cpp : 实现文件
//

#include "stdafx.h"
#include "FACE_ID_BaseLine1_0.h"
#include "dlg_recongnize.h"
#include "afxdialogex.h"


// dlg_recongnize 对话框

IMPLEMENT_DYNAMIC(dlg_recongnize, CDialogEx)

dlg_recongnize::dlg_recongnize(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RECONGNIZE, pParent)
	, eidt_rec_cmd(_T(""))
{

}

dlg_recongnize::~dlg_recongnize()
{
}

void dlg_recongnize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_REC_CMD, eidt_rec_cmd);
	DDX_Control(pDX, IDC_SLIDER1, thresholdSlider);
}




BEGIN_MESSAGE_MAP(dlg_recongnize, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SHOT, &dlg_recongnize::OnBnClickedShot)
	ON_BN_CLICKED(IDC_RESHOT, &dlg_recongnize::OnBnClickedReshot)
	ON_BN_CLICKED(IDC_RECONGNIZE, &dlg_recongnize::OnBnClickedRecongnize)
END_MESSAGE_MAP()


// dlg_recongnize 消息处理程序


BOOL dlg_recongnize::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(true);
	CString str = _T("Waiting...");
	eidt_rec_cmd = str;
	UpdateData(false);
	thresholdSlider.SetRange(-30,30);
	thresholdSlider.SetTicFreq(1);
	thresholdSlider.SetPos(0);

	CRect rect;
	GetDlgItem(IDC_STATIC2)->GetClientRect(rect);
	cv::namedWindow("myshowWnd2");
	cv::resizeWindow("myshowWnd2", rect.Width(), rect.Height());
	HWND hWnd = (HWND)cvGetWindowHandle("myshowWnd2");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC2)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void dlg_recongnize::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData(true);
	CSliderCtrl* pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	int valueINT = pSlidCtrl->GetPos();
	float value = (float)(0.01f*valueINT);
	thresholdSetting = value;
	//_cprintf("\n\nThreshold is set to %f", value);
	char comment[100] = "Threshold is set to: ";
	char floatValue[50];
	itoa(valueINT, floatValue, 10);
	strcat(comment, floatValue);
	CString str = CA2CT(comment);
	eidt_rec_cmd = str;
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	UpdateData(false);
	//_cprintf("%s\n", comment);
}


void dlg_recongnize::OnBnClickedShot()
{
	flagRecShot = true;
	cv::waitKey(1000);
	cv::imshow("myshowWnd2", background);
	cv::waitKey(5);
	cv::imshow("myshowWnd2", stranger);
	cv::waitKey(5);
	// TODO: 在此添加控件通知处理程序代码
}


void dlg_recongnize::OnBnClickedReshot()
{
	// TODO: 在此添加控件通知处理程序代码
	flagRecShot = true;
	cv::imshow("myshowWnd2", background);
	cv::waitKey(5);
	cv::imshow("myshowWnd2", stranger);
	cv::waitKey(5);
}


void dlg_recongnize::OnBnClickedRecongnize()
{
	if (stored_users.size() < 1) 
	{
		AfxMessageBox(_T("Database is empty! Have to record user first."),MB_OK,MB_ICONSTOP);
	}
	else 
	{
		_cprintf("\n\nRunning detection now...");
		float min_dis = 99999.99;
		int index = -1;
		for (int i = 0; i < stored_users.size(); i++)
		{
			cv::Mat cache;
			float scoreRef = stored_users[i].getAverageScore();
			scoreRef = (1.0f + thresholdSetting) * scoreRef;
			stranger.convertTo(cache, CV_32F);
			float scoreStranger = stored_users[i].computeScore(cache);
			
			float dis = scoreRef - scoreStranger;
			if (dis >= 0 && dis < min_dis) 
			{
				index = i;
				min_dis = dis;
			}
			_cprintf("\n\nuser #%d score: %f refRcore :%f\n\n",i+1,scoreStranger,scoreRef);
		}
		UpdateData(true);
		if (index == -1) 
		{
			CString str = _T("You are stranger.");
			eidt_rec_cmd = str;
		}
		else 
		{
			char comment[300] = "You are ";
			strcat(comment, stored_users[index].getId());
			strcat(comment, ".");
			CString str = CA2CT(comment);
			eidt_rec_cmd = str;
		}
		UpdateData(false);
	}


	



	// TODO: 在此添加控件通知处理程序代码
}
