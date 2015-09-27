#pragma once

#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "GMXBExcelXMLReader.h"
using namespace std;

const int bucket_col_size = 6;  // contain: bucket_type, _start, _key, _end, _size 

class Bucket_list
{
   vector<int>            bucket_list_id_;
   vector<string>         bucket_list_type_;
   vector<vector<double>> bucket_list_contenance_;
   vector<bool>           bucket_list_if_calib_;

public:
	//! constructor/ destructor
	//! TO DO:  copy_constructor/assignment need to be offered
	Bucket_list(){}; 
	Bucket_list(string& parameter_file);
   ~Bucket_list(){};
  
	//vector<string>         get_bucket_list_type();
    //vector<vector<double>> get_bucket_list_contenance();
	string                 get_bucket_list_type(int id_bucket);
    vector<double>         get_bucket_list_contenance(int id_bucket);
	bool                   get_bucket_list_if_calib(int id_bucket);

	void print();

};

    //! ne utilise pas .push_back, which copy_coller object!
	Bucket_list :: Bucket_list(string& parameter_file)
	{
		 GMXBExcelXMLReader xml_reader(parameter_file);
		 map<string, vector<string> > map_bucket_id;
		 xml_reader.parse_XML_sheet("bucket_list", map_bucket_id);


		 int bucket_row_size = map_bucket_id.size()-1; // size of the bucket existant in file "bucket_id"
		 bucket_list_id_   = vector<int>(bucket_row_size);
		 bucket_list_type_ = vector<string>(bucket_row_size);
		 bucket_list_contenance_ = vector<vector<double>>(bucket_row_size,vector<double>(bucket_col_size-2));
		 bucket_list_if_calib_ = vector<bool>(bucket_row_size);
	
		 int i=-1;
		 int id_bucket;
		 for(map<string, vector<string> >::iterator map_iterator = map_bucket_id.begin(); map_iterator!=map_bucket_id.end(); map_iterator++)
		 {   
			 string first_col((*map_iterator).first) ;
             if(first_col == "id") continue;  // if it is the head of the excel file 

			 i++;
			 id_bucket = atoi( first_col.c_str() );

			 bucket_list_id_[i] = atoi( first_col.c_str() );

			 stringstream ss; 
			 ss << id_bucket;

			 for(int j=0; j<bucket_col_size; j++)
			 {
					 if(j==0)
					    bucket_list_type_[i] =  map_bucket_id[ss.str()][j].c_str();

					 if(j>0 && j < bucket_col_size - 2)
						bucket_list_contenance_[i][j-1] = atof( map_bucket_id[ss.str()][j].c_str() );

					 if(j == bucket_col_size - 2)
						bucket_list_contenance_[i][j-1] = atof( map_bucket_id[ss.str()][j].c_str() )*1.0/100.0;

					 if(j == bucket_col_size -1)
					 {
						 string if_calib_s (map_bucket_id[ss.str()][j].c_str());
                         //cout << "********** " << if_calib_s << endl;
					     if(if_calib_s == "Y")
						 {
							 bucket_list_if_calib_[i] = true;
						 }
						 else if(if_calib_s == "N")
						 {
							 bucket_list_if_calib_[i] = false;
						 }
						 else
						 {
							 // Error !  throw Exception! 
							 char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							 sprintf_s(message, "Error in the Excel file: variable 'bucket_list' can only take value as: 'Y' or 'N'!  ");
							 throw new GMXBException(message);
						 }
					 }
					//! check if baseline is in the first line of the bucke_list file
                    if(i==0)
					{
					    if(bucket_list_type_[i]!= "baseline" || bucket_list_id_[i]!= 1)
						{
							char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Error in Excel file bucket_list: the first line is not the baseline ! Please change your configuration file");
							throw new GMXBException(message);
						}
					}

					//! TODO: check also if the baseline's if_calib == 1 (????)
			 }
		 }
		 //print();
	}

	//vector<string> Bucket_list :: get_bucket_list_type()
	//{
	//    return this->bucket_list_type_;
	//}

	//vector<vector<double>> Bucket_list :: get_bucket_list_contenance()
	//{
	//    return this->bucket_list_contenance_;
	//}

	string Bucket_list :: get_bucket_list_type(int id_bucket)
	{
		for(unsigned int i=0; i<bucket_list_id_.size(); i++)
		{
		     if(bucket_list_id_[i] == id_bucket)
				 return this->bucket_list_type_[i];
		}
		// Error !  throw Exception! 
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Error in fonction : get_bucket_list_type(...) ");
		throw new GMXBException(message);
	}

	vector<double> Bucket_list :: get_bucket_list_contenance(int id_bucket)
	{
		for(unsigned int i=0; i<bucket_list_id_.size(); i++)
		{
			if(bucket_list_id_[i] == id_bucket)
				 return this->bucket_list_contenance_[i];
		}
		// Error !  throw Exception! 
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Error in fonction : get_bucket_list_contenance(...) ");
		throw new GMXBException(message);
	}

	bool Bucket_list :: get_bucket_list_if_calib(int id_bucket)
	{
		for(unsigned int i=0; i<bucket_list_id_.size(); i++)
		{
		     if(bucket_list_id_[i] == id_bucket)
				 return this->bucket_list_if_calib_[i];
		}
		// Error !  throw Exception! 
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Error in fonction : get_bucket_list_if_calib(...) ");
		throw new GMXBException(message);
	}


	void Bucket_list :: print()
	{
		cout << "print bucket_list " << endl;
		for(unsigned int i=0; i<bucket_list_contenance_.size(); i++)
		{   
			cout << "id = "<< bucket_list_id_[i] << "  "; 
			cout << "type = " << bucket_list_type_[i] <<"  ";
		    for(unsigned int j=0; j<bucket_list_contenance_[i].size(); j++)
			{
			    cout << bucket_list_contenance_[i][j] <<"  ";
			}
			cout << bucket_list_if_calib_[i] <<"  ";
			cout << endl;
		}
	}
