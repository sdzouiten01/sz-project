#pragma once
#include <string>
#include "stdafx.h"
#include "Utility classes/Summary_configuration.h"


using namespace std;


class ITM_for_LS
{
	private:
		int t;
		int t0;
		double itm;
		string guarantee_type;
		string* ITM_choice;
		double av;
		double ab;
		double wb;
		double ib;
		double sb;
		double vestedb;
		double pwrx;
		double pwrt;
		double ten_years_rate;
		double r;
		double beta;
		double deferral;
		double * mortality;
		double AB_percentage;
		double rollup_rate;
		double regular_premium;
		double ib_bis;
		double ac_ag;
		double ac_ag_bis;

	public:
		ITM_for_LS (int time, string pCK, int maturity, string ITM_choice_1, string ITM_choice_2,
						string ITM_choice_3, double av, double ab, double sb, double wb,
						double ib, double ib_bis, double ac_ag, double ac_ag_bis,
						double r, double * mortality, double def_period, double AB_percentage,
						double rollup_rate, double regular_premium){
		this->t=time;
		this->t0=maturity;
		this->av=av;
		this->ab=ab;
		this->sb=sb;
		this->wb=wb;
		this->ib=ib;
		this->r=r;
		ITM_choice = new string [3];
		ITM_choice[0] = ITM_choice_1;
		ITM_choice[1] = ITM_choice_2;
		ITM_choice[2] = ITM_choice_3;
		this->mortality = mortality;
		this->guarantee_type_determination(pCK);
		this->deferral = def_period;
		this->AB_percentage = AB_percentage;
		this->rollup_rate = rollup_rate;
		this->regular_premium = regular_premium;
		this->ib_bis = ib_bis;
		this->ac_ag = ac_ag;
		this->ac_ag_bis = ac_ag_bis;
		}
		
		//Constructor for Australia Products
		ITM_for_LS (int time, string pCK, int maturity, string ITM_choice_1, string ITM_choice_2,
						string ITM_choice_3, double av, double ab, double wb,
						double vestedb, double r, double * mortality, 
						double pwrt, double pwrx,double ten_years_rate){
		this->t=time;
		this->t0=maturity;
		this->av=av;
		this->ab=ab;
		this->wb=wb;
		this->vestedb=vestedb;
		this->r=r;
		ITM_choice = new string [3];
		ITM_choice[0] = ITM_choice_1;
		ITM_choice[1] = ITM_choice_2;
		ITM_choice[2] = ITM_choice_3;
		this->mortality = mortality;
		this->guarantee_type_determination(pCK);

		this->pwrt = pwrt;
		this->pwrx = pwrx;
		this->ten_years_rate = ten_years_rate;

		}

		~ITM_for_LS(){
			delete[] ITM_choice;
		}


