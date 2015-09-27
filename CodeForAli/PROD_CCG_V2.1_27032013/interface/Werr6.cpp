// Werr6.cpp : implementation file
//

#include "stdafx.h"
#include "interface.h"
#include "Werr6.h"


// Werr6 dialog

IMPLEMENT_DYNAMIC(Werr6, CDialog)

Werr6::Werr6(CWnd* pParent /*=NULL*/)
	: CDialog(Werr6::IDD, pParent)
{

}

Werr6::~Werr6()
{
}

void Werr6::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Werr6, CDialog)
END_MESSAGE_MAP()


// Werr6 message handlers

BOOL Werr6::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}
