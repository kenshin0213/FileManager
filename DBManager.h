
// DBManager.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

// CDBManagerApp:
// �� Ŭ������ ������ ���ؼ��� DBManager.cpp�� �����Ͻʽÿ�.
//

class CDBManagerApp : public CWinApp
{
public:
	CDBManagerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CDBManagerApp theApp;
