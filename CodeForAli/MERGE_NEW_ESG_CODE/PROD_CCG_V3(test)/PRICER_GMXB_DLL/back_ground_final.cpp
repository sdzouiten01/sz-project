#include "back_ground_final.h"
#include <ctime>

#include "Utility classes/Write_results_manager.h"

#include "Projection classes/Country projection classes/Projection_BL_CH_WB_01.h"
#include "Projection classes/Country projection classes/Projection_BL_LUX_WBD_01.h"
#include "Projection classes/Country projection classes/Projection_BL_LUX_WBLD_01.h"
#include "Projection classes/Country projection classes/Projection_BL_LIE_CH_WB_01.h"
#include "Projection classes/Country projection classes/Projection_BL_LIE_CH_WBL_01.h"
#include "Projection classes/Country projection classes/Projection_BL_LIE_DEIT_WBD_01.h"
#include "Projection classes/Country projection classes/Projection_BL_LIE_DE_WBDL_01.h"

#include "../ESG_V0/Instruments/InterfaceESG0.h"
#include "../ESG_V0/GaussianGen/VarianceReduction.h"
#include "../ESG_V0/GaussianGen/GaussianGen.h"
#include "../ESG_V0/Calibration/HW1FCalibrator.h"
#include "../ESG_V0/Models/BS_HW1F.h"
#include "../ESG_V0/Scenario/ScenarioGen.h"

#include "Utility classes/Inforce_Manager.h"
#include "Utility classes/Scenario_manager.h"

Projection* projectionBuilder(const string& pCK, const char* assumption_file_name,const vector<vector <int>>& i_s_a,bool generate_debug_files, const string& debug_file_path)
	{
		Projection * pro;

		if (!pCK.compare("CH_WB01"))	pro = new Projection_BL_CH_GMWB_01(i_s_a, assumption_file_name,generate_debug_files, debug_file_path);
	
		else if (!pCK.compare("LIE_CH_WB01"))	pro = new Projection_BL_LIE_CH_GMWB_01(i_s_a, assumption_file_name,generate_debug_files, debug_file_path);
		else if ((!pCK.compare("LIE_DE_WB01"))||(!pCK.compare("LIE_IT_WBD01")))	  pro = new Projection_BL_LIE_DEIT_GMWB_01(i_s_a, assumption_file_name,generate_debug_files, debug_file_path);	
		else if (!pCK.compare("LIE_DE_WBL01"))   pro = new Projection_BL_LIE_DE_GMWBL_01(i_s_a, assumption_file_name,generate_debug_files, debug_file_path);
		else if (!pCK.compare("LIE_CH_WBL01"))  pro = new Projection_BL_LIE_CH_GMWBL_01(i_s_a, assumption_file_name,generate_debug_files, debug_file_path);
	
		else if (!pCK.compare("LUX_WB01"))    pro = new Projection_BL_LUX_GMWBD_01(i_s_a, assumption_file_name,generate_debug_files, debug_file_path);
		else if (!pCK.compare("LUX_WBL01"))	  pro = new Projection_BL_LUX_GMWBLD_01(i_s_a, assumption_file_name,generate_debug_files, debug_file_path);

		else {
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Projection Builder : The prod code key %s is not found", pCK.c_str());
			throw new GMXBException(message);
		}
		return pro;
	}
Projection * create_projection(const string& pCK, const char * assumption_file_name,const vector<vector <int>>& i_s_a,bool generate_debug_files, const string& debug_file_path)
	{
		Projection * p = projectionBuilder(pCK, assumption_file_name, i_s_a, generate_debug_files, debug_file_path);
		return p;
	} 


back_ground::~back_ground(){ if (this->projection!=0) delete this->projection;}	
back_ground::back_ground(){ this->projection = 0;}
	//Projection with Scenario file
