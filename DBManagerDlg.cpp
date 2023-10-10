
// DBManagerDlg.cpp : ���� ����
//
#include "stdafx.h"
#include "DBManager.h"
#include "DBManagerDlg.h"
#include "afxdialogex.h"
#include "util.h"
#include <mysql.h>

#pragma comment(lib, "libmariadb.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString _TRID_H1		= "h1";			// �������� �ǽð� ȣ��
const CString _TRID_K3		= "k3";			// �ڽ��� �ǽð� ȣ��
const CString _TRID_J8		= "j8";			// �������� �ǽð� ü��
const CString _TRID_K8		= "k8";			// �ڽ��� �ǽð� ü��



// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDBManagerDlg ��ȭ ����


CDBManagerDlg::CDBManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDBManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDBManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_listboxTrace);
	DDX_Control(pDX, IDC_H1, m_Check_H1);
	DDX_Control(pDX, IDC_K3, m_Check_K3);
	DDX_Control(pDX, IDC_J8, m_Check_J8);
	DDX_Control(pDX, IDC_K8, m_Check_K8);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_Edit_Enabler, m_Check_Edit_Enabler);
	DDX_Control(pDX, IDC_Date, m_Edit2);
	

	m_Check_H1.SetCheck(0);
	m_Check_K3.SetCheck(0);
	m_Check_J8.SetCheck(0);
	m_Check_K8.SetCheck(0);

	m_Edit1.EnableWindow(FALSE);
	m_Check_Edit_Enabler.SetCheck(0);
}

