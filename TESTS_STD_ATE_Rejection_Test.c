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


TSTE_TEST ( STD_REJECTION_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	int								iDB_NumberOfSweeps								=	0, 
									iDB_NumberOfMeasures							=	0;
	
	int								iSweepIndex										=	0,
									iMeasureIndex									=	0,
									iSequencyIndex									=	0,
									iCurrentPathIndex								=	0;  
										
	char							*pszPowerMeterStateFileName						=	NULL,
									*pszSpectrumAnalyzerStateNumber					=	NULL; 
									
	double							lfDB_ReferenceLevel								=	0.0,  
									lfDB_SweepTimeout								=	0.0; 
									
	int								iDB_NumberOfFrequnecies							=	0,
									iDB_NumberOfFrequnecyOffsets					=	0, 
									iDB_FrequencyIndex								=	0; 
									
	double							*pvlfDB_Frequencies								=	NULL, 
									*pvlfDB_FrequencyOffset							=	NULL;
									
	char							szFormatedString[512]							=	{0},
									szFormatedVariable[512]							=	{0}; 
									
	tsMeasureItem					tMeasurePower									=	{0.0};

	double							lfLossFactor									=	0.0,
									lfReferenceLevel								=	0.0, 
									lfRejectionResult								=	0.0,
									lfMarkerFrequency								=	0.0, 
									lfCurrentFrequency							   	=	0.0, 
									lfCarrierMeasureFreq   							=	0.0, 
									lfCarrierMeasurePower  							=	0.0, 
									lfRejectionMeasureFreq 							=	0.0, 
									lfRejectionMeasurePower							=	0.0, 
									lfSpectrumAnalyzerLossFactor					=	0.0;
									
	int								hSpectrumAnalyzer								=	0;
									
	char							*pCurrentTestPrefix								=	NULL;

	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;
	
	int								bDB_UseMaxHold									=	0,
									bDB_FrequencySecured							=	0, 
									bDB_UseTestFrequencySetting						=	0,
									bDB_UseInternalFrequencyLoop					=	0;
	
	int								bDB_EnableToStoreMeasureData					=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Rejection Test."); 

	GET_TEST_PREFIX(pCurrentTestPrefix);  
	
	GET_SA_EQUIPMENT_HANDLE(hSpectrumAnalyzer);
	
	iDB_NumberOfMeasures = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Measures" ); 
	
	PATH_GET_CURRENT_INDEX( iCurrentPathIndex );
		
	GET_SEGMENT_LOOP_NUM(iCurrentTemperatureSegmentLoop);	
	GET_SEGMENT_NUMBER(iCurrentTemperatureSegment);
	
	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	lfDB_ReferenceLevel = GetVarDouble( clb, VARTYPE_EQUIPMENT_USE , "Spectrum_Rejection_ReferenceLevel" );
	
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
		
	iDB_NumberOfSweeps = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Number_Of_Sweeps" , hSpectrumAnalyzer );
	lfDB_SweepTimeout = GetVarDouble( clb, VARTYPE_EQUIPMENT_USE , "Sweep_Timeout" , hSpectrumAnalyzer );
	bDB_UseMaxHold = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "UseMaxHold" , hSpectrumAnalyzer ); 

	pszSpectrumAnalyzerStateNumber = GetVarString( clb, VARTYPE_EQUIPMENT_USE , "State_FileName" , hSpectrumAnalyzer );  

	sprintf( szFormatedString , "Spectrum Analyzer :: Loading state (%s)" , pszSpectrumAnalyzerStateNumber );  
	LOG(CLB_LINE_NORMAL, szFormatedString ); 

	CHK_STDERR( DRV_SpectrumAnalyzer_Recall ( hSpectrumAnalyzer , pszSpectrumAnalyzerStateNumber )); 

	CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold ( hSpectrumAnalyzer , 0 ));

	CHK_STDERR( DRV_SpectrumAnalyzer_WaitForIntrumentDone ( hSpectrumAnalyzer , lfDB_SweepTimeout ));
	
	for ( iSequencyIndex = 0; iSequencyIndex < 2; iSequencyIndex++ )
	{
		if ( bDB_FrequencySecured )
			sprintf( szFormatedString , "Spectrum Analyzer :: Set Frequency Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
		else
			sprintf( szFormatedString , "Spectrum Analyzer :: Set Frequency %0.3lf GHz" , ( lfCurrentFrequency / 1E9) );   
	
		LOG(CLB_LINE_NORMAL, szFormatedString ); 																		  

		CHK_STDERR( DRV_SpectrumAnalyzer_SetFrequency ( hSpectrumAnalyzer , lfCurrentFrequency )); 
	
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

			CHK_STDERR( DRV_SpectrumAnalyzer_GetReferenceLevel( hSpectrumAnalyzer , &lfReferenceLevel ));  
					
			CHK_STDERR( DRIVER_MANAGER_CALIBRATION_Gel_3D_Factor( hSpectrumAnalyzer , lfReferenceLevel , lfMarkerFrequency , &lfSpectrumAnalyzerLossFactor ));  
	
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
		}

		if ( iSequencyIndex == 0 )   // Carrier Measure
		{
			lfCarrierMeasurePower = tMeasurePower.lfAverage;
			lfCarrierMeasureFreq = lfMarkerFrequency;
			
			if ( iDB_FrequencyIndex < iDB_NumberOfFrequnecyOffsets )
				lfCurrentFrequency += pvlfDB_FrequencyOffset[iDB_FrequencyIndex];
			else
				lfCurrentFrequency += pvlfDB_FrequencyOffset[0];
		
			if (( lfDB_ReferenceLevel > -100.0 ) && ( lfDB_ReferenceLevel < 20.0 )) 
			{
				sprintf( szFormatedString , "Spectrum Analyzer :: Set Reference Level %0.3lf dB" , lfDB_ReferenceLevel );   		   
				LOG(CLB_LINE_NORMAL, szFormatedString ); 																		  	   
																																	   
				CHK_STDERR( DRV_SpectrumAnalyzer_SetReferenceLevel ( hSpectrumAnalyzer , lfDB_ReferenceLevel )); 					   
			}
		}
		else    // Rejection Measure 
		{
			lfRejectionMeasurePower = tMeasurePower.lfAverage;
			lfRejectionMeasureFreq = lfMarkerFrequency;	
		}
	}
	
	if ( hSpectrumAnalyzer ) 
		lfCurrentFrequency =  lfMarkerFrequency;
	
	if ( bDB_FrequencySecured )
		sprintf( szFormatedString , "Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
	else
		sprintf( szFormatedString , "%0.3lf MHz" , lfCurrentFrequency );
	
	lfRejectionResult = ( lfCarrierMeasurePower - lfRejectionMeasurePower );
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Carrier_Power" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfCarrierMeasurePower ); 
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Carrier_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfCarrierMeasureFreq ); 
	}
	
	RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfCarrierMeasurePower , "Carrier Power" );
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Rejection_Power" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfRejectionMeasurePower ); 
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Rejection_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfRejectionMeasureFreq ); 
	}
	
	RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfRejectionMeasurePower , "Rejection Power" );
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Rejection_Result" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iDB_FrequencyIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfRejectionResult ); 
	}
	
	RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfRejectionResult , "Signal Rejection" );

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
	
	return iTestError;
}

