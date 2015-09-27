#pragma once

#include "stdafx.h"
#include "fund_for_aging_policies.h"
#include "MP_AP_BOM.h"
#include "MP_AP_BOM_no_LD.h"
#include "MP_AP_EOM.h"
#include "MP_AP_EOM_no_NB.h"
#include "GMXBException.h"
#include <map>
#include <vector>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>
#include <atlstr.h>


using namespace std;


class inforces_AP
{
private : 
	map<string, MP_AP_BOM_no_LD> inforce_BOM;
	map<string, MP_AP_EOM_no_NB> inforce_EOM;


public:	
	inforces_AP(void)
	{
	}

	inforces_AP(const inforces_AP& inforces)
	{
		this->inforce_BOM = inforces.inforce_BOM;
		this->inforce_EOM = inforces.inforce_EOM;
	}

	~inforces_AP(void)
	{
	}

	inforces_AP& operator= (const inforces_AP& inf)
	{
		this->inforce_BOM = inf.inforce_BOM;
		this->inforce_EOM = inf.inforce_EOM;
		return *this;
	}

	void import_BOM (const CString & adress)
	{
		/*char * str = (char *)adress.c_str();*/
		ifstream file (adress);
		string line;
		int inc;
		string nm;
		istringstream line_stream (istringstream::in);
		MP_AP_BOM_no_LD mp;
		while (!file.eof())
		{
			getline(file,line);
			if(line=="")
			{
			}
			else
			{
				line_stream.clear();
				line_stream.str(line.c_str());
				inc = 0;
				while(inc<7)
				{
					getline(line_stream,nm,',');
					inc++;
				}
				//error handle : policy ID is empty in BOM inforce file
				if (nm=="")
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the BOM inforce input file");
					throw new GMXBException(message);
				}
				try
				{
					mp = MP_AP_BOM_no_LD(line);
				}
				catch (GMXBException* e)
				{
					if (e->get_message()!="")
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, e->get_message());
						throw new GMXBException(message);
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "error from the BOM inforce input file");
						throw new GMXBException(message);
					}
				}
				inforce_BOM.insert(pair<string, MP_AP_BOM_no_LD> (nm, mp));
			}
		}
		file.close();
		cout<<"inforce BOM (" << adress << ") imported"<<endl;
	}

	void import_BOM (const string & adress)
	{
		/*char * str = (char *)adress.c_str();*/
		ifstream file (adress.c_str());
		string line;
		int inc;
		string nm;
		istringstream line_stream (istringstream::in);
		MP_AP_BOM_no_LD mp;
		while (!file.eof())
		{
			getline(file,line);
			if(line=="")
			{
			}
			else
			{
				line_stream.clear();
				line_stream.str(line.c_str());
				inc = 0;
				while(inc<7)
				{
					getline(line_stream,nm,',');
					inc++;
				}
				//error handle : policy ID is empty in BOM inforce file
				if (nm=="")
				{

					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the BOM inforce input file");
					throw new GMXBException(message);
				}
				try
				{
					mp = MP_AP_BOM_no_LD(line);
				}
				catch (GMXBException* e)
				{
					if (e->get_message()!="")
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, e->get_message());
						throw new GMXBException(message);
						
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "error from the BOM inforce input file");
						throw new GMXBException(message);
					}
				}
				inforce_BOM.insert(pair<string, MP_AP_BOM_no_LD> (nm, mp));
			}
		}
		file.close();
		cout<<"inforce BOM (" << adress << ") imported"<<endl;
	}

	void import_EOM (const CString& adress)
	{
		/*char * str = (char *)adress.c_str();*/
		ifstream file (adress);
		string line;
		int inc;
		string nm;
		istringstream line_stream (istringstream::in);
		MP_AP_EOM_no_NB mp;
		map<string,MP_AP_BOM_no_LD>::iterator it;

		while(!file.eof())
		{
			getline(file,line);
			if(line=="")
			{
			}
			else
			{
				line_stream.clear();
				line_stream.str(line.c_str());
				inc = 0;
				while(inc<7)
				{
					getline(line_stream,nm,',');
					inc++;
				}
				//error handle : policy ID is empty
				if (nm=="")
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the EOM inforce input");
					throw new GMXBException(message);
				}
				try
				{
					mp = MP_AP_EOM_no_NB(line);
					it = inforce_BOM.find(nm);
					if(it != inforce_BOM.end())
					{	
						mp.set_new_business(false);
						mp.set_fund_description(line);
					}
					else
					{
						mp.set_new_business(true);
					}
				}
				catch (GMXBException* e)
				{
					if (e->get_message()!="")
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, e->get_message());
						throw new GMXBException(message);
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "error from the EOM inforce input file");
						throw new GMXBException(message);
					}
				}

				inforce_EOM.insert (pair<string,MP_AP_EOM_no_NB> (nm,mp));
			}
			
		}
		
		file.close();
		cout<<"inforce EOM (" << adress << ") imported"<<endl;
	}

	void import_EOM (const string& adress)
	{
		/*char * str = (char *)adress.c_str();*/
		ifstream file (adress.c_str());
		string line;
		int inc;
		string nm;
		istringstream line_stream (istringstream::in);
		MP_AP_EOM_no_NB mp;
		map<string,MP_AP_BOM_no_LD>::iterator it;

		while(!file.eof())
		{
			getline(file,line);
			if(line=="")
			{
			}
			else
			{
				line_stream.clear();
				line_stream.str(line.c_str());
				inc = 0;
				while(inc<7)
				{
					getline(line_stream,nm,',');
					inc++;
				}
				//error handle : policy ID is empty
				if (nm=="")
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the EOM inforce input");
					throw new GMXBException(message);
				}
				try
				{
					mp = MP_AP_EOM_no_NB(line);
					it = inforce_BOM.find(nm);
					if(it != inforce_BOM.end())
					{	
						mp.set_new_business(false);
						mp.set_fund_description(line);
					}
					else
					{
						mp.set_new_business(true);
					}
				}
				catch (GMXBException* e)
				{
					if (e->get_message()!="")
					{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, e->get_message());
					throw new GMXBException(message);
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "error from the EOM inforce input file");
						throw new GMXBException(message);
					}
				}

				inforce_EOM.insert (pair<string,MP_AP_EOM_no_NB> (nm,mp));
			}
			
		}
		
		file.close();
		cout<<"inforce EOM (" << adress << ") imported"<<endl;
	}

	void set_BOM (const map<string,double> & valo_file,const map<string, vector<double>> & split_file)
	{
		map<string,MP_AP_BOM_no_LD>::iterator it;
		map<string,MP_AP_EOM_no_NB>::iterator res;
		MP_AP_BOM_no_LD mp;
		string name;
		for(it = inforce_BOM.begin(); it !=inforce_BOM.end(); it++)
		{
			res = inforce_EOM.find(it->first);
			if(res != inforce_EOM.end())
			{
				it->second.set_lapsed_or_dead(false);
				it->second.set_fund_description_EOM (res->second);
				it->second.switch_detection();
				if (it->second.get_switch()==false)
				{
					it->second.share_changed_detection();
				};
			}
			else
			{
				it->second.set_lapsed_or_dead (true);
			}
			it->second.set_params(valo_file, split_file);
		}
		cout<<"inforce BOM set"<<endl;
	}

	void import_valo_file (const CString& adress, map<string,double> & valo_file)
	{
		map<string,double> temp_map;
		/*char * str = (char *)adress.c_str();*/
		ifstream file (adress);
		//if(!file.is_open())
		//{
		//	char message [MAX_LENGTH_MESSAGE_EXCEPTION];
		//	sprintf(message,"file %d not found",adress);
		//	throw new GMXBException (message);
		//}

		string line;
		istringstream line_stream (istringstream::in); 
		string name;
		string chain;
		double valo;
		while (!file.eof())
		{
			getline(file,line);
			if (line == "") 
			{				/* we don't raise any exception when a line is empty, but we don't care of it*/		
			}
			else
			{
				line_stream.clear();
				//error handle : ...
				try
				{
					line_stream.str(line);
					getline(line_stream,name,',');
					//error handle : ...
					if (name == "")
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "error from the valorisation file");
						throw new GMXBException(message);
					}
					getline(line_stream,chain,',');
					getline(line_stream,chain,',');
					valo = atof(chain.c_str());
					valo_file[name]=valo;
				}
				catch(const exception& e)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the valorisation file %s", e.what());
					throw new GMXBException(message);
				}
			}
		}
		file.close();
		cout<<"EOM profiles valorisation file ("<<adress<<") imported"<<endl;
	}

	void import_valo_file (const string& adress, map<string,double> & valo_file)
	{
		map<string,double> temp_map;
		/*char * str = (char *)adress.c_str();*/
		ifstream file (adress.c_str());
		//if(!file.is_open())
		//{
		//	char message [MAX_LENGTH_MESSAGE_EXCEPTION];
		//	sprintf(message,"file %d not found",adress);
		//	throw new GMXBException (message);
		//}

		string line;
		istringstream line_stream (istringstream::in); 
		string name;
		string chain;
		double valo;
		while (!file.eof())
		{
			getline(file,line);
			if (line == "") 
			{				/* we don't raise any exception when a line is empty, but we don't care of it*/		
			}
			else
			{
				line_stream.clear();
				//error handle : ...
				try
				{
					line_stream.str(line);
					getline(line_stream,name,',');
					//error handle : ...
					if (name == "")
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "error from the valorisation file");
						throw new GMXBException(message);
					}
					getline(line_stream,chain,',');
					getline(line_stream,chain,',');
					valo = atof(chain.c_str());
					valo_file[name]=valo;
				}
				catch(const exception& e)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the valorisation file %s", e.what());
					throw new GMXBException(message);
				}
			}
		}
		file.close();
		cout<<"EOM profiles valorisation file ("<<adress<<") imported"<<endl;
	}
	void import_split_file (const CString& adress,map<string, vector<double>> & split_file)
	{
		/*char* str = (char*)adress.c_str();*/
		ifstream file (adress);
		//if(!file.is_open())
		//{
		//	char message [MAX_LENGTH_MESSAGE_EXCEPTION];
		//	sprintf(message,"file %d not found",adress);
		//	throw new GMXBException (message);
		//}
		string line;
		istringstream stream_line (istringstream::in); 
		string name;
		string name_bis;
		string chain;
		int i;
		vector<double> vect;

		while (!file.eof())
		{
			getline(file,line);
			if(line=="")
			{
			}
			else
			{
				stream_line.clear();
				try
				{
					stream_line.str(line);
					getline(stream_line,name,',');
					//error handle : ...
					if (name == "")
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "error from the split file");
						throw new GMXBException(message);
					}
					getline(stream_line,chain,',');
					//error handle : ...
					if ((chain.compare("Y")!=0)&&(chain.compare("N")!=0))
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "error from the split file");
						throw new GMXBException(message);
					}
					name+=chain;

					for (i=0;i<12;i++)
					{
						getline(stream_line,chain,',');
						vect.push_back(atof(chain.c_str()));
					}

					split_file.insert( pair<string,vector<double>> (name,vect));
					vect.clear();
				}
				catch (const exception& e)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the split file %s", e.what());
					throw new GMXBException(message);
				}
			}
			
		}
		file.close();
		cout<<"EOM profiles split file (" << adress <<") imported"<<endl;

	}

	void import_split_file (const string& adress,map<string, vector<double>> & split_file)
	{
		/*char* str = (char*)adress.c_str();*/
		ifstream file (adress.c_str());
		//if(!file.is_open())
		//{
		//	char message [MAX_LENGTH_MESSAGE_EXCEPTION];
		//	sprintf(message,"file %d not found",adress);
		//	throw new GMXBException (message);
		//}
		string line;
		istringstream stream_line (istringstream::in); 
		string name;
		string name_bis;
		string chain;
		int i;
		vector<double> vect;

		while (!file.eof())
		{
			getline(file,line);
			if(line=="")
			{
			}
			else
			{
				stream_line.clear();
				try
				{
					stream_line.str(line);
					getline(stream_line,name,',');
					//error handle : ...
					if (name == "")
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "error from the split file");
						throw new GMXBException(message);
					}
					getline(stream_line,chain,',');
					//error handle : ...
					if ((chain.compare("Y")!=0)&&(chain.compare("N")!=0))
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "error from the split file");
						throw new GMXBException(message);
					}
					name+=chain;

					for (i=0;i<12;i++)
					{
						getline(stream_line,chain,',');
						vect.push_back(atof(chain.c_str()));
					}

					split_file.insert( pair<string,vector<double>> (name,vect));
					vect.clear();
				}
				catch (const exception& e)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the split file %s", e.what());
					throw new GMXBException(message);
				}
			}
			
		}
		file.close();
		cout<<"EOM profiles split file (" << adress <<") imported"<<endl;

	}

	void get_perfs_file (const CString& adress, int mode)
	{
		ofstream perfs_stream (adress,ios::out);
		//if(!perfs_stream.is_open())
		//{
		//	char message [MAX_LENGTH_MESSAGE_EXCEPTION];
		//	sprintf(message,"file %d not found",adress);
		//	throw new GMXBException (message);
		//}

		map<string,MP_AP_BOM_no_LD>::iterator ite;
		string text;
		switch(mode)
		{
		
		case 0 : /*no specs*/ 
			{
				text = "regular";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					/*perfs_stream<<(ite->first);
					perfs_stream<<(",");*/
					perfs_stream<<(ite->second.get_performance());
					perfs_stream<<("\n");
				}
				break;
			}
		case 1 : /*without lapsed or dead*/
			{
				text = "without lapsed or dead policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_lapsed_or_dead())
					{
						perfs_stream<<(ite->second.get_performance());
						perfs_stream<<("\n");
					}
				}
				break;
			}
		case 2 : /*only lapsed or dead*/
			{
				text = "only lapsed or dead policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_lapsed_or_dead())
					{
						/*perfs_stream<<(ite->first);
						perfs_stream<<(",");*/
						perfs_stream<<(ite->second.get_performance());
						perfs_stream<<("\n");
					}
				}
				break;
			}
		case 3 : /*without switched*/
			{
				text = "without switched policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_switch())
					{
						perfs_stream<<(ite->second.get_performance());
						perfs_stream<<("\n");
					}
				}
				break;
			}
		case 4 : /*only switched*/
			{
				text = "only switched policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_switch())
					{
						perfs_stream<<(ite->second.get_performance());
						perfs_stream<<("\n");
					}
				}
				break;
			}
		case 5 : /*without changed of shares*/
			{
				text = "without changed of shares policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_change_of_shares())
					{
						perfs_stream<<(ite->second.get_performance());
						perfs_stream<<("\n");
					}
				}
				break;
			}
		case 6 : /*only changed of shares*/
			{
				text = "only changed of shares policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_change_of_shares())
					{
						perfs_stream<<(ite->second.get_performance());
						perfs_stream<<("\n");
					}
				}
				break;
			}
		default : /*wrong mode : raise exception*/
			{
				char message [MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message,"mode %d : unexpected mode ",mode);
				throw new GMXBException (message);
			}
		}

		perfs_stream.close();
		cout<<"policy performance file ("<<text<<") exported"<<endl;
	}

	void get_tw_file (const CString& adress, int mode)
	{
		ofstream tw_stream (adress,ios::out);
		if(!tw_stream.is_open())
		{
			char message [MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf(message,"file %d not found",adress);
			throw new GMXBException (message);
		}

		string text;
		map<string,MP_AP_BOM_no_LD>::iterator ite;
		int i;
		switch(mode)
		{
		
		case 0 : /*no specs*/ 
			{
				text = "regular";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					for(i=0;i<12;i++)
					{
						tw_stream<<(ite->second.get_target_weights()[i]);
						tw_stream<<(",");
					}
					/*tw_stream<<(ite->first);
					tw_stream<<(",");*/
					tw_stream<<("\n");
				}
				break;
			}
		case 1 : /*without lapsed or dead*/
			{
				text = "without lapsed or dead policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_lapsed_or_dead())
					{
						for(i=0;i<12;i++)
						{
							tw_stream<<(ite->second.get_target_weights()[i]);
							tw_stream<<(",");
						}
						tw_stream<<("\n");
					}
				}
				break;
			}
		case 2 : /*only lapsed or dead*/
			{
				text = "only lapsed or dead policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_lapsed_or_dead())
					{
						for(i=0;i<12;i++)
						{
							tw_stream<<(ite->second.get_target_weights()[i]);
							tw_stream<<(",");
						}
						/*tw_stream<<(ite->first);
						tw_stream<<(",");*/
						tw_stream<<("\n");
					}
				}
				break;
			}
		case 3 : /*without switched*/
			{
				text = "without switched policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_switch())
					{
						for(i=0;i<12;i++)
						{
							tw_stream<<(ite->second.get_target_weights()[i]);
							tw_stream<<(",");
						}
						tw_stream<<("\n");
					}
				}
				break;
			}
		case 4 : /*only switched*/
			{
				text = "only switched policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_switch())
					{
						for(i=0;i<12;i++)
						{
							tw_stream<<(ite->second.get_target_weights()[i]);
							tw_stream<<(",");
						}
						tw_stream<<("\n");
					}
				}
				break;
			}
		case 5 : /*without changed of shares*/
			{
				text = "without changed of shares policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_change_of_shares())
					{
						for(i=0;i<12;i++)
						{
							tw_stream<<(ite->second.get_target_weights()[i]);
							tw_stream<<(",");
						}
						tw_stream<<("\n");
					}
				}
				break;
			}
		case 6 : /*only changed of shares*/
			{
				text = "only changed of shares policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_change_of_shares())
					{
						for(i=0;i<12;i++)
						{
							tw_stream<<(ite->second.get_target_weights()[i]);
							tw_stream<<(",");
						}
						tw_stream<<("\n");
					}
				}
				break;
			}
		default : /*wrong mode : raise exception*/
			{
				char message [MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message,"mode %d : unexpected mode ",mode);
				throw new GMXBException (message);
			}
		}

		tw_stream.close();
		cout<<"policy target weights file ("<<text<<") exported"<<endl;
	}


	void get_cw_file (const CString& adress, int mode)
	{
		ofstream cw_stream (adress,ios::out);
		//if(!cw_stream.is_open())
		//{
		//	char message [MAX_LENGTH_MESSAGE_EXCEPTION];
		//	sprintf(message,"file %d not found",adress);
		//	throw new GMXBException (message);
		//}

		string text;
		map<string,MP_AP_BOM_no_LD>::iterator ite;
		int i;
		switch(mode)
		{
		
		case 0 : /*no specs*/ 
			{
				text = "regular";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					for(i=0;i<12;i++)
					{
						cw_stream<<(ite->second.get_current_weights()[i]);
						cw_stream<<(",");
					}
					cw_stream<<(ite->first);
					cw_stream<<(",");
					cw_stream<<("\n");
				}
				break;
			}
		case 1 : /*without lapsed or dead*/
			{
				text = "without lapsed or dead policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_lapsed_or_dead())
					{
						for(i=0;i<12;i++)
						{
							cw_stream<<(ite->second.get_current_weights()[i]);
							cw_stream<<(",");
						}
						cw_stream<<("\n");
					}
				}
				break;
			}
		case 2 : /*only lapsed or dead*/
			{
				text = "only lapsed or dead policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_lapsed_or_dead())
					{
						for(i=0;i<12;i++)
						{
							cw_stream<<(ite->second.get_current_weights()[i]);
							cw_stream<<(",");
						}
						cw_stream<<(ite->first);
						cw_stream<<(",");
						cw_stream<<("\n");
					}
				}
				break;
			}
		case 3 : /*without switched*/
			{
				text = "without switched policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_switch())
					{
						for(i=0;i<12;i++)
						{
							cw_stream<<(ite->second.get_current_weights()[i]);
							cw_stream<<(",");
						}
						cw_stream<<("\n");
					}
				}
				break;
			}
		case 4 : /*only switched*/
			{
				text = "only switched policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_switch())
					{
						for(i=0;i<12;i++)
						{
							cw_stream<<(ite->second.get_current_weights()[i]);
							cw_stream<<(",");
						}
						cw_stream<<("\n");
					}
				}
				break;
			}
		case 5 : /*without changed of shares*/
			{
				text = "without changed of shares policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_change_of_shares())
					{
						for(i=0;i<12;i++)
						{
							cw_stream<<(ite->second.get_current_weights()[i]);
							cw_stream<<(",");
						}
						cw_stream<<("\n");
					}
				}
				break;
			}
		case 6 : /*only changed of shares*/
			{
				text = "only changed of shares policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_change_of_shares())
					{
						for(i=0;i<12;i++)
						{
							cw_stream<<(ite->second.get_current_weights()[i]);
							cw_stream<<(",");
						}
						cw_stream<<("\n");
					}
				}
				break;
			}
		default : /*wrong mode : raise exception*/
			{
				char message [MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message,"mode %d : unexpected mode ",mode);
				throw new GMXBException (message);
			}
		}

		cw_stream.close();
		cout<<"policy current weights file ("<<text<<") exported"<<endl;
	}

	void get_BOM_file (const CString& adress, int mode)
	{
		ofstream BOM_stream (adress,ios::out);
		//if(!BOM_stream.is_open())
		//{
		//	char message [MAX_LENGTH_MESSAGE_EXCEPTION];
		//	sprintf(message,"file %d not found",adress);
		//	throw new GMXBException (message);
		//}

		string text;
		map<string,MP_AP_BOM_no_LD>::iterator ite;
		switch(mode)
		{
		
		case 0 : /*no specs*/ 
			{
				text = "regular";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					BOM_stream<<(BOM_stream,ite->second.get_row());
					BOM_stream<<("\n");
				}
				break;
			}
		case 1 : /*without lapsed or dead*/
			{
				text = "without lapsed or dead policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_lapsed_or_dead())
					{
						BOM_stream<<(BOM_stream,ite->second.get_row());
						BOM_stream<<("\n");
					}
				}
				break;
			}
		case 2 : /*only lapsed or dead*/
			{
				text = "only lapsed or dead policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_lapsed_or_dead())
					{
						BOM_stream<<(BOM_stream,ite->second.get_row());
						BOM_stream<<("\n");
					}
				}
				break;
			}
		case 3 : /*without switched*/
			{
				text = "without switched policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_switch())
					{
						BOM_stream<<(BOM_stream,ite->second.get_row());
						BOM_stream<<("\n");
					}
				}
				break;
			}
		case 4 : /*only switched*/
			{
				text = "only switched policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_switch())
					{
						BOM_stream<<(BOM_stream,ite->second.get_row());
						BOM_stream<<("\n");
					}
				}
				break;
			}
		case 5 : /*without changed of shares*/
			{
				text = "without changed of shares policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_change_of_shares())
					{
						BOM_stream<<(BOM_stream,ite->second.get_row());
						BOM_stream<<("\n");
					}
				}
				break;
			}
		case 6 : /*only changed of shares*/
			{
				text = "only changed of shares policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_change_of_shares())
					{
						BOM_stream<<(BOM_stream,ite->second.get_row());
						BOM_stream<<("\n");
					}
				}
				break;
			}
		default : /*wrong mode : raise exception*/
			{
				char message [MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message,"mode %d : unexpected mode ",mode);
				throw new GMXBException (message);
			}
		}

		BOM_stream.close();
		cout<<"BOM inforce file ("<<text<<") exported"<<endl;
	}

	void get_EOM_prod_no_NB_file (const CString& adress, int mode)
	{
		ofstream EOM_stream (adress,ios::out);
		if(!EOM_stream.is_open())
		{
			char message [MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf(message,"file %d not found",adress);
			throw new GMXBException (message);
		}

		string text;
		map<string,MP_AP_EOM_no_NB>::iterator ite;
		map<string,MP_AP_BOM_no_LD>::iterator ito;
		switch(mode)
		{
		case 0 : /*regular prod no NB*/
			{
				text = "regular";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{
					if(!ite->second.get_new_business())
					{
						EOM_stream<<(ite->second.get_row());
						EOM_stream<<("\n");
					}
				}
				break;
			}
		case 1 : /*without lapsed or dead*/
			{
				text = "without lapsed or dead policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					ito = inforce_BOM.find(ite->first);
					if((!ite->second.get_new_business())&&(!ito->second.get_lapsed_or_dead()))
					{
						EOM_stream<<(ite->second.get_row());
						EOM_stream<<("\n");
					}
				}
				break;
			}
		case 2 : /*only lapsed or dead*/
			{
				text = "only lapsed or dead policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					ito = inforce_BOM.find(ite->first);
					if((!ite->second.get_new_business())&&(ito->second.get_lapsed_or_dead()))
					{
						EOM_stream<<(ite->second.get_row());
						EOM_stream<<("\n");
					}
				}
				break;
			}
		case 3 : /*without switched*/
			{
				text = "without switched policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					ito = inforce_BOM.find(ite->first);
					if((!ite->second.get_new_business())&&(!ito->second.get_switch()))
					{
						EOM_stream<<(ite->second.get_row());
						EOM_stream<<("\n");
					}
				}
				break;
			}
		case 4 : /*only switched*/
			{
				text = "only switched policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					ito = inforce_BOM.find(ite->first);
					if((!ite->second.get_new_business())&&(ito->second.get_switch()))
					{
						EOM_stream<<(ite->second.get_row());
						EOM_stream<<("\n");
					}
				}
				break;
			}
		case 5 : /*without changed of shares*/
			{
				text = "without changed of shares policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					ito = inforce_BOM.find(ite->first);
					if((!ite->second.get_new_business())&&(!ito->second.get_change_of_shares()))
					{
						EOM_stream<<(ite->second.get_row());
						EOM_stream<<("\n");
					}
				}
				break;
			}
		case 6 : /*only changed of shares*/
			{
				text = "only changed of shares policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					ito = inforce_BOM.find(ite->first);
					if((!ite->second.get_new_business())&&(ito->second.get_change_of_shares()))
					{
						EOM_stream<<(ite->second.get_row());
						EOM_stream<<("\n");
					}
				}
				break;
			}
		default : /*wrong mode : raise exception*/
			{
				char message [MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message,"mode %d : unexpected mode ",mode);
				throw new GMXBException (message);
			}
		}
		EOM_stream.close();
		cout<<"EOM no NB inforce file ("<<text<<") exported"<<endl;
	}


