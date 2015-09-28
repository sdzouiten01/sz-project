#pragma once

// Begining main array and subbaccount definition
#define	ADMIN_FEE	0   
#define	ANN_CLAIMS_AV	1
#define	AV_B	2
#define	AV_B_BEF	3
#define	AV_E	4
#define	AV_E_BEF	5
#define	AV_GROWTH	6
//#define	AV_GROWTH_AVE	7
#define	AV_GROWTH_RATE	8
#define	AV_REBAL	9
#define	BOND_RATE_FOR_LAPSES	10
#define	DISCOUNT_FACTOR	11
#define	DEATH_CLAIMS_AV	12
#define	GMXB_CHARGE	13
#define AV_E_BEF_CLAIMS 14
#define	M_AND_E_FEE	15
#define	MAT_CLAIMS_AV	16
#define	PREMIUM_ALLOC	17
#define	PREMIUM_BONUS	18
#define	SURR_CLAIMS_AV	19
#define	SURR_FPW	20
#define	GMWB_ANN_PAID	21
#define	PD_UP_SURR_CHG	22
#define AV_B_BEF_BIS 23
#define GMWB_PAYMENT_FROM_AV 24
// End of subbaccount definition

//Beginning of main array
#define	ANN_CLAIMS_GMIB	25
#define	ANN_RATE	26
#define	ANN_RATE_BASE	27
#define	GMDB_CLAIMS	28
#define	DEATH_RATE	29
#define	FPW_RATE_AV	30
#define	GMAB_IF	31
#define	GMAB_CLAIMS	32
#define	GMAB_NAR	33
#define	GMDB_AV_IF	34
#define	GMDB_AV_IF_BEF	35
#define	GMDB_IF	36
#define	GMDB_IF_BEF	37
#define	GMDB_PAYMENT	38
#define	GMDB_NAR_BEF	39
#define	GMDB_PREM_IF	40
#define	GMDB_PREM_IF_BEF	41
#define	GMDB_ROLLUP_CAP_IF	42
#define	GMDB_ROLLUP_CAP_IF_BEF	43
#define	GMIB_AX_CURR	44
//#define	GMIB_AX_GUAR	45
#define	GMIB_CURR_DEATH_RATE	46
#define	GMIB_GUAR_DEATH_RATE	47
#define	GMIB_IF	48
#define	GMIB_IF_BEF	49
#define	GMIB_NAR	50
#define	GMIB_NAR_BEF	51
#define	GMIB_RATCHET_IF	52
#define	GMIB_RATCHET_IF_BEF	53
#define	GMIB_ROLLUP_CAP_IF	54
#define	GMIB_ROLLUP_CAP_IF_BEF	55
#define	GMIB_ROLLUP_IF	56
#define	GMIB_ROLLUP_IF_BEF	57
#define	LAPSE_RATE	58
#define	SURV_CUM	59
#define	PREMIUM	60
#define	PREMIUM_ACCUM	61
#define	PREMIUM_ACCUM_BEF	62
#define	PREMIUM_ANN_IF_B	63
#define	PREMIUM_ANN_IF_E	64
#define	SURV_MTH	65
#define	GMWB_CLAIMS	66
#define	GMWB_AV_IF	67
#define	GMWB_AV_IF_BEF	68
#define	GMWB_IF	69
#define	GMWB_IF_BEF	70
#define	GMWB_PREM_IF	71
#define	GMWB_PREM_IF_BEF	72
#define	GMWB_ROLLUP_CAP_IF	73
#define	GMWB_ROLLUP_CAP_IF_BEF	74
#define	GMWB_MAX_WDL_AMT	75
#define GMWB_PAYMENT 76 
#define	ONG_PREM_SURR_CHG	77
#define	HC_PREM	78
#define	GMIB_DAX_GUAR	79
#define	GMSB_PREM_IF	80
#define	GMSB_GUAR_PAYMENT	81
#define	GMSB_CLAIMS	82
#define GMAB_RATCHET_BASE 83
#define GMAB_ROLLUP_BASE 84
#define SURV_INDEX 85
#define GMWB_RATCHET_PREV 86
#define GMWB_ROLLUP_PREV 87
#define ITM 88
#define GMSB_IF 89
#define  GMIB_CLAIMS 90
#define TECHNICAL_RATE 91
// End main array constants

#define t_high 60  // must match with SUBACCT_MAX_PERIOD	
#define SUBACCT_MAX_PERIOD 61	//Must match t_high

#define MAXIMUM_NUMBER_OF_INDEXES	12		//Total number of funds (Fixed and separate accounts)
//#define MAXIMUM_NUMBER_OF_SHORTSELLABLE_FUNDS	70
#define	SUBACCT_NUM_COLS 	25		//Number of columns 
#define	MAIN_PROJ_NUM_COLS 	92  //Number of main projection columns
#define	DECALAGE_MAX 	12		//Number of months between eval date and the following policy anniversary		
#define CALC_TOT_INDEX 	0			//Array index used to hold the not calculated indicator (if not calculated yet) or colunm total (if values have been calculated)
#define SMALL_DOUBLE 0.0000000003

// Begining PVs constants
#define BEGINING_ACCOUNT_VALUE 0
#define PV_GMDB 1
#define PV_GMWB 2
#define PV_GMAB 3
#define PV_GMIB 4
#define PV_GMSB 5
#define PV_GMXB_REAL_CHG 6
#define PV_GMXB_EHC_CHG 7
#define DISCOUNT_FACTOR_VALUE 8
#define PV_DEP_FEE 9
#define TOTAL_OUTPUT_COLUMNS 10
// End PVs constants

// Begining timing items constants
#define POL_MONTH 0
#define POL_YEAR 1
#define AGE_LAST 2
#define PRORATA 3
#define TOTAL_TIMING_ITEMS 4
// End timing items constants 


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
// Options de la valorisation
#define EACH_T 1
#define EACH_BEG_YEAR 2
#define EACH_END_YEAR 3
// Nombre de colonnes max

#define DLL_VERSION "V14.1"
