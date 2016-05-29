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


TSTE_TEST ( STD_HARMONICS_OPT_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	int								iDB_StartHarmonics								=	0,
									iDB_NumberOfHarmonics							=	0;
	
	int								iCurrentPathIndex								=	0,  
									iCurrentHarmonicsIndex							=	0; 
									
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
									
	double							lfCurrentFrequency							   	=	0.0;
									
	int								hSpectrumAnalyzer								=	0;
									
	char							*pCurrentTestPrefix								=	NULL;

	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;
	
	int								bDB_FrequencySecured							=	0, 
									bDB_UseTestFrequencySetting						=	0,
									bDB_UseInternalFrequencyLoop					=	0;
									
	int								iHarmonicsIndex									=	0;
	
	double							*pvlfResultValuesArray							=	NULL,
									*pvlfResultPositionArray						=	NULL;
	
	int								bDB_EnableToStoreMeasureData					=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Harmonics Test."); 

	GET_TEST_PREFIX(pCurrentTestPrefix);  
	
	GET_SA_EQUIPMENT_HANDLE(hSpectrumAnalyzer);
	
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
		
	iDB_StartHarmonics = GetVarInt( clb, VARTYPE_TEST , "Start_From_Harmonics" );     
	
	iDB_NumberOfHarmonics = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Harmonics" );
	lfDB_SweepTimeout = GetVarDouble( clb, VARTYPE_EQUIPMENT_USE , "Sweep_Timeout" , hSpectrumAnalyzer );

	pszSpectrumAnalyzerStateNumber = GetVarString( clb, VARTYPE_EQUIPMENT_USE , "State_FileName" , hSpectrumAnalyzer );  

	sprintf( szFormatedString , "Spectrum Analyzer :: Loading state (%s)" , pszSpectrumAnalyzerStateNumber );  
	LOG(CLB_LINE_NORMAL, szFormatedString ); 

	CHK_STDERR( DRV_SpectrumAnalyzer_Recall ( hSpectrumAnalyzer , pszSpectrumAnalyzerStateNumber )); 

	CHK_STDERR( DRV_SpectrumAnalyzer_WaitForIntrumentDone ( hSpectrumAnalyzer , lfDB_SweepTimeout ));
	
	if ( bDB_FrequencySecured )
		sprintf( szFormatedString , "Spectrum Analyzer :: Set Frequency Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
	else
		sprintf( szFormatedString , "Spectrum Analyzer :: Set Frequency %0.3lf GHz" , ( lfCurrentFrequency / 1E9) );   

	LOG(CLB_LINE_NORMAL, szFormatedString ); 																		  

	CHK_STDERR( DRV_SpectrumAnalyzer_SetFrequency ( hSpectrumAnalyzer , lfCurrentFrequency )); 

	CHK_STDERR( DRV_SpectrumAnalyzer_RestartMeasure( hSpectrumAnalyzer ));

	CHK_STDERR( DRV_SpectrumAnalyzer_WaitForIntrumentDone( hSpectrumAnalyzer , lfDB_SweepTimeout ));
	
	CALLOC_ERR( pvlfResultValuesArray , iDB_NumberOfHarmonics , sizeof(double));	
	
	LOG(CLB_LINE_NORMAL, "Spectrum Analyzer :: Obtaining Measure Data..." );
	
	iCurrentHarmonicsIndex = iDB_StartHarmonics - 1;
	
	for ( iHarmonicsIndex = 0; iHarmonicsIndex < iDB_NumberOfHarmonics; iHarmonicsIndex++ )
	{
		CHK_STDERR( DRV_SpectrumAnalyzer_Read_Harmonics_Amplitude( hSpectrumAnalyzer , iCurrentHarmonicsIndex , &pvlfResultValuesArray[iHarmonicsIndex] ));	
		
		iCurrentHarmonicsIndex++;
	}
	
	iCurrentHarmonicsIndex = iDB_StartHarmonics - 1;
	
	for ( iHarmonicsIndex = 0; iHarmonicsIndex < iDB_NumberOfHarmonics; iHarmonicsIndex++ )
	{
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Harmonia%d_HarmonicsAmplitude" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iHarmonicsIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , pvlfResultValuesArray[iHarmonicsIndex] ); 
		}
		
		sprintf( szFormatedString , "Harmonia %d" , (iCurrentHarmonicsIndex+1)); 
		
		RESULT( CLB_RESULT_DOUBLE , szFormatedString , pvlfResultValuesArray[iHarmonicsIndex] , "Harmonics Amplitude" );
		
		iCurrentHarmonicsIndex++;  
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
	
	FREE(pvlfResultValuesArray);	
	FREE(pvlfResultPositionArray);
	
	return iTestError;
}

