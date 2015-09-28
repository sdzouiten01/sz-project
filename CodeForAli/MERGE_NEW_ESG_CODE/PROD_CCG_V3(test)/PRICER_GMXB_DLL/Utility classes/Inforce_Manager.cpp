#include "Inforce_Manager.h"
//#include "Utility classes/Builder.h"
#include "../Model point classes/Country model point classes/model_point_BL_LUX_01.h"

#include "../Model point classes/Country model point classes/model_point_BL_RST_SWISS_01.h"

#include "../Model point classes/Country model point classes/model_point_BL_RS_CH_LIE_01.h"
#include "../Model point classes/Country model point classes/model_point_BL_RS_DE_LIE_01.h"
#include "../Model point classes/Country model point classes/model_point_BL_RST_CH_LIE_01.h"
#include "../Model point classes/Country model point classes/model_point_BL_RST_DEIT_LIE_01.h"
/*
* This function build the right Model_point class
* according to the passed product code key.
* const string& pCK : product code Key of the model point to build
* vector<string>& row : One row of the inforce file converted in vector of string (the separator is the comma)
* int rank : The rank of the model_point in the inforce file (necessary for the dispatching of the policies)
* Scenario_configuration& scenario_config : Contain all the information to build the model point object (size of the shocks ...)
*/

Model_Point* modelPointBuilder(const string& pCK, vector<string>& row, int rank, Scenario_configuration& scenario_config)
{
	Model_Point* pMP;
	if (   pCK == "")    pMP = new Model_Point(row, rank, scenario_config);
	
	else if(pCK == "CH_WB01")	pMP = new Model_Point_BL_RST_SWISS_01(row, rank, scenario_config);
	
	else if(pCK == "LIE_CH_WB01")	pMP = new Model_Point_BL_RST_CH_LIE_01(row, rank, scenario_config);
	else if(pCK == "LIE_DE_WB01"||pCK == "LIE_IT_WBD01")	pMP = new Model_Point_BL_RST_DEIT_LIE_01(row, rank, scenario_config);
	else if(pCK == "LIE_DE_WBL01")	pMP = new Model_Point_BL_RS_DE_LIE_01(row, rank, scenario_config);
	else if(pCK == "LIE_CH_WBL01")	pMP = new Model_Point_BL_RS_CH_LIE_01(row, rank, scenario_config);
	
	else if(pCK == "LUX_WB01" || pCK == "LUX_WBL01")	pMP = new Model_Point_LUX_01(row, rank, scenario_config);

	else {
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Model point builder : The prod code key %s is not found", pCK.c_str());
		throw new GMXBException(message);
	}
	return pMP;
}










int Inforce_Manager::get_number_of_columns_in_row(const string& row)
	{
		int nb_col = 0;
		string element;
		istringstream row_streamed;
		row_streamed.str(row);
		while ( getline(row_streamed, element, ',') )
		{
				nb_col++;	
		}
		return nb_col;
}
 
