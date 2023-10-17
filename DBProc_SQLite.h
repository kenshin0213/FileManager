#pragma once

#include "sqlite3.h"
#include "trio_inv.h"

using namespace std;

// CDBProc_SQLite

class CDBProc_SQLite : public CWnd
{
	DECLARE_DYNAMIC(CDBProc_SQLite)

private:
	CString m_csTodayDate;
	CString m_dbFileName;
	sqlite3 *myDB;

	CStdioFile *ptrLogFile;
	

public:
	CDBProc_SQLite();
	CDBProc_SQLite(CString dbFileName);
	virtual ~CDBProc_SQLite();


	// 함수 선언부 시작-----------------------------------------------------------------------------	
	static int callback(void *NotUsed, int argc, char **argv, char **azColName);

	int Sqlite3ExecBegin(sqlite3* dbName = NULL);
	int Sqlite3ExecCommit(sqlite3* dbName = NULL);

	CString OpenDB(sqlite3* dbName = NULL);
	CString CloseDB(sqlite3* dbName = NULL);

	void InsertSymbolCode(Tp1005OutBlock* pSymbolInfo, CString mktType);		
	void InsertSymbolMaster(Tc1101OutBlock*	pc1101outblock);	

	void InsertH1(char* pSiseData, int iIdx);
	void InsertK3(char* pSiseData, int iIdx);
	void InsertJ8(char* pSiseData, int iIdx);
	void InsertK8(char* pSiseData, int iIdx);

	CString FileToDB(CString csTrID, CString csFileName);

	void SelectSymbolMastInfo(CString csSymbol, Tc1101OutBlock *ptrTc1101 );
	void SelectInsertMastInfo();
	void ChangeDate(CString csDate);
	CString CreateDBFile();
	CString GetDate();
	CString SetDate(CString csDate);
	CString SetLogFilePtr(CStdioFile* ptrLogFile);
	void WriteLog(CString logMsg);

protected:
	DECLARE_MESSAGE_MAP()
};
