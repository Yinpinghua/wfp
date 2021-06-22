// WfpSampleExeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WfpSampleExe.h"
#include "WfpSampleExeDlg.h"
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CWfpSampleExeDlg dialog




CWfpSampleExeDlg::CWfpSampleExeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWfpSampleExeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWfpSampleExeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWfpSampleExeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CWfpSampleExeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CWfpSampleExeDlg message handlers

BOOL CWfpSampleExeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWfpSampleExeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWfpSampleExeDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWfpSampleExeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWfpSampleExeDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//获取端口号
	CString str;
	GetDlgItemText(IDC_EDIT_PORT, str);
	const char* cstr;
	char temp[100];
	::wsprintfA(temp, "%ls", (LPCTSTR)str);
	cstr = temp;
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return ;
	}
	hostent *host = gethostbyname(cstr);
	ULONG a[10] = { 0 };
	int x = 0;
	for (x;; x++)
	{
		
			ULONG c1 = inet_addr(inet_ntoa(*((in_addr *)host->h_addr_list[x])));
			
			
			a[x] = htonl(c1);
			
	
		if (host->h_addr_list[x] + host->h_length >= host->h_name)
		{
		 break;
		 }
	}
	HANDLE hFile = CreateFile(_T("\\\\.\\wfp_sample_device"),GENERIC_ALL,0,NULL,OPEN_EXISTING,0,NULL);
	if( hFile == INVALID_HANDLE_VALUE )
	{
		MessageBox(L"OUT");
		return;
	}

	// 读者可以在此基础上增加其他域的过滤，如IP，协议类型、数据包方向等
	// Info.m_uRemotePort = uPort;
	for (x; x >= 0; x--)
	{
		ST_WFP_NETINFO Info = { 0 };
		Info.m_ulRemoteIPAddr = a[x];
		DWORD dwNeedSize = 0;
		BOOL rValue = DeviceIoControl(hFile, IOCTL_WFP_SAMPLE_ADD_RULE, (LPVOID)&Info, sizeof(Info), NULL, 0, &dwNeedSize, NULL);
		if (rValue == 0)
		{
			MessageBox(L"failed");
		}
		else
		{
			CString temp_value = _T("");
			temp_value.Format(_T("%x"), a[x]);
			MessageBox(temp_value);
		}
	}
	
	CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;
	return;
}