void Inforce_Manager::extract_model_points_from_file(Product_configuration& product_config, Scenario_configuration & scenario_config, char separator)
{
	int nb_records;
	vector<int> index_vect_mp_to_build;
	int remain;
	int lower_bound;
	int upper_bound;
	int nb_mp_to_build;
	ifstream file_op;
	string row;
	vector<string> row_vector;
	istringstream row_streamed;
	string element;

	//actuarial tool
	vector<string> vector_ap;



	if (product_config.get_extract_all_mp() || product_config.get_tot_number_mps() > 0)
	{
		nb_records = 0;
		nb_mp_to_build = 0;

		file_op.open(product_config.get_model_point_file().c_str(), ios::in);
		if (!file_op.fail())
		{
			while(!file_op.eof())
			{
				if (!product_config.get_extract_all_mp())
					if (nb_records+1 > product_config.get_tot_number_mps())
						break;

				getline(file_op, row);
				if (nb_records == 0)
				{
					this->number_of_columns = get_number_of_columns_in_row(row);
				}
				if (row.length() > 0)
				{
					nb_records++;
					row_streamed.str(row);
					for(int i=0; i<=PROD_CODE; i++)
					{
						getline(row_streamed, element, separator);
					}
					if(!strcmp(element.c_str(), product_config.get_prod_code_key().c_str()))
					{
						nb_mp_to_build++;
						index_vect_mp_to_build.push_back(nb_records);
					}
					else
					{
						index_vect_mp_to_build.push_back(-1);
					}
				}
			}
		}
		file_op.close();
		file_op.clear();
	}
	else 
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "The file %s is not found", product_config.get_model_point_file().c_str());
		throw new GMXBException(message);
	}

	remain = nb_mp_to_build % this->tot_nb_workers;
	if (remain != 0 && (this->w_id <= remain))
	{
		this->nb_model_points = nb_mp_to_build / this->tot_nb_workers + 1;
		lower_bound = (this->w_id - 1) * this->nb_model_points + 1;
		upper_bound = this->w_id * this->nb_model_points + 1;
	} 
	else
	{
		this->nb_model_points = nb_mp_to_build / this->tot_nb_workers;
		lower_bound = (this->w_id - 1) * this->nb_model_points + remain + 1;
		upper_bound = this->w_id * this->nb_model_points + remain + 1;
	}

	if (this->is_he)
	{
	// HE (P.GARNERO)
	inforce= new string*[nb_records];
	this->nb_mp=nb_records;
	for (int i=0;i< nb_records; i++)
	{
		inforce[i]=new string[number_of_columns];
		///inforce[i] = "";
		}
	}
    else
	{
		if(this->is_ra)
		{
			 inforce= new string*[nb_mp_to_build];
			 for (int i=0;i< nb_mp_to_build; i++)
			 {
				inforce[i]=new string[number_of_columns];
			 }
		}
	}
	int k = 0;

	// each worker will calculate [lower_bound, upper_bound[ of the inforces.
	file_op.open(product_config.get_model_point_file().c_str(), ios::in);
	for(int i=0, j=0; i<nb_records; i++)
	{
		getline(file_op, row);
		if (index_vect_mp_to_build[i]!=-1)
		{
			j++;
			if (j>=upper_bound)
			{
				break; 
			}
			else if (j>=lower_bound )
			{
				if (row.length() > 0)
				{
					row_streamed.str(row);
					k=0;
					while ( getline(row_streamed, element, separator) )
					{
						row_vector.push_back(element);
						//For Hedge effectiveness
						if (this->is_he)
						{
						// Modif Paul
						inforce[i][k]=element;
						k++;
						}
						//for risk appetite
						if(this->is_ra)
						{
							this->inforce[j-1][k]=element;
							k++;
						}
						if(this->is_ap)
						{
							vector_ap.push_back(element);
						}
					}
					if(this->is_ap)
					{
						inforce_ap.push_back(vector_ap);
						vector_ap.clear();

					}
					this->number_of_columns = row_vector.size();
					this->mp_vector.push_back(modelPointBuilder(product_config.get_prod_code_key(), row_vector, 
													index_vect_mp_to_build[i], /*product_config,*/ scenario_config));
					row_streamed.clear();
					row_vector.clear();
				}
			}
		}
	}
	file_op.close();
}

Inforce_Manager::~Inforce_Manager()
{
		if (this->is_he)
		{
		for (unsigned int i=0;i< mp_vector.size(); i++)
			delete[] inforce[i];
		if (inforce != 0)
			delete[] inforce;
		}
		vector<Model_Point*>::iterator itr;
		for(itr=this->mp_vector.begin(); itr!=this->mp_vector.end(); ++itr)
			delete *itr;
}

Inforce_Manager::Inforce_Manager(){inforce = 0;	}

Inforce_Manager::Inforce_Manager(char separator, int total_number_of_workers, int worker_id, Product_configuration& product_config,
					Scenario_configuration& scenario_config, bool is_HE, bool is_AP, bool is_RA)
{
		this->tot_nb_workers = total_number_of_workers;
		this->w_id = worker_id;
		this->is_he = is_HE;
		this->is_ap = is_AP;
		this->is_ra = is_RA;
		this->extract_model_points_from_file(product_config, scenario_config, separator);
}

	// Add by Yaokun
	// 04-02-2010
#pragma region Updated Inforce Manager
Inforce_Manager::Inforce_Manager(int total_number_of_workers, int worker_id, bool is_HE, bool is_AP, bool is_RA)
{
		this->tot_nb_workers = total_number_of_workers;
		this->w_id = worker_id;
		this->is_he = is_HE;
		this->is_ap = is_AP;
		this->is_ra = is_RA;
		inforce = 0;
}
	
