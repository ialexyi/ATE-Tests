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
#define								OUTPUT_TRACE_SIZE						1024
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


TSTE_TEST ( STD_GAIN_FLATNESS_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	int								iDB_MeasureChannel								=	0, 
									iDB_NumberOfSweeps								=	0, 
									iDB_NumberOfMeasures							=	0;
	
	int								iSweepIndex										=	0,
									iMeasureIndex									=	0,
									iCurrentPathIndex								=	0,  
									iInternalFrequencyIndex							=	0;
									
	char							*pszGetLastState								=	NULL,   
									*pszPowerMeterStateFileName						=	NULL,
									*pszSpectrumAnalyzerStateNumber					=	NULL; 
									
	double							lfDB_InputPower									=	0.0,
									lfDB_MeasurementDelay							=	0.0,  
									lfDB_MeasureTimeout								=	0.0,
									lfDB_SweepTimeout								=	0.0, 
									lfDB_MaximumValue 								=	0.0,
									lfDB_MinimumValue 					   			=	0.0;
									
	int								iDB_NumberOfFrequnecies							=	0,
									iDB_NumberOfFrequnecyOffsets					=	0, 
									iDB_FrequencyIndex								=	0; 
									
	double							*pvlfDB_Frequencies								=	NULL,
									*pvlfDB_FrequencyOffset							=	NULL;
	
	int								*pvDB_FrequencySecuredList						=	NULL;
	
	char							szFormatedString[512]							=	{0},
									szFormatedVariable[512]							=	{0}; 
									
	tsMeasureItem					tMeasurePower									=	{0.0};

	double							lfLossFactor									=	0.0,
									lfInputLossFactor								=	0.0,
									lfMarkerFrequency								=	0.0, 
									lfCurrentFrequency							   	=	0.0, 
									lfResult										=	0.0,
									lfGainResult									=	0.0,
									lfCurrentPower									=	0.0,
									lfFlatnessValue									=	0.0,
									lfReferenceLevel								=	0.0, 
									lfSignalGeneratorLossFactor						=	0.0, 
									lfSpectrumAnalyzerLossFactor					=	0.0;
									
	int								hPowerMeter										=	0,
									hSpectrumAnalyzer								=	0,
									hSignalGenerator								=	0;
	
	char							*pCurrentTestPrefix								=	NULL;

	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;
	
	int								bDB_UseMaxHold									=	0,
									bDB_FrequencySecured							=	0, 
									bDB_UseTestFrequencySetting						=	0,
									bDB_UseInternalFrequencyLoop					=	0;
									
	int								bDB_EnableToStoreMeasureData					=	0;
	
	int								bDB_GainResult									=	0, 
									bDB_GainMinMaxResult  							=	0,
									bDB_AbsoluteResult  							=	0,
									bDB_AbsoluteMinMaxResult						=	0,
									bDB_FlatnessResult  							=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Gain and Flatness Test."); 

	GET_TEST_PREFIX(pCurrentTestPrefix);  

	GET_SG_EQUIPMENT_HANDLE(hSignalGenerator);
	
	GET_SA_EQUIPMENT_HANDLE_IF_EXISTS(hSpectrumAnalyzer);
	
	if ( hSpectrumAnalyzer == 0 )
	{
		GET_PM_EQUIPMENT_HANDLE(hPowerMeter);	
	
		iDB_MeasureChannel = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Measure_Channel" , hPowerMeter );
		
		lfDB_MeasureTimeout = GetVarDouble( clb, VARTYPE_EQUIPMENT_USE , "Measure_Timeout" , hPowerMeter ); 
	}
	
	lfDB_InputPower = GetVarDouble( clb, VARTYPE_TEST , "InputPower" ); 
	
	lfDB_MeasurementDelay = GetVarDouble( clb, VARTYPE_TEST , "MeasurementDelay" );  
	
	iDB_NumberOfMeasures = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Measures" ); 
	
	PATH_GET_CURRENT_INDEX( iCurrentPathIndex );
		
	GET_SEGMENT_LOOP_NUM(iCurrentTemperatureSegmentLoop);	
	GET_SEGMENT_NUMBER(iCurrentTemperatureSegment);
	
	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	bDB_UseTestFrequencySetting = GetVarInt( clb, VARTYPE_TEST , "UseTestFrequencySetting" );   
	
	bDB_UseInternalFrequencyLoop = GetVarInt( clb, VARTYPE_TEST , "UsePathIndexForFrequency" );
	
	bDB_GainResult = GetVarInt( clb, VARTYPE_TEST , "GainResultEnable" );
	bDB_GainMinMaxResult = GetVarInt( clb, VARTYPE_TEST , "GainMinMaxResultEnable" ); 
	bDB_AbsoluteResult = GetVarInt( clb, VARTYPE_TEST , "AbsoluteResultEnable" );
	bDB_AbsoluteMinMaxResult = GetVarInt( clb, VARTYPE_TEST , "AbsoluteMinMaxResultEnable" );
	bDB_FlatnessResult = GetVarInt( clb, VARTYPE_TEST , "FlatnessResultEnable" );
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_MaximumValue" , pCurrentTestPrefix );
		lfDB_MaximumValue = GetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable );
	
		sprintf( szFormatedVariable , "%s_MinimumValue" , pCurrentTestPrefix );
		lfDB_MinimumValue = GetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable );
	}
	
	GET_FREQUENCIES_LOOP_INFO_SINGLE( lfCurrentFrequency , bDB_FrequencySecured , iDB_FrequencyIndex );
	
	if ( bDB_UseTestFrequencySetting )
	{
		iDB_NumberOfFrequnecies = GetVarDoubleArray( clb, VARTYPE_TEST , "Frequnecies" , &pvlfDB_Frequencies);
		
		if ( iDB_FrequencyIndex < iDB_NumberOfFrequnecies )
			lfCurrentFrequency = pvlfDB_Frequencies[iDB_FrequencyIndex];
		else
			lfCurrentFrequency = pvlfDB_Frequencies[0];
	}
	else
	{
		GET_FREQUENCIES_LOOP_INFO_LIST( pvlfDB_Frequencies , pvDB_FrequencySecuredList , iDB_NumberOfFrequnecies );
	}
	
	iDB_NumberOfFrequnecyOffsets = GetVarDoubleArray( clb, VARTYPE_TEST , "Measure_Frequency_Offset" , &pvlfDB_FrequencyOffset );
		
	if ( hPowerMeter )
	{
		LOG(CLB_LINE_NORMAL, "Power Meter :: Reset" );
		
		CHK_STDERR( DRV_PowerMeter_Reset( hPowerMeter )); 
		
		pszPowerMeterStateFileName = GetVarString( clb, VARTYPE_EQUIPMENT_USE , "State_FileName" , hPowerMeter );  

		if ( pszPowerMeterStateFileName )
		{
			sprintf( szFormatedString , "Power Meter :: Loading state (%s)" , pszPowerMeterStateFileName );   //pulse_param.scp
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
			CHK_STDERR( DRV_PowerMeter_RecallState_ByName( hPowerMeter , pszPowerMeterStateFileName ));
		}
		
		CHK_STDERR( DRV_PowerMeter_SetActiveChannel( hPowerMeter , iDB_MeasureChannel )); 
	}
	
	if ( hSpectrumAnalyzer )
	{
		iDB_NumberOfSweeps = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Number_Of_Sweeps" , hSpectrumAnalyzer );
		lfDB_SweepTimeout = GetVarDouble( clb, VARTYPE_EQUIPMENT_USE , "Sweep_Timeout" , hSpectrumAnalyzer );
		bDB_UseMaxHold = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "UseMaxHold" , hSpectrumAnalyzer ); 
		
		pszSpectrumAnalyzerStateNumber = GetVarString( clb, VARTYPE_EQUIPMENT_USE , "State_FileName" , hSpectrumAnalyzer );  

		if ( pszSpectrumAnalyzerStateNumber )
		{
			CHK_STDERR( DRV_SpectrumAnalyzer_GetLastStateFileName( hSpectrumAnalyzer , &pszGetLastState ));
	
			if (( pszGetLastState == NULL ) || ( strcmp(pszGetLastState,pszSpectrumAnalyzerStateNumber)))
			{
				sprintf( szFormatedString , "Spectrum Analyzer :: Loading state (%s)" , pszSpectrumAnalyzerStateNumber );  
				LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
				CHK_STDERR( DRV_SpectrumAnalyzer_Recall ( hSpectrumAnalyzer , pszSpectrumAnalyzerStateNumber )); 
			}
		}
		
		CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold ( hSpectrumAnalyzer , 0 ));
	
		CHK_STDERR( DRV_SpectrumAnalyzer_WaitForIntrumentDone ( hSpectrumAnalyzer , lfDB_SweepTimeout ));
	}
		
	for ( iInternalFrequencyIndex = 0; ((( iInternalFrequencyIndex < iDB_NumberOfFrequnecies ) && ( bDB_UseInternalFrequencyLoop )) || ( iInternalFrequencyIndex == 0 )); iInternalFrequencyIndex++ )
	{
		if ( bDB_UseInternalFrequencyLoop )
		{
			bDB_FrequencySecured = pvDB_FrequencySecuredList[iInternalFrequencyIndex];	
			iDB_FrequencyIndex = iInternalFrequencyIndex;
			lfCurrentFrequency = pvlfDB_Frequencies[iInternalFrequencyIndex];
		}
		
		if ( iDB_FrequencyIndex < iDB_NumberOfFrequnecyOffsets )
			lfCurrentFrequency += pvlfDB_FrequencyOffset[iDB_FrequencyIndex];
		else
			lfCurrentFrequency += pvlfDB_FrequencyOffset[0];
	
		if ( hPowerMeter ) 
		{
			PATH_GET_LOSS_VALUE( lfCurrentFrequency , lfLossFactor ); 

			sprintf( szFormatedString , "Loss Factor at (%0.3lf MHz) = %0.5lf dBm" , ( lfCurrentFrequency / 1E6 ), lfLossFactor );
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
			if ( bDB_FrequencySecured )
				sprintf( szFormatedString , "Power Meter :: Set Frequency Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
			else
				sprintf( szFormatedString , "Power Meter :: Set Frequency %0.3lf GHz" , ( lfCurrentFrequency / 1E9) );   
			
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
			CHK_STDERR( DRV_PowerMeter_SetFrequency ( hPowerMeter , iDB_MeasureChannel , lfCurrentFrequency ));  
		}
	
		if ( hSpectrumAnalyzer ) 
		{
			if ( bDB_FrequencySecured )
				sprintf( szFormatedString , "Spectrum Analyzer :: Set Frequency Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
			else
				sprintf( szFormatedString , "Spectrum Analyzer :: Set Frequency %0.3lf GHz" , ( lfCurrentFrequency / 1E9) );   
		
			LOG(CLB_LINE_NORMAL, szFormatedString ); 																		  

			CHK_STDERR( DRV_SpectrumAnalyzer_SetFrequency ( hSpectrumAnalyzer , lfCurrentFrequency )); 
		}
	
		PATH_PART_GET_LOSS_VALUE( 0 , lfCurrentFrequency , lfLossFactor ); 
	
		sprintf( szFormatedString , "Loss Factor = %0.5lf dBm" , lfLossFactor );
		LOG(CLB_LINE_NORMAL, szFormatedString ); 

		lfCurrentPower = lfDB_InputPower - lfLossFactor;
	
		CHK_STDERR( DRIVER_MANAGER_CALIBRATION_Gel_3D_Factor( hSignalGenerator , lfCurrentPower , lfCurrentFrequency , &lfSignalGeneratorLossFactor ));  
		
		if ( lfSignalGeneratorLossFactor != 0.0 )
		{
			sprintf( szFormatedString , "Signal Generator Factor = %0.3lf dBm" , lfSignalGeneratorLossFactor );  
			LOG(CLB_LINE_NORMAL,szFormatedString);
		}
	
		lfInputLossFactor = lfSignalGeneratorLossFactor - lfLossFactor;
		lfCurrentPower -= lfSignalGeneratorLossFactor;
	
		sprintf( szFormatedString , "Signal Generator :: Set Power to %0.3lf dBm" , lfCurrentPower );  
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
		CHK_STDERR( DRV_SignalGenerator_SetPower ( hSignalGenerator , 0 , lfCurrentPower )); 
		
		if ( bDB_FrequencySecured == 0 )
			sprintf( szFormatedString , "Signal Generator :: Set Frequency Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
		else
			sprintf( szFormatedString , "Signal Generator :: Set Frequency to %0.3lf MHz" , ( lfCurrentFrequency / 1E6 ));  

		LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
		CHK_STDERR( DRV_SignalGenerator_SetFrequency ( hSignalGenerator , 0 , lfCurrentFrequency )); 
		
		CHK_STDERR( DRV_SignalGenerator_RF_On( hSignalGenerator , 1 ));
		
		memset( &tMeasurePower , 0 , sizeof(tsMeasureItem));

		for ( iMeasureIndex = 0; iMeasureIndex < iDB_NumberOfMeasures; iMeasureIndex++ )
		{
			if ( hPowerMeter ) 
			{
				sprintf( szFormatedString , "Measure - %d" , iMeasureIndex+1 );
				LOG(CLB_LINE_NORMAL, szFormatedString ); 

				UPDATERR( DRV_PowerMeter_FetchPower( hPowerMeter , iDB_MeasureChannel , lfDB_MeasureTimeout , &(tMeasurePower.lfLastValue) )); 
			}
	
			if ( hSpectrumAnalyzer ) 
			{
				if ( bDB_UseMaxHold )
				{
					CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold ( hSpectrumAnalyzer , 1 )); 
				}
		
				WAIT( "" , lfDB_MeasurementDelay );
				
				for ( iSweepIndex = 0; iSweepIndex < iDB_NumberOfSweeps; iSweepIndex++ )
				{
					sprintf( szFormatedString , "Sweep - %d" , iMeasureIndex+1 );
					LOG(CLB_LINE_NORMAL, szFormatedString ); 

					DRV_SpectrumAnalyzer_GetSweep ( hSpectrumAnalyzer , lfDB_SweepTimeout ); 
				}
		
				CHK_STDERR( DRV_SpectrumAnalyzer_SearchMarkerPeak( hSpectrumAnalyzer , 1 ));

				CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerFrequency( hSpectrumAnalyzer , 1 , &lfMarkerFrequency )); 
				CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerAmplitude( hSpectrumAnalyzer , 1 , &(tMeasurePower.lfLastValue) )); 

				PATH_PART_GET_LOSS_VALUE( 1 , lfMarkerFrequency , lfLossFactor );

				if ( bDB_FrequencySecured ) 
					sprintf( szFormatedString , "Loss Factor at is %0.5lf dBm" , lfLossFactor );
				else
					sprintf( szFormatedString , "Loss Factor at (%0.3lf MHz) = %0.5lf dBm" , ( lfMarkerFrequency / 1E6 ) , lfLossFactor );
		
				LOG(CLB_LINE_NORMAL, szFormatedString ); 

				if ( bDB_FrequencySecured )
					sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf dBm" , (tMeasurePower.lfLastValue) );   
				else
					sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf MHz / %0.3lf dBm" , (lfMarkerFrequency / 1E6) , (tMeasurePower.lfLastValue) );   
			
				LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
				if ( bDB_UseMaxHold )
				{
					CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold ( hSpectrumAnalyzer , 0 )); 
				}
			}
	
			if (hSpectrumAnalyzer )
			{
				CHK_STDERR( DRV_SpectrumAnalyzer_GetReferenceLevel( hSpectrumAnalyzer , &lfReferenceLevel ));  
					
				CHK_STDERR( DRIVER_MANAGER_CALIBRATION_Gel_3D_Factor( hSpectrumAnalyzer , lfReferenceLevel , lfMarkerFrequency , &lfSpectrumAnalyzerLossFactor ));  
	
				if ( lfSpectrumAnalyzerLossFactor != 0.0 )
				{
					sprintf( szFormatedString , "Spectrum Analyzer :: Factor = %0.3lf dBm" , lfSpectrumAnalyzerLossFactor );  
					LOG(CLB_LINE_NORMAL,szFormatedString);
			
					lfLossFactor += lfSpectrumAnalyzerLossFactor;
				}
			}
	
			tMeasurePower.lfLastValue -= lfLossFactor;
		
			if ( IS_NOT_OK )
				continue;
		
			sprintf( szFormatedString , "Measured Power = %0.5lf dBm" , tMeasurePower.lfLastValue );
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
			AddCalculateMasureItem(&tMeasurePower);
		}

		if ( hSpectrumAnalyzer ) 
			lfCurrentFrequency =  lfMarkerFrequency;
			
		if ( bDB_FrequencySecured )
			sprintf( szFormatedString , "Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
		else
			sprintf( szFormatedString , "%0.3lf MHz" , ( lfCurrentFrequency / 1E6 ));
	
		if ( bDB_AbsoluteResult )
		{
			lfResult = tMeasurePower.lfAverage;
	
			if ( iDB_FrequencyIndex == 0 )
			{
				lfDB_MaximumValue = lfResult;
				lfDB_MinimumValue = lfResult;
			}
			else
			{
				if ( lfResult > lfDB_MaximumValue )
					lfDB_MaximumValue = lfResult;
		
				if ( lfResult < lfDB_MinimumValue )
					lfDB_MinimumValue = lfResult;
			}
	
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_AbsoluteResult" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfResult ); 
			}
		
			RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfResult , "Result" );	 
		}
		
		if ( bDB_AbsoluteMinMaxResult )
		{
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_MaximumResult" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDB_MaximumValue ); 
			}
		
			RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfDB_MaximumValue , "MaximumResult" );
	
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_MinimumResult" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDB_MinimumValue ); 
			}
		
			RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfDB_MinimumValue , "MinimumResult" );
			
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_MaximumValue" , pCurrentTestPrefix );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDB_MaximumValue );
	
				sprintf( szFormatedVariable , "%s_MinimumValue" , pCurrentTestPrefix );									   
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDB_MinimumValue );					   
			}
		}  	
		
		if ( bDB_GainResult )
		{
			lfGainResult = tMeasurePower.lfAverage - lfDB_InputPower;
	
			if ( iDB_FrequencyIndex == 0 )
			{
				lfDB_MaximumValue = lfGainResult;
				lfDB_MinimumValue = lfGainResult;
			}
			else
			{
				if ( lfGainResult > lfDB_MaximumValue )
					lfDB_MaximumValue = lfGainResult;
		
				if ( lfGainResult < lfDB_MinimumValue )
					lfDB_MinimumValue = lfGainResult;
			}
	
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_GainResult" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfGainResult ); 
			}
		
			RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfGainResult , "Gain" );
	
		}
		
		if ( bDB_GainMinMaxResult )
		{
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_MaximumGain" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDB_MaximumValue ); 
			}
		
			RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfDB_MaximumValue , "MaximumGain" );
	
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_MinimumGain" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDB_MinimumValue ); 
			}
		
			RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfDB_MinimumValue , "MinimumGain" );
			
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_MaximumValue" , pCurrentTestPrefix );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDB_MaximumValue );
	
				sprintf( szFormatedVariable , "%s_MinimumValue" , pCurrentTestPrefix );									   
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDB_MinimumValue );					   
			}
		}
		
		if ( bDB_FlatnessResult )
		{
			lfFlatnessValue = lfDB_MaximumValue - lfDB_MinimumValue;
	
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Flatness" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfFlatnessValue ); 
			}
		
			RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfFlatnessValue , "Flatness" );
		}
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

	DRV_SignalGenerator_RF_Off( hSignalGenerator , 1 );
	
	FREE(pszGetLastState);
	FREE(pszPowerMeterStateFileName); 
	FREE(pszSpectrumAnalyzerStateNumber);
	
	FREE(pvlfDB_Frequencies);
	FREE(pvlfDB_FrequencyOffset);
	
	FREE(pvDB_FrequencySecuredList);
	
	return iTestError;
}

