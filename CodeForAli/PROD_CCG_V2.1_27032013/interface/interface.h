// interface.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CinterfaceApp:
// See interface.cpp for the implementation of this class
//

class CinterfaceApp : public CWinApp
{
public:
	CinterfaceApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CinterfaceApp theApp;