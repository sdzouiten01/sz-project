// Werr2.cpp : implementation file
//

#include "stdafx.h"
#include "interface.h"
#include "Werr2.h"


// Werr2 dialog

IMPLEMENT_DYNAMIC(Werr2, CDialog)

Werr2::Werr2(CWnd* pParent /*=NULL*/)
	: CDialog(Werr2::IDD, pParent)
{

}

Werr2::~Werr2()
{
}

void Werr2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Werr2, CDialog)
END_MESSAGE_MAP()


// Werr2 message handlers
BOOL Werr2::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}