BEGIN_MESSAGE_MAP(CDBManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
ON_BN_CLICKED(ID_GetDir, &CDBManagerDlg::OnBnClickedGetdir)
ON_BN_CLICKED(ID_DB_Insert, &CDBManagerDlg::OnBnClickedDbInsert)
ON_BN_CLICKED(ID_DB_Open, &CDBManagerDlg::OnBnClickedDbOpen)
ON_BN_CLICKED(ID_DB_Close, &CDBManagerDlg::OnBnClickedDbClose)
ON_BN_CLICKED(IDC_Edit_Enabler, &CDBManagerDlg::OnBnClickedEditEnabler)
ON_BN_CLICKED(ID_DateChg, &CDBManagerDlg::OnBnClickedDatechg)
ON_BN_CLICKED(ID_CHECK_DATE, &CDBManagerDlg::OnBnClickedCheckDate)
ON_BN_CLICKED(ID_Covert_CSV, &CDBManagerDlg::OnBnClickedCovertCsv)
ON_BN_CLICKED(ID_Covert_CSV2, &CDBManagerDlg::OnBnClickedCovertCsv2)
ON_BN_CLICKED(ID_DB_Insert2, &CDBManagerDlg::OnBnClickedDbInsert2)
ON_BN_CLICKED(ID_DB_Open2, &CDBManagerDlg::OnBnClickedDbOpen2)
ON_BN_CLICKED(ID_DB_Open3, &CDBManagerDlg::OnBnClickedDbOpen3)
END_MESSAGE_MAP()


// CDBManagerDlg �޽��� ó����

BOOL CDBManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CDBManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CDBManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CDBManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDBManagerDlg::OnBnClickedGetdir()
{
	CString csPath;
	csPath = GetDirPath();
	m_Edit1.SetWindowTextA(csPath);
}

CString CDBManagerDlg::GetDirPath( )
{
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // ������� ���� 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	//BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = "������ �����ϼ���";
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // ���ϰ�� �о����

        // ��θ� ������ ����� ���, Edit Control �� �� ����
	CString csDirPath;
	csDirPath.Format("%s", szBuffer);

	return csDirPath;
}

void CDBManagerDlg::OnBnClickedDbInsert()
{
	if (m_Check_H1.GetCheck())
		Insert(_TRID_H1);

	if (m_Check_K3.GetCheck())
		Insert(_TRID_K3);

	if (m_Check_J8.GetCheck())
		Insert(_TRID_J8);

	if (m_Check_K8.GetCheck())
		Insert(_TRID_K8);
}

int CDBManagerDlg::Insert(CString _csTRID)
{	
	int iRtn = 0;
	CString csMsg, csLog;
	CString csPath, csSubPath, csFullPath;

	m_Edit1.GetWindowText(csPath);
	csPath = csPath +"\\" + _csTRID;
	csFullPath = csPath + "\\*.*";		

	iRtn = GetFileList(csFullPath);
		
	CString csFilePath, csSiseData;
	CStdioFile ptrFile;

	int size = m_vtFileList.size();

	csMsg.Format("Insert Start[%s]", _csTRID);
	LogPrint(csMsg);
	for (int i=0; i < size; i++)
	{
		// DB �μ�Ʈ �߿� �ٸ� �� �� �� �ֵ��� �ϱ� ���� PeekMessage A ���
		MSG msg;
		while(::PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}	

		csFilePath = csPath + "\\" + m_vtFileList[i];
		csMsg = m_DBProc_SQLite.FileToDB(_csTRID, csFilePath);
		LogPrint(csMsg);	
	}
	csMsg.Format("Insert End[%s]", _csTRID);
	LogPrint(csMsg);

	return 1;
}	

int CDBManagerDlg::GetFileList(CString _csPath, CString _csFileType)
{
	//�˻� Ŭ����
	CFileFind finder;
 
	//CFileFind�� ����, ���͸��� �����ϸ� TRUE �� ��ȯ��
	BOOL bWorking = finder.FindFile(_csPath); //
 
	CString fileName;
	CString DirName;
	m_vtFileList.clear();

	while (bWorking)
	{
		//���� ���� / ���� �� �����ϸ�ٸ� TRUE ��ȯ
		bWorking = finder.FindNextFile();
		//���� �϶�
		if (finder.IsArchived())
		{
			//������ �̸�
			CString _fileName = finder.GetFileName();
 
			// �������� �������� ����������� ����
			if (_fileName == (".") ||
				_fileName == ("..") ||
				_fileName == ("Thumbs.db")) continue;
 
			// _csFileType �⺻���� ".txt"
			if (_fileName.Find((_csFileType)))
			{
				m_vtFileList.push_back(_fileName);
			}
			//�о�� ���� �̸��� ����Ʈ�ڽ��� ����
		}
	}
	return 1;
}

void CDBManagerDlg::LogPrint(CString _csMsg)
{
	CString csTime;
	SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);
	csTime.Format("[%02d:%02d:%02d.%03d]",cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);

	m_listboxTrace.AddString(csTime+_csMsg);
	m_listboxTrace.SetTopIndex(	m_listboxTrace.GetCount()-1	);
	m_listboxTrace.SetSel(m_listboxTrace.GetCount()-1);	
}

void CDBManagerDlg::OnBnClickedDbOpen()
{
	LogPrint(m_DBProc_SQLite.OpenDB());
}


void CDBManagerDlg::OnBnClickedDbClose()
{
	LogPrint(m_DBProc_SQLite.CloseDB());
}


void CDBManagerDlg::OnBnClickedEditEnabler()
{
	if(m_Check_Edit_Enabler.GetCheck() != 1)
	{
		m_Check_Edit_Enabler.SetCheck(0);
		m_Edit1.EnableWindow(FALSE);
	}
	else
	{
		m_Check_Edit_Enabler.SetCheck(1);
		m_Edit1.EnableWindow(TRUE);
	}
}


void CDBManagerDlg::OnBnClickedDatechg()
{	
	CString csDate;
	m_Edit1.GetWindowTextA(csDate);
	if(!csDate.IsEmpty())
	{
		m_DBProc_SQLite.ChangeDate(csDate);
		LogPrint("��¥�� ����Ǿ����ϴ�. [" + csDate +"]");
		LogPrint(m_DBProc_SQLite.CreateDBFile());
	}
	else
	{
		LogPrint("��¥�� �Է����ּ���.");
		m_Edit1.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->SetFocus();
	}
}


