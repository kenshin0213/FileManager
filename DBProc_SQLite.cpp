// DBProc_SQLite.cpp : 구현 파일입니다.
//

#pragma comment(lib, "sqlite3.lib")
#include "stdafx.h"
#include "DBProc_SQLite.h"
#include "util.h"



const CString _TRID_H1		= "h1";			// 유가증권 실시간 호가
const CString _TRID_K3		= "k3";			// 코스닥 실시간 호가
const CString _TRID_J8		= "j8";			// 유가증권 실시간 체결
const CString _TRID_K8		= "k8";			// 코스닥 실시간 체결

const CString csH1_K3Columns = " (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?\
					            , ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) ";
const CString csJ8_K8Columns = " (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) ";	

const CString _H1_QRY = "INSERT INTO H1 VALUES " + csH1_K3Columns;
const CString _K3_QRY = "INSERT INTO K3 VALUES " + csH1_K3Columns;
const CString _J8_QRY = "INSERT INTO J8 VALUES " + csJ8_K8Columns;
const CString _K8_QRY = "INSERT INTO K8 VALUES " + csJ8_K8Columns;


// CDBProc_SQLite

IMPLEMENT_DYNAMIC(CDBProc_SQLite, CWnd)

CDBProc_SQLite::CDBProc_SQLite()
{
	//m_dbFileName =	_T("myDB.db");
	// %Y: 전체(2019), %y: 뒤 2자리(19)
	m_csTodayDate= GetDateTime(8);	

	CreateDBFile();
}

CDBProc_SQLite::CDBProc_SQLite(CString _dbFileName)
{
	m_dbFileName =	_dbFileName;
	// %Y: 전체(2019), %y: 뒤 2자리(19)
	m_csTodayDate= GetDateTime(8);

	CreateDBFile();
	
}

CDBProc_SQLite::~CDBProc_SQLite()
{
}


BEGIN_MESSAGE_MAP(CDBProc_SQLite, CWnd)
END_MESSAGE_MAP()
// CDBProc_SQLite 메시지 처리기입니다.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// SQLite에서 사용될 콜백 함수
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int CDBProc_SQLite::callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    CString csMsg;
    for(i = 0; i<argc; i++) {
        //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	    csMsg.Format("%10s = %10s", azColName[i], argv[i] ? argv[i] : "NULL");
		//LogPrint(csMsg);        
	}
   //printf("\n");
   return 0;
}

int CDBProc_SQLite::Sqlite3ExecBegin(sqlite3* _dbName)
{
	sqlite3* dbName = myDB;
	if (_dbName != NULL)
		dbName = _dbName;
	return sqlite3_exec(dbName, "BEGIN;", NULL, NULL, NULL);
}

int CDBProc_SQLite::Sqlite3ExecCommit(sqlite3* _dbName)
{
	sqlite3* dbName = myDB;
	if (_dbName != NULL)
		dbName = _dbName;
	return sqlite3_exec(dbName, "COMMIT;", NULL, NULL, NULL);
}

CString CDBProc_SQLite::OpenDB(sqlite3* _dbName)
{
	CString csMsg;
	int rc;
	sqlite3* dbName = myDB;
	if (_dbName != NULL)
		dbName = _dbName;

	rc = sqlite3_open(m_dbFileName, &dbName);

	if(rc == SQLITE_OK)
	{
		csMsg.Format("Opened [%s] database successfully!!", m_dbFileName);
	}	
	else
	{		
		csMsg.Format("Can't open [%s] database. ERROR CODE [%s]", m_dbFileName, sqlite3_errmsg(*(&dbName)));
	}	
	return csMsg;
}

CString CDBProc_SQLite::CloseDB(sqlite3* _dbName)
{
	CString csMsg;
	int rc;
	sqlite3* dbName = myDB;
	if (_dbName != NULL)
		dbName = _dbName;

	rc = sqlite3_close(dbName);

	if(rc == SQLITE_OK)
	{
		csMsg.Format("Closed [%s] database successfully!!", m_dbFileName);
	}	
	else
	{
		csMsg.Format("Closed [%s] database failed...", m_dbFileName);
	}
	return csMsg;
}


