#include "Rw_Scn.h"
#include "Assumptions.h"
Rw_Scn::Rw_Scn(Assumptions* assumptions,int model_period,string file_input_name,int scen_no)
{
	this->model_period=model_period;
	this->scen_no=scen_no;
	this->objects_needed.assumptions=assumptions;
	this->init_nb_of_mty(file_input_name);
	this->init_maturities(file_input_name);
	this->init_row_to_start(file_input_name);
	this->init_tab(file_input_name);
	/*test.open("rwscn.txt");
	test<<"nb of mty= "<<this->nb_of_mty<<endl;
	test<<endl;
	for(int i=0;i<this->nb_of_mty;++i)
	{
		test<<this->maturities[i]<<" ";
	}
	test<<endl;
	for(int i=0;i<model_period;++i)
	{
		for(int j=0;j<this->nb_of_mty;++j)
		{
			test<<this->tab[i][j]<<" ";
		}
		test<<endl;
	}
	test<<endl;
	test.close();*/
}
void Rw_Scn::init_nb_of_mty(string file_input_name)
{
	this->nb_of_mty=0;
	fstream file_input_stream(file_input_name.c_str());
	string row;
	getline(file_input_stream,row);
	char * pch;
	char * next_token;
	pch = strtok_s((char*)row.c_str(),",",&next_token);
	pch = strtok_s(NULL, ",",&next_token);
	pch = strtok_s(NULL, ",",&next_token);

//	while(pch!=NULL)
	while(strcmp(pch,"Beta")!=0)
	{
		this->nb_of_mty++;
		pch = strtok_s(NULL, ",",&next_token);
	}
	file_input_stream.close();


}
void Rw_Scn::init_maturities(string file_input_name)
{
	//ofstream tt("DEBUG_RW_SCN.txt");
	this->maturities=new int[this->nb_of_mty];
	//tt<<this->nb_of_mty<<endl;
	fstream file_input_stream(file_input_name.c_str());
	string row;
	getline(file_input_stream,row);
	char * pch;
	char* next_token;
	pch = strtok_s((char*)row.c_str(),",",&next_token);
	pch = strtok_s(NULL, ",",&next_token);
	pch = strtok_s(NULL, ",",&next_token);
	int i=0;
	while (strcmp(pch, "Beta"))
	//while(pch != "Beta")
	{

		// cout << "pch " << pch  << endl;

		this->maturities[i]=atoi(pch);
		//tt<<pch<<endl;
		//tt<<this->maturities[i]<<endl;;
		++i;
		pch = strtok_s(NULL, ",",&next_token);

	}
	file_input_stream.close();

}

void Rw_Scn::init_row_to_start(string file_input_name)
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

void Rw_Scn::init_tab(string file_input_name)
{
	this->tab=new double*[model_period];
	for(int i=0;i<model_period;++i)
	{
		this->tab[i]=new double[this->nb_of_mty];
	}
	fstream file_input_stream(file_input_name.c_str());
	string row;
	getline(file_input_stream,row);
	char * pch;
	char * next_token;
	pch = strtok_s((char*)row.c_str(),",",&next_token);
	int start=(scen_no-1)*model_period+1;
	int end=scen_no*model_period;

	//jump "row_to_start" lines
	for (int i=0;i<this->row_to_start;++i)
	{	
		getline(file_input_stream,row);
	}
	
	
	for (int cur_row=0;cur_row<model_period;++cur_row)
	{
		pch = strtok_s((char*)row.c_str(),",",&next_token);
		pch = strtok_s(NULL, ",",&next_token);
		pch = strtok_s(NULL, ",",&next_token);
		for(int mty=0;mty<this->nb_of_mty;++mty)
		{
			this->tab[cur_row][mty]=atof(pch);
			pch = strtok_s(NULL, ",",&next_token);
		}
		getline(file_input_stream,row);
	}
	file_input_stream.close();

}
Rw_Scn::~Rw_Scn(void)
{

	
	for(int i=0;i<this->model_period;++i)
	{
		delete [] this->tab[i];
	}
	delete [] this->tab;
	//chth



	
}