back_ground::back_ground(vector<Product_configuration>& product_config, int tot_nb_workers, int w_id, string& trace_log_pth,double alpha, double sigma){
		this->projection = 0;
		char  trace_log_name[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(trace_log_name, "%strace.log", trace_log_pth.c_str());
		trace.open(trace_log_name);

		run_dll_projection_file_V2(product_config, tot_nb_workers, w_id, alpha, sigma);
		trace.close();
	}
	// ESG and Projection by the DLL
back_ground::back_ground (vector<Product_configuration>& product_config,string& trace_log_pth,ofstream& log_strm,int nbWorker,int idWorker,
						  Dispatch_Manager& dispatch_manager){
	   	this->projection = 0;
		char  trace_log_name[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(trace_log_name, "%strace_%d.log", trace_log_pth.c_str(), idWorker);
		trace.open(trace_log_name);

		run_dll_projection_esg_V2(product_config, log_strm, nbWorker,idWorker, dispatch_manager);
		trace.close();
	}
	// Entry point of aging policies
back_ground::back_ground(vector<Product_configuration>& product_configurations_out,int tot_nb_workers,int w_id, string& trace_log_pth_out){
		UNREFERENCED_PARAMETER(w_id);
		UNREFERENCED_PARAMETER(tot_nb_workers);
		UNREFERENCED_PARAMETER(product_configurations_out);
		this->projection = 0;
		char  trace_log_name_out[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(trace_log_name_out, "%strace_out.log", trace_log_pth_out.c_str());
		trace_out.open(trace_log_name_out);

		//run_aging_policies_projection(product_configurations_out,ap_conf, tot_nb_workers, w_id);
		trace_out.close();
	}

void back_ground::create_payoff(const string& pCK, const char * assumption_file_name,const vector<vector <int>>& i_s_a, bool generate_debug_files,
	const string& debug_file_path)
	{
		if (this->projection!=0){
			delete this->projection;
			this->projection = 0;
		}
		this->projection = projectionBuilder(pCK, assumption_file_name, i_s_a, generate_debug_files, debug_file_path);
	}
	/*
	* Projection by the DLL with a scenario as input
	*/
void back_ground::run_dll_projection_file_V2(vector<Product_configuration>& product_config, int tot_nb_workers, int w_id, double alpha, double sigma)
	{
		int number_of_products_to_run = product_config.size();
		int t_high_shorter = -1;

		for (int prod_configuration_number = 0; prod_configuration_number < number_of_products_to_run; prod_configuration_number++)
		{
			// Get the product configuration
			Product_configuration product = product_config[prod_configuration_number];

			// Get the scenario list
			vector<Scenario_configuration> list_of_scenarios = product.get_scenario_list();

			// Initialize the scenario manager
			// ---- Suppose that: all the scenario files used for the same product have the same maturity
			scenario_manager scen_mgr(product.get_number_of_scenarios_to_run(),product.get_scenario_index_columns(), product.get_yield_columns_for_pv(),
				product.get_lapse_dyn_treas(),product.get_rates_indic_index(),product.get_index_number(),t_high_shorter,list_of_scenarios[0].get_scenario_file_name(),
				product.get_time_step()); 

			int number_of_scenarios_to_run = list_of_scenarios.size();
			for (int scenario_number = 0; scenario_number < number_of_scenarios_to_run; scenario_number++)
			{
				Scenario_configuration scen_configuration = list_of_scenarios[scenario_number];

				scen_mgr.init_scenario_manager(scen_configuration.get_scenario_file_name(), 1);

				// Create the Inforce manager
				Inforce_Manager inforce_mgr(',',tot_nb_workers,w_id,product,scen_configuration,false,false,false);

				// Create the projection
				this->create_payoff(product.get_prod_code_key(), product.get_config_file().c_str(), scen_configuration.get_index_shock_array()
					,product.get_generate_debug_file_indic(),product.get_debug_file_path());

				this->projection->set_total_number_scn((int)(scen_mgr.get_nb_scenarios()));

				// Loop over all the model points 
				for( int mp_number=0; mp_number < inforce_mgr.nb_model_points ; mp_number++ )
				{
					this->projection->set_current_model_point(inforce_mgr.get_model_points_vector()[mp_number]);
					Projection::compt_sen = 0;					
					for(int scen_nb=0; scen_nb < scen_mgr.get_nb_scenarios() ; scen_nb = scen_nb + 1)
					{
						projection->set_monthly_rates(scen_mgr.get_monthly_rates(scen_nb+1), scen_nb+1);
						this->projection->project();
					}
 				}				

				write_results(this->projection, product.get_run_name(),scen_configuration.get_index_shock_array(), inforce_mgr.get_model_points_vector(),
					product.get_result_file_path(),w_id, scen_configuration.get_scenario_pass_number(), 
					product.get_number_of_scenarios_to_run(), scen_configuration.get_index_shock_array().size(),
					product.get_write_valo_policy_by_policy(), alpha, sigma,scen_configuration);		
			}
		}
	}
void back_ground::run_dll_projection_esg_V2(vector<Product_configuration>& product_config,ofstream& log_strm,int tot_nb_workers,int w_id,Dispatch_Manager& dispatch_manager)	
	{
		double b = clock();
		map<string, Inforce_Manager*> map_infMgr;
		Inforce_Manager::Create_Inforce_Managers(',',tot_nb_workers,w_id,product_config,product_config[0].get_scenario_list()[0], false ,false,false, map_infMgr);
		double e = clock();
		log_strm << endl << "Time for inforce reading : " << e - b  << endl; 
		

		b = clock();
		vector<Projection*> v_of_projections;		
		for (unsigned int i = 0; i <  product_config.size(); i++){
			//Product_configuration product = product_config[i];
			/*vector<Scenario_configuration> v_scn = product.get_scenario_list();*/
			vector<vector <int>> index_shock_array;
			v_of_projections.push_back(create_projection(product_config[i].get_prod_code_key(),product_config[i].get_config_file().c_str(), index_shock_array,
				product_config[i].get_generate_debug_file_indic(), product_config[i].get_debug_file_path()));
		}
		e = clock();
		log_strm << "Time for projection creation : " << e - b  << endl; 


		
		Scenario_list scenario_list_table = dispatch_manager.get_Scenario_list_reference();

		Bucket_list   bucket_list_table   = dispatch_manager.get_Bucket_list_reference();
		vector<Regroup_by_Scenario_PN> regrouped_scenario_product(dispatch_manager.get_regrouped_scenario_pns());
		vector<int>         bucket_id_list_;
		vector<vector<int>>	product_index_list_;  
       
		for(size_t itr_regrouped = 0; itr_regrouped < regrouped_scenario_product.size(); itr_regrouped++)
		{
			int scen_pn = regrouped_scenario_product[itr_regrouped].get_scenario_pn();
			string config_ESG(scenario_list_table.get_ESG_config_path(scen_pn)); 

			//InterfaceCalibration iCalib( config_ESG );
			//InterfaceEsg iEsg(config_ESG);

			/////////////////////////////////////
			InterfaceESG0* iEsg=new InterfaceESG0(config_ESG);
			YC* Yield_curve_vector=new YC(iEsg);
			HW1FCalibrator *Calibrator=new HW1FCalibrator(iEsg,*Yield_curve_vector);
			HW1FSwaptionPricer *model=new HW1FSwaptionPricer(iEsg->Calib_Error_Function_Type,iEsg->Calibration_On,iEsg->Param_initial,iEsg->Param_min,iEsg->Param_max);
			double error=0.00000001;		
			//Calibrator->calibrate(model,error,LM_NEW); //DFPMIN, LM_NEW	

			/////////////////////////////////////////////////////

		    vector<int>         bucket_id_list_(regrouped_scenario_product[itr_regrouped].get_bucket_id_list_reference());
			vector<vector<int>>	product_index_list_(regrouped_scenario_product[itr_regrouped].get_product_index_list_reference());   
			
			int t_high_shorter;	
			int total_period = (iEsg->Number_Year * iEsg->Time_Step ) + 1;//Ali
			if((total_period-1)%12!=0 || total_period > 721){
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Error in function int scenario_manager :: init_t_high_shorter, (file_mode) (total_period-1)%12!=0 || total_period > 721" );
				throw new GMXBException(message);
			}
			else{
				t_high_shorter = (total_period-1)/12;
			}

			//double a_baseline = -99999;
			//double sigma_baseline = -99999;

			for(unsigned int iterator_total_bucket_list = 0; iterator_total_bucket_list< bucket_id_list_.size(); iterator_total_bucket_list++)
			{
				int id_total_bucket_list = bucket_id_list_[iterator_total_bucket_list];
				string bucket_type = bucket_list_table.get_bucket_list_type(id_total_bucket_list);
				vector<double> bucekt_contenance_temp = bucket_list_table.get_bucket_list_contenance(id_total_bucket_list);
				bool  bucket_if_calib = bucket_list_table.get_bucket_list_if_calib(id_total_bucket_list);

				//////////////////////////////////////////
				Yield_curve_vector->ShockSwap(bucekt_contenance_temp[0], bucekt_contenance_temp[1],bucekt_contenance_temp[2],bucekt_contenance_temp[3], bucket_type );//Ali
				//////////////////////////////////////////
				//iCalib.ShockSwap(bucekt_contenance_temp[0], bucekt_contenance_temp[1],bucekt_contenance_temp[2],bucekt_contenance_temp[3], bucket_type );

				log_strm << endl << "Begining of scenario generation number :  " << id_total_bucket_list  << endl;
				b = clock();
				
				//////////////////////////////////////////
				if(id_total_bucket_list == 1 || bucket_if_calib){	
					Calibrator->calibrate(model,error,LM_NEW); //DFPMIN, LM_NEW	//Ali
				}
				double aCalib= model->getMRV();
				double sigmaCalib = model->getsigma() ;
				string Model_rand="Mersenne_Twister";		
				string Model_diff="BS_HW1F";			
				string Model_guassian="Polar";			
				
				
				if(iEsg->Generate_All_Scenarios_In_One_Step==1) 
				{
					//Generate the Scenario
					ScenarioGen *scenario1=new ScenarioGen(iEsg,model,Yield_curve_vector,Model_rand,Model_guassian,Model_diff, 0);						
					scenario1->Generate_Scenario();	
	
					// iterate the list of products, whose use iterator_total_bucket_list			
					for (unsigned int iterator_product_index = 0; iterator_product_index < product_index_list_[iterator_total_bucket_list].size(); iterator_product_index++)
					{
						// Get the product configuration
						Product_configuration product = product_config[ product_index_list_[iterator_total_bucket_list][iterator_product_index] ];  
						// Check if the list of scenario contain the passed scenario //Ali
						scenario_manager scen_mgr(  scenario1->Scenario_vector,product.get_number_of_scenarios_to_run(),product.get_scenario_index_columns(), product.get_yield_columns_for_pv(),
							product.get_lapse_dyn_treas(),product.get_rates_indic_index(),product.get_index_number(),t_high_shorter,product.get_time_step());
						
						scen_mgr.init_scenario_manager_ESG(1, false);

						// Initialize the table of shocks				
						vector<Scenario_configuration> scen_configuration;				
						vector<Scenario_configuration> p_v_scen = product.get_scenario_list();                
						for(unsigned int i_iterator=0; i_iterator<product.get_scenario_list().size(); ++i_iterator)
						{					
							Scenario_configuration scn = p_v_scen[i_iterator];                  									
							if(scn.get_scenario_pass_number() == scen_pn)	scen_configuration.push_back(scn);				
						}

						for(unsigned int iterator_bucket_of_SF = 0; iterator_bucket_of_SF<scen_configuration.size(); iterator_bucket_of_SF++){
							int id_bucket_of_SF = scen_configuration[iterator_bucket_of_SF].get_bucket_id();
							if(id_total_bucket_list == id_bucket_of_SF)
							{						
								Inforce_Manager *inforce_mgr =  map_infMgr[product.get_run_name()];													
								Projection * proj = v_of_projections[product_index_list_[iterator_total_bucket_list][iterator_product_index]];						
								proj->set_total_number_scn((int)(scen_mgr.get_nb_scenarios()));						
					
								proj->set_index_shock_array(scen_configuration[iterator_bucket_of_SF].get_index_shock_array());						
								log_strm << "    Model point size : " << inforce_mgr->get_model_points_vector().size() << endl;						
							
								for (unsigned int k = 0; k < inforce_mgr->get_model_points_vector().size(); k++){
									Model_Point * mp = inforce_mgr->get_model_points_vector()[k];
									mp->set_pv_index_shock_array(scen_configuration[iterator_bucket_of_SF]);
								}

								for( int mp_number=0; mp_number < inforce_mgr->nb_model_points ; mp_number++ )
								{
									proj->set_current_model_point(inforce_mgr->get_model_points_vector()[mp_number]);
									proj->init_Count_scen_to_zero();
									//Projection::compt_sen = 0;
									for(int scen_nb=0; scen_nb < scen_mgr.get_nb_scenarios() ; scen_nb = scen_nb + 1)
									{
										proj->set_monthly_rates(scen_mgr.get_monthly_rates(scen_nb+1), scen_nb+1);
										proj->project();
									}
								}

								write_results(proj, product.get_run_name(), scen_configuration[iterator_bucket_of_SF].get_index_shock_array(), 
									inforce_mgr->get_model_points_vector(), product.get_result_file_path(),w_id, scen_pn,
									product.get_number_of_scenarios_to_run(),scen_configuration[iterator_bucket_of_SF].get_index_shock_array().size(),
									product.get_write_valo_policy_by_policy(),aCalib, sigmaCalib,scen_configuration[iterator_bucket_of_SF],id_total_bucket_list); 

							}
						}
					}
					delete  scenario1;
				}		
				else
				{
					ScenarioGen *scenario1=new ScenarioGen(iEsg,model,Yield_curve_vector,Model_rand,Model_guassian,Model_diff, 0,2);

	
					// iterate the list of products, whose use iterator_total_bucket_list			
					for (unsigned int iterator_product_index = 0; iterator_product_index < product_index_list_[iterator_total_bucket_list].size(); iterator_product_index++)
					{
						// Get the product configuration
						Product_configuration product = product_config[ product_index_list_[iterator_total_bucket_list][iterator_product_index] ];  
						// Initialize the table of shocks				
						vector<Scenario_configuration> scen_configuration;				
						vector<Scenario_configuration> p_v_scen = product.get_scenario_list();                
						for(unsigned int i_iterator=0; i_iterator<product.get_scenario_list().size(); ++i_iterator)
						{					
							Scenario_configuration scn = p_v_scen[i_iterator];                  									
							if(scn.get_scenario_pass_number() == scen_pn)	scen_configuration.push_back(scn);				
						}

						for(unsigned int iterator_bucket_of_SF = 0; iterator_bucket_of_SF<scen_configuration.size(); iterator_bucket_of_SF++){
							int id_bucket_of_SF = scen_configuration[iterator_bucket_of_SF].get_bucket_id();
							if(id_total_bucket_list == id_bucket_of_SF)
							{						
								Inforce_Manager *inforce_mgr =  map_infMgr[product.get_run_name()];													
								Projection * proj = v_of_projections[product_index_list_[iterator_total_bucket_list][iterator_product_index]];						
								proj->set_total_number_scn((int)(product.get_number_of_scenarios_to_run()));						
					
								proj->set_index_shock_array(scen_configuration[iterator_bucket_of_SF].get_index_shock_array());						
								log_strm << "    Model point size : " << inforce_mgr->get_model_points_vector().size() << endl;						
							
								for (unsigned int k = 0; k < inforce_mgr->get_model_points_vector().size(); k++){
									Model_Point * mp = inforce_mgr->get_model_points_vector()[k];
									mp->set_pv_index_shock_array(scen_configuration[iterator_bucket_of_SF]);
								}
								
								scenario1->Generate_Scenario();	
								scenario_manager scen_mgr(  scenario1->Scenario_vector,2,product.get_scenario_index_columns(), product.get_yield_columns_for_pv(),
												product.get_lapse_dyn_treas(),product.get_rates_indic_index(),product.get_index_number(),t_high_shorter,product.get_time_step());


									//Projection::compt_sen = 0;
									

								for(int scen_nb=0; scen_nb < product.get_number_of_scenarios_to_run()/2. ; scen_nb = scen_nb + 1)
								{
									scenario1->Generate_Scenario();	
									scen_mgr.init_scen_vector(scenario1->Scenario_vector);
									scen_mgr.init_scenario_manager_ESG(1, false);
										
									for ( int anti=0; anti<2; anti++)
									{
										for( int mp_number=0; mp_number < inforce_mgr->nb_model_points ; mp_number++ )
										{
											proj->set_current_model_point(inforce_mgr->get_model_points_vector()[mp_number]);
											if(scen_nb == 0 && anti==0) proj->init_Count_scen_to_zero();
											
											proj->set_monthly_rates(scen_mgr.get_monthly_rates(anti+1), anti+1);

											proj->project();
										}
									}
								}


								write_results(proj, product.get_run_name(), scen_configuration[iterator_bucket_of_SF].get_index_shock_array(), 
									inforce_mgr->get_model_points_vector(), product.get_result_file_path(),w_id, scen_pn,
									product.get_number_of_scenarios_to_run(),scen_configuration[iterator_bucket_of_SF].get_index_shock_array().size(),
									product.get_write_valo_policy_by_policy(),aCalib, sigmaCalib,scen_configuration[iterator_bucket_of_SF],id_total_bucket_list); 

							}
						}
					}				
					delete  scenario1;
				}			

				
			}
			delete  Calibrator;						
			delete model;			
			delete Yield_curve_vector;			
			delete  iEsg;
		}

		// Delete all projections
		int v_of_projection_size = v_of_projections.size();
		for (int p = 0 ; p < v_of_projection_size; p++)
			delete v_of_projections[p];

		// Delete all inforce managers
		for (map<string, Inforce_Manager*>::iterator it_inf_mgr = map_infMgr.begin();it_inf_mgr!= map_infMgr.end(); ++it_inf_mgr){
			Inforce_Manager * if_mgr = it_inf_mgr->second;
			delete if_mgr;
		}
	}
//void back_ground::run_dll_projection_esg_V2(vector<Product_configuration>& product_config,ofstream& log_strm,int tot_nb_workers,int w_id,Dispatch_Manager& dispatch_manager)	
//	{
//		double b = clock();
//		map<string, Inforce_Manager*> map_infMgr;
//		Inforce_Manager::Create_Inforce_Managers(',',tot_nb_workers,w_id,product_config,product_config[0].get_scenario_list()[0], false ,false,false, map_infMgr);
//		double e = clock();
//		log_strm << endl << "Time for inforce reading : " << e - b  << endl; 
//		
//
//		b = clock();
//		vector<Projection*> v_of_projections;		
//		for (unsigned int i = 0; i <  product_config.size(); i++){
//			//Product_configuration product = product_config[i];
//			/*vector<Scenario_configuration> v_scn = product.get_scenario_list();*/
//			vector<vector <int>> index_shock_array;
//			v_of_projections.push_back(create_projection(product_config[i].get_prod_code_key(),product_config[i].get_config_file().c_str(), index_shock_array,
//				product_config[i].get_generate_debug_file_indic(), product_config[i].get_debug_file_path()));
//		}
//		e = clock();
//		log_strm << "Time for projection creation : " << e - b  << endl; 
//
//
//		
//		Scenario_list scenario_list_table = dispatch_manager.get_Scenario_list_reference();
//
//		Bucket_list   bucket_list_table   = dispatch_manager.get_Bucket_list_reference();
//		vector<Regroup_by_Scenario_PN> regrouped_scenario_product(dispatch_manager.get_regrouped_scenario_pns());
//		vector<int>         bucket_id_list_;
//		vector<vector<int>>	product_index_list_;  
//       
//		for(size_t itr_regrouped = 0; itr_regrouped < regrouped_scenario_product.size(); itr_regrouped++)
//		{
//			int scen_pn = regrouped_scenario_product[itr_regrouped].get_scenario_pn();
//			string config_ESG(scenario_list_table.get_ESG_config_path(scen_pn)); 
//
//			//InterfaceCalibration iCalib( config_ESG );
//			//InterfaceEsg iEsg(config_ESG);
//
//			/////////////////////////////////////
//			InterfaceESG0* iEsg=new InterfaceESG0(config_ESG);
//			YC* Yield_curve_vector=new YC(iEsg);
//			HW1FCalibrator *Calibrator=new HW1FCalibrator(iEsg,*Yield_curve_vector);
//			HW1FSwaptionPricer *model=new HW1FSwaptionPricer(iEsg->Calib_Error_Function_Type,iEsg->Calibration_On,iEsg->Param_initial,iEsg->Param_min,iEsg->Param_max);
//			double error=0.00000001;		
//			//Calibrator->calibrate(model,error,LM_NEW); //DFPMIN, LM_NEW	
//
//			/////////////////////////////////////////////////////
//
//		    vector<int>         bucket_id_list_(regrouped_scenario_product[itr_regrouped].get_bucket_id_list_reference());
//			vector<vector<int>>	product_index_list_(regrouped_scenario_product[itr_regrouped].get_product_index_list_reference());   
//			
//			int t_high_shorter;	
//			int total_period = (iEsg->Number_Year * iEsg->Time_Step ) + 1;//Ali
//			if((total_period-1)%12!=0 || total_period > 721){
//				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
//				sprintf_s(message, "Error in function int scenario_manager :: init_t_high_shorter, (file_mode) (total_period-1)%12!=0 || total_period > 721" );
//				throw new GMXBException(message);
//			}
//			else{
//				t_high_shorter = (total_period-1)/12;
//			}
//
//			//double a_baseline = -99999;
//			//double sigma_baseline = -99999;
//
//			for(unsigned int iterator_total_bucket_list = 0; iterator_total_bucket_list< bucket_id_list_.size(); iterator_total_bucket_list++)
//			{
//				int id_total_bucket_list = bucket_id_list_[iterator_total_bucket_list];
//				string bucket_type = bucket_list_table.get_bucket_list_type(id_total_bucket_list);
//				vector<double> bucekt_contenance_temp = bucket_list_table.get_bucket_list_contenance(id_total_bucket_list);
//				bool  bucket_if_calib = bucket_list_table.get_bucket_list_if_calib(id_total_bucket_list);
//
//				//////////////////////////////////////////
//				Yield_curve_vector->ShockSwap(bucekt_contenance_temp[0], bucekt_contenance_temp[1],bucekt_contenance_temp[2],bucekt_contenance_temp[3], bucket_type );//Ali
//				//////////////////////////////////////////
//				//iCalib.ShockSwap(bucekt_contenance_temp[0], bucekt_contenance_temp[1],bucekt_contenance_temp[2],bucekt_contenance_temp[3], bucket_type );
//
//				log_strm << endl << "Begining of scenario generation number :  " << id_total_bucket_list  << endl;
//				b = clock();
//				
//				//////////////////////////////////////////
//				if(id_total_bucket_list == 1 || bucket_if_calib){	
//					Calibrator->calibrate(model,error,LM_NEW); //DFPMIN, LM_NEW	//Ali
//				}
//				double aCalib= model->getMRV();
//				double sigmaCalib = model->getsigma() ;
//				string Model_rand="Mersenne_Twister";		
//				string Model_diff="BS_HW1F";			
//				string Model_guassian="Polar";			
//				ScenarioGen *scenario1=new ScenarioGen(iEsg,model,Yield_curve_vector,Model_rand,Model_guassian,Model_diff, 0);						
//				if(iEsg->Generate_All_Scenarios_In_One_Step==1) scenario1->Generate_Scenario();			
//				else{//Shouldn't be used in this mode				
//					for(int it=0; it <iEsg->Local_Number_Scenario/2;it++)					
//						scenario1->Generate_Scenario();			
//				}			
//				/////////////////////////////////////////						
//				//double *** va_scen = iEsg.Gen(iCalib,aCalib,sigmaCalib,scen_pn,bucket_if_calib,id_total_bucket_list, w_id); 			
//				e = clock();			
//				log_strm << "End of scenario generation number :  " << id_total_bucket_list << " time : " << e - b  << endl;			
///*				if (id_total_bucket_list == 1){				
//					a_baseline = aCalib ;				
//					sigma_baseline = sigmaCalib ;			
//				}*/			
//				// iterate the list of products, whose use iterator_total_bucket_list			
//				for (unsigned int iterator_product_index = 0; iterator_product_index < product_index_list_[iterator_total_bucket_list].size(); iterator_product_index++)
//				{
//					// Get the product configuration
//					Product_configuration product = product_config[ product_index_list_[iterator_total_bucket_list][iterator_product_index] ];  
//					// Check if the list of scenario contain the passed scenario //Ali
//					scenario_manager scen_mgr(  scenario1->Scenario_vector,product.get_number_of_scenarios_to_run(),product.get_scenario_index_columns(), 
//											product.get_yield_columns_for_pv(),product.get_lapse_dyn_treas(),product.get_rates_indic_index(),
//											product.get_index_number(),t_high_shorter,product.get_time_step());
//		
//					bool if_he_inner_mode = false;			
//					scen_mgr.init_scenario_manager_ESG(1, if_he_inner_mode);
//	
//					// Initialize the table of shocks				
//					vector<Scenario_configuration> scen_configuration;				
//					vector<Scenario_configuration> p_v_scen = product.get_scenario_list();                
//					for(unsigned int i_iterator=0; i_iterator<product.get_scenario_list().size(); ++i_iterator)
//					{					
//						Scenario_configuration scn = p_v_scen[i_iterator];                  									
//						if(scn.get_scenario_pass_number() == scen_pn)					
//							scen_configuration.push_back(scn);				
//					}								
//					for(unsigned int iterator_bucket_of_SF = 0; iterator_bucket_of_SF<scen_configuration.size(); iterator_bucket_of_SF++){
//						int id_bucket_of_SF = scen_configuration[iterator_bucket_of_SF].get_bucket_id();
//						if(id_total_bucket_list == id_bucket_of_SF)
//						{						
//							Inforce_Manager *inforce_mgr =  map_infMgr[product.get_run_name()];			
//							log_strm << "    Beg Processing run : " << product.get_run_name() << " for bucket id " << id_bucket_of_SF << endl;						
//							b = clock();						
//							// Remove create payoff, set t_high, set longevity , index shock array + fund_shock_array						
//							Projection * proj = v_of_projections[product_index_list_[iterator_total_bucket_list][iterator_product_index]];						
//							proj->set_total_number_scn((int)(scen_mgr.get_nb_scenarios()));						
//					
//							proj->set_index_shock_array(scen_configuration[iterator_bucket_of_SF].get_index_shock_array());						
//							log_strm << "    Model point size : " << inforce_mgr->get_model_points_vector().size() << endl;						
//							
//							for (unsigned int k = 0; k < inforce_mgr->get_model_points_vector().size(); k++){
//								Model_Point * mp = inforce_mgr->get_model_points_vector()[k];
//								mp->set_pv_index_shock_array(scen_configuration[iterator_bucket_of_SF]);
//							}
//
//							for( int mp_number=0; mp_number < inforce_mgr->nb_model_points ; mp_number++ )
//							{
//								proj->set_current_model_point(inforce_mgr->get_model_points_vector()[mp_number]);
//								Projection::compt_sen = 0;
//								for(int scen_nb=0; scen_nb < scen_mgr.get_nb_scenarios() ; scen_nb = scen_nb + 1)
//								{
//									proj->set_monthly_rates(scen_mgr.get_monthly_rates(scen_nb+1), scen_nb+1);
//									proj->project();
//								}
//							}
//
//							write_results(proj, product.get_run_name(), scen_configuration[iterator_bucket_of_SF].get_index_shock_array(), 
//								inforce_mgr->get_model_points_vector(), product.get_result_file_path(),w_id, scen_pn,
//								product.get_number_of_scenarios_to_run(),scen_configuration[iterator_bucket_of_SF].get_index_shock_array().size(),
//								product.get_write_valo_policy_by_policy(),aCalib, sigmaCalib,scen_configuration[iterator_bucket_of_SF],id_total_bucket_list); 
//
//							e = clock();
//							log_strm << "    End Processing run : " << product.get_run_name() << " for bucket id " << id_bucket_of_SF << " Time : " << e - b << endl;
//						}
//					}
//				}
//				delete  scenario1;
//			}
//			delete  Calibrator;						
//			delete model;			
//			delete Yield_curve_vector;			
//			delete  iEsg;
//		}
//
//		// Delete all projections
//		int v_of_projection_size = v_of_projections.size();
//		for (int p = 0 ; p < v_of_projection_size; p++)
//			delete v_of_projections[p];
//
//		// Delete all inforce managers
//		for (map<string, Inforce_Manager*>::iterator it_inf_mgr = map_infMgr.begin();it_inf_mgr!= map_infMgr.end(); ++it_inf_mgr){
//			Inforce_Manager * if_mgr = it_inf_mgr->second;
//			delete if_mgr;
//		}
//	}
//

