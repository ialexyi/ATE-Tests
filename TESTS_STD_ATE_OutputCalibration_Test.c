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
typedef enum{ CONVER_TYPE_SIMPLE , CONVER_TYPE_BINARY , CONVER_TYPE_REVAL , CONVER_TYPE_SENTINEL } teConvergenceType;

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions


TSTE_TEST ( STD_OUTPUT_CALIBRATION_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	int								iDB_MeasureChannel								=	0, 
									iDB_NumberOfSweeps								=	0, 
									iDB_NumberOfMeasures							=	0;
	
	int								iSweepIndex										=	0,
									iCommandIndex									=	0,
									iBinaryUpperCommandIndex						=	0, 
									iBinaryLowerCommandIndex						=	0, 
									iCalibrationStepsCounter						=	0,
									iMeasureIndex									=	0,
									iPreviousCommandIndex							=	0,
									iCurrentPathIndex								=	0;  
										
	char							*pszPowerMeterStateFileName						=	NULL,
									*pszSpectrumAnalyzerStateNumber					=	NULL; 
									
	double							lfDB_MeasureTimeout								=	0.0,
									lfDB_NeededPower								=	0.0, 
									lfDB_NeededPowerAllowedDelta					=	0.0, 
									lfDB_MeasureDelay								=	0.0,
									lfDB_SweepTimeout								=	0.0; 
									
	int								iDB_NumberOfFrequnecies							=	0,
									iDB_FrequencyIndex								=	0; 
									
	double							*pvlfDB_Frequencies								=	NULL; 
									
	char							szFormatedString[512]							=	{0},
									szFormatedVariable[512]							=	{0};
	
	char							*pszControlDeviceName							=	NULL; 
									
	tsMeasureItem					tMeasurePower									=	{0.0};

	double							lfLossFactor									=	0.0,
									lfMarkerFrequency								=	0.0, 
									lfCurrentFrequency							   	=	0.0, 
									lfReferenceLevel								=	0.0, 
									lfPreviousMeasure								=	0.0, 
									lfSpectrumAnalyzerLossFactor					=	0.0;
									
	int								hPowerMeter										=	0,
									hDeviceHandle									=	0,
									hSpectrumAnalyzer								=	0;
									
	char							*pCurrentTestPrefix								=	NULL;

	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;
	
	int								bDB_UseMaxHold									=	0,
									bDB_FrequencySecured							=	0, 
									bDB_ConvergenceTypeBinary						=	0, 
									bDB_ConvergenceTypeRevaluation					=	0, 
									bDB_UseTestFrequencySetting						=	0,
									bDB_UseInternalFrequencyLoop					=	0,
									bDB_UseCalibrationValueFromHistory				=	0,
									bDB_ConvergenceTypeRevaluation_Min_Steps		=	0; 
									
	char							**pvDB_OutputCalibrationCommands				=	NULL;
	
	int								*pDB_OutputCalibrationValues					=	NULL;
	
	int								iOutputCalibrationCommandsLength				=	0,
									iOutputCalibrationValuesLength					=	0; 
									
	int								iDB_OutputCalibrationStartValue					=	0,
									iDB_OutputCalibrationStepValue					=	0,
									iDB_MaximumCalibrationSteps						=	0; 
									
	teConvergenceType				tConvergenceType								=	0;
	
	int								bDB_EnableToStoreMeasureData					=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Output Calibration Test."); 

	GET_TEST_PREFIX(pCurrentTestPrefix);  
	
	GET_SA_EQUIPMENT_HANDLE_IF_EXISTS(hSpectrumAnalyzer);
	
	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	if ( hSpectrumAnalyzer == 0 )
	{
		GET_PM_EQUIPMENT_HANDLE(hPowerMeter);	
	
		iDB_MeasureChannel = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Measure_Channel" , hPowerMeter );
		
		lfDB_MeasureTimeout = GetVarDouble( clb, VARTYPE_EQUIPMENT_USE , "Measure_Timeout" , hPowerMeter ); 
	}
	
	iDB_NumberOfMeasures = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Measures" ); 
	
	iDB_MaximumCalibrationSteps = GetVarInt( clb, VARTYPE_TEST , "Maximum_Calibration_Steps" ); 
	
	lfDB_MeasureDelay = GetVarDouble( clb, VARTYPE_TEST , "Measure_Delay" );  
	
	lfDB_NeededPower = GetVarDouble( clb, VARTYPE_TEST , "Needed_Power" ); 
	lfDB_NeededPowerAllowedDelta = GetVarDouble( clb, VARTYPE_TEST , "Needed_Power_AllowedDelta" ); 
	
	bDB_ConvergenceTypeBinary = GetVarInt( clb, VARTYPE_TEST , "Convergence_Type_Binary" );
	
	bDB_UseCalibrationValueFromHistory = GetVarInt( clb, VARTYPE_TEST , "Use_Calibration_Value_From_History" ); 
	
	if ( bDB_ConvergenceTypeBinary == 0 )
	{
		bDB_ConvergenceTypeRevaluation = GetVarInt( clb, VARTYPE_TEST , "Convergence_Type_Revaluation" ); 
		
		if ( bDB_ConvergenceTypeRevaluation )
		{
			bDB_ConvergenceTypeRevaluation_Min_Steps = GetVarInt( clb, VARTYPE_TEST , "Convergence_Type_Revaluation_Min_Steps" );
			
			if ( bDB_ConvergenceTypeRevaluation_Min_Steps < 3 )
				bDB_ConvergenceTypeRevaluation_Min_Steps = 3;
		}
	}
	else
	{
		tConvergenceType = CONVER_TYPE_BINARY;	
	}
	
	pszControlDeviceName = GetVarString( clb, VARTYPE_TEST , "Control_Device_Name" );             
	
	if ( pszControlDeviceName && strlen(pszControlDeviceName) )
	{
		GET_DEV_EQUIPMENT_HANDLE_BY_ALIAS_IF_EXISTS( pszControlDeviceName , hDeviceHandle );
		  
		pvDB_OutputCalibrationCommands = GetVarStringArray( clb, VARTYPE_EQUIPMENT_USE , "Output_Calibration_Commands" , &iOutputCalibrationCommandsLength , hDeviceHandle );
		
		iOutputCalibrationValuesLength = GetVarIntArray( clb, VARTYPE_EQUIPMENT_USE, "Output_Calibration_Values" , &pDB_OutputCalibrationValues , hDeviceHandle );     
		
		iDB_OutputCalibrationStartValue = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Output_Calibration_StartValue" , hDeviceHandle );
		iDB_OutputCalibrationStepValue = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Output_Calibration_StepValue" , hDeviceHandle );  
		
		IF (( iOutputCalibrationValuesLength != iOutputCalibrationCommandsLength ) , "Output Calibration Length Mismatch between Values and Commands." );
	}
	
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
	
	iCommandIndex = iDB_OutputCalibrationStartValue - iDB_OutputCalibrationStepValue;
	iBinaryUpperCommandIndex = iOutputCalibrationCommandsLength-1;
	iBinaryLowerCommandIndex = iDB_OutputCalibrationStartValue;
	
	if ( bDB_UseCalibrationValueFromHistory == 0 )
	{
		iPreviousCommandIndex = -1;
	}
	else
	{
		sprintf( szFormatedVariable , "%s_PreviousCommandIndex" , pCurrentTestPrefix );
		iPreviousCommandIndex = GetVarInt( clb, VARTYPE_MEASURE_STORE , szFormatedVariable );	
	}
	
	do //---------- Calibration ----------------------//
	{
		//if (( bDB_ConvergenceTypeRevaluation ) && ( iCalibrationStepsCounter >= bDB_ConvergenceTypeRevaluation_Min_Steps )
		//	tConvergenceType = CONVER_TYPE_REVAL;

		if ( tConvergenceType == CONVER_TYPE_SIMPLE )
		{
			iCommandIndex += iDB_OutputCalibrationStepValue;
		}
		
		if ( tConvergenceType == CONVER_TYPE_BINARY )
		{
			if ( iPreviousCommandIndex >= 0 )
				if ( lfPreviousMeasure < tMeasurePower.lfAverage )
					iBinaryLowerCommandIndex = iPreviousCommandIndex;	
				else
					iBinaryUpperCommandIndex = iPreviousCommandIndex;	
		
			iCommandIndex = iBinaryUpperCommandIndex + (( iBinaryUpperCommandIndex - iBinaryLowerCommandIndex ) / 2 );
		}
			
		if ( tConvergenceType == CONVER_TYPE_REVAL )
		{
				
		}
		
		lfPreviousMeasure = tMeasurePower.lfAverage;
		iPreviousCommandIndex = iCommandIndex;
		
		if (( iCommandIndex < 0 ) || ( iCommandIndex >= iOutputCalibrationCommandsLength ))
			break;
		
		CHK_STDERR( DRV_StandardDevice_SetValue( hDeviceHandle , pvDB_OutputCalibrationCommands[iCommandIndex] , &(pDB_OutputCalibrationValues[iCommandIndex]) , 1 , sizeof(int)));
	
		WAIT( "" , lfDB_MeasureDelay );
		
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
					sprintf( szFormatedString , "Sweep - %d" , iSweepIndex+1 );
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

		if ( fabs( tMeasurePower.lfAverage - lfDB_NeededPower ) < lfDB_NeededPowerAllowedDelta )
			break;
		
		iCalibrationStepsCounter++;
		
	} while( iCalibrationStepsCounter < iDB_MaximumCalibrationSteps );
	
	if ( hSpectrumAnalyzer ) 
		lfCurrentFrequency =  lfMarkerFrequency;
	
	if ( bDB_FrequencySecured )
		sprintf( szFormatedString , "Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
	else
		sprintf( szFormatedString , "%0.3lf MHz" , lfCurrentFrequency );
	
	sprintf( szFormatedVariable , "%s_PreviousCommandIndex" , pCurrentTestPrefix );
	SetVarInt( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , iCommandIndex );	
		
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Power_Average" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasurePower.lfAverage ); 
	}
	
	RESULT( CLB_RESULT_DOUBLE , szFormatedString , tMeasurePower.lfAverage , "Average Value" );
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Power_Maximum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasurePower.lfMaxValue );
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Power_Maximum_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfCurrentFrequency );
	}
	
	RESULT( CLB_RESULT_DOUBLE , szFormatedString , tMeasurePower.lfMaxValue , "Maximum Value" );
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Power_Minimum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasurePower.lfMinValue );
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Power_Minimum_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfCurrentFrequency );
	}
	
	RESULT( CLB_RESULT_DOUBLE , szFormatedString , tMeasurePower.lfMinValue , "Minimum Value" );

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

	FREE(pszControlDeviceName);
	
	FREE_LIST( pvDB_OutputCalibrationCommands , iOutputCalibrationCommandsLength );
	FREE(pDB_OutputCalibrationValues);
	
	return iTestError;
}