void CDBManagerDlg::OnBnClickedCheckDate()
{
	m_Edit2.SetWindowTextA("Date : " + m_DBProc_SQLite.GetDate());
}

void CDBManagerDlg::ConvertToCSV(CString _csFileName)
{
	CString csMsg;
	CString csSiseData;
	CString csDestFileName;
	CStdioFile csSourceFileName, pDestFile;

	_csFileName.MakeLower();
	
	//CheckDirectory((_csFileName.Left(_csFileName.GetLength()-13))+"\\csv");

	BOOL bFlag;
	bFlag = csSourceFileName.Open(_csFileName, CFile::modeRead, NULL);
	if (bFlag)
	{
		csSourceFileName.SeekToBegin();
		
		csDestFileName = _csFileName.Left(_csFileName.GetLength()-3) + "csv";
		bFlag = pDestFile.Open(csDestFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, NULL);
		
		if (!bFlag)
		{
			csMsg.Format("[%s] ������ ������ �� �����ϴ�.", csDestFileName);
			return;
		}

	}
	else
	{
		csMsg.Format("[%s] ������ ���� ���� �ʽ��ϴ�.", _csFileName);
		return;
	}
	
	// H1, K3 ����ü ����
	if ((_csFileName.Find(_TRID_H1) > 0) || (_csFileName.Find(_TRID_K3) > 0))
	{
		Th1OutBlock* pSiseData;
		CString csCsvData;
		csCsvData = "�����ڵ�,�ð�,\
					�ŵ�ȣ��,�ż�ȣ��,�ŵ�ȣ���ܷ�,�ż�ȣ���ܷ�,\
					�ŵ�2ȣ��,�ż�2ȣ��,�ŵ�2ȣ���ܷ�,�ż�2ȣ���ܷ�,\
					�ŵ�3ȣ��,�ż�3ȣ��,�ŵ�3ȣ���ܷ�,�ż�3ȣ���ܷ�,\
					�ŵ�4ȣ��,�ż�4ȣ��,�ŵ�4ȣ���ܷ�,�ż�4ȣ���ܷ�,\
					�ŵ�5ȣ��,�ż�5ȣ��,�ŵ�5ȣ���ܷ�,�ż�5ȣ���ܷ�,\
					�Ѹŵ�ȣ���ܷ�,�Ѹż�ȣ���ܷ�,\
					�ŵ�6ȣ��,�ż�6ȣ��,�ŵ�6ȣ���ܷ�,�ż�6ȣ���ܷ�,\
					�ŵ�7ȣ��,�ż�7ȣ��,�ŵ�7ȣ���ܷ�,�ż�7ȣ���ܷ�,\
					�ŵ�8ȣ��,�ż�8ȣ��,�ŵ�8ȣ���ܷ�,�ż�8ȣ���ܷ�,\
					�ŵ�9ȣ��,�ż�9ȣ��,�ŵ�9ȣ���ܷ�,�ż�9ȣ���ܷ�,\
					�ŵ�10ȣ��,�ż�10ȣ��,�ŵ�10ȣ���ܷ�,�ż�10ȣ���ܷ�,\
					�����ŷ���";
		
		pDestFile.SeekToEnd();
		pDestFile.WriteString(csCsvData);
		pDestFile.WriteString("\n");

		while(csSourceFileName.ReadString(csSiseData))
		{
			 pSiseData = (Th1OutBlock*) (((LPSTR)(LPCTSTR)csSiseData)+3);
			 csCsvData.Format("A%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\
				 ,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s"
			 , SCOPY(pSiseData->code        ), SCOPY(pSiseData->hotime      )
			 , SCOPY(pSiseData->offer       ), SCOPY(pSiseData->bid         )
			 , SCOPY(pSiseData->offerrem    ), SCOPY(pSiseData->bidrem      )
			 , SCOPY(pSiseData->P_offer     ), SCOPY(pSiseData->P_bid       )
			 , SCOPY(pSiseData->P_offerrem  ), SCOPY(pSiseData->P_bidrem    )
			 , SCOPY(pSiseData->S_offer     ), SCOPY(pSiseData->S_bid       )
			 , SCOPY(pSiseData->S_offerrem  ), SCOPY(pSiseData->S_bidrem    )
			 , SCOPY(pSiseData->S4_offer    ), SCOPY(pSiseData->S4_bid      )
			 , SCOPY(pSiseData->S4_offerrem ), SCOPY(pSiseData->S4_bidrem   )
			 , SCOPY(pSiseData->S5_offer    ), SCOPY(pSiseData->S5_bid      )
			 , SCOPY(pSiseData->S5_offerrem ), SCOPY(pSiseData->S5_bidrem   )
			 , SCOPY(pSiseData->T_offerrem  ), SCOPY(pSiseData->T_bidrem    )
			 , SCOPY(pSiseData->S6_offer    ), SCOPY(pSiseData->S6_bid      )
			 , SCOPY(pSiseData->S6_offerrem ), SCOPY(pSiseData->S6_bidrem   )
			 , SCOPY(pSiseData->S7_offer    ), SCOPY(pSiseData->S7_bid      )
			 , SCOPY(pSiseData->S7_offerrem ), SCOPY(pSiseData->S7_bidrem   )
			 , SCOPY(pSiseData->S8_offer    ), SCOPY(pSiseData->S8_bid      )
			 , SCOPY(pSiseData->S8_offerrem ), SCOPY(pSiseData->S8_bidrem   )
			 , SCOPY(pSiseData->S9_offer    ), SCOPY(pSiseData->S9_bid      )
			 , SCOPY(pSiseData->S9_offerrem ), SCOPY(pSiseData->S9_bidrem   )
			 , SCOPY(pSiseData->S10_offer   ), SCOPY(pSiseData->S10_bid     )
			 , SCOPY(pSiseData->S10_offerrem), SCOPY(pSiseData->S10_bidrem  )
			 , SCOPY(pSiseData->volume      )		
			);

		pDestFile.SeekToEnd();
		pDestFile.WriteString(csCsvData);
		pDestFile.WriteString("\n");
		}
		csMsg.Format("End Convert [%s]", csDestFileName);
		LogPrint(csMsg);
	}
		
	else if(_csFileName.Find(_TRID_J8) > 0)
	{
		Tj8OutBlock* pSiseData;
		CString csCsvData;
		csCsvData = "�����ڵ�,�ð�,���簡,���������������ȣ,\
					����������񰡰ݵ����,���������������,\
					��,����,�ŵ�ȣ��,�ż�ȣ��,\
					�����ŷ���,�ŷ������Ϻ�,�����ŷ���(ü�����),\
					�����ŷ����,�ð�,������հ�,�屸��";
		
		pDestFile.SeekToEnd();
		pDestFile.WriteString(csCsvData);
		pDestFile.WriteString("\n");

		while(csSourceFileName.ReadString(csSiseData))
		{				
			 pSiseData = (Tj8OutBlock*) (((LPSTR)(LPCTSTR)csSiseData) +3);
			 csCsvData.Format("A%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s"
			 , SCOPY(pSiseData->code      ), SCOPY(pSiseData->time      )
			 , SCOPY(pSiseData->price     ), SCOPY(pSiseData->sign      )
			 , SCOPY(pSiseData->change    ), SCOPY(pSiseData->chrate    )
			 , SCOPY(pSiseData->high      ), SCOPY(pSiseData->low       )
			 , SCOPY(pSiseData->offer     ), SCOPY(pSiseData->bid       )
			 , SCOPY(pSiseData->volume    ), SCOPY(pSiseData->volrate   )
			 , SCOPY(pSiseData->movolume  ), SCOPY(pSiseData->value     )
			 , SCOPY(pSiseData->open      ), SCOPY(pSiseData->avgprice  )
			 , SCOPY(pSiseData->janggubun )
			);

		pDestFile.SeekToEnd();
		pDestFile.WriteString(csCsvData);
		pDestFile.WriteString("\n");
		}
		csMsg.Format("End Convert [%s]", csDestFileName);
		LogPrint(csMsg);
	}

	else if(_csFileName.Find(_TRID_K8) > 0)
	{
		Tk8OutBlock* pSiseData;
		CString csCsvData;
		csCsvData = "�����ڵ�,�ð�,���簡,���������������ȣ,\
					����������񰡰ݵ����,���������������,\
					��,����,�ŵ�ȣ��,�ż�ȣ��,\
					�����ŷ���,�ŷ������Ϻ�,�����ŷ���(ü�����),\
					�����ŷ����,�ð�,������հ�,�屸��";
		
		pDestFile.SeekToEnd();
		pDestFile.WriteString(csCsvData);
		pDestFile.WriteString("\n");

		while(csSourceFileName.ReadString(csSiseData))
		{				
			 pSiseData = (Tk8OutBlock*) (((LPSTR)(LPCTSTR)csSiseData)+3);
			 csCsvData.Format("A%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s"
			 , SCOPY(pSiseData->code      ), SCOPY(pSiseData->time      )
			 , SCOPY(pSiseData->price     ), SCOPY(pSiseData->sign      )
			 , SCOPY(pSiseData->change    ), SCOPY(pSiseData->chrate    )
			 , SCOPY(pSiseData->high      ), SCOPY(pSiseData->low       )
			 , SCOPY(pSiseData->offer     ), SCOPY(pSiseData->bid       )
			 , SCOPY(pSiseData->volume    ), SCOPY(pSiseData->volrate   )
			 , SCOPY(pSiseData->movolume  ), SCOPY(pSiseData->value     )
			 , SCOPY(pSiseData->open      ), SCOPY(pSiseData->avgprice  )
			 , SCOPY(pSiseData->janggubun )
			);

		pDestFile.SeekToEnd();
		pDestFile.WriteString(csCsvData);
		pDestFile.WriteString("\n");
		}
		csMsg.Format("End Convert [%s]", csDestFileName);
		LogPrint(csMsg);
	}
	else{
		csMsg.Format("[%s] �ü�������(h1/k3/j8/k8)�� �ƴմϴ�.", _csFileName);		
	}

	pDestFile.Close();
	csSourceFileName.Close();
}