void get_new_business_file (const CString& adress)
	{
		ofstream nb_stream (adress, ios::out);
		//if(!nb_stream.is_open())
		//{
		//	char message [MAX_LENGTH_MESSAGE_EXCEPTION];
		//	sprintf(message,"file %d not found",adress);
		//	throw new GMXBException (message);
		//}

		map<string,MP_AP_EOM_no_NB>::iterator ite;

		for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
		{
			if(ite->second.get_new_business())
			{
				nb_stream<<(ite->second.get_row());
				nb_stream<<("\n");
			}
		}

		nb_stream.close();
		cout<<"New Buisiness inforce file exported"<<endl;
	}

void get_eom_pack_via_prod_key (const string& prod_key, const CString& eom_file, const CString& nb_file, int mode)
	{
		ofstream EOM_stream (eom_file, ios::out);
		ofstream nb_stream (nb_file, ios::out);

		string text;

		map<string,MP_AP_EOM_no_NB>::iterator ite;
		map<string,MP_AP_BOM_no_LD>::iterator ito;

		switch(mode)
		{
		case 0 : /*regular prod no NB*/
			{
				text = "regular";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{
					if (ite->second.get_prod_key()==prod_key)
					{
						if(!ite->second.get_new_business())
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		case 1 : /*without lapsed or dead*/
			{
				text = "without lapsed or dead policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					if (ite->second.get_prod_key()==prod_key)
					{
						ito = inforce_BOM.find(ite->first);
						if((!ite->second.get_new_business())&&(!ito->second.get_lapsed_or_dead()))
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		case 2 : /*only lapsed or dead*/
			{
				text = "only lapsed or dead policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					if (ite->second.get_prod_key()==prod_key)
					{
						ito = inforce_BOM.find(ite->first);
						if((!ite->second.get_new_business())&&(ito->second.get_lapsed_or_dead()))
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		case 3 : /*without switched*/
			{
				text = "without switched policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					if (ite->second.get_prod_key()==prod_key)
					{
						ito = inforce_BOM.find(ite->first);
						if((!ite->second.get_new_business())&&(!ito->second.get_switch()))
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		case 4 : /*only switched*/
			{
				text = "only switched policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					if (ite->second.get_prod_key()==prod_key)
					{
						ito = inforce_BOM.find(ite->first);
						if((!ite->second.get_new_business())&&(ito->second.get_switch()))
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		case 5 : /*without changed of shares*/
			{
				text = "without changed of shares policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					if (ite->second.get_prod_key()==prod_key)
					{
						ito = inforce_BOM.find(ite->first);
						if((!ite->second.get_new_business())&&(!ito->second.get_change_of_shares()))
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		case 6 : /*only changed of shares*/
			{
				text = "only changed of shares policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					if (ite->second.get_prod_key()==prod_key)
					{
						ito = inforce_BOM.find(ite->first);
						if((!ite->second.get_new_business())&&(ito->second.get_change_of_shares()))
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		default : /*wrong mode : raise exception*/
			{
				char message [MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message,"mode %d : unexpected mode ",mode);
				throw new GMXBException (message);
			}
		}
		
		EOM_stream.close();
		nb_stream.close();
		cout<<"EOM pack ("<<text<<") exported"<<endl;
	}

	void get_eom_pack_via_prod_key (const string& prod_key, const string& eom_file, const string& nb_file, int mode)
	{
		ofstream EOM_stream (eom_file.c_str(), ios::out);
		ofstream nb_stream (nb_file.c_str(), ios::out);

		string text;

		map<string,MP_AP_EOM_no_NB>::iterator ite;
		map<string,MP_AP_BOM_no_LD>::iterator ito;

		switch(mode)
		{
		case 0 : /*regular prod no NB*/
			{
				text = "regular";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{
					if (ite->second.get_prod_key()==prod_key)
					{
						if(!ite->second.get_new_business())
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		case 1 : /*without lapsed or dead*/
			{
				text = "without lapsed or dead policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					if (ite->second.get_prod_key()==prod_key)
					{
						ito = inforce_BOM.find(ite->first);
						if((!ite->second.get_new_business())&&(!ito->second.get_lapsed_or_dead()))
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		case 2 : /*only lapsed or dead*/
			{
				text = "only lapsed or dead policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					if (ite->second.get_prod_key()==prod_key)
					{
						ito = inforce_BOM.find(ite->first);
						if((!ite->second.get_new_business())&&(ito->second.get_lapsed_or_dead()))
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		case 3 : /*without switched*/
			{
				text = "without switched policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					if (ite->second.get_prod_key()==prod_key)
					{
						ito = inforce_BOM.find(ite->first);
						if((!ite->second.get_new_business())&&(!ito->second.get_switch()))
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		case 4 : /*only switched*/
			{
				text = "only switched policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					if (ite->second.get_prod_key()==prod_key)
					{
						ito = inforce_BOM.find(ite->first);
						if((!ite->second.get_new_business())&&(ito->second.get_switch()))
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		case 5 : /*without changed of shares*/
			{
				text = "without changed of shares policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					if (ite->second.get_prod_key()==prod_key)
					{
						ito = inforce_BOM.find(ite->first);
						if((!ite->second.get_new_business())&&(!ito->second.get_change_of_shares()))
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		case 6 : /*only changed of shares*/
			{
				text = "only changed of shares policies";
				for(ite=inforce_EOM.begin();ite!=inforce_EOM.end();ite++)
				{	
					if (ite->second.get_prod_key()==prod_key)
					{
						ito = inforce_BOM.find(ite->first);
						if((!ite->second.get_new_business())&&(ito->second.get_change_of_shares()))
						{
							EOM_stream<<(ite->second.get_row());
							EOM_stream<<("\n");
						}
						if(ite->second.get_new_business())
						{
							nb_stream<<(ite->second.get_row());
							nb_stream<<("\n");
						}
					}
				}
				break;
			}
		default : /*wrong mode : raise exception*/
			{
				char message [MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message,"mode %d : unexpected mode ",mode);
				throw new GMXBException (message);
			}
		}
		
		EOM_stream.close();
		nb_stream.close();
		cout<<"EOM pack ("<<text<<") exported"<<endl;
	}

	void get_dll_inputs (const string& bom_file_ab,const string& bom_file_wb,
		const string& perfs_file_ab,const string& perfs_file_wb,
		const string& tw_file_ab,const string& tw_file_wb, 
		const string& cw_file_ab,const string& cw_file_wb,
		int mode)
	{
		ofstream bom_stream_ab (bom_file_ab.c_str(), ios::out);
		ofstream perfs_stream_ab (perfs_file_ab.c_str(), ios::out);
		ofstream tw_stream_ab (tw_file_ab.c_str(), ios::out);
		ofstream cw_stream_ab (cw_file_ab.c_str(), ios::out);
		ofstream bom_stream_wb (bom_file_wb.c_str(), ios::out);
		ofstream perfs_stream_wb (perfs_file_wb.c_str(), ios::out);
		ofstream tw_stream_wb (tw_file_wb.c_str(), ios::out);
		ofstream cw_stream_wb (cw_file_wb.c_str(), ios::out);

		string text;
		string str;
		istringstream line_stream(istringstream::in);
		int i;

		map<string,MP_AP_BOM_no_LD>::iterator ite;
		switch(mode)
		{
		
		case 0 : /*no specs*/ 
			{
				text = "regular";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					str = ite->second.get_prod_key();
					line_stream.clear();
					line_stream.str(str.c_str());
					getline(line_stream,str,'_');
					getline(line_stream,str,'_');
					if (str==(string)"ADB01")
					{
						bom_stream_ab<<(bom_stream_ab,ite->second.get_row());
						bom_stream_ab<<("\n");

						perfs_stream_ab<<(ite->second.get_performance());
						perfs_stream_ab<<("\n");

						for(i=0;i<12;i++)
						{
							tw_stream_ab<<(ite->second.get_target_weights()[i]);
							tw_stream_ab<<(",");
							cw_stream_ab<<(ite->second.get_current_weights()[i]);
							cw_stream_ab<<(",");
						}
						tw_stream_ab<<("\n");
						cw_stream_ab<<("\n");
					}
					else
					{
						bom_stream_wb<<(bom_stream_wb,ite->second.get_row());
						bom_stream_wb<<("\n");

						perfs_stream_wb<<(ite->second.get_performance());
						perfs_stream_wb<<("\n");

						for(i=0;i<12;i++)
						{
							tw_stream_wb<<(ite->second.get_target_weights()[i]);
							tw_stream_wb<<(",");
							cw_stream_wb<<(ite->second.get_current_weights()[i]);
							cw_stream_wb<<(",");
						}
						tw_stream_wb<<("\n");
						cw_stream_wb<<("\n");
					}
				}
				break;
			}
		case 1 : /*without lapsed or dead*/
			{
				text = "without lapsed or dead policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_lapsed_or_dead())
					{
						str = ite->second.get_prod_key();
						line_stream.clear();
						line_stream.str(str.c_str());
						getline(line_stream,str,'_');
						getline(line_stream,str,'_');
						if (str==(string)"ADB01")
						{
							bom_stream_ab<<(bom_stream_ab,ite->second.get_row());
							bom_stream_ab<<("\n");

							perfs_stream_ab<<(ite->second.get_performance());
							perfs_stream_ab<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream_ab<<(ite->second.get_target_weights()[i]);
								tw_stream_ab<<(",");
								cw_stream_ab<<(ite->second.get_current_weights()[i]);
								cw_stream_ab<<(",");
							}
							tw_stream_ab<<("\n");
							cw_stream_ab<<("\n");
						}
						else
						{
							bom_stream_wb<<(bom_stream_wb,ite->second.get_row());
							bom_stream_wb<<("\n");

							perfs_stream_wb<<(ite->second.get_performance());
							perfs_stream_wb<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream_wb<<(ite->second.get_target_weights()[i]);
								tw_stream_wb<<(",");
								cw_stream_wb<<(ite->second.get_current_weights()[i]);
								cw_stream_wb<<(",");
							}
							tw_stream_wb<<("\n");
							cw_stream_wb<<("\n");
						}
					}
				}
				
				break;
			}
		case 2 : /*only lapsed or dead*/
			{
				text = "only lapsed or dead policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_lapsed_or_dead())
					{
						str = ite->second.get_prod_key();
						line_stream.clear();
						line_stream.str(str.c_str());
						getline(line_stream,str,'_');
						getline(line_stream,str,'_');
						if (str==(string)"ADB01")
						{
							bom_stream_ab<<(bom_stream_ab,ite->second.get_row());
							bom_stream_ab<<("\n");

							perfs_stream_ab<<(ite->second.get_performance());
							perfs_stream_ab<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream_ab<<(ite->second.get_target_weights()[i]);
								tw_stream_ab<<(",");
								cw_stream_ab<<(ite->second.get_current_weights()[i]);
								cw_stream_ab<<(",");
							}
							tw_stream_ab<<("\n");
							cw_stream_ab<<("\n");
						}
						else
						{
							bom_stream_wb<<(bom_stream_wb,ite->second.get_row());
							bom_stream_wb<<("\n");

							perfs_stream_wb<<(ite->second.get_performance());
							perfs_stream_wb<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream_wb<<(ite->second.get_target_weights()[i]);
								tw_stream_wb<<(",");
								cw_stream_wb<<(ite->second.get_current_weights()[i]);
								cw_stream_wb<<(",");
							}
							tw_stream_wb<<("\n");
							cw_stream_wb<<("\n");
						}
					}
					
				}
				break;
			}
		case 3 : /*without switched*/
			{
				text = "without switched policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_switch())
					{
						str = ite->second.get_prod_key();
						line_stream.clear();
						line_stream.str(str.c_str());
						getline(line_stream,str,'_');
						getline(line_stream,str,'_');
						if (str==(string)"ADB01")
						{
							bom_stream_ab<<(bom_stream_ab,ite->second.get_row());
							bom_stream_ab<<("\n");

							perfs_stream_ab<<(ite->second.get_performance());
							perfs_stream_ab<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream_ab<<(ite->second.get_target_weights()[i]);
								tw_stream_ab<<(",");
								cw_stream_ab<<(ite->second.get_current_weights()[i]);
								cw_stream_ab<<(",");
							}
							tw_stream_ab<<("\n");
							cw_stream_ab<<("\n");
						}
						else
						{
							bom_stream_wb<<(bom_stream_wb,ite->second.get_row());
							bom_stream_wb<<("\n");

							perfs_stream_wb<<(ite->second.get_performance());
							perfs_stream_wb<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream_wb<<(ite->second.get_target_weights()[i]);
								tw_stream_wb<<(",");
								cw_stream_wb<<(ite->second.get_current_weights()[i]);
								cw_stream_wb<<(",");
							}
							tw_stream_wb<<("\n");
							cw_stream_wb<<("\n");
						}
					}
				}
				break;
			}
		case 4 : /*only switched*/
			{
				text = "only switched policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_switch())
					{
						str = ite->second.get_prod_key();
						line_stream.clear();
						line_stream.str(str.c_str());
						getline(line_stream,str,'_');
						getline(line_stream,str,'_');
						if (str==(string)"ADB01")
						{
							bom_stream_ab<<(bom_stream_ab,ite->second.get_row());
							bom_stream_ab<<("\n");

							perfs_stream_ab<<(ite->second.get_performance());
							perfs_stream_ab<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream_ab<<(ite->second.get_target_weights()[i]);
								tw_stream_ab<<(",");
								cw_stream_ab<<(ite->second.get_current_weights()[i]);
								cw_stream_ab<<(",");
							}
							tw_stream_ab<<("\n");
							cw_stream_ab<<("\n");
						}
						else
						{
							bom_stream_wb<<(bom_stream_wb,ite->second.get_row());
							bom_stream_wb<<("\n");

							perfs_stream_wb<<(ite->second.get_performance());
							perfs_stream_wb<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream_wb<<(ite->second.get_target_weights()[i]);
								tw_stream_wb<<(",");
								cw_stream_wb<<(ite->second.get_current_weights()[i]);
								cw_stream_wb<<(",");
							}
							tw_stream_wb<<("\n");
							cw_stream_wb<<("\n");
						}
					}
				}
				break;
			}
		case 5 : /*without changed of shares*/
			{
				text = "without changed of shares policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_change_of_shares())
					{
						str = ite->second.get_prod_key();
						line_stream.clear();
						line_stream.str(str.c_str());
						getline(line_stream,str,'_');
						getline(line_stream,str,'_');
						if (str==(string)"ADB01")
						{
							bom_stream_ab<<(bom_stream_ab,ite->second.get_row());
							bom_stream_ab<<("\n");

							perfs_stream_ab<<(ite->second.get_performance());
							perfs_stream_ab<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream_ab<<(ite->second.get_target_weights()[i]);
								tw_stream_ab<<(",");
								cw_stream_ab<<(ite->second.get_current_weights()[i]);
								cw_stream_ab<<(",");
							}
							tw_stream_ab<<("\n");
							cw_stream_ab<<("\n");
						}
						else
						{
							bom_stream_wb<<(bom_stream_wb,ite->second.get_row());
							bom_stream_wb<<("\n");

							perfs_stream_wb<<(ite->second.get_performance());
							perfs_stream_wb<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream_wb<<(ite->second.get_target_weights()[i]);
								tw_stream_wb<<(",");
								cw_stream_wb<<(ite->second.get_current_weights()[i]);
								cw_stream_wb<<(",");
							}
							tw_stream_wb<<("\n");
							cw_stream_wb<<("\n");
						}
					}
				}
				break;
			}
		case 6 : /*only changed of shares*/
			{
				text = "only changed of shares policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_change_of_shares())
					{
						str = ite->second.get_prod_key();
						line_stream.clear();
						line_stream.str(str.c_str());
						getline(line_stream,str,'_');
						getline(line_stream,str,'_');
						if (str==(string)"ADB01")
						{
							bom_stream_ab<<(bom_stream_ab,ite->second.get_row());
							bom_stream_ab<<("\n");

							perfs_stream_ab<<(ite->second.get_performance());
							perfs_stream_ab<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream_ab<<(ite->second.get_target_weights()[i]);
								tw_stream_ab<<(",");
								cw_stream_ab<<(ite->second.get_current_weights()[i]);
								cw_stream_ab<<(",");
							}
							tw_stream_ab<<("\n");
							cw_stream_ab<<("\n");
						}
						else
						{
							bom_stream_wb<<(bom_stream_wb,ite->second.get_row());
							bom_stream_wb<<("\n");

							perfs_stream_wb<<(ite->second.get_performance());
							perfs_stream_wb<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream_wb<<(ite->second.get_target_weights()[i]);
								tw_stream_wb<<(",");
								cw_stream_wb<<(ite->second.get_current_weights()[i]);
								cw_stream_wb<<(",");
							}
							tw_stream_wb<<("\n");
							cw_stream_wb<<("\n");
						}
					}
				}
				break;
			}
		default : /*wrong mode : raise exception*/
			{
				char message [MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message,"mode %d : unexpected mode ",mode);
				throw new GMXBException (message);
			}
		}

		bom_stream_ab.close();
		perfs_stream_ab.close();
		tw_stream_ab.close();
		cw_stream_ab.close();
		bom_stream_wb.close();
		perfs_stream_wb.close();
		tw_stream_wb.close();
		cw_stream_wb.close();

		cout<<"inputs package ("<<text<<") exported"<<endl;
	
	}

	void get_dll_inputs_via_prod_key (const string& prod_key,
		const string& bom_file,
		const string& perfs_file,
		const string& tw_file, 
		const string& cw_file,
		int mode)
	{
		ofstream bom_stream (bom_file.c_str(), ios::out);
		ofstream perfs_stream (perfs_file.c_str(), ios::out);
		ofstream tw_stream (tw_file.c_str(), ios::out);
		ofstream cw_stream (cw_file.c_str(), ios::out);

		string text;
		string str;
		istringstream line_stream(istringstream::in);
		int i;

		map<string,MP_AP_BOM_no_LD>::iterator ite;
		switch(mode)
		{
		
		case 0 : /*no specs*/ 
			{
				text = "regular";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					str = ite->second.get_prod_key();
					if (str==prod_key)
					{
						bom_stream<<(bom_stream,ite->second.get_row());
						bom_stream<<("\n");

						perfs_stream<<(ite->second.get_performance());
						perfs_stream<<("\n");

						for(i=0;i<12;i++)
						{
							tw_stream<<(ite->second.get_target_weights()[i]);
							tw_stream<<(",");
							cw_stream<<(ite->second.get_current_weights()[i]);
							cw_stream<<(",");
						}
						tw_stream<<("\n");
						cw_stream<<("\n");
					}
					else
					{
					}
				}
				break;
			}
		case 1 : /*without lapsed or dead*/
			{
				text = "without lapsed or dead policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_lapsed_or_dead())
					{
						str = ite->second.get_prod_key();
						if (str==prod_key)
						{
							bom_stream<<(bom_stream,ite->second.get_row());
							bom_stream<<("\n");

							perfs_stream<<(ite->second.get_performance());
							perfs_stream<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream<<(ite->second.get_target_weights()[i]);
								tw_stream<<(",");
								cw_stream<<(ite->second.get_current_weights()[i]);
								cw_stream<<(",");
							}
							tw_stream<<("\n");
							cw_stream<<("\n");
						}
						else
						{
						}
					}
				}
				
				break;
			}
		case 2 : /*only lapsed or dead*/
			{
				text = "only lapsed or dead policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_lapsed_or_dead())
					{
						str = ite->second.get_prod_key();
						if (str==prod_key)
						{
							bom_stream<<(bom_stream,ite->second.get_row());
							bom_stream<<("\n");

							perfs_stream<<(ite->second.get_performance());
							perfs_stream<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream<<(ite->second.get_target_weights()[i]);
								tw_stream<<(",");
								cw_stream<<(ite->second.get_current_weights()[i]);
								cw_stream<<(",");
							}
							tw_stream<<("\n");
							cw_stream<<("\n");
						}
						else
						{
						}
					}
					
				}
				break;
			}
		case 3 : /*without switched*/
			{
				text = "without switched policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_switch())
					{
						str = ite->second.get_prod_key();
						if (str==prod_key)
						{
							bom_stream<<(bom_stream,ite->second.get_row());
							bom_stream<<("\n");

							perfs_stream<<(ite->second.get_performance());
							perfs_stream<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream<<(ite->second.get_target_weights()[i]);
								tw_stream<<(",");
								cw_stream<<(ite->second.get_current_weights()[i]);
								cw_stream<<(",");
							}
							tw_stream<<("\n");
							cw_stream<<("\n");
						}
						else
						{
						}
					}
				}
				break;
			}
		case 4 : /*only switched*/
			{
				text = "only switched policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_switch())
					{
						str = ite->second.get_prod_key();
						if (str==prod_key)
						{
							bom_stream<<(bom_stream,ite->second.get_row());
							bom_stream<<("\n");

							perfs_stream<<(ite->second.get_performance());
							perfs_stream<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream<<(ite->second.get_target_weights()[i]);
								tw_stream<<(",");
								cw_stream<<(ite->second.get_current_weights()[i]);
								cw_stream<<(",");
							}
							tw_stream<<("\n");
							cw_stream<<("\n");
						}
						else
						{
						}
					}
				}
				break;
			}
		case 5 : /*without changed of shares*/
			{
				text = "without changed of shares policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (!ite->second.get_change_of_shares())
					{
						str = ite->second.get_prod_key();
						if (str==prod_key)
						{
							bom_stream<<(bom_stream,ite->second.get_row());
							bom_stream<<("\n");

							perfs_stream<<(ite->second.get_performance());
							perfs_stream<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream<<(ite->second.get_target_weights()[i]);
								tw_stream<<(",");
								cw_stream<<(ite->second.get_current_weights()[i]);
								cw_stream<<(",");
							}
							tw_stream<<("\n");
							cw_stream<<("\n");
						}
						else
						{
						}
					}
				}
				break;
			}
		case 6 : /*only changed of shares*/
			{
				text = "only changed of shares policies";
				for(ite=inforce_BOM.begin();ite!=inforce_BOM.end();ite++)
				{	
					if (ite->second.get_change_of_shares())
					{
						str = ite->second.get_prod_key();
						if (str==prod_key)
						{
							bom_stream<<(bom_stream,ite->second.get_row());
							bom_stream<<("\n");

							perfs_stream<<(ite->second.get_performance());
							perfs_stream<<("\n");

							for(i=0;i<12;i++)
							{
								tw_stream<<(ite->second.get_target_weights()[i]);
								tw_stream<<(",");
								cw_stream<<(ite->second.get_current_weights()[i]);
								cw_stream<<(",");
							}
							tw_stream<<("\n");
							cw_stream<<("\n");
						}
						else
						{
						}
					}
				}
				break;
			}
		default : /*wrong mode : raise exception*/
			{
				char message [MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message,"mode %d : unexpected mode ",mode);
				throw new GMXBException (message);
			}
		}

		bom_stream.close();
		perfs_stream.close();
		tw_stream.close();
		cw_stream.close();

		cout<<"inputs package ("<<text<<") exported"<<endl;
	
	}
};