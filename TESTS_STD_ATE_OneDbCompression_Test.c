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


TSTE_TEST ( STD_P_1_DB_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	int								iDB_MeasureChannel								=	0, 
									iDB_NumberOfSweeps								=	0, 
									iDB_NumberOfMeasures							=	0;
	
	int								iSweepIndex										=	0,
									iMeasureIndex									=	0,
									iCurrentPathIndex								=	0;  
										
	char							*pszPowerMeterStateFileName						=	NULL,
									*pszSpectrumAnalyzerStateNumber					=	NULL; 
									
	double							lfDB_PowerStep									=	0.0,
									lfDB_StartPower									=	0.0,
									lfDB_MeasureTimeout								=	0.0,
									lfDB_SweepTimeout								=	0.0, 
									lfDB_MaximumInputPower							=	0.0, 
									lfDB_ConvergenceAccuracy						=	0.0;
									
	int								iDB_NumberOfFrequnecies							=	0,
									iDB_NumberOfFrequnecyOffsets					=	0, 
									iDB_FrequencyIndex								=	0; 
									
	double							*pvlfDB_Frequencies								=	NULL,
									*pvlfDB_FrequencyOffset							=	NULL;
	
	double							*pOutputResponds								=	NULL,
									*pInputStimulus									=	NULL,
									*pPinResponds									=	NULL,
									*pPoutResponds									=	NULL;
	
	int								iOutputArraySize								=	0,
									iNumberOfOutputPoints							=	0;
	
	char							szFormatedString[512]							=	{0},
									szFormatedVariable[512]							=	{0}; 
									
	tsMeasureItem					tMeasurePower									=	{0.0};

	void							*pGraphItem										=	NULL;
	
	double							lfLossFactor									=	0.0,
									lfInputLossFactor								=	0.0,
									lfMarkerFrequency								=	0.0, 
									lfCurrentFrequency							   	=	0.0, 
									lfReferencePower								=	0.0, 
									lfCurrentStep									=	0.0,
									lfInputPower									=	0.0,  
									lfCurrentPower									=	0.0,
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
									
	int								bBreakConvergence								=	0,
									bFirstMeasure									=	0;
	
	int								bDB_EnableToStoreMeasureData					=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Power At One DB Compression Test."); 

	GET_TEST_PREFIX(pCurrentTestPrefix);  

	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	GET_SG_EQUIPMENT_HANDLE(hSignalGenerator);
	
	GET_SA_EQUIPMENT_HANDLE_IF_EXISTS(hSpectrumAnalyzer);
	
	if ( hSpectrumAnalyzer == 0 )
	{
		GET_PM_EQUIPMENT_HANDLE(hPowerMeter);	
	
		iDB_MeasureChannel = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Measure_Channel" , hPowerMeter );
		
		lfDB_MeasureTimeout = GetVarDouble( clb, VARTYPE_EQUIPMENT_USE , "Measure_Timeout" , hPowerMeter ); 
	}
	
	lfDB_StartPower = GetVarDouble( clb, VARTYPE_TEST , "StartPower" ); 
	lfDB_MaximumInputPower = GetVarDouble( clb, VARTYPE_TEST , "MaximumInputPower" ); 
	lfDB_PowerStep = GetVarDouble( clb, VARTYPE_TEST , "PowerStep" );
	lfDB_ConvergenceAccuracy = GetVarDouble( clb, VARTYPE_TEST , "ConvergenceAccuracy" );
	
	iDB_NumberOfMeasures = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Measures" ); 
	
	PATH_GET_CURRENT_INDEX( iCurrentPathIndex );
		
	GET_SEGMENT_LOOP_NUM(iCurrentTemperatureSegmentLoop);	
	GET_SEGMENT_NUMBER(iCurrentTemperatureSegment);
	
	bDB_UseTestFrequencySetting = GetVarInt( clb, VARTYPE_TEST , "UseTestFrequencySetting" );   
	
	bDB_UseInternalFrequencyLoop = GetVarInt( clb, VARTYPE_TEST , "UsePathIndexForFrequency" );
	
	GET_FREQUENCIES_LOOP_INFO_SINGLE( lfCurrentFrequency , bDB_FrequencySecured , iDB_FrequencyIndex );
	
	if ( bDB_UseTestFrequencySetting )
	{
		iDB_NumberOfFrequnecies = GetVarDoubleArray( clb, VARTYPE_TEST , "Frequnecies" , &pvlfDB_Frequencies);
		
		if ( iDB_FrequencyIndex < iDB_NumberOfFrequnecies )
			lfCurrentFrequency = pvlfDB_Frequencies[iDB_FrequencyIndex];
		else
			lfCurrentFrequency = pvlfDB_Frequencies[0];
	}
		
	iDB_NumberOfFrequnecyOffsets = GetVarDoubleArray( clb, VARTYPE_TEST , "Measure_Frequency_Offset" , &pvlfDB_FrequencyOffset );
		
	if ( iDB_FrequencyIndex < iDB_NumberOfFrequnecyOffsets )
		lfCurrentFrequency += pvlfDB_FrequencyOffset[iDB_FrequencyIndex];
	else
		lfCurrentFrequency += pvlfDB_FrequencyOffset[0];
		
	if ( hPowerMeter )
	{
		LOG(CLB_LINE_NORMAL, "Power Meter :: Reset" );
		
		CHK_STDERR( DRV_PowerMeter_Reset( hPowerMeter )); 
		
		pszPowerMeterStateFileName = GetVarString( clb, VARTYPE_EQUIPMENT_USE , "State_FileName" , hPowerMeter );  

		sprintf( szFormatedString , "Power Meter :: Loading state (%s)" , pszPowerMeterStateFileName );   //pulse_param.scp
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
		CHK_STDERR( DRV_PowerMeter_RecallState_ByName( hPowerMeter , pszPowerMeterStateFileName ));
		
		CHK_STDERR( DRV_PowerMeter_SetActiveChannel( hPowerMeter , iDB_MeasureChannel )); 
	}
	
	if ( hSpectrumAnalyzer )
	{
		iDB_NumberOfSweeps = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Number_Of_Sweeps" , hSpectrumAnalyzer );
		lfDB_SweepTimeout = GetVarDouble( clb, VARTYPE_EQUIPMENT_USE , "Sweep_Timeout" , hSpectrumAnalyzer );
		bDB_UseMaxHold = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "UseMaxHold" , hSpectrumAnalyzer ); 
		
		pszSpectrumAnalyzerStateNumber = GetVarString( clb, VARTYPE_EQUIPMENT_USE , "State_FileName" , hSpectrumAnalyzer );  

		sprintf( szFormatedString , "Spectrum Analyzer :: Loading state (%s)" , pszSpectrumAnalyzerStateNumber );  
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
		CHK_STDERR( DRV_SpectrumAnalyzer_Recall ( hSpectrumAnalyzer , pszSpectrumAnalyzerStateNumber )); 
	
		CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold ( hSpectrumAnalyzer , 0 ));
	
		CHK_STDERR( DRV_SpectrumAnalyzer_WaitForIntrumentDone ( hSpectrumAnalyzer , lfDB_SweepTimeout ));
	}
		
	if ( hPowerMeter ) 
	{
		PATH_GET_LOSS_VALUE( lfCurrentFrequency , lfLossFactor ); 

		sprintf( szFormatedString , "Loss Factor at (%0.3lf MHz) = %0.5lf dBm" , lfCurrentFrequency , lfLossFactor );
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
	
	PATH_GET_LOSS_VALUE( lfCurrentFrequency , lfLossFactor ); 
	
	sprintf( szFormatedString , "Loss Factor = %0.5lf dBm" , lfLossFactor );
	LOG(CLB_LINE_NORMAL, szFormatedString ); 

	lfCurrentPower = lfDB_StartPower - lfLossFactor;
	
	CHK_STDERR( DRIVER_MANAGER_CALIBRATION_Gel_3D_Factor( hSignalGenerator , lfCurrentPower , lfCurrentFrequency , &lfSignalGeneratorLossFactor ));  
		
	if ( lfSignalGeneratorLossFactor < 0.0 )
	{
		sprintf( szFormatedString , "Signal Generator Factor = %0.3lf dBm" , lfSignalGeneratorLossFactor );  
		LOG(CLB_LINE_NORMAL,szFormatedString);
	}
	
	lfInputLossFactor = lfSignalGeneratorLossFactor - lfLossFactor;
	lfCurrentPower -= lfSignalGeneratorLossFactor;
	
	lfCurrentStep = lfDB_PowerStep;
	bFirstMeasure = 1;
	
	do
	{
		if ( iNumberOfOutputPoints >= iOutputArraySize )
		{
			iOutputArraySize = ( iOutputArraySize * 2 ) + 128;
			
			pOutputResponds = realloc( pOutputResponds , ( iOutputArraySize * sizeof(double)));
			pInputStimulus = realloc( pInputStimulus , ( iOutputArraySize * sizeof(double)));
		}
									
		if ( lfCurrentPower >= ( lfDB_MaximumInputPower - lfInputLossFactor ))
		{
			lfCurrentPower = lfDB_StartPower;
			bBreakConvergence = 1;	
		}
		
		sprintf( szFormatedString , "Signal Generator :: Set Power to %0.3lf dBm" , lfCurrentPower );  
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
		CHK_STDERR( DRV_SignalGenerator_SetPower ( hSignalGenerator , 0 , lfCurrentPower )); 
		
		if ( bBreakConvergence )
			break;

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
			
				for ( iSweepIndex = 0; iSweepIndex < iDB_NumberOfSweeps; iSweepIndex++ )
				{
					sprintf( szFormatedString , "Sweep - %d" , iMeasureIndex+1 );
					LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
					DRV_SpectrumAnalyzer_GetSweep ( hSpectrumAnalyzer , lfDB_SweepTimeout ); 
				}
			
				CHK_STDERR( DRV_SpectrumAnalyzer_SearchMarkerPeak( hSpectrumAnalyzer , 1 ));
	
				CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerFrequency( hSpectrumAnalyzer , 1 , &lfMarkerFrequency )); 
				CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerAmplitude( hSpectrumAnalyzer , 1 , &(tMeasurePower.lfLastValue) )); 
	
				PATH_GET_LOSS_VALUE( lfMarkerFrequency , lfLossFactor );
	
				if ( bDB_FrequencySecured ) 
					sprintf( szFormatedString , "Loss Factor at is %0.5lf dBm" , lfLossFactor );
				else
					sprintf( szFormatedString , "Loss Factor at (%0.3lf MHz) = %0.5lf dBm" , lfMarkerFrequency , lfLossFactor );
			
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
		
				if ( lfSpectrumAnalyzerLossFactor < 0.0 )
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

		pOutputResponds[iNumberOfOutputPoints] = tMeasurePower.lfAverage;
		pInputStimulus[iNumberOfOutputPoints] = lfCurrentPower;
		
		iNumberOfOutputPoints++;
		
		if ( bFirstMeasure )
		{
			lfReferencePower = tMeasurePower.lfAverage;	
			bFirstMeasure = 0;
		}
		else
		{
			if ( fabs( tMeasurePower.lfAverage - lfReferencePower ) >= 1.0 )
			{
				lfCurrentStep /= 2.0;
				lfCurrentPower -= lfCurrentStep;
			}
			else
			{
				lfInputPower = lfCurrentPower;	
				lfCurrentPower += lfCurrentStep;
			}
		}
							
	} while( lfCurrentStep > lfDB_ConvergenceAccuracy );
	
	if ( hSpectrumAnalyzer ) 
		lfCurrentFrequency =  lfMarkerFrequency;
	
	if ( bDB_FrequencySecured )
		sprintf( szFormatedString , "Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
	else
		sprintf( szFormatedString , "%0.3lf MHz" , lfCurrentFrequency );
	
	APPLY_NON_LINEAR_INTERPOLATION( pInputStimulus , pOutputResponds , iNumberOfOutputPoints , OUTPUT_TRACE_SIZE  , lfDB_StartPower , lfDB_MaximumInputPower , pPinResponds , pPoutResponds );

	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_P1dB_Out_Trace" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex );
		SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , pPoutResponds , OUTPUT_TRACE_SIZE ); 
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_P1dB_In_Trace" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex );
		SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , pPinResponds , OUTPUT_TRACE_SIZE ); 
	}
	
	GET_CURRENT_RESULT_GRAPH_ITEM(pGraphItem);
	ADD_TRACE_TO_CURRENT_RESULT_GRAPH(pGraphItem,pPoutResponds,OUTPUT_TRACE_SIZE,0,OUTPUT_TRACE_SIZE,lfDB_StartPower,lfDB_MaximumInputPower,"Trace",VAL_GREEN);
	
	RESULT( CLB_RESULT_GRAPH,"", pGraphItem , "Trace" );
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_PowerAtOneDb" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfInputPower ); 
	}
	
	RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfInputPower , "Power At One DB" );

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
	FREE(pszSpectrumAnalyzerStateNumber);
	
	FREE(pvlfDB_Frequencies);
	FREE(pvlfDB_FrequencyOffset);
	
	FREE(pPinResponds);
	FREE(pPoutResponds);
	FREE(pInputStimulus);
	FREE(pOutputResponds);
	
	return iTestError;
}

