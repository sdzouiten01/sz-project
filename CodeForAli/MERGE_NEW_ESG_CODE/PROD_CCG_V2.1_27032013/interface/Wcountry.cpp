// Wcountry.cpp : implementation file
//

#include "stdafx.h"
#include "interface.h"
#include "Wcountry.h"


// Wcountry dialog

IMPLEMENT_DYNAMIC(Wcountry, CDialog)

Wcountry::Wcountry(CWnd* pParent /*=NULL*/)
	: CDialog(Wcountry::IDD, pParent)
{

}

Wcountry::~Wcountry()
{
}

void Wcountry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Wcountry, CDialog)
END_MESSAGE_MAP()


// Wcountry message handlers

BOOL Wcountry::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

