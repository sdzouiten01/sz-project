// interfaceDlg.h : header file
//

#pragma once
#include "in_bom.h"
#include "afxwin.h"
#include "afxdtctl.h"


// CinterfaceDlg dialog
class CinterfaceDlg : public CDialog
{
// Construction
public:
	CinterfaceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_INTERFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CIn_bom m_in_bom;
	CIn_bom m_in_eom;
	CIn_bom m_in_valo;
	CIn_bom m_in_splits;
	CIn_bom m_eom_prod;
	CIn_bom m_eom_model;
	CComboBox m_country;
	CComboBox m_mode;

	double m_rrc;
	double m_lapse;
	double m_death;
	CButton m_run;
	CButton m_reset;
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedRun();
	DECLARE_EVENTSINK_MAP()
	CIn_bom m_new_business;
	afx_msg void OnBnClickedButton1();
	CButton m_keep;
	CButton m_keep_envi;
	CButton m_keep_options;
	CEdit m_dir_out_bis;
	CDateTimeCtrl m_date_bom;
	CDateTimeCtrl m_date_eom;
	CIn_bom m_log;
};
