 
// GraphicEdit.h : GraphicEdit ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CGraphicEditApp:
// �� Ŭ������ ������ ���ؼ��� GraphicEdit.cpp�� �����Ͻʽÿ�.
//

class CGraphicEditApp : public CWinApp
{
public:
	CGraphicEditApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGraphicEditApp theApp;
