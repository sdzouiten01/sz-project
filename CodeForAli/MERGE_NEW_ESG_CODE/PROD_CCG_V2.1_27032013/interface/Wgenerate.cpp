// Wgenerate.cpp : implementation file
//

#include "stdafx.h"
#include "interface.h"
#include "Wgenerate.h"


// Wgenerate dialog

IMPLEMENT_DYNAMIC(Wgenerate, CDialog)

Wgenerate::Wgenerate(CWnd* pParent /*=NULL*/)
	: CDialog(Wgenerate::IDD, pParent)
{

}

Wgenerate::~Wgenerate()
{
}

void Wgenerate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Wgenerate, CDialog)
END_MESSAGE_MAP()


// Wgenerate message handlers