void CDBManagerDlg::OnBnClickedCovertCsv()
{
	int iRtn = 0;
	CString csMsg;
	CString csPath, csSubPath, csFullPath, csFilePath;

	m_Edit1.GetWindowText(csPath);
	
	if (m_Check_H1.GetCheck())
	{
		csPath = csPath +"\\" + _TRID_H1;
		csFullPath = csPath + "\\*.*";	

		GetFileList(csFullPath);

		int size = m_vtFileList.size();
		for (int i=0; i < size; i++)
		{
			csFilePath = csPath + "\\" + m_vtFileList[i];
			ConvertToCSV(csFilePath);

			MSG msg;
			while(::PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}		
		}		
	}

	if (m_Check_K3.GetCheck())
	{
		csPath = csPath +"\\" + _TRID_K3;
		csFullPath = csPath + "\\*.*";	

		GetFileList(csFullPath);

		int size = m_vtFileList.size();
		for (int i=0; i < size; i++)
		{
			csFilePath = csPath + "\\" + m_vtFileList[i];
			ConvertToCSV(csFilePath);

			MSG msg;
			while(::PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}		
		}
	}

	if (m_Check_J8.GetCheck())	
	{
		csPath = csPath +"\\" + _TRID_J8;
		csFullPath = csPath + "\\*.*";

		GetFileList(csFullPath);

		int size = m_vtFileList.size();
		for (int i=0; i < size; i++)
		{
			csFilePath = csPath + "\\" + m_vtFileList[i];
			ConvertToCSV(csFilePath);

			MSG msg;
			while(::PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}		
		}
	}

	if (m_Check_K8.GetCheck())
	{
		csPath = csPath +"\\" + _TRID_K8;
		csFullPath = csPath + "\\*.*";

		GetFileList(csFullPath);

		int size = m_vtFileList.size();
		for (int i=0; i < size; i++)
		{
			csFilePath = csPath + "\\" + m_vtFileList[i];
			ConvertToCSV(csFilePath);

			MSG msg;
			while(::PeekMessageA(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}		
		}
	}
}


// CSV ���� ��ȯ
void CDBManagerDlg::OnBnClickedCovertCsv2()
{
	CString csMsg;
	CString csSourceFileName, csDestFileName;
	static TCHAR BASED_CODE szFilter[] = _T("�ؽ�Ʈ ����(*.txt) | *.TXT;*.txt;|�������(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);

	if(IDOK == dlg.DoModal())
	{
		csSourceFileName = dlg.GetPathName();	
		csMsg.Format("Start Convert [%s]", csSourceFileName);
		LogPrint(csMsg);
		ConvertToCSV(csSourceFileName);
	}

	if(csSourceFileName.IsEmpty())
		return;
}


// DB ���� Insert
void CDBManagerDlg::OnBnClickedDbInsert2()
{
	CString csMsg;
	CString csSourceFileName, csDestFileName;
	static TCHAR BASED_CODE szFilter[] = _T("�ؽ�Ʈ ����(*.txt) | *.TXT;*.txt;|�������(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);

	if(IDOK == dlg.DoModal())
	{
		csSourceFileName = dlg.GetPathName();	
		csMsg.Format("Start Convert [%s]", csSourceFileName);
		LogPrint(csMsg);
		
		CString csTRID;
		int nCheck = 0;


		if (m_Check_H1.GetCheck())
		{
			nCheck++;
			csTRID = _TRID_H1;
		}

		if (m_Check_K3.GetCheck())
		{
			nCheck++;
			csTRID = _TRID_K3;
		}

		if (m_Check_J8.GetCheck())
		{
			nCheck++;
			csTRID = _TRID_J8;
		}
		if (m_Check_K8.GetCheck())
		{
			nCheck++;
			csTRID = _TRID_K8;
		}

		if(nCheck == 1)
			csMsg = m_DBProc_SQLite.FileToDB(csTRID, csSourceFileName);

		else
			csMsg.Format("1���� üũ�ڽ��� �����ϼ���.");

		LogPrint(csMsg);
			
	}
}


void CDBManagerDlg::OnBnClickedDbOpen2()
{
	int iRtn = 0;
	CString csMsg, csLog;
	CString csPath, csSubPath, csFullPath;

	m_Edit1.GetWindowText(csPath);
	csFullPath = csPath + "\\*.*";

	GetFileList(csFullPath, ".db");
}


int CDBManagerDlg::mariadbConnectTest()
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

	conn = mysql_init(NULL);

	if (conn == NULL) {
		fprintf(stderr, "mysql_init() failed\n");
		return 1;
	}

	if (mysql_real_connect(conn, "localhost", "root", "0213", "marketdata", 0, NULL, 0) == NULL) {
		fprintf(stderr, "mysql_real_connect() failed\n");
		mysql_close(conn);
		return 1;
	}

	if (mysql_query(conn, "SELECT code FROM j8 group by code")) {
		fprintf(stderr, "SELECT * FROM your_table failed. Error: %s\n", mysql_error(conn));
		mysql_close(conn);
		return 1;
	}

	res = mysql_store_result(conn);

	while ((row = mysql_fetch_row(res))) {
		printf("Data: %s\n", row[0]); // ��� ó��
	}

	mysql_free_result(res);
	mysql_close(conn);

	return 0;

}

void CDBManagerDlg::OnBnClickedDbOpen3()
{
	// TODO: Add your control notification handler code here
	mariadbConnectTest();
}
