#include "Assumptions.h"
#include "Hedgeff.h"

Hedgeff::Hedgeff(Assumptions* assumptions,int model_period,string file_input_name,int index_no,int scen_no,string rw_scn_file)
{
	this->objects_needed.assumptions=assumptions;
	this->index_no=index_no;
	this->scen_no=scen_no;
	this->model_period=model_period;
	this->init_fields(file_input_name,rw_scn_file);	
}

Hedgeff::~Hedgeff(void)
{	

	//chth
	for(int i=0;i<this->objects_needed.assumptions->nb_maturities_to_hedge;++i)
	{
		delete[] this->rho_liab[i];
	}
	delete[] this->rho_liab;

	delete this->period;
	delete this->liab_ov;
	delete this->delta_liab;
	delete this->gamma_liab;
	delete this->discount;

	delete Total_charges;
	delete claims;

}
void Hedgeff::init_row_to_start(string file_input_name)
{	
	
	this->row_to_start=0;
	fstream file(file_input_name.c_str());
	string row;
	char * pch;
	char * next_token;
	getline(file,row);
	while(!file.eof())
	{
		getline(file,row);
		this->row_to_start++;

		pch = strtok_s((char*)row.c_str(),",",&next_token);
		if (atoi(pch)==scen_no)
		break;
	}

	file.close();
}

void Hedgeff::init_fields(string file_input_name, string rw_scn_file)
{
	this->init_row_to_start(file_input_name);

	this->period=new double[model_period];
//	this->av_e=new double[model_period];
	this->liab_ov=new double[model_period];
	this->delta_liab=new double[model_period];
	this->gamma_liab=new double[model_period];
	this->discount=new double[model_period];
	this->index=new double[model_period];
	this->av_index=new double[model_period];

	this->Total_charges = new double[model_period];
	this->claims  = new double[model_period];

	this->rho_liab=new double*[this->objects_needed.assumptions->nb_maturities_to_hedge];
	for (int i=0;i<this->objects_needed.assumptions->nb_maturities_to_hedge;++i)
	{
		this->rho_liab[i]=new double[model_period];
	}

//	this->gamma_1_as=new double[model_period];

	string delta_1_lb_str;
	delta_1_lb_str="delta_";
	char* tst1=new char[3];
	_itoa_s(this->index_no,tst1,3,10);
	delta_1_lb_str+=(string)tst1;
	delta_1_lb_str+="_lb";
	
	string gamma_1_lb_str;
	gamma_1_lb_str="gamma_";
	char* tst2=new char[3];
	_itoa_s(this->index_no,tst2,3,10);
	gamma_1_lb_str+=(string)tst2;
	gamma_1_lb_str+="_lb";

	string gamma_1_as_str;
	gamma_1_as_str="gamma_";
	char* tst3=new char[3];
	_itoa_s(this->index_no,tst3,3,10);
	gamma_1_as_str+=(string)tst3;
	gamma_1_as_str+="_as";

	string index1_str;
	index1_str="index";
	char* tst4=new char[3];
	_itoa_s(this->index_no,tst4,3,10);
	index1_str+=(string)tst4;

	string av_index1_str;
	av_index1_str="av_index_";
	char* tst5=new char[3];
	_itoa_s(this->index_no,tst5,3,10);
	av_index1_str+=(string)tst5;
	
	
	
	this->init_field(file_input_name,this->period,"Period","period");
	this->init_field(file_input_name,this->liab_ov,"liab_ov","liab_ov");
	this->init_field(file_input_name,this->delta_liab,delta_1_lb_str,delta_1_lb_str);
	this->init_field(file_input_name,this->gamma_liab,gamma_1_lb_str,gamma_1_lb_str);
	this->init_field(file_input_name,this->av_index,av_index1_str,av_index1_str);

	this->init_field(file_input_name,this->Total_charges,"Total_charges","Total_charges");
	this->init_field(file_input_name,this->claims,"claims","claims");

	this->init_row_to_start(rw_scn_file);
	this->init_field(rw_scn_file, this->index,index1_str,index1_str);
	this->init_row_to_start(file_input_name);


	this->index[0]=this->objects_needed.assumptions->index_base;
	
	for (int i=1;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->index[i]=this->index[i-1]*(1+this->index[i]/100);
	}
	for (int i=0;i<this->objects_needed.assumptions->nb_maturities_to_hedge;++i)
	{
		string rho_1_lb_str;
		rho_1_lb_str="rho_";
		char* tst2=new char[3];
		_itoa_s((int)(this->objects_needed.assumptions->maturities_to_hedge[i]),tst2,3,10);
		rho_1_lb_str+=(string)tst2;
		rho_1_lb_str+="_lb";
		this->init_field(file_input_name,this->rho_liab[i],rho_1_lb_str,rho_1_lb_str);
	}
	
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->delta_liab[i]*=(this->av_index[i]/this->index[i]);
		this->gamma_liab[i]*=((this->av_index[i]/this->index[i])*
			(this->av_index[i]/this->index[i]));
	}
 
	//chth
	delete [] tst1;
	delete [] tst2;
	delete [] tst3;
	delete [] tst4;
	delete [] tst5;
delete [] this->av_index;
}



void Hedgeff::init_field(string file_input_name,
						 double * field,
						 string field_name_in_csv,
						 string field_name)
{
	int col_nb=1;
	int cur_col=1;

	fstream file_input_stream(file_input_name.c_str());
	string row;
	getline(file_input_stream,row);
	char * pch;
	char * next_token;
	pch = strtok_s((char*)row.c_str(),",",&next_token);
	
	while(strcmp(pch,field_name_in_csv.c_str())!=0)
	{
		col_nb++;
		pch = strtok_s(NULL, ",",&next_token);
	}
	
	for (int i=0;i<this->row_to_start;++i)
	{	
		getline(file_input_stream,row);
	}
	
	for (int cur_row=0;cur_row<model_period;++cur_row)
	{
		pch = strtok_s((char*)row.c_str(),",",&next_token);
		cur_col=1;
		while(cur_col!=col_nb)
		{
			pch = strtok_s(NULL, ",",&next_token);
			cur_col++;
		}
		field[cur_row]=atof(pch);
		getline(file_input_stream,row);
	}

	file_input_stream.close();

}

void Hedgeff::affiche(double* one_dim_tab,int start,int end,char* name)
{
	for(int i=start;i<=end;++i)
	{
	}
}