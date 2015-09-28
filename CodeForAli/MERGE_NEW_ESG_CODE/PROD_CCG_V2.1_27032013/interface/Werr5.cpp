// Werr5.cpp : implementation file
//

#include "stdafx.h"
#include "interface.h"
#include "Werr5.h"


// Werr5 dialog

IMPLEMENT_DYNAMIC(Werr5, CDialog)

Werr5::Werr5(CWnd* pParent /*=NULL*/)
	: CDialog(Werr5::IDD, pParent)
{

}

Werr5::~Werr5()
{
}

void Werr5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Werr5, CDialog)
END_MESSAGE_MAP()


// Werr5 message handlers

BOOL Werr5::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}
