#include "Delta_Puts.h"

Delta_Puts::Delta_Puts(Assumptions* assumptions,
		Und_Base* und_base,
		Underlying* underlying,
		Tte* tte,
		Rf* rf)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.und_base=und_base;
	this->objects_needed.underlying=underlying;
	this->objects_needed.tte=tte;
	this->objects_needed.rf=rf;

	this->init_tab();
}
void Delta_Puts::init_tab()
{
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			//if(i==92 && j==76)
			//{
				//ofstream tt("DEBUG_DELTA_PUTS.txt");
				//tt<<this->objects_needed.underlying->tab[i][j]<<endl;
				//tt<<this->objects_needed.und_base->tab[i][j]<<endl;
				//tt<<this->objects_needed.tte->tab[i][j]<<endl;
				//tt<<this->objects_needed.rf->tab[i][j]<<endl;
				//tt<<0<<endl;
				//tt<<this->objects_needed.assumptions->volatility<<endl;
				//double lala=Math_Tools::d0(this->objects_needed.underlying->tab[i][j],this->objects_needed.underlying->tab[i][j]*exp(this->objects_needed.rf->tab[i][j]*this->objects_needed.tte->tab[i][j]),this->objects_needed.und_base->tab[i][j],this->objects_needed.assumptions->volatility);
				//tt<<lala<<endl;
				//tt<<Math_Tools::N(lala)<<endl;
				//tt<<Math_Tools::delta_BS(this->objects_needed.underlying->tab[i][j],this->objects_needed.und_base->tab[i][j],this->objects_needed.tte->tab[i][j],this->objects_needed.rf->tab[i][j],0,this->objects_needed.assumptions->volatility);
				//tt.close();

			//}
			if(i>=this->objects_needed.assumptions->length_of_option_contract
				+j-this->objects_needed.assumptions->sell_at_time_to_expiry||
				i<j)
				this->tab[i][j]=0;
			else
			this->tab[i][j]=Math_Tools::delta_BS(
				this->objects_needed.underlying->tab[i][j],
				this->objects_needed.und_base->tab[i][j],
				this->objects_needed.tte->tab[i][j],
				this->objects_needed.rf->tab[i][j],
				0,
				this->objects_needed.assumptions->volatility);
		}
	}
}
Delta_Puts::~Delta_Puts(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
}
