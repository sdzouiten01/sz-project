#pragma once
#include "afxwin.h"
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;


// Wfini dialog

class Wfini : public CDialog
{
	DECLARE_DYNAMIC(Wfini)

public:
	Wfini(CWnd* pParent = NULL);   // standard constructor
	virtual ~Wfini();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//virtual BOOL OnInitDialog(CString time);

	DECLARE_MESSAGE_MAP()
public:
	/*CEdit m_time_msg;*/
	
	afx_msg void OnBnClickedButton1();
};