void Inforce_Manager::Create_Inforce_Managers(char separator, int total_number_of_workers, int worker_id, vector<Product_configuration>& vec_prod_cfg,
		Scenario_configuration& scen_cfg, bool is_HE, bool is_AP, bool is_RA, map<string, Inforce_Manager*>& inforce_mgr_map)
	{
		inforce_mgr_map.clear();
		map<string, vector<Product_configuration*>> prod_cfg_maps;
		for (vector<Product_configuration>::iterator iter = vec_prod_cfg.begin(); iter != vec_prod_cfg.end(); ++iter)
		{
			prod_cfg_maps[iter->get_model_point_file()].push_back(&(*iter));
		}

		for (map<string, vector<Product_configuration*>>::iterator iter = prod_cfg_maps.begin(); iter != prod_cfg_maps.end(); ++iter)
		{
			int size_to_build =iter->second.size();
			vector<Inforce_Manager*> inforce_mgrs(size_to_build);
			for (int i = 0; i < size_to_build; ++i)
			{
				inforce_mgrs[i] = new Inforce_Manager(total_number_of_workers, worker_id, is_HE, is_AP, is_RA);
			}
			extract_model_points_from_file(separator, iter->second, scen_cfg, inforce_mgrs);
			vector<Product_configuration*>& vec_cfg = iter->second;
			for (int i = 0; i < size_to_build; ++i)
			{
				inforce_mgr_map[vec_cfg[i]->get_run_name()] = inforce_mgrs[i];
			}
		}
	}