// 유가증권 호가 Insert
void CDBProc_SQLite::InsertH1(char* _pSiseData, int _iIdx)
{
	CString csMsg;
	CString csQry, csQry1, csQry2;	
	CString csTableName = "H1";
	char *zErrMsg = 0;
	int rc;	
	Th1OutBlock* pSiseData = (Th1OutBlock*)(_pSiseData + 3);	
	
	csQry2.Format("('%s','%s','%d','%s','%s','%s','%s','%s','%s','%s', \
					'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s', \
					'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s', \
					'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s', \
					'%s','%s','%s','%s','%s','%s','%s');"
				
					, m_csTodayDate                 , SCOPY(pSiseData->code        )
					, _iIdx							, SCOPY(pSiseData->hotime      )
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
					, SCOPY(pSiseData->volume      ));
	
	csQry = _H1_QRY + csQry2;
	rc = sqlite3_exec(myDB, csQry, callback, 0, &zErrMsg);
	
	if(rc != SQLITE_OK)
	{
		csMsg.Format("Insert [%s] Table failed..ErrMsg[%s]", csTableName, zErrMsg);
		//LogPrint(csMsg);
		sqlite3_free(zErrMsg);		
	}	
}

// 코스닥 호가 Insert
void CDBProc_SQLite::InsertK3(char* _pSiseData, int _iIdx)
{
	CString csMsg;
	CString csQry, csQry1, csQry2;	
	CString csTableName = "K3";
	char *zErrMsg = 0;
	int rc;
	Tk3OutBlock* pSiseData = (Tk3OutBlock*)(_pSiseData + 3);	

	csQry2.Format("('%s','%s','%d','%s','%s','%s','%s','%s','%s','%s', \
					'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s', \
					'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s', \
					'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s', \
					'%s','%s','%s','%s','%s','%s','%s');"
				
					, m_csTodayDate                 , SCOPY(pSiseData->code        )
					, _iIdx							, SCOPY(pSiseData->hotime	   )
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
					, SCOPY(pSiseData->volume      ));
	
	csQry = _K3_QRY + csQry2;
	rc = sqlite3_exec(myDB, csQry, callback, 0, &zErrMsg);
	
	if(rc != SQLITE_OK)
	{
		csMsg.Format("Insert [%s] Table failed..ErrMsg[%s]", csTableName, zErrMsg);
		//LogPrint(csMsg);
		sqlite3_free(zErrMsg);		
	}
}

// 유가증권 실시간 체결 Insert
// 코스닥 구조체와 'PRICE'부분이 다름
void CDBProc_SQLite::InsertJ8(char* _pSiseData, int _iIdx)
{	
	CString csMsg;
	CString csQry, csQry1, csQry2;	
	CString csTableName = "J8";
	char *zErrMsg = 0;
	int rc;
	Tj8OutBlock* pSiseData = (Tj8OutBlock*)(_pSiseData + 3);	

	csQry2.Format("('%s','%s','%d','%s','%s','%s','%s','%s','%s','%s', \
					'%s','%s','%s','%s','%s','%s','%s','%s','%s');"

					, m_csTodayDate                 , SCOPY(pSiseData->code        )
					, _iIdx							, SCOPY(pSiseData->time		   )
					, SCOPY(pSiseData->sign        ), SCOPY(pSiseData->change      )
					, SCOPY(pSiseData->price	   ), SCOPY(pSiseData->chrate      )
					, SCOPY(pSiseData->high        ), SCOPY(pSiseData->low         )
					, SCOPY(pSiseData->offer       ), SCOPY(pSiseData->bid         )
					, SCOPY(pSiseData->volume      ), SCOPY(pSiseData->volrate     )
					, SCOPY(pSiseData->movolume    ), SCOPY(pSiseData->value       )
					, SCOPY(pSiseData->open        ), SCOPY(pSiseData->avgprice    )
					, SCOPY(pSiseData->janggubun   ));

	csQry = _J8_QRY + csQry2;
	rc = sqlite3_exec(myDB, csQry, callback, 0, &zErrMsg);

	if(rc != SQLITE_OK)
	{		
		csMsg.Format("Insert [%s] Table failed..ErrMsg[%s]", csTableName, zErrMsg);
		//LogPrint(csMsg);
		sqlite3_free(zErrMsg);		
	}
}

