#include "stdafx.h"
#include "util.h"


//--------------------------------------------------------------------------------------------------
// HH:MM:SS -> 숫자값으로 변경
// 30초 단위로 변경, 
//    시간을 30초 단위로 변경했을경우 1시간에 120, 장이 9시에 시작하므로 -9를 한 후에 * 120 
//    분을 30초 단위로 변경 시 * 2, 
//    초가 0 이거나, 30초 보다 클 경우 '0', 나머지 '1'
// ex) 09:23:24 
// HH: (09-9) * 120 = 0
// MM: 23 * 2 = 46
// SS: 0이거나 30보다 크면 -> 0, 나머지 -> 1, 따라서 '0' // 2020.06.03 수정
// 0 + 46 + 0 = 46
//--------------------------------------------------------------------------------------------------
UINT ConvertTime(char* _time)
{
	int iTime, iHour, iMinute, iSecond;
	char cHour[3] = {0,};
	char cMinute[3] = {0};
	char cSecond[3] = {0};

	memcpy(cHour, _time, 2);
	memcpy(cMinute, _time+3, 2);
	memcpy(cSecond, _time+6, 2);

	iHour = atoi(cHour);
	iMinute = atoi(cMinute);
	iSecond = atoi(cSecond);

	// 0이면 0, 1~30 이면 1, 31~59면 2	
	if(iSecond == 0)
		iSecond = 0;
	else if (iSecond < 31)
		iSecond = 1;
	else
		iSecond = 2;
	
	iTime = ((iHour - 9) * 120) + (iMinute * 2) + iSecond;

	if (iTime < 0) // 장전시간외종가시간(마이너스)은 0으로 리턴
		return 0;
	else if (iTime < 801) // 정규장시간 0 ~ 800 까지는 1 ~ 801로 리턴
		return iTime + 1;
	else if (iTime < 841) // 시간외종가는 802로 리턴, 	
		return 802;
	else 
		return -1;	
}

//	문자열을 구조체 필드로 적절히 채우는 유틸리티 함수입니다. (넘치면 잘림)
void	_smove(char* szTarget,int nSize,CString strSource)
{
	int	nMin	=min(nSize,strSource.GetLength());

	memset(szTarget,' ',nSize);
	strncpy(szTarget,strSource,nMin);
}

//	숫자를 구조체 필드로 적절히 채우는 유틸리티 함수입니다.	  (넘치면 버림)
void	_nmove(char* szTarget,int nTarget,CString strSource)
{
	if(strSource.GetLength()>nTarget)
		return;

	memset(szTarget,'0',nTarget);
	strncpy(szTarget+nTarget-strSource.GetLength(),strSource,strSource.GetLength());
}

//	구조체 필드값을 문자열 형태로 변환하는 유틸리티 함수입니다.
CString	_scopy(const char* szData,int nSize)
{
	char	szBuf[256];				//필드의 최대 크기는 상황에 따라 조절할 필요가 있음
	memset(szBuf,0,sizeof szBuf);
	strncpy(szBuf,szData,nSize);
	
	return szBuf;
}

//	구조체 필드값을 숫자 형태로 변환하는 유틸리티 함수입니다.
CString	_ncopy(const char* szData,int nSize)
{
	CString	strInput = _scopy(szData,nSize);

	for(int i=0;i<strInput.GetLength();i++)
		if(strInput.GetAt(i)=='0')
		{
			//마지막 '0' 또는 다음에 소수점이 오는 경우에는 지우지 않는다
			if(i!=strInput.GetLength()-1 && strInput.GetAt(i+1)!='.')	
				strInput.SetAt(i,' ');
		}
		else if(strInput.GetAt(i)=='-')	//minus
			;
		else if(strInput.GetAt(i)==' ')	//숫자형임에도 공백문자가 오는 경우가 있음
			;
		else
			break;

	return	strInput;
}

