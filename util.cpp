#include "stdafx.h"
#include "util.h"


//--------------------------------------------------------------------------------------------------
// HH:MM:SS -> ���ڰ����� ����
// 30�� ������ ����, 
//    �ð��� 30�� ������ ����������� 1�ð��� 120, ���� 9�ÿ� �����ϹǷ� -9�� �� �Ŀ� * 120 
//    ���� 30�� ������ ���� �� * 2, 
//    �ʰ� 0 �̰ų�, 30�� ���� Ŭ ��� '0', ������ '1'
// ex) 09:23:24 
// HH: (09-9) * 120 = 0
// MM: 23 * 2 = 46
// SS: 0�̰ų� 30���� ũ�� -> 0, ������ -> 1, ���� '0' // 2020.06.03 ����
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

	// 0�̸� 0, 1~30 �̸� 1, 31~59�� 2	
	if(iSecond == 0)
		iSecond = 0;
	else if (iSecond < 31)
		iSecond = 1;
	else
		iSecond = 2;
	
	iTime = ((iHour - 9) * 120) + (iMinute * 2) + iSecond;

	if (iTime < 0) // �����ð��������ð�(���̳ʽ�)�� 0���� ����
		return 0;
	else if (iTime < 801) // ������ð� 0 ~ 800 ������ 1 ~ 801�� ����
		return iTime + 1;
	else if (iTime < 841) // �ð��������� 802�� ����, 	
		return 802;
	else 
		return -1;	
}

//	���ڿ��� ����ü �ʵ�� ������ ä��� ��ƿ��Ƽ �Լ��Դϴ�. (��ġ�� �߸�)
void	_smove(char* szTarget,int nSize,CString strSource)
{
	int	nMin	=min(nSize,strSource.GetLength());

	memset(szTarget,' ',nSize);
	strncpy(szTarget,strSource,nMin);
}

//	���ڸ� ����ü �ʵ�� ������ ä��� ��ƿ��Ƽ �Լ��Դϴ�.	  (��ġ�� ����)
void	_nmove(char* szTarget,int nTarget,CString strSource)
{
	if(strSource.GetLength()>nTarget)
		return;

	memset(szTarget,'0',nTarget);
	strncpy(szTarget+nTarget-strSource.GetLength(),strSource,strSource.GetLength());
}

//	����ü �ʵ尪�� ���ڿ� ���·� ��ȯ�ϴ� ��ƿ��Ƽ �Լ��Դϴ�.
CString	_scopy(const char* szData,int nSize)
{
	char	szBuf[256];				//�ʵ��� �ִ� ũ��� ��Ȳ�� ���� ������ �ʿ䰡 ����
	memset(szBuf,0,sizeof szBuf);
	strncpy(szBuf,szData,nSize);
	
	return szBuf;
}

//	����ü �ʵ尪�� ���� ���·� ��ȯ�ϴ� ��ƿ��Ƽ �Լ��Դϴ�.
CString	_ncopy(const char* szData,int nSize)
{
	CString	strInput = _scopy(szData,nSize);

	for(int i=0;i<strInput.GetLength();i++)
		if(strInput.GetAt(i)=='0')
		{
			//������ '0' �Ǵ� ������ �Ҽ����� ���� ��쿡�� ������ �ʴ´�
			if(i!=strInput.GetLength()-1 && strInput.GetAt(i+1)!='.')	
				strInput.SetAt(i,' ');
		}
		else if(strInput.GetAt(i)=='-')	//minus
			;
		else if(strInput.GetAt(i)==' ')	//�������ӿ��� ���鹮�ڰ� ���� ��찡 ����
			;
		else
			break;

	return	strInput;
}

//	 ���ڿ� õ ������ �ĸ� �����ϴ� ��ƿ��Ƽ �Լ��Դϴ�.
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

// ���丮 üũ
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

// ��¥/�ð� �������� �Լ�
CString GetDateTime(int _iType)
{
	CTime cTime = CTime::GetCurrentTime();		
	// %Y: ��ü(2019), %y: �� 2�ڸ�(19)
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
		// %Y: ��ü(2019), %y: �� 2�ڸ�(19)
		csDate= cTime.Format("%Y%m%d");
		break;
	case 9:
		// �и�������
		SYSTEMTIME cur_time;
		GetLocalTime(&cur_time);
		csDate.Format("[%02d:%02d:%02d.%03d]",cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);
		break;
	default:
		csDate = cTime.Format("%Y%m%d");
		break;
	}

	return csDate;

	/* ��� Ÿ�� ����
	��
	%y - �� 2�ڸ� (09)
	%Y - ��ü (2009) 

	��
	%m - 01~12 (03)
	%b - ���� 3���� (Mar)
	%B - ���� ��ü (March) 

	��
	%d - 01~31 �� ���� (10)
	%j - 001~365 �� ���� (069)
 
	��
	%H - 00~23 24�� ���� (14)
	%I (�빮�� ����) - 01~12 12�� ���� (02) 

	��
	%M - 00~59 (12) 

	��
	%S - 00~59 (15) 

	����
	%a - ��� (Tue)
	%A - ���� (Tuesday)
	%w - �Ͽ��� 0 ���� (2)

	��
	%U - 00~53 ��������� ������ �Ͽ��� (10)
	%W - 00~53 ��������� ������ ������ (10) 

	��Ÿ
	%p - AM/PM (PM)
	%x - ��/��/�� (03/10/09)
	%X - ��:��:�� (14:12:15)
	%c - ��/��/�� ��:��:�� (03/10/09 14:12:15)
	%% - % ��ȣ (%)

	%z, %Z - �ð��� �̸� (���ѹα� ǥ�ؽ�)
	*/

}