		void guarantee_type_determination(string pCK){

			if (pCK ==("DEU_IDB01"))
			{
				guarantee_type = "IB";
			}
			else if (pCK ==("DEU_IDB02"))
			{
				guarantee_type = "IB";
			}
			else if (pCK == ("DEU_IAB01"))
			{	
				guarantee_type = "IB";
			}
			else if (pCK == ("DEU_IAB02"))
			{	
				guarantee_type = "IB";
			}
			else if (pCK == ("DEU_IDB02_M"))
			{	
				guarantee_type = "IB";
			}
			else if (pCK == ("DEU_IAB03"))
			{	
				guarantee_type = "IB";
			}
			else if (pCK == ("DEU_IDB03"))
			{	
				guarantee_type = "IB";
			}
			else if ( (pCK == ("ITA_ADB01")) || (pCK == ("ESP_ADB01")) || (pCK == ("PRT_ADB01")) )
			{
					guarantee_type = "AB";
			}
			else if ( (pCK == ("ITA_WDB01")) || (pCK == ("ESP_WDB01")) || (pCK == ("PRT_WDB01")) )
			{
					guarantee_type = "WB";
			}
			else if (pCK == ("CHE_ADB01"))
			{	
					guarantee_type = "AB";
			}
			else if (pCK == ("CHE_WB02"))
			{	
					guarantee_type = "WB";
			}
			else if (pCK == ("CHE_WB03"))
			{	
					guarantee_type = "WB";
			}
			else if (pCK == ("MPF_ADS01"))
			{	
					guarantee_type = "SB";
			}
			else if (pCK == ("BEL_WB01"))
			{	
					guarantee_type = "WB";
			}
			else if (pCK == ("FRA_ALWB01"))
			{	
					guarantee_type = "WB";
			}
			else if (pCK == ("FRC_WB01"))
			{	
					guarantee_type = "WB";
			}
			else if (pCK == ("JPN_WDB01_QVC"))
			{	
					guarantee_type = "WB";
			}
			else if (pCK == ("JPN_WDB02_QVC"))
			{	
					guarantee_type = "WB";
			}
			else if ( (pCK == ("JPA_WDB03")) )
			{
					guarantee_type = "WB";
			}
			//Jerome - LSM - Add for JPN_ADB03 (GMAB)
			else if ( (pCK == ("JPN_ADB03")) )
			{
					guarantee_type = "AB";
			}
			else if ( (pCK == ("AUS_AB01")) || (pCK == ("AUS_AB01N")) || (pCK == ("AUS_AB02")) )
			{
					guarantee_type = "AB";
			}
			//Jerome - LSM - Add for PI Series 2
			else if ( (pCK == ("AUS_PI01")) || (pCK == ("AUS_PI01N")) || (pCK == ("AUS_PI02")) )
			{
					guarantee_type = "AB";
			}
			//Jerome - LSM - Products 'R' deleted
			/*else if (  (pCK == ("AUS_AB01R")) || (pCK == ("AUS_AB01NR")) || (pCK == ("AUS_AB02R")) )
			{
					guarantee_type = "AB";
			}*/
			//Jerome  - LSM - Special guarantee for Australia PG Series 1 and Series 2
			else if (pCK == ("AUS_SWB01") || pCK == ("AUS_SWB01N")  || pCK == ("AUS_SWB02")  )
			{
				guarantee_type = "AUS_PG";
			}
			//Jerome  - LSM - Special guarantee for Australia PG Series 1 and Series 2
			else if (pCK == ("AUS_PG01") || pCK == ("AUS_PG01N")  || pCK == ("AUS_PG02")  )
			{
				guarantee_type = "AUS_PG";
			}
			//Jerome  - LSM - Special guarantee for Australia WB
			else if (pCK == ("AUS_WB01") )
			{
				guarantee_type = "AUS_WB";
			}
			else if (pCK == ("HKG_ADB01"))
			{	
					guarantee_type = "AB";
			}
			else if (pCK == ("CHN_AB01"))
			{	
					guarantee_type = "AB";
			}
			else if (pCK == ("AGD_ADB01"))
			{
				guarantee_type = "AB";
			}
			else if (pCK == ("AGD_WDB01"))
			{
				guarantee_type = "WB";
			}
			else if (pCK == ("AGD_UK_WDB01"))
			{
				guarantee_type = "WB";
			}
			else if (pCK == ("AGD_UK_ADB01"))
			{
				guarantee_type = "AB";
			}
			else if (pCK == ("AGD_ESP_WDB01"))
			{
				guarantee_type = "WB";
			}
			else if (pCK == ("AGD_ESP_ADB01"))
			{
				guarantee_type = "AB";
			}
			else 
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "ITM_for_LS_boundary : The prod code key %s is not found", pCK.c_str());
				throw new GMXBException(message);

			}
		}

		double itm_calculation(int nb_ITM){
			if (ITM_choice[nb_ITM-1] == "AV"){
				itm = this->av;
				return itm;
			}
			if (ITM_choice[nb_ITM-1] == "Rate"){
				itm = this->r;
				return itm;
			}
			if (guarantee_type=="AB"){
				if (ITM_choice[nb_ITM-1] == "Base"){
					itm = this->ab;
				}
				else{
					if (this->ab < epsilon){itm = 0;}
					else{
						if (ITM_choice[nb_ITM-1] == "ITM 1"){
							itm = (av/ab)*exp((t0-1-t)*log(1+r));
						}
						else{
							itm = this->av / this->ab;
						}
					}
				}
			}
			
			//Jerome - LSM - Add for Australia
			if (guarantee_type=="AUS_PG"){
				if (ITM_choice[nb_ITM-1] == "Protected Base"){
					itm = this->wb;
				}
				else if (ITM_choice[nb_ITM-1] == "Vested Base"){
					itm = this->vestedb;
				}
				else if (ITM_choice[nb_ITM-1] == "ITM 1" || ITM_choice[nb_ITM-1] == "ITM 3"){
					if (this->wb < epsilon){itm = 0;}
					else{
						if (ITM_choice[nb_ITM-1] == "ITM 1"){
							itm = this->av / this->wb;
						}
						else{
							itm = (av/wb)*exp((t0-1-t)*log(1+r));
						}

					}
				}
				else{
					if (this->vestedb < epsilon){itm = 0;}
					else{
						if (ITM_choice[nb_ITM-1] == "ITM 2"){
							itm = this->av / this->vestedb;
						}
						else{
							itm = (av/this->vestedb)*exp((t0-1-t)*log(1+r));
						}
					}
				}
			}

			else if (guarantee_type=="AUS_WB"){
				if (ITM_choice[nb_ITM-1] == "Base"){
					itm = this->wb;
				}
				else if (ITM_choice[nb_ITM-1] == "Rate 10Y"){
					itm = this->ten_years_rate;
				}
				else{
					if (this->wb < epsilon){itm = 0;}
					else if (ITM_choice[nb_ITM-1] == "ITM 1"){
						itm = this->av / this->wb;
					}
					else if (ITM_choice[nb_ITM-1] == "ITM 2"){
						itm = this->av * this->r / this->wb;
					}
					else if (ITM_choice[nb_ITM-1] == "ITM 3") {
						double withdrawal_cumul = 0;
						for (int i = this->t + 1; i<this->t0; i++){   
							if (i > this->deferral){
								withdrawal_cumul += exp(-(i-this->t) * log(1 + this->r)) * this->mortality[i] / this->mortality[this->t];
							}
						}
						if (withdrawal_cumul < epsilon){
							itm = 0;
						}
						else{
							itm = this->av / (withdrawal_cumul * this->wb);
						}
					}
					else {
						if (ten_years_rate<pwrt+0.012)
						{
							if (this->av< epsilon){itm = 0;}
							itm = pwrt - pwrx * max(wb/av,1);
						}
						else
						{
							if (this->av< epsilon){itm = 0;}
							itm = ten_years_rate - pwrx * max(wb/av,1) -0.012;
						}
					}
				}
			}



			else if (guarantee_type=="IB"){
				//double base = (1 - AB_percentage) * ib + AB_percentage * max(ab,ib);
				if (ITM_choice[nb_ITM-1] == "Base"){
					itm = (1 - AB_percentage) * max(ib * ac_ag, ib_bis * ac_ag_bis) + AB_percentage * max(max(ab,ib * ac_ag), ib_bis * ac_ag_bis);
				}
				else{
					if (this->ib < epsilon){itm = 0;}
					else{
						if (ITM_choice[nb_ITM-1] == "ITM 1"){
							double av_loc = av * exp((t0 -1 - t) * log(1 + r));
							double ib_loc = ib * exp((t0 -1 - t) * log(1 + rollup_rate));
							double ib_bis_loc = ib_bis * exp((t0 -1 - t) * log(1 + r)); // Should have no growth if AB_IB_1 
							double ab_loc = ab;
							if(regular_premium > epsilon){
								for (int i=t+1; i<t0; i++){   
									av_loc += regular_premium * exp((t0 -1 - i) * log(1 + r));
									ib_loc += regular_premium * exp((t0 -1 - i) * log(1 + rollup_rate));
									ib_bis_loc += regular_premium * exp((t0 -1 - i) * log(1 + r)); // Should have no growth if AB_IB_1
									ab_loc += regular_premium;
								}
							}
							double base_loc = (1 - AB_percentage) * max(ib_loc * ac_ag, ib_bis_loc * ac_ag_bis) + AB_percentage * max(ab_loc, max(ib_loc * ac_ag, ib_bis_loc * ac_ag_bis));
							itm = (av_loc / base_loc);
						}
						//itm = (av / base) * exp((t0 -1 - t) * log(1+r));
						else{
							double av_loc = av + regular_premium * (t0 - t - 1);
							double ib_loc = ib + regular_premium * (t0 - t - 1);
							double ib_bis_loc = ib_bis + regular_premium * (t0 - t - 1);
							double ab_loc = ab + regular_premium * (t0 - t - 1);
							double base_loc = (1 - AB_percentage) * max(ib_loc * ac_ag, ib_bis_loc * ac_ag_bis) + AB_percentage * max(ab_loc, max(ib_loc * ac_ag, ib_bis_loc * ac_ag_bis));
							itm = (av_loc / base_loc);
							
							//itm = this->av / base;
						}
					}
				}
			}
			else if (guarantee_type=="WB"){
				if (ITM_choice[nb_ITM-1] == "Base"){
					itm = this->wb;
				}
				else{
					if (this->wb < epsilon){itm = 0;}
					else if (ITM_choice[nb_ITM-1] == "ITM 1"){
						itm = this->av * this->r / this->wb;
					}
					else {
						double withdrawal_cumul = 0;
						for (int i = this->t + 1; i<this->t0; i++){   
							if (i > this->deferral){
								withdrawal_cumul += exp(-(i-this->t) * log(1 + this->r)) * this->mortality[i] / this->mortality[this->t];
							}
						}
						if (withdrawal_cumul < epsilon){
							itm = 0;
						}
						else{
							itm = this->av / (withdrawal_cumul * this->wb);
						}
					}
				}
			}
			else if (guarantee_type=="SB"){
				if (ITM_choice[nb_ITM-1] == "Base"){
					itm = this->sb;
				}
				else{
					if (ITM_choice[nb_ITM-1] == "ITM 1"){
						itm = 2;
					}
					else {
						itm = 1;
					}
				}
			}
			
			return itm;
		}
		

		


};
