
// DBManagerDlg.h : 헤더 파일
//

#pragma once

#include "DBProc_SQLite.h"
#include <vector>

// CDBManagerDlg 대화 상자
class CDBManagerDlg : public CDialogEx
{
// 생성입니다.
public:
	CDBManagerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	~CDBManagerDlg();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DBMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	vector<CString> m_vtFileList;

	CDBProc_SQLite m_DBProc_SQLite;

	CListBox m_listboxTrace;
	CButton m_Check_H1;
	CButton m_Check_K3;
	CButton m_Check_J8;
	CButton m_Check_K8;
	CEdit   m_Edit1;
	CEdit   m_Edit2;
	CButton m_Check_Edit_Enabler;


public:	

	CStdioFile csLogFile;

	int GetFileList(CString csPath, CString csFileType =".txt");
	CString GetDirPath();
	int Insert(CString csTRID);
	void LogPrint(CString csMsg);		
	void ConvertToCSV(CString _csFileName);
	int mariadbConnectTest();


	afx_msg void OnBnClickedGetdir();
	afx_msg void OnBnClickedDbInsert();
	afx_msg void OnBnClickedDbOpen();
	afx_msg void OnBnClickedDbClose();
	afx_msg void OnBnClickedEditEnabler();
	afx_msg void OnBnClickedDatechg();
	afx_msg void OnBnClickedCheckDate();
	afx_msg void OnBnClickedCovertCsv();
	afx_msg void OnBnClickedCovertCsv2();
	afx_msg void OnBnClickedDbInsert2();
	afx_msg void OnBnClickedDbOpen2();
	afx_msg void OnBnClickedDbOpen3();
};
