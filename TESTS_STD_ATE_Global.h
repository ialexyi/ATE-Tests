#ifndef __TESTS_STD_ATE_Global_H__
#define __TESTS_STD_ATE_Global_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants

//==============================================================================
// Types
typedef struct
{
	double							lfSetup_OverVoltage,
									lfSetup_Voltage,
									lfSetup_Current,
									lfSetup_MaxCurrent,
									lfSetup_Frequency,
									lfSetup_Phase;
									
	double							lfMeasure_Current,
									lfMeasure_Voltage,
									lfMeasure_Power, 
									lfMeasure_MaxValue,
									lfMeasure_MinValue;

	int								iNumberOfMeasures;
	
	int								hPowerSupplyHandle;
	
	int								iSetup_Range;
	
	int								iChannelNumber;
	
	int								bIsItOutputStar,
									bIsItAcPowerSupply;
	
	int								iAcPowerMeasure_Watt_VA;
	
	
} tsPowerSupplyItem;

typedef	struct
{
	double		lfAverageCalc,
				lfAverage,
				lfMaxValue,
				lfMinValue,
				lfLastValue;
	
	int			iMeasureCount;
	
} tsMeasureItem;

//==============================================================================
// External variables

//==============================================================================
// Global functions

void AddCalculateMasureItem( tsMeasureItem *ptMeasureItem );

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __TESTS_STD_ATE_Global_H__ */
