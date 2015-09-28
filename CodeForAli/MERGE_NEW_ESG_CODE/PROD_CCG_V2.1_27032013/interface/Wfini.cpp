// Wfini.cpp : implementation file
//

#include "stdafx.h"
#include "interface.h"
#include "Wfini.h"
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

// Wfini dialog

IMPLEMENT_DYNAMIC(Wfini, CDialog)

Wfini::Wfini(CWnd* pParent /*=NULL*/)
	: CDialog(Wfini::IDD, pParent)
{

}

Wfini::~Wfini()
{
}

void Wfini::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT1, m_time_msg);
}


BEGIN_MESSAGE_MAP(Wfini, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &Wfini::OnBnClickedButton1)
END_MESSAGE_MAP()


// Wfini message handlers

//BOOL Wfini::OnInitDialog(CString time)
//{
//	CDialog::OnInitDialog();
//	CString str;
//	str =(CString) "Work done in ";
//	str+= time;
//	str+= " ms ";
//	m_time_msg.SetWindowTextW (str);
//	m_time_msg.SetReadOnly(true);
//
//	return TRUE;  // return TRUE  unless you set the focus to a control
//}

BOOL Wfini::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void Wfini::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	OnOK();
	/*Wfini::CloseWindow();*/
}