void Inforce_Manager::extract_model_points_from_file(char separator, vector<Product_configuration*>& vec_prod_cfg, Scenario_configuration& scen_cfg, vector<Inforce_Manager*>& inforce_mgrs)
	{
	#pragma region Internal Struct
		struct Records{
			int upper_bound;
			int lower_bound;
			int nb_mp_to_build;
			int j;
			Records():upper_bound(0), lower_bound(0), nb_mp_to_build(0), j(0){}
		};
	#pragma endregion
		bool extract_all_mp = false;
		int nb_mp_to_get = 0;		// record the max number of model point need to get

		int size_to_build = vec_prod_cfg.size();

		vector<string> prod_codes(size_to_build);
		vector<bool> isReplicated(size_to_build, false);
		for (int i = 0; i < size_to_build; ++i)
		{
			prod_codes[i] = vec_prod_cfg[i]->get_prod_code_key();
			for (int j = 0; j < i; ++j)
			{
				if ( prod_codes[i] == prod_codes[j] )
				{
					isReplicated[i] = true;
					isReplicated[j] = true;
					//break;
				}
			}
		}

		for (int i = 0; i < size_to_build; ++i)
		{
			if (vec_prod_cfg[i]->get_extract_all_mp())
			{
				extract_all_mp = true;
				break;
			}
			nb_mp_to_get = max( nb_mp_to_get, vec_prod_cfg[i]->get_tot_number_mps() );
		}

		int nb_records = 0;
		int number_of_columns = 0;									// !!! To assgin this to All InforceManager
		vector<vector<int>> index_mp_to_build;						// index_mp_to_build[i] = prod_i : i-th linein Inforce file is for vec_prod_cfg[prod_i]
		vector<Records> records(size_to_build);

		ifstream file_op;
		string row, element;
		vector<string> row_vector;
		istringstream row_streamed;
		if ( extract_all_mp || nb_mp_to_get > 0)
		{
			
			// Begin Read File
			file_op.open(vec_prod_cfg[0]->get_model_point_file().c_str(), ios::in);
			if (!file_op.fail())
			{
				while(!file_op.eof())
				{
					// Check if we can exit now, i.e all model points have been read.
					if ( !extract_all_mp && nb_mp_to_get == 0)
						break;

					getline(file_op, row);
					if (nb_records == 0)
					{
						number_of_columns = get_number_of_columns_in_row(row);
					}
					if (row.length() > 0)
					{
						row_streamed.str(row);
						index_mp_to_build.push_back(vector<int>());
						for(int i = 0; i <= PROD_CODE; i++)
						{
							getline(row_streamed, element, separator);
						}
						// Search if current prod code is what we want to parse
						for (int prod_i = 0; prod_i < size_to_build; ++prod_i)
						{
							// Yes, Parse it
							if( element == prod_codes[prod_i] )
							{
								records[prod_i].nb_mp_to_build++;

								index_mp_to_build[nb_records].push_back(prod_i);	// Record Line number for prod_i

								// This line is only for one product
								if (isReplicated[prod_i] == false)
									break;
							}
						}
						nb_records++;
					}
				}
			}
		}
		else 
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "The file %s is not found", vec_prod_cfg[0]->get_model_point_file().c_str());
			throw new GMXBException(message);
		}
		
		Inforce_Manager* cur_mgr = NULL;
		int tot_nb_workers = inforce_mgrs[0]->tot_nb_workers;
		for (int i = 0; i < size_to_build; ++i)
		{
			cur_mgr = inforce_mgrs[i];
			cur_mgr->number_of_columns = number_of_columns;

			int remain = 0;
			remain = records[i].nb_mp_to_build % tot_nb_workers;
			if (remain != 0 && (cur_mgr->w_id <= remain))
			{
				inforce_mgrs[i]->nb_model_points = records[i].nb_mp_to_build / tot_nb_workers + 1;
				records[i].lower_bound = (cur_mgr->w_id - 1) * cur_mgr->nb_model_points + 1;
				records[i].upper_bound = cur_mgr->w_id * cur_mgr->nb_model_points + 1;
			} 
			else
			{
				cur_mgr->nb_model_points = records[i].nb_mp_to_build / cur_mgr->tot_nb_workers;
				records[i].lower_bound = (cur_mgr->w_id - 1) * cur_mgr->nb_model_points + remain + 1;
				records[i].upper_bound = cur_mgr->w_id * cur_mgr->nb_model_points + remain + 1;
			}

			if (cur_mgr->is_he)
			{
				// HE (P.GARNERO)
				cur_mgr->inforce= new string*[nb_records];
				cur_mgr->nb_mp = nb_records;
				for (int i = 0; i< nb_records; i++)
				{
					cur_mgr->inforce[i] = new string[number_of_columns];
					///inforce[i] = "";
				}
			}
			else
			{
				if(cur_mgr->is_ra)
				{
					cur_mgr->inforce = new string*[records[i].nb_mp_to_build];
					for (int i=0;i< records[i].nb_mp_to_build; i++)
					{
						cur_mgr->inforce[i] = new string[number_of_columns];
					}
				}
			}
		}
		file_op.clear();
		file_op.seekg(ios::beg);
		// each worker will calculate [lower_bound, upper_bound[ of the inforces.
		vector<bool> isFinish(size_to_build, false);
		int rest_prod = size_to_build;
		for(int i = 0; i < nb_records; ++i)
		{
			getline(file_op, row);
			if (index_mp_to_build[i].size() == 0)
			{
				continue;
			}

			for (unsigned int idx = 0; idx < index_mp_to_build[i].size(); ++idx)
			{
				int prod_i =  index_mp_to_build[i][idx];

				if ( prod_i != -1 && !isFinish[prod_i])	// valid product_code and has not finished its parsing
				{
					++records[prod_i].j;
					if ( records[prod_i].j >= records[prod_i].upper_bound )
					{
						isFinish[prod_i] = true;
						--rest_prod;
						if (rest_prod == 0)
							break;
					}
					else if (records[prod_i].j >= records[prod_i].lower_bound )
					{
						if (row.length() > 0)
						{
							row_streamed.str(row);
							int k = 0;
							//actuarial tool
							vector<string> vector_ap;

							while ( getline(row_streamed, element, separator) )
							{
								row_vector.push_back(element);
								//For Hedge effectiveness
								if (inforce_mgrs[prod_i]->is_he)
								{
									// Modif Paul
									inforce_mgrs[prod_i]->inforce[i][k] = element;
									k++;
								}
								//for risk appetite
								if(inforce_mgrs[prod_i]->is_ra)
								{
									inforce_mgrs[prod_i]->inforce[records[prod_i].j-1][k] = element;
									k++;
								}
								if(inforce_mgrs[prod_i]->is_ap)
								{
									vector_ap.push_back(element);
								}
							}
							if(inforce_mgrs[prod_i]->is_ap)
							{
								inforce_mgrs[prod_i]->inforce_ap.push_back(vector_ap);
							}
							vector_ap.clear();
							//this->number_of_columns = row_vector.size();

							inforce_mgrs[prod_i]->mp_vector.push_back(modelPointBuilder(prod_codes[prod_i], row_vector,
								i+1,/* *vec_prod_cfg[prod_i],*/ scen_cfg));
							row_streamed.clear();
							row_vector.clear();
						}
					}
				}
			}
		}
		file_op.close();
	}
