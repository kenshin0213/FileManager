#pragma once
#pragma warning(disable:4996) // C4996 에러를 무시

#define SMOVE(fTarget,strSource)	_smove(fTarget,sizeof fTarget,strSource)
#define NMOVE(fTarget,strSource)	_nmove(fTarget,sizeof fTarget,strSource)
#define SCOPY(x)	_scopy(x,sizeof x)
#define NCOPY(x)	_ncopy(x,sizeof x)
#define COMMA(x)	_comma(x,sizeof x)
#define COMMAF(x,d)	_comma(x,sizeof x,d)

void	_smove(char* szTarget,int nSize,CString strSource);
void	_nmove(char* szTarget,int nTarget,CString strSource);
CString	_scopy(const char* szData,int nSize);
CString	_ncopy(const char* szData,int nSize);
CString	_comma(const char* szData,int nSize,int decimal=0);

void CheckDirectory(CString _csPath);
// HH:MM:SS 를 int값을 변경(수신 시각을 30초 단위 int 값으로 변경
UINT ConvertTime(char* _time); 

CString GetDateTime(int iType = 0);
