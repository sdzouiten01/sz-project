// interfaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include <string>
#include <map>

#include "interface.h"
#include "interfaceDlg.h"
#include "Wcountry.h"
#include "Wfilename.h"
#include "Wfileexist.h"
#include "Wgenerate.h"
#include "Wfini.h"
#include "Wdate.h"
#include "WerrorDLL.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "GMXBException.h"
//#include "C:\Hedge_Eff_configuration\aging_policies\aging_policies\ap_exception.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Werr1.h"
#include "Werr2.h"
#include "Werr3.h"
#include "Werr4.h"
#include "Werr5.h"
#include "Werr6.h"



//int _declspec (dllimport) main_dll(string& execut_config_file);

extern "C"
{
_declspec (dllimport) int run_aging_policies( string& config, ofstream& log_strm, int tot_nb_workers, int w_id, string& s);
_declspec (dllimport) void XML_set(const string& xml_file,const map<string,map<string,string>>& pack);
}

#include <windows.h>

//#include "C:\Hedge_Eff_configuration\aging_policies\aging_policies\aging_policies.cpp"
// #include "C:\Hedge_Eff_configuration\tutu\tutu\tutu.cpp"


using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About




class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CinterfaceDlg dialog




CinterfaceDlg::CinterfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CinterfaceDlg::IDD, pParent)
	, m_rrc(0)
	, m_lapse(0)
	, m_death(0)

{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CinterfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IN_BOM, m_in_bom);
	DDX_Control(pDX, IDC_IN_EOM, m_in_eom);
	DDX_Control(pDX, IDC_IN_VALO, m_in_valo);
	DDX_Control(pDX, IDC_IN_SPLIT, m_in_splits);
	DDX_Control(pDX, IDC_EOM_PROD, m_eom_prod);
	DDX_Control(pDX, IDC_EOM_MODEL, m_eom_model);
	DDX_Control(pDX, IDC_COUNTRY, m_country);
	DDX_Control(pDX, IDC_MODE, m_mode);
	DDX_Text(pDX, IDC_RRC, m_rrc);
	DDX_Text(pDX, IDC_LAPSE, m_lapse);
	DDX_Text(pDX, IDC_DEATH, m_death);
	DDX_Control(pDX, IDC_RUN, m_run);
	DDX_Control(pDX, IDC_RESET, m_reset);

	DDX_Control(pDX, IDC_EOM_MODEL2, m_new_business);
	DDX_Control(pDX, IDC_TEMP_Y_N2, m_keep);
	DDX_Control(pDX, IDC_TEMP_Y_N3, m_keep_envi);
	DDX_Control(pDX, IDC_TEMP_Y_N4, m_keep_options);
	DDX_Control(pDX, IDC_EDIT1, m_dir_out_bis);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_date_bom);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_date_eom);
	DDX_Control(pDX, IDC_SELECTFILE1, m_log);
}

BEGIN_MESSAGE_MAP(CinterfaceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RESET, &CinterfaceDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_RUN, &CinterfaceDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_BUTTON1, &CinterfaceDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CinterfaceDlg message handlers

BOOL CinterfaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_mode.SetCurSel(3);
	m_rrc = 1;
	m_lapse = 1;
	m_death = 1;

	UpdateData(false);

	//m_dir_out_bis.SetWindowTextW((CString)"C:\\Actuarial Module");
	//for setup
	m_dir_out_bis.SetWindowTextW((CString)"C:\\Program Files\\Actuarial Module");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CinterfaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CinterfaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CinterfaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CinterfaceDlg::OnBnClickedReset()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	
	CString str = _T("");
	m_in_bom.put_FileName(str);
	m_in_eom.put_FileName(str);

	if (m_keep.GetCheck()==0)
	{
		m_in_valo.put_FileName(str);
		m_in_splits.put_FileName(str);
	}

	m_eom_prod.put_FileName(str);
	m_eom_model.put_FileName(str);
	m_new_business.put_FileName(str);
	m_log.put_FileName(str);

	if (m_keep_envi.GetCheck()==0)
	{
		m_country.SetCurSel(-1);
		//initialiser les date time picker à la date d'aujourd'hui
		//LPSYSTEMTIME t = 0;
		//m_date_bom.operator HWND();
		//m_date_eom.SetTime(t);
		
	}

	if (m_keep_options.GetCheck() == 0)
	{
		m_mode.SetCurSel(3);
		m_rrc = 1;
		m_lapse = 1;
		m_death = 1;
	}

	UpdateData(false);
}

