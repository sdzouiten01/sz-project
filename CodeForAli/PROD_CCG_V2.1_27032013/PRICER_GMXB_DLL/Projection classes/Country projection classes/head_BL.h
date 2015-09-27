#pragma once

///////////////////////////////////////////////// Begining const vector table
#define	YEAR_BL 0
#define	MONTH_BL 1
#define	DURATION_YEAR_BL 2
#define	DURATION_MONTH_BL 3
#define	AGE_YEAR_BL 4
#define	AGE_MONTH_BL 5
#define	DATE_YEAR_BL 6
#define	DATE_MONTH_BL 7


#define	IND_CONTRACT_PERIOD_BL 8
#define	IND_DEFERAL_PERIOD_BL 9
#define	IND_RATCHET_MONTH_BL 10
#define	IND_COUPON_DATE_BL 11
#define	IND_BONUS_DATE_BL 12
#define	IND_BONUS_VALUATION_DATE_BL 13
#define	GROSS_PREMIUM_BL 14

#define	DEATH_R_1_BL 15
#define	DEATH_R_2_BL 16
#define	DEATH_R_BL 17
#define	LAPSE_RATE_BL 18
#define	NUMBER_COUPON_PAID 19
#define	SRVIVAL_WITHOUT_LAPSE 20

#define TOTAL_CONSTVECTOR_COLS_BL 21 //Number of const vector columns

/////////////////////////////////////////////// End const vector table


///////////////////////////////////////////////// Begining MainProjArrayVector 
#define FUND_RETURN_BL	0
#define DISCOUNT_FACTOR_BL 1

#define	MAIN_PROJ_NUM_COLS_BL 	2  //Number of MainProjArrayVector columns

///////////////////////////////////////////////// end MainProjArrayVector 
///////////////////////////////////////////////// Begining debug proj array 
#define INITIAL_BASIS_CAPITAL_BL	0
#define GUAR_GMDB_BL	1
#define COUPON_VALUE_BL	2
#define BONUS_VALUE_BL	3
#define HEDGE_COST_FEES_BL	4
#define ACCOUNT_VALUE_END_BL	5
#define CLAIMS_WB_BL	6
#define CLAIMS_DB_BL	7
#define SURVIVAL_INDEX_BL 8
#define BASIS_KAPITAL_BL 9
#define ACCOUNT_VALUE_BEF_COUPON_BL 10
#define ADD_COUPON_RATCHET_BL 11
#define SURVIVAL_INDEX_STEP_BL 12
#define PERFORMANCE_BL 13
#define DYNAMIC_LAPSE 14

#define	DEBUG_NUM_COLS_BL 	15 //Number of debug projection columns

///////////////////////////////////////////////// Begining present value
#define BEGINING_ACCOUNT_VALUE_BL 0
#define PV_RRC_RATE_TOT_BL 1
#define PV_GUARANTEE_FEES_RATE_BL 2
#define PV_FIXED_FEES_RATE_BL 3
#define PV_ONGOING_COMM_RATE_BL 4
#define PV_TAIL_COMM_RATE_BL 5
#define PV_EHC_INTERNAL_RATE_BL 6
#define PV_EHC_NEW_RE_RATE_BL 7
#define PV_CLAIMS_GMWB_BL 8
#define PV_CLAIMS_GMDB_BL 9
#define PV_CLAIMS_GMAB_BL 10
#define PV_CLAIMS_GMIB_BL 11
#define PV_CLAIMS_TOT_BL 12
#define PV_AV_TOT_BL 13
#define PV_AV_NEW_RE_BL 14
#define PV_RRC_TOT_CHARGES_BL 15
#define PV_GUARANTEE_FEES_CHARGES_BL 16
#define PV_FIXED_FEES_CHARGES_BL 17
#define PV_ONGOING_COMM_CHARGES_BL 18
#define PV_TAIL_COMM_CHARGES_BL 19
#define PV_FUND_FEES_CHARGES_BL 20
#define PV_RETRO_FUND_CHARGES_BL 21
#define PV_EXPENSES_CHARGES_BL 22
#define PV_REAL_CHARGES_BL 23
#define PV_EHC_INTERNAL_CHARGES_BL 24
#define PV_EHC_NEW_RE_CHARGES_BL 25
#define PV_LAPSE_CHARGES_BL 26
#define PV_LOV_INTERNAL_BL 27
#define PV_LOV_NEW_RE_BL 28
#define PV_ITM_BL 29

#define TOTAL_PV_OUTPUT_COLUMNS_BL 30
///////////////////////////////////////////////// end present value 
