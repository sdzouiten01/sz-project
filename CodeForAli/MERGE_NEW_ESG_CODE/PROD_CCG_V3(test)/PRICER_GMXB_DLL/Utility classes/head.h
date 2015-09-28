#pragma once



#define t_high 60  // must match with SUBACCT_MAX_PERIOD	
#define SUBACCT_MAX_PERIOD 61	//Must match t_high

#define MAXIMUM_NUMBER_OF_INDEXES	12		//Total number of funds (Fixed and separate accounts)
//#define MAXIMUM_NUMBER_OF_SHORTSELLABLE_FUNDS	70
#define	SUBACCT_NUM_COLS 	25		//Number of columns 
//#define	MAIN_PROJ_NUM_COLS 	92  //Number of main projection columns
#define	DECALAGE_MAX 	12		//Number of months between eval date and the following policy anniversary		
#define CALC_TOT_INDEX 	0			//Array index used to hold the not calculated indicator (if not calculated yet) or colunm total (if values have been calculated)
#define SMALL_DOUBLE 0.0000000003




#define TOTAL_CONSTVECTOR_COLS 25 //Number of const vector columns
#define	DEBUG_NUM_COLS 	25 //Number of debug projection columns

///////////////////////////////////////////////// Begining MainProjArrayVector 
#define FUND_RETURN	0
#define DISCOUNT_FACTOR 1

#define	MAIN_PROJ_NUM_COLS 	2  //Number of MainProjArrayVector columns
///////////////////////////////////////////////// end MainProjArrayVector 
///////////////////////////////////////////////// Begining present value
#define BEGINING_ACCOUNT_VALUE 0
#define PV_RRC_RATE_TOT 1
#define PV_GUARANTEE_FEES_RATE 2
#define PV_FIXED_FEES_RATE 3
#define PV_ONGOING_COMM_RATE 4
#define PV_TAIL_COMM_RATE 5
#define PV_EHC_INTERNAL_RATE 6
#define PV_EHC_NEW_RE_RATE 7
#define PV_CLAIMS_GMWB 8
#define PV_CLAIMS_GMDB 9
#define PV_CLAIMS_GMAB 10
#define PV_CLAIMS_GMIB 11
#define PV_CLAIMS_TOT 12
#define PV_AV_TOT 13
#define PV_AV_NEW_RE 14
#define PV_RRC_TOT_CHARGES 15
#define PV_GUARANTEE_FEES_CHARGES 16
#define PV_FIXED_FEES_CHARGES 17
#define PV_ONGOING_COMM_CHARGES 18
#define PV_TAIL_COMM_CHARGES 19
#define PV_FUND_FEES_CHARGES 20
#define PV_RETRO_FUND_CHARGES 21
#define PV_EXPENSES_CHARGES 22
#define PV_REAL_CHARGES 23
#define PV_EHC_INTERNAL_CHARGES 24
#define PV_EHC_NEW_RE_CHARGES 25
#define PV_LAPSE_CHARGES 26
#define PV_LOV_INTERNAL 27
#define PV_LOV_NEW_RE 28
#define PV_ITM 29

#define TOTAL_PV_OUTPUT_COLUMNS 30
///////////////////////////////////////////////// end present value 

// End PVs constants


// Scenario constants
#define PERIOD 721 
#define DECALAGE_MAX 12
// #define ITERATION_COL_NB 0
#define PERIOD_COL_NB 1
// End of scenario constants

#define MAX_DELTA_SHOCKS_PER_INDEX 20


//#define MAX_LENGTH_MESSAGE_EXCEPTION 1000
#define MAX_LENGTH_FILE_NAME 1000

// maximum period of the projection (in months)
#define t_high_mth 720				// Must match with SUBACCT_MAX_PERIOD_MTH	
#define SUBACCT_MAX_PERIOD_MTH 720	// Must match t_high_mth
// steps of the projection 
#define ANNUAL 1
#define MONTHLY 12
#define WEEKLY 52


#define DLL_VERSION "V14.1"