string CStr_to_str (const CString& CStr)
{
	CStringA strA(CStr);
	std::string Mystring =strA.GetBuffer();
	return Mystring;
}

string double_to_str (double num)
{
	std::ostringstream oss;
	oss<<num;
	string str;
	str = oss.str();
	return str;
}



void CinterfaceDlg::OnBnClickedRun()
{
	// TODO: Add your control notification handler code here
	try
	{
		clock_t start, end;
		start = clock();
		
		//CString temp_dir = _T("C:\\Actuarial Module\\Temporary\\");
		//for setup
		CString temp_dir = _T("C:\\Program Files\\Actuarial Module\\Temporary\\");
		
		WIN32_FIND_DATA lpFindFileData;
		HANDLE hFind = FindFirstFile(temp_dir,&lpFindFileData);
		if (hFind=INVALID_HANDLE_VALUE)
		{
			CreateDirectoryW(temp_dir,0);
		}

		CString log;
		log = m_log.get_FileName();
		ofstream file_log (log,ios::out);
		if (file_log == _T(""))
		{
			Wfilename dia000;
			dia000.DoModal();
			return;
		}

		if (!file_log.is_open())
		{
			Wfileexist dia001;
			dia001.DoModal();
			return;
		}

		file_log<<"AGING POLICIES : LOG";
		file_log<<("\n");
		file_log<<("\n");


		CString in_bom;
		in_bom = m_in_bom.get_FileName();
		ifstream file1 (in_bom);

		file_log<<("INPUTS : ");
		file_log<<("\n");
		file_log<<("inforce BOM : ");
		file_log<<CStr_to_str(in_bom);
		file_log<<("\n");
		
		CString in_eom;
		in_eom = m_in_eom.get_FileName();
		ifstream file2 (in_eom);
		file_log<<("inforce EOM : ");
		file_log<<CStr_to_str(in_eom);
		file_log<<("\n");

		CString in_valo;
		in_valo = m_in_valo.get_FileName();
		ifstream file3 (in_valo);
		file_log<<("Profiles Valorisation file (EOM) : ");
		file_log<<CStr_to_str(in_valo);
		file_log<<("\n");

		CString in_splits;
		in_splits = m_in_splits.get_FileName();
		ifstream file4 (in_splits);
		file_log<<("Profiles Split file (EOM) : ");
		file_log<<CStr_to_str(in_splits);
		file_log<<("\n");

		file_log<<"\n";
		file_log<<"OUTPUTS : ";
		file_log<<"\n";
		
		CString eom_prod;
		eom_prod = m_eom_prod.get_FileName();
		ifstream file5 (eom_prod);
		file_log<<("EOM prod inforce file (without new business policies) : ");
		file_log<<CStr_to_str(eom_prod);
		file_log<<("\n");
		
		CString eom_model;
		eom_model = m_eom_model.get_FileName();
		ifstream file6 (eom_model);
		file_log<<("EOM model inforce file (EOM) : ");
		file_log<<CStr_to_str(eom_model);
		file_log<<("\n");
		
		CString new_busi;
		new_busi = m_new_business.get_FileName();
		ifstream file7 (new_busi);
		file_log<<("New Business inforce file (EOM) : ");
		file_log<<CStr_to_str(new_busi);
		file_log<<("\n");


		if ((in_bom==_T(""))||(in_eom==_T(""))||(in_valo==_T(""))||(eom_prod==_T(""))||(eom_model==_T(""))||(new_busi==_T("")))
		{
			Wfilename dia1;
			dia1.DoModal();
			return;
		}

		if(!file1.is_open()||!file2.is_open()||!file3.is_open()||!file4.is_open()||!file5.is_open()||!file6.is_open()||!file7.is_open())
		{
			Wfileexist dia5;
			dia5.DoModal();
			return;
		}
		
		file_log<<("\n");
		file_log<<"ENVIRONEMENT : ";
		file_log<<"\n";

		CString country;
		int n = m_country.GetCurSel();
		if (n==-1)
		{
			Wcountry wng;
			wng.DoModal();
			return;
		}
		m_country.GetLBText(n,country);
		file_log<<"country : ";
		file_log<<CStr_to_str(country);
		file_log<<"\n";
		
		int * mo = new int [7];
		mo[0]=6;
		mo[1]=2;
		mo[2]=4;
		mo[3]=0;
		mo[4]=5;
		mo[5]=1;
		mo[6]=3;
		n = m_mode.GetCurSel();
		CString s_mode;
		m_mode.GetLBText(n,s_mode);
		int mo_n = mo[n];
		delete [] mo;

		UpdateData(true);

		CTime bom;
		CTime eom;
		m_date_bom.GetTime(bom);
		m_date_eom.GetTime(eom);
		int m_eom = eom.GetMonth();
		int d_eom = eom.GetDay();
		int y_eom = eom.GetYear();
		int m_bom = bom.GetMonth();
		int d_bom = bom.GetDay();
		int y_bom = bom.GetYear();
		file_log<<"date BOM : ";
		file_log<<d_bom;
		file_log<<"/";
		file_log<<m_bom;
		file_log<<"/";
		file_log<<y_bom;
		file_log<<"\n";
		file_log<<"date EOM : ";
		file_log<<d_eom;
		file_log<<"/";
		file_log<<m_eom;
		file_log<<"/";
		file_log<<y_eom;
		file_log<<"\n";

		string bom_date;
		string eom_date;
		bom_date += double_to_str(d_bom) + "/"  + double_to_str(m_bom)+"/" + double_to_str(y_bom);
		eom_date += double_to_str(d_eom) + "/" + double_to_str(m_eom)+"/" + double_to_str(y_eom);
		

		if (bom==eom)
		{
			Wdate dd;
			dd.DoModal();
			return;
		}

		file_log<<"OPTIONS : ";
		file_log<<"\n";
		file_log<<"mode : ";
		file_log<<CStr_to_str(s_mode);
		file_log<<"\n";
		file_log<<"RRC multiplier = ";
		file_log<<m_rrc;
		file_log<<"\n";
		file_log<<"lapse rate multiplier = ";
		file_log<<m_lapse;
		file_log<<"\n";
		file_log<<"death rate multiplier = ";
		file_log<<m_death;
		file_log<<"\n";
		file_log<<"\n";

		string listeR;
		string listeA;

		if (country==_T("SPAIN"))
		{
			listeR = "ESP_ADB,ESP_WDB";
			listeA = "ESP_ADB01,ESP_WDB01";
		}
		else
		{
			if (country==_T("ITALY"))
			{
				listeR = "ITA_ADB,ITA_WDB";
				listeA = "ITA_ADB01,ITA_WDB01";
			}
			else
			{
				if(country==_T("PORTUGAL"))
				{
					listeR = "PRT_ADB,PRT_WDB";
					listeA = "PRT_ADB01,PRT_WDB01";
				}
				else
				{
					if(country==_T("MPS"))
					{
						listeR = "MPF_ADB,MPF_WDB";
						listeA = "ITA_ADB01,ITA_WDB01";
					}
				}
			}
		}


		map<string,map<string,string>> pp;
		
		map<string,string> ap_conf;
		map<string,string> run_conf;

		ap_conf.insert(pair<string,string>("inforce_bom_prod",CStr_to_str(in_bom)));
		ap_conf.insert(pair<string,string>("inforce_eom_prod",CStr_to_str(in_eom)));
		ap_conf.insert(pair<string,string>("fund_split_eom",CStr_to_str(in_splits)));
		ap_conf.insert(pair<string,string>("fund_valo_eom",CStr_to_str(in_valo)));

		ap_conf.insert(pair<string,string>("BOM_date",bom_date));
		ap_conf.insert(pair<string,string>("EOM_date",eom_date));
		ap_conf.insert(pair<string,string>("lapse_multiplier",double_to_str(m_lapse)));
		ap_conf.insert(pair<string,string>("charges_multiplier",double_to_str(m_rrc)));
		ap_conf.insert(pair<string,string>("death_rate_multiplier",double_to_str(m_death)));
		ap_conf.insert(pair<string,string>("mode",double_to_str(mo_n)));
		
		ap_conf.insert(pair<string,string>("inforce_eom_prod_no_nb",CStr_to_str(eom_prod)));
		ap_conf.insert(pair<string,string>("inforce_eom_model",CStr_to_str(eom_model)));
		ap_conf.insert(pair<string,string>("inforce_prod_new_business",CStr_to_str(new_busi)));

		ap_conf.insert(pair<string,string>("temporary_directory",CStr_to_str(temp_dir)));
		ap_conf.insert(pair<string,string>("products",listeA));

		run_conf.insert(pair<string,string>("list_of_products_to_run",listeR));

		pp.insert(pair<string,map<string,string>>("AP_configuration",ap_conf));
		pp.insert(pair<string,map<string,string>>("runs_configuration",run_conf));

		//string config = "C:\\Actuarial Module\\Configuration\\projection_configuration_aging_policies.xml";
		//for setup
		string config = "C:\\Program Files\\Actuarial Module\\Configuration\\projection_configuration_aging_policies_for_setup.xml";
		XML_set(config,pp);

		string s("");
		
		run_aging_policies( config, file_log, 1,  1, s);

		end = clock();
		file_log<<("temps de run total : ");
		file_log<<(end-start);
		file_log<<(" ms");
		file_log<<("\n");
		file_log.close();
		
		Wfini dio;
		dio.DoModal();
		return;
	}
	catch(GMXBException *g)
	{
		WerrorDLL da;
		da.DoModal();
		return;
	}
	
}
BEGIN_EVENTSINK_MAP(CinterfaceDlg, CDialog)
END_EVENTSINK_MAP()



void CinterfaceDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString directory;
	CString str;
	CString str1;
	CString country;
	CString mode;
	int n;
	WIN32_FIND_DATA lpFindFileData;
	WIN32_FIND_DATA lpFindFileData0;

	CTime bom;
	CTime eom;
	m_date_bom.GetTime(bom);
	m_date_eom.GetTime(eom);
	int m_eom = eom.GetMonth();
	int d_eom = eom.GetDay();
	int y_eom = eom.GetYear();
	int m_bom = bom.GetMonth();
	int d_bom = bom.GetDay();
	int y_bom = bom.GetYear();

	UpdateData(true);
	
	m_dir_out_bis.GetWindowTextW(directory);
	
	if (directory==_T(""))
	{
		Wfilename dio;
		dio.DoModal();
		return;
	}

	str = directory;
	str+=(CString)"\\Outputs";
	HANDLE hFind0 = FindFirstFile(str,&lpFindFileData0);
	if (hFind0=INVALID_HANDLE_VALUE)
	{
		CreateDirectoryW(str,0);
	}
	
    std::ostringstream oss;
	n = m_country.GetCurSel();
	if (n==-1)
	{
		Wcountry wng;
		wng.DoModal();
		return;
	}
	m_country.GetLBText(n,country);
	str+=(CString)"\\EOM_";
	str+=country;
	str+=(CString)"_";
	if (d_eom<10)
	{
		oss<<"0";
	}
	oss<<d_eom;
	oss<<".";
	if (m_eom<10)
	{
		oss<<"0";
	}
	oss<<m_eom;
	oss<<".";
	if (y_eom<10)
	{
		oss<<"0";
	}
	oss<<y_eom;
	str+=oss.str().c_str();
	n = m_mode.GetCurSel();
	m_mode.GetLBText(n,mode);
	str+="(";
	str+=mode;
	if (m_rrc != 1)
	{
		oss.str("");
		oss<<m_rrc;
		str+=",RRC=";
		str+=oss.str().c_str();
	}
	if (m_lapse != 1)
	{
		oss.str("");
		oss<<m_lapse;
		str+=",LAPSE=";
		str+=oss.str().c_str();
	}
	if (m_death != 1)
	{
		oss.str("");
		oss<<m_death;
		str+=",DEATH=";
		str+=oss.str().c_str();
	}
	str+=")";


	HANDLE hFind = FindFirstFile(str,&lpFindFileData);
	if (hFind!=INVALID_HANDLE_VALUE)
	{
		Wgenerate diu;
		diu.DoModal();
		return;
	}
	CreateDirectoryW(str,0);
	directory = str;
	CString str00 = directory + (CString)"\\log.txt"; 
	std::ofstream outfile00 (str00,ios::out);
	outfile00.close();
	m_log.put_FileName(str00);
	
	str1 = str;
	str1+="\\EOMprodinforce.csv";
	std::ofstream outfile4 (str1,ios::out);
	outfile4.close();
	m_eom_prod.put_FileName(str1);
	
	str1 = str;
	str1+="\\EOMmodelinforce.csv";
	std::ofstream outfile5 (str1,ios::out);
	outfile5.close();
	m_eom_model.put_FileName(str1);
	
	str1 = str;
	str1+="\\NewBusiness.csv";
	std::ofstream outfile6 (str1,ios::out);
	outfile6.close();
	m_new_business.put_FileName(str1);

}