#pragma endregion

void Inforce_Manager::get_list_of_model_points_per_product(const string& product_code, 
											vector<Model_Point*>& mps, Product_configuration& product_config,
											Scenario_configuration& scenario_config)
	{
		ifstream file_op;
		file_op.open(product_config.get_model_point_file().c_str(), ios::in);
		int rank = 0;
		if (file_op)
		{

			while(!file_op.eof())
			{
				string row;
				getline(file_op, row);
				if (row.length() > 0)
				{
					vector<string> row_vector;
					istringstream row_streamed;
					string element;
					rank++;
					row_streamed.str(row);
					while (getline(row_streamed, element, ','))
					{
						row_vector.push_back(element);
					}

					if(!strcmp(row_vector[PROD_CODE].c_str(), product_code.c_str()))
					{
						mps.push_back(modelPointBuilder(product_code, row_vector,rank, /*product_config,*/ scenario_config));
					}
				}
			}
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Inforce Manager error : the file %s is not found", product_config.get_model_point_file().c_str());
			throw new GMXBException(message);
		}
	}

void Inforce_Manager::get_list_of_all_model_points(const string& product_code, 
											vector<Model_Point*>& mps, Product_configuration& product_config,
											Scenario_configuration& scenario_config)
	{
		ifstream file_op;
		file_op.open(product_config.get_model_point_file().c_str(), ios::in);
		int rank = 0;
		if (file_op)
		{

			while(!file_op.eof())
			{
				string row;
				getline(file_op, row);
				if (row.length() > 0)
				{
					vector<string> row_vector;
					istringstream row_streamed;
					string element;
					rank++;
					row_streamed.str(row);
					while (getline(row_streamed, element, ','))
					{
						row_vector.push_back(element);
					}

					mps.push_back(modelPointBuilder(product_code, row_vector,rank, /*product_config, */scenario_config));
				}
			}
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Inforce Manager error : the file %s is not found", product_config.get_model_point_file().c_str());
			throw new GMXBException(message);
		}
	}

void Inforce_Manager::get_list_of_model_points_per_product_and_gmxb_type(const string& product_code,string& gmxb_type,vector<Model_Point*>& mps,
															Product_configuration& product_config,
															Scenario_configuration& scenario_config)
	{
		ifstream file_op;
		file_op.open(product_config.get_model_point_file().c_str(), ios::in);
		int rank = 0;
		if (file_op)
		{

			while(!file_op.eof())
			{
				string row;
				getline(file_op, row);
				if (row.length() > 0)
				{
					vector<string> row_vector;
					istringstream row_streamed;
					string element;
					rank++;
					row_streamed.str(row);
					while (getline(row_streamed, element, ','))
					{
						row_vector.push_back(element);
					}

					if (gmxb_type == "EMPTY")
						gmxb_type = "";

					if(!strcmp(row_vector[PROD_CODE].c_str(), product_code.c_str()))
					{
						mps.push_back(modelPointBuilder(product_code, row_vector,rank, /*product_config,*/ scenario_config));
					}
				}
			}
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Inforce Manager error : the file %s is not found", product_config.get_model_point_file().c_str());
			throw new GMXBException(message);
		}
	}
vector<Model_Point*>& Inforce_Manager::get_model_points_vector()
	{
		return this->mp_vector;
	}

string** Inforce_Manager::get_inforce()
	{
		return this->inforce;
	}

int Inforce_Manager::get_nb_mp()
	{
		return this->nb_model_points;
	}

	// For actuarial tool
void Inforce_Manager::set_ap_indicator (bool ind)
	{
		this->is_ap = ind;
	}

vector<vector<string>>& Inforce_Manager::get_inforce_ap(void)
	{
		return this->inforce_ap;
	}

