// WerrorDLL.cpp : implementation file
//

#include "stdafx.h"
#include "interface.h"
#include "WerrorDLL.h"


// WerrorDLL dialog

IMPLEMENT_DYNAMIC(WerrorDLL, CDialog)

WerrorDLL::WerrorDLL(CWnd* pParent /*=NULL*/)
	: CDialog(WerrorDLL::IDD, pParent)
{

}

WerrorDLL::~WerrorDLL()
{
}

void WerrorDLL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(WerrorDLL, CDialog)
END_MESSAGE_MAP()


// WerrorDLL message handlers

BOOL WerrorDLL::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}
