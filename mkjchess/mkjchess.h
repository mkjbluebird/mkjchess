
// mkjchess.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "define.h"

// CmkjchessApp: 
// �йش����ʵ�֣������ mkjchess.cpp
//

class CmkjchessApp : public CWinApp
{
public:
	CmkjchessApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CmkjchessApp theApp;