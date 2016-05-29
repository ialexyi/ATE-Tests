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


TSTE_TEST ( STD_SPUR_EMISSION_OPT_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	int								iDB_StartRangeNumber							=	0,
									iDB_NumberOfSweeps								=	0, 
									iDB_NumberOfRanges								=	0;
	
	int								iCurrentPathIndex								=	0;  
										
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
									
	double							lfResult										=	0.0,
									lfCurrentFrequency							   	=	0.0,
									lfMarkerFrequency								=	0.0,
									lfCarrierAmplitude								=	0.0; 
									
	int								hSpectrumAnalyzer								=	0;
									
	char							*pCurrentTestPrefix								=	NULL;

	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;
	
	int								bDB_UseMaxHold									=	0,
									bDB_FrequencySecured							=	0, 
									bDB_UseTestFrequencySetting						=	0,
									bDB_ResultReferenceToCarrier					=	0,
									bDB_UseInternalFrequencyLoop					=	0;
									
	int								iSpurIndex										=	0,
									iSweepIndex										=	0, 
									iRangeIndex										=	0;
	
	double							*pvlfResultValuesArray							=	NULL,
									*pvlfResultPositionArray						=	NULL;
	
	int								iNumberOfSpurious								=	0;
	
	int								*pvRangeNumber									=	NULL,
									*pvPassFail										=	NULL;
						
	double							*pvlfSpurFrequency								=	NULL,
									*pvlfSpurAmplitude								=	NULL,
									*pvlfSpurLimit									=	NULL;
	
	int								bDB_EnableToStoreMeasureData					=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Spurious Emission Test."); 

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
		
	bDB_ResultReferenceToCarrier = GetVarInt( clb, VARTYPE_TEST , "bResultReferenceToCarrier" ); 
	
	iDB_NumberOfFrequnecyOffsets = GetVarDoubleArray( clb, VARTYPE_TEST , "Measure_Frequency_Offset" , &pvlfDB_FrequencyOffset );
		
	iDB_StartRangeNumber = GetVarInt( clb, VARTYPE_TEST , "Start_From_RangeNumber" );     
	
	iDB_NumberOfRanges = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Ranges" );
	lfDB_SweepTimeout = GetVarDouble( clb, VARTYPE_EQUIPMENT_USE , "Sweep_Timeout" , hSpectrumAnalyzer );

	if ( bDB_ResultReferenceToCarrier )
	{
		bDB_UseMaxHold = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "UseMaxHold" , hSpectrumAnalyzer ); 
		iDB_NumberOfSweeps = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Number_Of_Sweeps" , hSpectrumAnalyzer );
		
		pszSpectrumAnalyzerStateNumber = GetVarString( clb, VARTYPE_EQUIPMENT_USE , "CarrierMeasState_FileName" , hSpectrumAnalyzer );  

		sprintf( szFormatedString , "Spectrum Analyzer :: Loading state (%s)" , pszSpectrumAnalyzerStateNumber );  
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
		CHK_STDERR( DRV_SpectrumAnalyzer_Recall ( hSpectrumAnalyzer , pszSpectrumAnalyzerStateNumber )); 
	
		CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold ( hSpectrumAnalyzer , 0 ));
	
		CHK_STDERR( DRV_SpectrumAnalyzer_WaitForIntrumentDone ( hSpectrumAnalyzer , lfDB_SweepTimeout ));	
		
		if ( bDB_FrequencySecured )
			sprintf( szFormatedString , "Spectrum Analyzer :: Set Frequency Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
		else
			sprintf( szFormatedString , "Spectrum Analyzer :: Set Frequency %0.3lf GHz" , ( lfCurrentFrequency / 1E9) );   
		
		LOG(CLB_LINE_NORMAL, szFormatedString ); 																		  

		CHK_STDERR( DRV_SpectrumAnalyzer_SetFrequency ( hSpectrumAnalyzer , lfCurrentFrequency )); 
		
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
		CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerAmplitude( hSpectrumAnalyzer , 1 , &lfCarrierAmplitude )); 
		
		if ( bDB_FrequencySecured )
			sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf dBm" , lfCarrierAmplitude );   
		else
			sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf MHz / %0.3lf dBm" , (lfMarkerFrequency / 1E6) , lfCarrierAmplitude );   
			
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
		if ( bDB_UseMaxHold )
		{
			CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold ( hSpectrumAnalyzer , 0 )); 
		}
		
		sprintf( szFormatedString , "%0.3lf MHz" , ( lfMarkerFrequency / 1E6 ));
	
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Carrier_Amplitude" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , iRangeIndex );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfCarrierAmplitude ); 
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Carrier_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , iRangeIndex );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfMarkerFrequency ); 
		}
		
		RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfCarrierAmplitude , "Carrier Amplitude" );
		
		FREE(pszSpectrumAnalyzerStateNumber);
	}
	
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
	
	LOG(CLB_LINE_NORMAL, "Spectrum Analyzer :: Obtaining Measure Data..." );
	
	CHK_STDERR( DRV_SpectrumAnalyzer_Read_Spurious_Emissions_List( hSpectrumAnalyzer , &iNumberOfSpurious , &pvRangeNumber , &pvPassFail , &pvlfSpurFrequency , &pvlfSpurAmplitude , &pvlfSpurLimit ));  

	for ( iRangeIndex = iDB_StartRangeNumber; iRangeIndex <= iDB_NumberOfRanges; iRangeIndex++ )
	{
		for ( iSpurIndex = 0; iSpurIndex < iNumberOfSpurious; iSpurIndex++ )    
		{
			if( pvRangeNumber[iSpurIndex] < iRangeIndex )
				continue;
		
			break;
		}
			
		sprintf( szFormatedString , "%0.3lf MHz" , ( pvlfSpurFrequency[iSpurIndex] / 1E6 ));
	
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Range%d_Spurious_Amplitude" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , iRangeIndex );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , pvlfSpurAmplitude[iSpurIndex] ); 
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Range%d_Spurious_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , iRangeIndex );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , pvlfSpurFrequency[iSpurIndex] ); 
		}
		
		RESULT( CLB_RESULT_DOUBLE , szFormatedString , pvlfSpurAmplitude[iSpurIndex] , "Spurious Amplitude" );
		
		lfResult = lfCarrierAmplitude - pvlfSpurAmplitude[iSpurIndex];
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_Range%d_Spurious_dBc" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , iRangeIndex );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfResult );
		}
		
		if ( bDB_ResultReferenceToCarrier )
		{
			RESULT( CLB_RESULT_DOUBLE , "" , lfResult , "Spurious" ); 
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

	FREE(pszPowerMeterStateFileName); 
	FREE(pszSpectrumAnalyzerStateNumber);
	
	FREE(pvlfDB_Frequencies);
	FREE(pvlfDB_FrequencyOffset);
	
	FREE(pvlfResultValuesArray);	
	FREE(pvlfResultPositionArray);
	
	FREE(pvRangeNumber);		
	FREE(pvPassFail);			
	                   
	FREE(pvlfSpurFrequency);	
	FREE(pvlfSpurAmplitude);	
	FREE(pvlfSpurLimit);
	
	return iTestError;
}