// 코스닥 실시간 체결 Insert
// 유가증권 구조체와 'PRICE'부분이 다름
void CDBProc_SQLite::InsertK8(char* _pSiseData, int _iIdx)
{	
	CString csMsg;
	CString csQry, csQry1, csQry2;	
	CString csTableName = "K8";
	char *zErrMsg = 0;
	int rc;
	Tk8OutBlock* pSiseData = (Tk8OutBlock*)(_pSiseData + 3);	
	
	csQry2.Format("('%s','%s','%d','%s','%s','%s','%s','%s','%s','%s', \
					'%s','%s','%s','%s','%s','%s','%s','%s','%s');"

					, m_csTodayDate                 , SCOPY(pSiseData->code        )
					, _iIdx							, SCOPY(pSiseData->time		   )
					, SCOPY(pSiseData->price	   ), SCOPY(pSiseData->sign        )
					, SCOPY(pSiseData->change      ), SCOPY(pSiseData->chrate      )
					, SCOPY(pSiseData->high        ), SCOPY(pSiseData->low         )
					, SCOPY(pSiseData->offer       ), SCOPY(pSiseData->bid         )
					, SCOPY(pSiseData->volume      ), SCOPY(pSiseData->volrate     )
					, SCOPY(pSiseData->movolume    ), SCOPY(pSiseData->value       )
					, SCOPY(pSiseData->open        ), SCOPY(pSiseData->avgprice    )
					, SCOPY(pSiseData->janggubun   ));

	csQry = _K8_QRY + csQry2;
	rc = sqlite3_exec(myDB, csQry, callback, 0, &zErrMsg);

	if(rc != SQLITE_OK)
	{
		csMsg.Format("Insert [%s] Table failed..ErrMsg[%s]", csTableName, zErrMsg);
		//LogPrint(csMsg);
		sqlite3_free(zErrMsg);
	}
}


