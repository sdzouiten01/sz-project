#pragma once


// Werr3 dialog

class Werr3 : public CDialog
{
	DECLARE_DYNAMIC(Werr3)

public:
	Werr3(CWnd* pParent = NULL);   // standard constructor
	virtual ~Werr3();

// Dialog Data
	enum { IDD = IDD_DIALOG10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
