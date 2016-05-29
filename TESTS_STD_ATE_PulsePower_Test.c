//==============================================================================
// Include files
#include "Windows.h"
#include <analysis.h>
#include <formatio.h>
#include "toolbox.h"
#include <ansi_c.h>
#include <userint.h>

#include <utility.h>
#include "TESTS_STD_ATE_Tests.h"
#include "common.h"
#include "TESTS_STD_ATE_Global.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions


TSTE_TEST ( STD_PULSE_POWER_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	int								iDB_MeasureChannel								=	0, 
									iDB_NumberOfMeasures							=	0,
									iDB_NumberOfPulseLoops							=	0;
	
	int								iMeasureIndex									=	0,
									iPulseLoopIndex									=	0,   
									iCurrentPathIndex								=	0;  
										
	char							*pszPowerMeterStateFileName						=	NULL;
	
	double							lfDB_MeasureTimeout								=	0.0,
									lfDB_LossFrequnecy							   	=	0.0; 
										
	int								iDB_FrequencyIndex								=	0,
									iDB_NumberOfFrequnecies							=	0;
	
	double							*pvlfOffsets									=	NULL,
									*pvlfDB_LossFrequency							=	NULL; 
									
	char							szFormatedString[512]							=	{0},
									szFormatedVariable[512]							=	{0}; 
									
	tsMeasureItem					tMeasurePower									=	{0.0};

	double							lfLossFactor									=	0, 
									lfCurrentFrequency								=	0; 
									
	int								hPowerMeter										=	0;
									
	char							*pCurrentTestPrefix								=	NULL;

	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;
	
	int								bDB_FrequencySecured							=	0,
									bDB_UseTestFrequencySetting						=	0, 
									bDB_UseInternalFrequencyLoop					=	0;
	
	double							*pvlfDB_Frequnecies								=	NULL;
	
	int								bDB_EnableToStoreMeasureData					=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Pulse Power Test."); 

	GET_TEST_PREFIX(pCurrentTestPrefix);  
	
	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	GET_PM_EQUIPMENT_HANDLE(hPowerMeter);	
	
	iDB_MeasureChannel = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Measure_Channel" , hPowerMeter );
	
	iDB_NumberOfMeasures = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Measures" ); 
	
	lfDB_MeasureTimeout = GetVarDouble( clb, VARTYPE_TEST , "Measure_Timeout" ); 
	
	iDB_NumberOfFrequnecies = GetVarDoubleArray( clb, VARTYPE_TEST , "Loss_Frequnecy" , &pvlfDB_LossFrequency);
	
	GET_TEST_PREFIX(pCurrentTestPrefix); 
	
	PATH_GET_CURRENT_INDEX( iCurrentPathIndex );
	
	if ( bDB_UseInternalFrequencyLoop )
		iDB_FrequencyIndex = iCurrentPathIndex;
		
	GET_SEGMENT_LOOP_NUM(iCurrentTemperatureSegmentLoop);	
	GET_SEGMENT_NUMBER(iCurrentTemperatureSegment);

	if ( bDB_UseTestFrequencySetting )
	{
		iDB_NumberOfFrequnecies = GetVarDoubleArray( clb, VARTYPE_TEST , "Frequnecies" , &pvlfDB_Frequnecies);
		
		if ( iDB_FrequencyIndex < iDB_NumberOfFrequnecies )
			lfCurrentFrequency = pvlfDB_Frequnecies[iDB_FrequencyIndex];
		else
			lfCurrentFrequency = pvlfDB_Frequnecies[0];
	}
		
	PATH_GET_LOSS_VALUE( lfDB_LossFrequnecy , lfLossFactor ); 
	
	sprintf( szFormatedString , "Loss Factor = %0.5lf dBm" , lfLossFactor );
	LOG(CLB_LINE_NORMAL, szFormatedString ); 
				
	pszPowerMeterStateFileName = GetVarString( clb, VARTYPE_EQUIPMENT_USE , "State_FileName" , hPowerMeter );  

	sprintf( szFormatedString , "Loading power meter state (%s)" , pszPowerMeterStateFileName );   //pulse_param.scp
	LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
	CHK_STDERR( DRV_PowerMeter_RecallState_ByName( hPowerMeter , pszPowerMeterStateFileName ));
		
	iDB_NumberOfPulseLoops = GetVarDoubleArray( clb, VARTYPE_EQUIPMENT_USE , "Pulse_Offset_List" , &pvlfOffsets , hPowerMeter ); 
		
	if ( bDB_FrequencySecured == 0 )
		sprintf( szFormatedString , "Power Meter :: Set Frequency Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
	else
		sprintf( szFormatedString , "Power Meter :: Set Frequency to %0.3lf MHz" , ( lfCurrentFrequency / 1E6 ));  
	
	LOG(CLB_LINE_NORMAL, szFormatedString ); 
			
	CHK_STDERR( DRV_PowerMeter_SetFrequency ( hPowerMeter , iDB_MeasureChannel , lfCurrentFrequency ));  

	for ( iPulseLoopIndex = 0; iPulseLoopIndex < iDB_NumberOfPulseLoops; iPulseLoopIndex++ )
	{ 
		sprintf( szFormatedString , "Pulse Check, loop - %d" , iPulseLoopIndex+1 );
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
		CHK_STDERR( DRV_PowerMeter_Gating_ConfigOffsetTime ( hPowerMeter , iDB_MeasureChannel , 1 , 0.0 , pvlfOffsets[iPulseLoopIndex] , 0.0 , 0.0 ));               
		
		memset( &tMeasurePower , 0 , sizeof(tsMeasureItem));
	
		for ( iMeasureIndex = 0; iMeasureIndex < iDB_NumberOfMeasures; iMeasureIndex++ )
		{
			sprintf( szFormatedString , "Measure - %d" , iMeasureIndex+1 );
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
			UPDATERR( DRV_PowerMeter_Gating_ReadMaxPower ( hPowerMeter , iDB_MeasureChannel , 1 , lfDB_MeasureTimeout , &(tMeasurePower.lfLastValue) )); 
		
			tMeasurePower.lfLastValue -= lfLossFactor;
				
			if ( IS_NOT_OK )
				continue;
				
			sprintf( szFormatedString , "Measured Power = %0.5lf dBm" , tMeasurePower.lfLastValue );
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
				
			AddCalculateMasureItem(&tMeasurePower);
		}

		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Average" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasurePower.lfAverage ); 
		}
		
		RESULT( CLB_RESULT_DOUBLE , "" , tMeasurePower.lfAverage , "Average Power" );
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Maximum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasurePower.lfMaxValue );
		}
		
		RESULT( CLB_RESULT_DOUBLE , "" , tMeasurePower.lfMaxValue , "Maximum Power" );
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Minimum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasurePower.lfMinValue );
		}
		
		RESULT( CLB_RESULT_DOUBLE , "" , tMeasurePower.lfMinValue , "Minimum Power" );
	}																 
		

CATCH

	UPDATE_ERROR(StdError);
	
	if ( StdError.pszErrorDescription )
	{
		LOG_END(CLB_LINE_WARNING, (char*)StdError.pszErrorDescription );
	}
	
	iTestError = -1;
	
FINALLY
	
	LOG_END(CLB_LINE_NORMAL, "Finalizing Test." );

	FREE(pszPowerMeterStateFileName); 
	FREE(pvlfDB_LossFrequency);
	FREE(pvlfOffsets);
	
	return iTestError;
}