// 시세데이터를 파일로 받은 뒤 DB로 변환하는 함수
CString CDBProc_SQLite::FileToDB(CString _csTrID, CString _csFileName)
{	
	CString csMsg;
	CString csSiseData;
	CStdioFile ptrFile;
	
	BOOL bFlag;
	bFlag = ptrFile.Open(_csFileName, CFile::modeRead, NULL);
	if (bFlag)
	{
		ptrFile.SeekToBegin();
	}
	else
	{
		csMsg.Format("[%s] 파일이 존재 하지 않습니다.", _csFileName);
		return csMsg;
	}
	int idx = 0;
	int rc;
	
	sqlite3_stmt * stmt;

	if( _csTrID == _TRID_H1)	{		
		
		CString csTableName = "H1";
		char *zErrMsg = 0;
		
		sqlite3_exec(myDB, "PRAGMA synchronous=OFF", NULL, NULL, NULL);
		sqlite3_exec(myDB, "PRAGMA count_changes=OFF", NULL, NULL, NULL);
		sqlite3_exec(myDB, "PRAGMA journal_mode=MEMORY", NULL, NULL, NULL);
		sqlite3_exec(myDB, "PRAGMA temp_store=MEMORY", NULL, NULL, NULL);
	
		sqlite3_prepare_v2(myDB, (LPSTR)(LPCTSTR)_H1_QRY, -1, &stmt, NULL);
		rc = sqlite3_exec(myDB, "BEGIN IMMEDIATE TRANSACTION;", NULL, NULL, NULL);

		while(ptrFile.ReadString(csSiseData))
		{			
	
			Th1OutBlock* pSiseData = (Th1OutBlock*) (((LPSTR)(LPCTSTR)csSiseData)+3);

			sqlite3_bind_text(stmt,  1, m_csTodayDate                 , -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  2, SCOPY(pSiseData->code        ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt ,  3, ++idx);
			sqlite3_bind_text(stmt,  4, SCOPY(pSiseData->hotime      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  5, SCOPY(pSiseData->offer       ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  6, SCOPY(pSiseData->bid         ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  7, SCOPY(pSiseData->offerrem    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  8, SCOPY(pSiseData->bidrem      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  9, SCOPY(pSiseData->P_offer     ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 10, SCOPY(pSiseData->P_bid       ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 11, SCOPY(pSiseData->P_offerrem  ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 12, SCOPY(pSiseData->P_bidrem    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 13, SCOPY(pSiseData->S_offer     ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 14, SCOPY(pSiseData->S_bid       ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 15, SCOPY(pSiseData->S_offerrem  ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 16, SCOPY(pSiseData->S_bidrem    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 17, SCOPY(pSiseData->S4_offer    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 18, SCOPY(pSiseData->S4_bid      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 19, SCOPY(pSiseData->S4_offerrem ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 20, SCOPY(pSiseData->S4_bidrem   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 21, SCOPY(pSiseData->S5_offer    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 22, SCOPY(pSiseData->S5_bid      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 23, SCOPY(pSiseData->S5_offerrem ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 24, SCOPY(pSiseData->S5_bidrem   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 25, SCOPY(pSiseData->T_offerrem  ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 26, SCOPY(pSiseData->T_bidrem    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 27, SCOPY(pSiseData->S6_offer    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 28, SCOPY(pSiseData->S6_bid      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 29, SCOPY(pSiseData->S6_offerrem ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 30, SCOPY(pSiseData->S6_bidrem   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 31, SCOPY(pSiseData->S7_offer    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 32, SCOPY(pSiseData->S7_bid      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 33, SCOPY(pSiseData->S7_offerrem ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 34, SCOPY(pSiseData->S7_bidrem   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 35, SCOPY(pSiseData->S8_offer    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 36, SCOPY(pSiseData->S8_bid      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 37, SCOPY(pSiseData->S8_offerrem ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 38, SCOPY(pSiseData->S8_bidrem   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 39, SCOPY(pSiseData->S9_offer    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 40, SCOPY(pSiseData->S9_bid      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 41, SCOPY(pSiseData->S9_offerrem ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 42, SCOPY(pSiseData->S9_bidrem   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 43, SCOPY(pSiseData->S10_offer   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 44, SCOPY(pSiseData->S10_bid     ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 45, SCOPY(pSiseData->S10_offerrem), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 46, SCOPY(pSiseData->S10_bidrem  ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 47, SCOPY(pSiseData->volume      ), -1, SQLITE_TRANSIENT);
			
			sqlite3_step(stmt);
			sqlite3_clear_bindings(stmt);
			sqlite3_reset(stmt);

			if(rc != SQLITE_OK)
			{
				csMsg.Format("Insert [%s] Table failed..ErrMsg[%s]", csTableName, zErrMsg);
				sqlite3_free(zErrMsg);		
			}
		}
		csMsg.Format("[%s][%s] Insert complete", _csTrID, _csFileName);
		ptrFile.Close();
	}
	else if( _csTrID == _TRID_K3)
	{
		
		CString csTableName = "K3";
		char *zErrMsg = 0;		

		sqlite3_exec(myDB, "PRAGMA synchronous=OFF", NULL, NULL, NULL);
		sqlite3_exec(myDB, "PRAGMA count_changes=OFF", NULL, NULL, NULL);
		sqlite3_exec(myDB, "PRAGMA journal_mode=MEMORY", NULL, NULL, NULL);
		sqlite3_exec(myDB, "PRAGMA temp_store=MEMORY", NULL, NULL, NULL);	

		sqlite3_prepare_v2(myDB, (LPSTR)(LPCTSTR)_K3_QRY, -1, &stmt, NULL);
		rc = sqlite3_exec(myDB, "BEGIN IMMEDIATE TRANSACTION;", NULL, NULL, NULL);

		while(ptrFile.ReadString(csSiseData))
		{	
			Tk3OutBlock* pSiseData = (Tk3OutBlock*) (((LPSTR)(LPCTSTR)csSiseData)+3);
			
			sqlite3_bind_text(stmt,  1, m_csTodayDate                 , -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  2, SCOPY(pSiseData->code        ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt ,  3, ++idx);
			sqlite3_bind_text(stmt,  4, SCOPY(pSiseData->hotime      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  5, SCOPY(pSiseData->offer       ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  6, SCOPY(pSiseData->bid         ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  7, SCOPY(pSiseData->offerrem    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  8, SCOPY(pSiseData->bidrem      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  9, SCOPY(pSiseData->P_offer     ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 10, SCOPY(pSiseData->P_bid       ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 11, SCOPY(pSiseData->P_offerrem  ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 12, SCOPY(pSiseData->P_bidrem    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 13, SCOPY(pSiseData->S_offer     ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 14, SCOPY(pSiseData->S_bid       ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 15, SCOPY(pSiseData->S_offerrem  ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 16, SCOPY(pSiseData->S_bidrem    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 17, SCOPY(pSiseData->S4_offer    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 18, SCOPY(pSiseData->S4_bid      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 19, SCOPY(pSiseData->S4_offerrem ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 20, SCOPY(pSiseData->S4_bidrem   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 21, SCOPY(pSiseData->S5_offer    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 22, SCOPY(pSiseData->S5_bid      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 23, SCOPY(pSiseData->S5_offerrem ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 24, SCOPY(pSiseData->S5_bidrem   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 25, SCOPY(pSiseData->T_offerrem  ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 26, SCOPY(pSiseData->T_bidrem    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 27, SCOPY(pSiseData->S6_offer    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 28, SCOPY(pSiseData->S6_bid      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 29, SCOPY(pSiseData->S6_offerrem ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 30, SCOPY(pSiseData->S6_bidrem   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 31, SCOPY(pSiseData->S7_offer    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 32, SCOPY(pSiseData->S7_bid      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 33, SCOPY(pSiseData->S7_offerrem ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 34, SCOPY(pSiseData->S7_bidrem   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 35, SCOPY(pSiseData->S8_offer    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 36, SCOPY(pSiseData->S8_bid      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 37, SCOPY(pSiseData->S8_offerrem ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 38, SCOPY(pSiseData->S8_bidrem   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 39, SCOPY(pSiseData->S9_offer    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 40, SCOPY(pSiseData->S9_bid      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 41, SCOPY(pSiseData->S9_offerrem ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 42, SCOPY(pSiseData->S9_bidrem   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 43, SCOPY(pSiseData->S10_offer   ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 44, SCOPY(pSiseData->S10_bid     ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 45, SCOPY(pSiseData->S10_offerrem), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 46, SCOPY(pSiseData->S10_bidrem  ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 47, SCOPY(pSiseData->volume      ), -1, SQLITE_TRANSIENT);
			
			sqlite3_step(stmt);
			sqlite3_clear_bindings(stmt);
			sqlite3_reset(stmt);

			if(rc != SQLITE_OK)
			{
				csMsg.Format("Insert [%s] Table failed..ErrMsg[%s]", csTableName, zErrMsg);
				sqlite3_free(zErrMsg);		
			}
		}
		csMsg.Format("[%s][%s] Insert complete", _csTrID, _csFileName);
		ptrFile.Close();
	}
	else if( _csTrID == _TRID_J8)
	{
		CString csTableName = "J8";
		char *zErrMsg = 0;		

		sqlite3_exec(myDB, "PRAGMA synchronous=OFF", NULL, NULL, NULL);
		sqlite3_exec(myDB, "PRAGMA count_changes=OFF", NULL, NULL, NULL);
		sqlite3_exec(myDB, "PRAGMA journal_mode=MEMORY", NULL, NULL, NULL);
		sqlite3_exec(myDB, "PRAGMA temp_store=MEMORY", NULL, NULL, NULL);
	
		sqlite3_prepare_v2(myDB, (LPSTR)(LPCTSTR)_J8_QRY, -1, &stmt, NULL);
		rc = sqlite3_exec(myDB, "BEGIN IMMEDIATE TRANSACTION;", NULL, NULL, NULL);

		while(ptrFile.ReadString(csSiseData))
		{	
			Tj8OutBlock* pSiseData = (Tj8OutBlock*) (((LPSTR)(LPCTSTR)csSiseData)+3);
			CString csMovolume;
			csMovolume.Format("%c",pSiseData->_movolume);

			sqlite3_bind_text(stmt,  1, m_csTodayDate                 , -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  2, SCOPY(pSiseData->code        ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt ,  3, ++idx);
			sqlite3_bind_text(stmt,  4, SCOPY(pSiseData->time        ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  5, SCOPY(pSiseData->sign        ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  6, SCOPY(pSiseData->change      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  7, SCOPY(pSiseData->price	     ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  8, SCOPY(pSiseData->chrate      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  9, SCOPY(pSiseData->high        ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 10, SCOPY(pSiseData->low         ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 11, SCOPY(pSiseData->offer       ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 12, SCOPY(pSiseData->bid         ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 13, SCOPY(pSiseData->volume      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 14, SCOPY(pSiseData->volrate     ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 15, SCOPY(pSiseData->movolume    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 16, csMovolume                    , -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 17, SCOPY(pSiseData->value       ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 18, SCOPY(pSiseData->open        ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 19, SCOPY(pSiseData->avgprice    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 20, SCOPY(pSiseData->janggubun   ), -1, SQLITE_TRANSIENT);
						
			sqlite3_step(stmt);
			sqlite3_clear_bindings(stmt);
			sqlite3_reset(stmt);

			if(rc != SQLITE_OK)
			{
				csMsg.Format("Insert [%s] Table failed..ErrMsg[%s]", csTableName, zErrMsg);
				sqlite3_free(zErrMsg);		
			}
		}
		csMsg.Format("[%s][%s] Insert complete", _csTrID, _csFileName);
		ptrFile.Close();
	}
	else if( _csTrID == _TRID_K8)
	{		
		CString csTableName = "K8";
		char *zErrMsg = 0;		

		sqlite3_exec(myDB, "PRAGMA synchronous=OFF", NULL, NULL, NULL);
		sqlite3_exec(myDB, "PRAGMA count_changes=OFF", NULL, NULL, NULL);
		sqlite3_exec(myDB, "PRAGMA journal_mode=MEMORY", NULL, NULL, NULL);
		sqlite3_exec(myDB, "PRAGMA temp_store=MEMORY", NULL, NULL, NULL);
	
		sqlite3_prepare_v2(myDB, (LPSTR)(LPCTSTR)_K8_QRY, -1, &stmt, NULL);
		rc = sqlite3_exec(myDB, "BEGIN IMMEDIATE TRANSACTION;", NULL, NULL, NULL);

		while(ptrFile.ReadString(csSiseData))
		{	
			Tk8OutBlock* pSiseData = (Tk8OutBlock*) (((LPSTR)(LPCTSTR)csSiseData)+3);
			CString csMovolume;
			csMovolume.Format("%c",pSiseData->_movolume);

			sqlite3_bind_text(stmt,  1, m_csTodayDate                 , -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  2, SCOPY(pSiseData->code        ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt ,  3, ++idx);
			sqlite3_bind_text(stmt,  4, SCOPY(pSiseData->time		 ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  5, SCOPY(pSiseData->price	     ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  6, SCOPY(pSiseData->sign        ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  7, SCOPY(pSiseData->change      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  8, SCOPY(pSiseData->chrate      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt,  9, SCOPY(pSiseData->high        ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 10, SCOPY(pSiseData->low         ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 11, SCOPY(pSiseData->offer       ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 12, SCOPY(pSiseData->bid         ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 13, SCOPY(pSiseData->volume      ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 14, SCOPY(pSiseData->volrate     ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 15, SCOPY(pSiseData->movolume    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 16, csMovolume                    , -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 17, SCOPY(pSiseData->value       ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 18, SCOPY(pSiseData->open        ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 19, SCOPY(pSiseData->avgprice    ), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 20, SCOPY(pSiseData->janggubun   ), -1, SQLITE_TRANSIENT);
			
			sqlite3_step(stmt);
			sqlite3_clear_bindings(stmt);
			sqlite3_reset(stmt);

			if(rc != SQLITE_OK)
			{
				csMsg.Format("Insert [%s] Table failed..ErrMsg[%s]", csTableName, zErrMsg);
				sqlite3_free(zErrMsg);		
			}
		}
		csMsg.Format("[%s][%s] Insert complete", _csTrID, _csFileName);
		ptrFile.Close();
	}
	else 
	{
		ptrFile.Close();
				
		csMsg.Format("[%s] 파일이 존재 하지 않습니다.", _csFileName);
	}

	rc = sqlite3_exec(myDB, "COMMIT;", NULL, NULL, NULL);	
	return csMsg;
}

void CDBProc_SQLite::SelectSymbolMastInfo(CString _csSymbol, Tc1101OutBlock *_ptrTc1101 )
{
	CString csQry, csMsg;
	//csQry.Format("SELECT * FROM SYMBOL_MASTER WHERE CODE = '%s' and DATE = '%s'", _csSymbol, m_csTodayDate);
	csQry.Format("SELECT uplmtprice, high , open, low, dnlmtprice, prepricez7, vi_recprice, vi_hprice, vi_lprice FROM SYMBOL_MASTER WHERE CODE = '%s' and DATE = '%s'", _csSymbol, m_csTodayDate);
	sqlite3_stmt *stmt;	// fetch 방식으로 가져오기
	int rc = sqlite3_prepare_v2(myDB, csQry, -1, &stmt, NULL);//preparing the statement

	if(rc != SQLITE_OK)
	{
		csMsg.Format("sqlite3_prepare_v2  failed..");
		//LogPrint(csMsg);
	}			
		
	while(1)
	{
		rc = sqlite3_step(stmt);//executing the statement    

		if(rc == SQLITE_DONE)
			break;
	
		memcpy(_ptrTc1101->uplmtprice	, sqlite3_column_text(stmt, 0), sizeof(_ptrTc1101->uplmtprice) );	// 상한가	
		memcpy(_ptrTc1101->high			, sqlite3_column_text(stmt, 1), sizeof(_ptrTc1101->high)		);	// 고가
		memcpy(_ptrTc1101->open			, sqlite3_column_text(stmt, 2), sizeof(_ptrTc1101->open)		);	// 시가
		memcpy(_ptrTc1101->low			, sqlite3_column_text(stmt, 3), sizeof(_ptrTc1101->low)		);	// 저가
		memcpy(_ptrTc1101->dnlmtprice	, sqlite3_column_text(stmt, 4), sizeof(_ptrTc1101->dnlmtprice) );	// 하한가
		memcpy(_ptrTc1101->prepricez7	, sqlite3_column_text(stmt, 5), sizeof(_ptrTc1101->prepricez7) );	// 전일종가
		memcpy(_ptrTc1101->vi_recprice	, sqlite3_column_text(stmt, 6),sizeof(_ptrTc1101->vi_recprice));	// VI기준가
		memcpy(_ptrTc1101->vi_hprice	, sqlite3_column_text(stmt, 7),sizeof(_ptrTc1101->vi_hprice)	);	//VI상승발동가
		memcpy(_ptrTc1101->vi_lprice	, sqlite3_column_text(stmt, 8),sizeof(_ptrTc1101->vi_lprice)	);	//VI하락발동가
	}

}

void CDBProc_SQLite::ChangeDate(CString _csDate)
{
	m_csTodayDate = _csDate;
}

CString CDBProc_SQLite::GetDate()
{
	return m_csTodayDate;
}

CString CDBProc_SQLite::SetDate(CString _csDate)
{
	m_csTodayDate= _csDate;
	return _T("[" + m_csTodayDate +"] 날짜가 변경 되었습니다."); 
}
CString CDBProc_SQLite::CreateDBFile(void)
{
	// ex) myDB_20210117.db
	m_dbFileName =	_T("myDB_") + m_csTodayDate +_T(".db");

	// DB 파일이 있는지 확인후 없으면 DB 파일 만들기 myDB_Sample.db -> myDB_YYYYMMDD.db
	CFileFind pFind;	
	if (!pFind.FindFile(m_dbFileName))
	{
		CopyFile(_T("myDB_Sample.db"), m_dbFileName, false);
		return _T("[" + m_dbFileName +"] 파일이 생성 되었습니다.");
	}
	else
		return _T("[" + m_dbFileName +"] 파일이 존재 합니다.");
	
}

//--------------------------------------------------------------------------------------------------------------
// 임시로 만들어서 컴파일까지만 해놨음.
// 원본 DB와 목적지 DB를 인자로 받아서 처리해야 되나?
//--------------------------------------------------------------------------------------------------------------
void CDBProc_SQLite::SelectInsertMastInfo()
{
	CString csSelectQry, csInsertQry, csMsg;

	char *zErrMsg = 0;	
	
	sqlite3_stmt* stmt_src;	// fetch 방식으로 가져오기
	sqlite3_stmt* stmt_dest;

	// select Qry 수정하기
	csSelectQry.Format("SELECT uplmtprice, high , open, low, dnlmtprice, prepricez7, vi_recprice, vi_hprice, vi_lprice FROM SYMBOL_MASTER");
	int rc = sqlite3_prepare_v2(myDB, csSelectQry, -1, &stmt_src, NULL);//preparing the statement

	// 조회 값이 없으면.
	if (rc != SQLITE_OK)
	{
		csMsg.Format("sqlite3_prepare_v2  failed..");	
		return;
	}

	// 목적지 DB 설정 TO DO: 목적지 DB이름 넣어주기
	sqlite3_exec(myDB, "PRAGMA synchronous=OFF", NULL, NULL, NULL);
	sqlite3_exec(myDB, "PRAGMA count_changes=OFF", NULL, NULL, NULL);
	sqlite3_exec(myDB, "PRAGMA journal_mode=MEMORY", NULL, NULL, NULL);
	sqlite3_exec(myDB, "PRAGMA temp_store=MEMORY", NULL, NULL, NULL);

    // TO DO Insert Qry 만들어 넣기
	csInsertQry.Format("INSERT INTO uplmtprice, high , open, low, dnlmtprice, prepricez7, vi_recprice, vi_hprice, vi_lprice FROM SYMBOL_MASTER");
	sqlite3_prepare_v2(myDB, (LPSTR)(LPCTSTR)"INSERT QRY", -1, &stmt_dest, NULL);
	rc = sqlite3_exec(myDB, "BEGIN IMMEDIATE TRANSACTION;", NULL, NULL, NULL);


	if (rc != SQLITE_OK)
	{
		csMsg.Format("sqlite3_prepare_v2  failed..");		
		return;
	}

	int idx = 0;
	while (1)
	{
		rc = sqlite3_step(stmt_src);//executing the statement    

		if (rc == SQLITE_DONE)
			break;

		/*
		memcpy(_ptrTc1101->uplmtprice, sqlite3_column_text(stmt_src, 0), sizeof(_ptrTc1101->uplmtprice));	// 상한가	
		memcpy(_ptrTc1101->high, sqlite3_column_text(stmt_src, 1), sizeof(_ptrTc1101->high));	// 고가
		memcpy(_ptrTc1101->open, sqlite3_column_text(stmt_src, 2), sizeof(_ptrTc1101->open));	// 시가
		memcpy(_ptrTc1101->low, sqlite3_column_text(stmt_src, 3), sizeof(_ptrTc1101->low));	// 저가
		memcpy(_ptrTc1101->dnlmtprice, sqlite3_column_text(stmt_src, 4), sizeof(_ptrTc1101->dnlmtprice));	// 하한가
		memcpy(_ptrTc1101->prepricez7, sqlite3_column_text(stmt_src, 5), sizeof(_ptrTc1101->prepricez7));	// 전일종가
		memcpy(_ptrTc1101->vi_recprice, sqlite3_column_text(stmt_src, 6), sizeof(_ptrTc1101->vi_recprice));	// VI기준가
		memcpy(_ptrTc1101->vi_hprice, sqlite3_column_text(stmt_src, 7), sizeof(_ptrTc1101->vi_hprice));	//VI상승발동가
		memcpy(_ptrTc1101->vi_lprice, sqlite3_column_text(stmt_src, 8), sizeof(_ptrTc1101->vi_lprice));	//VI하락발동가
		*/
		
		CString csMovolume;
		csMovolume.Format("%c", "TEST");

		sqlite3_bind_text(stmt_dest, 1, (const char*)sqlite3_column_text(stmt_src, 0), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt_dest, 2, (const char*)sqlite3_column_text(stmt_src, 1), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt_dest, 3, ++idx);		
		sqlite3_bind_text(stmt_dest, 4, (const char*)sqlite3_column_text(stmt_src, 3), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt_dest, 5, (const char*)sqlite3_column_text(stmt_src, 4), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt_dest, 6, (const char*)sqlite3_column_text(stmt_src, 5), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt_dest, 7, (const char*)sqlite3_column_text(stmt_src, 6), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt_dest, 8, (const char*)sqlite3_column_text(stmt_src, 7), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt_dest, 9, (const char*)sqlite3_column_text(stmt_src, 8), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt_dest, 10, (const char*)sqlite3_column_text(stmt_src, 9), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt_dest, 11, (const char*)sqlite3_column_text(stmt_src, 10), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt_dest, 16, (const char*)csMovolume, -1, SQLITE_TRANSIENT);

		sqlite3_step(stmt_dest);
		sqlite3_clear_bindings(stmt_dest);
		sqlite3_reset(stmt_dest);

		if (rc != SQLITE_OK)
		{
			//csMsg.Format("Insert [%s] Table failed..ErrMsg[%s]", csTableName, zErrMsg);
			sqlite3_free(zErrMsg);
		}				
	}
}