//	 숫자에 천 단위로 컴마 삽입하는 유틸리티 함수입니다.
CString	_comma(const char* szData,int nSize,int decimal)
{
	CString	strInput	=_scopy(szData,nSize);
	strInput.TrimLeft();
	strInput.TrimRight();
	
	double	fInput	=atof(strInput);

	CString	strTemp;
	strTemp.Format("%0.*f",decimal,fInput);

	if(fInput>=1000000000.0F)
		strTemp.Insert(strTemp.GetLength()-9,',');
	if(fInput>=1000000.0F)
		strTemp.Insert(strTemp.GetLength()-6,',');
	if(fInput>=1000.0F)
		strTemp.Insert(strTemp.GetLength()-3,',');

	CString	strOutput;
	strOutput.Format("%*s",nSize,strTemp);

	return strOutput;
}

// 디렉토리 체크
void CheckDirectory(CString _csPath)
{
	CString csPrefix(_T("")), csToken(_T(""));
    int nStart = 0, nEnd;
    while( (nEnd = _csPath.Find('/', nStart)) >= 0)
    {
        CString csToken = _csPath.Mid(nStart, nEnd-nStart);
        CreateDirectory(csPrefix + csToken, NULL);

        csPrefix += csToken;
        csPrefix += _T("/");
        nStart = nEnd+1;
    }
    csToken = _csPath.Mid(nStart);
    CreateDirectory(csPrefix + csToken, NULL);
}

// 날짜/시간 가져오는 함수
CString GetDateTime(int _iType)
{
	CTime cTime = CTime::GetCurrentTime();		
	// %Y: 전체(2019), %y: 뒤 2자리(19)
	CString csDate;
	
	switch(_iType)
	{
	case 1:
		csDate.Format("%04d", cTime.GetYear());
		break;
	case 2:
		csDate.Format("%02d", cTime.GetMonth());
		break;
	case 3:
		csDate.Format("%02d", cTime.GetDay());
		break;
	case 4:
		csDate.Format("%02d", cTime.GetHour());
		break;
	case 5:
		csDate.Format("%02d", cTime.GetMinute());
		break;
	case 6:
		csDate.Format("%02d", cTime.GetSecond());
		break;
	case 7:
		// 09:23:59
		csDate = cTime.Format("%H:%M:%S");
		break;
	case 8:
		// %Y: 전체(2019), %y: 뒤 2자리(19)
		csDate= cTime.Format("%Y%m%d");
		break;
	case 9:
		// 밀리세컨드
		SYSTEMTIME cur_time;
		GetLocalTime(&cur_time);
		csDate.Format("[%02d:%02d:%02d.%03d]",cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);
		break;
	default:
		csDate = cTime.Format("%Y%m%d");
		break;
	}

	return csDate;

	/* 출력 타입 참고
	년
	%y - 뒤 2자리 (09)
	%Y - 전체 (2009) 

	월
	%m - 01~12 (03)
	%b - 영어 3글자 (Mar)
	%B - 영어 전체 (March) 

	일
	%d - 01~31 월 기준 (10)
	%j - 001~365 년 기준 (069)
 
	시
	%H - 00~23 24시 기준 (14)
	%I (대문자 아이) - 01~12 12시 기준 (02) 

	분
	%M - 00~59 (12) 

	초
	%S - 00~59 (15) 

	요일
	%a - 요약 (Tue)
	%A - 요일 (Tuesday)
	%w - 일요일 0 기준 (2)

	주
	%U - 00~53 년기준으로 시작주 일요일 (10)
	%W - 00~53 년기준으로 시작주 월요일 (10) 

	기타
	%p - AM/PM (PM)
	%x - 월/일/년 (03/10/09)
	%X - 시:분:초 (14:12:15)
	%c - 월/일/년 시:분:초 (03/10/09 14:12:15)
	%% - % 기호 (%)

	%z, %Z - 시간대 이름 (대한민국 표준시)
	*/

}