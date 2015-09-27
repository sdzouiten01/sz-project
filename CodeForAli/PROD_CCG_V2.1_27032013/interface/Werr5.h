#pragma once


// Werr5 dialog

class Werr5 : public CDialog
{
	DECLARE_DYNAMIC(Werr5)

public:
	Werr5(CWnd* pParent = NULL);   // standard constructor
	virtual ~Werr5();

// Dialog Data
	enum { IDD = IDD_DIALOG12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
