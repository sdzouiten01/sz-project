#ifndef __HEAD_NEW_H_
#define __HEAD_NEW_H_

// Begining main array and subbaccount definition
#define	ADMIN_FEE	0   
#define	ANN_CLAIMS_AV	1
#define	AV_B	2
#define	AV_B_BEF	3
#define	AV_E	4
#define	AV_E_BEF	5
#define	PRORATA	6
#define	AV_GROWTH	7
#define	AV_GROWTH_AVE	8
#define	AV_GROWTH_RATE	9
#define	AV_REBAL	10
#define	BOND_RATE_FOR_LAPSES	11
#define	DISCOUNT_FACTOR	12
#define	DEATH_CLAIMS_AV	13
#define	GMXB_CHARGE	14
#define	GMAB_CHARGE	15
#define	GMDB_CHARGE	16
#define	GMIB_CHARGE	17
#define	GMWB_CHARGE	18
//#define	INV_FEE_REFUND	19// No more need	commented by Tun
//added by Tun
#define AV_E_BEF_CLAIMS 19
#define	M_AND_E_FEE	20
#define	MAT_CLAIMS_AV	21
#define	PREMIUM_ALLOC	22
#define	PREMIUM_BONUS	23
#define	SURR_CLAIMS_AV	24
#define	SURR_FPW	25
#define	GMWB_ANN_PAID	26
#define	PD_UP_SURR_CHG	27
#define	AGE_LAST	28
//added by Yann
#define AV_B_BEF_BIS 29
#define GMWB_PAYMENT_FROM_AV 30
// End of subbaccount definition

//free spaces for new variables in main_proj_array
//from 31 to 70

//Beginning of main array
#define	ANN_CLAIMS_GMIB	71
#define	ANN_RATE	72
#define	ANN_RATE_BASE	73
#define	GMDB_CLAIMS	74
#define	DEATH_RATE	75
#define	FPW_RATE_AV	76
#define	GMAB_IF	77
#define	GMAB_CLAIMS	78
#define	GMAB_NAR	79
#define	GMDB_AV_IF	80
#define	GMDB_AV_IF_BEF	81
#define	GMDB_IF	82
#define	GMDB_IF_BEF	83
#define	GMDB_PAYMENT	84
#define	GMDB_NAR_BEF	85
#define	GMDB_PREM_IF	86
#define	GMDB_PREM_IF_BEF	87
#define	GMDB_ROLLUP_CAP_IF	88
#define	GMDB_ROLLUP_CAP_IF_BEF	89
#define	GMIB_AX_CURR	90
#define	GMIB_AX_GUAR	91
#define	GMIB_CURR_DEATH_RATE	92
#define	GMIB_GUAR_DEATH_RATE	93
#define	GMIB_IF	94
#define	GMIB_IF_BEF	95
#define	GMIB_NAR	96
#define	GMIB_NAR_BEF	97
#define	GMIB_RATCHET_IF	98
#define	GMIB_RATCHET_IF_BEF	99
#define	GMIB_ROLLUP_CAP_IF	100
#define	GMIB_ROLLUP_CAP_IF_BEF	101
#define	GMIB_ROLLUP_IF	102
#define	GMIB_ROLLUP_IF_BEF	103
#define	LAPSE_RATE	104
#define	SURV_CUM	105
#define	POL_MONTH	106
#define	POL_YEAR	107
#define	PREMIUM	108
#define	PREMIUM_ACCUM	109
#define	PREMIUM_ACCUM_BEF	110
#define	PREMIUM_ANN_IF_B	111
#define	PREMIUM_ANN_IF_E	112
#define	SURV_MTH	113
#define	GMWB_CLAIMS	114
#define	GMWB_AV_IF	115
#define	GMWB_AV_IF_BEF	116
#define	GMWB_IF	117
#define	GMWB_IF_BEF	118
#define	GMWB_PREM_IF	119
#define	GMWB_PREM_IF_BEF	120
#define	GMWB_ROLLUP_CAP_IF	121
#define	GMWB_ROLLUP_CAP_IF_BEF	122
#define	GMWB_MAX_WDL_AMT	123
#define GMWB_PAYMENT 124 //added by Yann
#define	ONG_PREM_SURR_CHG	125
#define	HC_PREM	126
#define	GMIB_DAX_GUAR	127
#define	GMSB_PREM_IF	128
#define	GMSB_GUAR_PAYMENT	129
#define	GMSB_CLAIMS	130
//new variables added by Tun
#define GMAB_RATCHET_BASE 131
#define GMAB_ROLLUP_BASE 132
#define SURV_INDEX 133
#define GMWB_RATCHET_PREV 134
#define GMWB_ROLLUP_PREV 135
#define ITM 136
#define GMSB_IF 137
//#define GMAB_ROLLUP_PREV 98 if the change is made

//free spaces for new variables for main_proj_array
//from 138 to infinate



// End main array constants
#define	SUBACCT_NUM_COLS 	31		//Number of columns (modifiable according the free spaces)
#define	MAIN_PROJ_NUM_COLS 	138  //Number of main projection columns(modifiable according the free spaces)





// Begining PVs constants
#define BEGINING_ACCOUNT_VALUE 0
#define PV_GMDB 1
#define PV_GMWB 2
#define PV_GMAB 3
#define PV_GMIB 4
#define PV_GMSB 5
#define PV_GMXB_REAL_CHG 6
#define PV_GMXB_EHC_CHG 7
#define DISCOUNT_FACTOR 8
#define PV_DEP_FEE 9
#define TOTAL_OUTPUT_COLUMNS 10
// End PVs constants



#define	ACC_FIXED	0	//Assumes element 0 is always the fixed account
#define	ACC_SEP		2	//Assume all other element are separate accounts
	
#define SUBACCT_MAX_PERIOD 60	//Must match t_high
#define SUBACCT_NUM_FUNDS	12		//Total number of funds (Fixed and separate accounts)
#define	DECALAGE_MAX 	12		//Number of months between eval date and the following policy anniversary		
#define CALC_TOT_INDEX 	0			//Array index used to hold the not calculated indicator (if not calculated yet) or colunm total (if values have been calculated)
#define	NO_CALC			-99999999	//Not calculated indicator (Assumes -99999999 is not a legitimate value)
//typedef double floating_pt_type;	//Use double precision floating point numbers
#define t_high 60  // must match with SUBACCT_MAX_PERIOD

#endif