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


TSTE_TEST ( STD_FREQUENCY_ACCURACY_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	int								iDB_NumberOfSweeps								=	0, 
									iDB_NumberOfMeasures							=	0;
	
	int								iSweepIndex										=	0,
									iMeasureIndex									=	0,
									iCurrentPathIndex								=	0,  
									iInternalFrequencyIndex							=	0;
									
	char							*pszPowerMeterStateFileName						=	NULL,
									*pszSpectrumAnalyzerStateNumber					=	NULL; 
									
	double							lfDB_SweepTimeout								=	0.0, 
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
									
	tsMeasureItem					tMeasurePower									=	{0.0},
									tMeasureFrequency								=	{0.0};
	
	double							lfLossFactor									=	0.0,
									lfCurrentFrequency							   	=	0.0, 
									lfReferenceLevel								=	0.0, 
									lfSpectrumAnalyzerLossFactor					=	0.0;
									
	int								hSpectrumAnalyzer								=	0;
	
	char							*pCurrentTestPrefix								=	NULL;

	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;
	
	int								bDB_UseMaxHold									=	0,
									bDB_FrequencySecured							=	0, 
									bDB_UseTestFrequencySetting						=	0,
									bDB_UseInternalFrequencyLoop					=	0,
									bDB_UsePathIndexForFrequency					=	0,
									bDB_DisableUpdatingSpectrumFrequency			=	0;  
									
	int								bDB_EnableToStoreMeasureData					=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Frequency Accuracy Test."); 

	GET_TEST_PREFIX(pCurrentTestPrefix);  

	GET_SA_EQUIPMENT_HANDLE(hSpectrumAnalyzer);

	iDB_NumberOfMeasures = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Measures" ); 
	
	PATH_GET_CURRENT_INDEX( iCurrentPathIndex );
		
	GET_SEGMENT_LOOP_NUM(iCurrentTemperatureSegmentLoop);	
	GET_SEGMENT_NUMBER(iCurrentTemperatureSegment);
	
	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	bDB_UseTestFrequencySetting = GetVarInt( clb, VARTYPE_TEST , "UseTestFrequencySetting" );   
	
	bDB_UseInternalFrequencyLoop = GetVarInt( clb, VARTYPE_TEST , "UseInternalFrequencyLoop" );
	
	bDB_UsePathIndexForFrequency = GetVarInt( clb, VARTYPE_TEST , "UsePathIndexForFrequency" );
	
	bDB_DisableUpdatingSpectrumFrequency = GetVarInt( clb, VARTYPE_TEST , "DisableUpdatingSpectrumFrequency" ); 
	
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
		
	iDB_NumberOfSweeps = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Number_Of_Sweeps" , hSpectrumAnalyzer );
	lfDB_SweepTimeout = GetVarDouble( clb, VARTYPE_EQUIPMENT_USE , "Sweep_Timeout" , hSpectrumAnalyzer );
	bDB_UseMaxHold = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "UseMaxHold" , hSpectrumAnalyzer ); 
	
	pszSpectrumAnalyzerStateNumber = GetVarString( clb, VARTYPE_EQUIPMENT_USE , "State_FileName" , hSpectrumAnalyzer );  

	sprintf( szFormatedString , "Spectrum Analyzer :: Loading state (%s)" , pszSpectrumAnalyzerStateNumber );  
	LOG(CLB_LINE_NORMAL, szFormatedString ); 

	CHK_STDERR( DRV_SpectrumAnalyzer_Recall ( hSpectrumAnalyzer , pszSpectrumAnalyzerStateNumber )); 

	if ( bDB_UseMaxHold )
	{
		CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold ( hSpectrumAnalyzer , 0 ));
	}
	
	CHK_STDERR( DRV_SpectrumAnalyzer_WaitForIntrumentDone ( hSpectrumAnalyzer , lfDB_SweepTimeout ));
		
	for ( iInternalFrequencyIndex = 0; ((( iInternalFrequencyIndex < iDB_NumberOfFrequnecies ) && ( bDB_UseInternalFrequencyLoop )) || ( iInternalFrequencyIndex == 0 )); iInternalFrequencyIndex++ )
	{
		if ( bDB_UseInternalFrequencyLoop )
		{
			if ( pvDB_FrequencySecuredList )
				bDB_FrequencySecured = pvDB_FrequencySecuredList[iInternalFrequencyIndex];	
			
			iDB_FrequencyIndex = iInternalFrequencyIndex;
			
			lfCurrentFrequency = pvlfDB_Frequencies[iInternalFrequencyIndex];
		}
		
		if ( bDB_UsePathIndexForFrequency )
		{
			if ( pvDB_FrequencySecuredList )
				bDB_FrequencySecured = pvDB_FrequencySecuredList[iInternalFrequencyIndex];	
			
			iDB_FrequencyIndex = iCurrentPathIndex;
			
			lfCurrentFrequency = pvlfDB_Frequencies[iCurrentPathIndex];	
		}
		
		if ( iDB_FrequencyIndex < iDB_NumberOfFrequnecyOffsets )
			lfCurrentFrequency += pvlfDB_FrequencyOffset[iDB_FrequencyIndex];
		else
			lfCurrentFrequency += pvlfDB_FrequencyOffset[0];
	
		if ( bDB_FrequencySecured )
			sprintf( szFormatedString , "Spectrum Analyzer :: Set Frequency Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
		else
			sprintf( szFormatedString , "Spectrum Analyzer :: Set Frequency %0.3lf GHz" , ( lfCurrentFrequency / 1E9) );   
	
		LOG(CLB_LINE_NORMAL, szFormatedString ); 																		  

		if ( bDB_DisableUpdatingSpectrumFrequency == 0 )
		{
			CHK_STDERR( DRV_SpectrumAnalyzer_SetFrequency ( hSpectrumAnalyzer , lfCurrentFrequency )); 
		}
		
		memset( &tMeasurePower , 0 , sizeof(tsMeasureItem));

		for ( iMeasureIndex = 0; iMeasureIndex < iDB_NumberOfMeasures; iMeasureIndex++ )
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
	
			CHK_STDERR( DRV_SpectrumAnalyzer_SetSweepMode( hSpectrumAnalyzer , 0 ));
			
			CHK_STDERR( DRV_SpectrumAnalyzer_MeasureMarkerFrequencyCounter( hSpectrumAnalyzer, 1 , lfDB_SweepTimeout , &(tMeasureFrequency.lfLastValue)));  
			
			CHK_STDERR( DRV_SpectrumAnalyzer_SearchMarkerPeak( hSpectrumAnalyzer , 1 ));
			CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerAmplitude( hSpectrumAnalyzer , 1 , &(tMeasurePower.lfLastValue) )); 

			CHK_STDERR( DRV_SpectrumAnalyzer_SetSweepMode( hSpectrumAnalyzer , 1 )); 
			
			PATH_GET_LOSS_VALUE( (tMeasureFrequency.lfLastValue) , lfLossFactor );

			if ( bDB_FrequencySecured ) 
				sprintf( szFormatedString , "Loss Factor at is %0.5lf dBm" , lfLossFactor );
			else
				sprintf( szFormatedString , "Loss Factor at (%0.3lf MHz) = %0.5lf dBm" , (tMeasureFrequency.lfLastValue) , lfLossFactor );
	
			LOG(CLB_LINE_NORMAL, szFormatedString ); 

			if ( bDB_FrequencySecured )
				sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf dBm" , (tMeasurePower.lfLastValue) );   
			else
				sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf MHz / %0.3lf dBm" , ((tMeasureFrequency.lfLastValue) / 1E6) , (tMeasurePower.lfLastValue) );   
		
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
			if ( bDB_UseMaxHold )
			{
				CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold ( hSpectrumAnalyzer , 0 )); 
			}

			CHK_STDERR( DRV_SpectrumAnalyzer_GetReferenceLevel( hSpectrumAnalyzer , &lfReferenceLevel ));  
				
			CHK_STDERR( DRIVER_MANAGER_CALIBRATION_Gel_3D_Factor( hSpectrumAnalyzer , lfReferenceLevel , (tMeasureFrequency.lfLastValue) , &lfSpectrumAnalyzerLossFactor ));  

			if ( lfSpectrumAnalyzerLossFactor < 0.0 )
			{
				sprintf( szFormatedString , "Spectrum Analyzer :: Factor = %0.3lf dBm" , lfSpectrumAnalyzerLossFactor );  
				LOG(CLB_LINE_NORMAL,szFormatedString);
		
				lfLossFactor += lfSpectrumAnalyzerLossFactor;
			}
	
			tMeasurePower.lfLastValue -= lfLossFactor;
		
			if ( IS_NOT_OK )
				continue;
		
			sprintf( szFormatedString , "Measured Power = %0.5lf dBm" , tMeasurePower.lfLastValue );
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
			AddCalculateMasureItem(&tMeasurePower);
			AddCalculateMasureItem(&tMeasureFrequency); 
		}

		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_FrequencyValue" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureFrequency.lfAverage ); 
		}
		
		RESULT( CLB_RESULT_DOUBLE , "" , tMeasureFrequency.lfAverage , "Frequency Value" );
	
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_MaximumFrequency" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDB_MaximumValue ); 
		}
		
		RESULT( CLB_RESULT_DOUBLE , "" , tMeasureFrequency.lfMaxValue , "Maximum Frequency" );
	
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_MinimumFrequency" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDB_MinimumValue ); 
		}
		
		RESULT( CLB_RESULT_DOUBLE , "" , tMeasureFrequency.lfMinValue , "Minimum Frequency" );
	
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_AmplitudeValue" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasurePower.lfAverage ); 
		}
		
		RESULT( CLB_RESULT_DOUBLE , "" , tMeasurePower.lfAverage , "Amplitude Value" );
					   
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
	FREE(pszSpectrumAnalyzerStateNumber);
	
	FREE(pvlfDB_Frequencies);
	FREE(pvlfDB_FrequencyOffset);
	
	FREE(pvDB_FrequencySecuredList);
	
	return iTestError;
}

