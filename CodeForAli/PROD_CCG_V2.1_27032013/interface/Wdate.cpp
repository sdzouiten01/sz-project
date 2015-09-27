// Wdate.cpp : implementation file
//

#include "stdafx.h"
#include "interface.h"
#include "Wdate.h"


// Wdate dialog

IMPLEMENT_DYNAMIC(Wdate, CDialog)

Wdate::Wdate(CWnd* pParent /*=NULL*/)
	: CDialog(Wdate::IDD, pParent)
{

}

Wdate::~Wdate()
{
}

void Wdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Wdate, CDialog)
END_MESSAGE_MAP()


// Wdate message handlers

BOOL Wdate::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}
