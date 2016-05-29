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

TSTE_TEST ( STD_SPURIOUS_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	double							*pvlfDB_Frequencies								=	NULL;

	char							szFormatedString[512]							=	{0};

	int								iDB_NumberOfFrequencies							=	0,
									iDB_FrequencyIndex								=	0; 

	int								iDB_NumberOfSweeps								=	0;
	
	double							lfDB_SweepTimeout								=	0.0;
	
	int								iFrequencyIndex									=	0,
									iCurrentPathIndex								=	0;  

	double							lfDB_MeasureDelay								=	0.0;
	
	double							lfMarkerFrequency 								=	0.0,
									lfMarkerAmplitude 								=	0.0;
	
	double							lfLossFactor									=	0.0;
	
	double							lfCarrierPower									=	0.0,
									lfMaxSpurious									=	0.0,
									lfCurrentSpur									=	0.0,
									lfStartFrequency								=	0.0, 
									lfStopFrequency									=	0.0,
									lfSpanFrequency									=	0.0,   
									lfCurrentFrequency							   	=	0.0, 
									lfMaxSpuriousFreq								=	0.0,  
									lfCenterFrequency								=	0.0, 
									lfReferenceLevel								=	0.0,  
									lfSpectrumAnalyzerLossFactor					=	0.0; 
										
	char							*pDB_SpectrumAnalyzerCarrierState				=	NULL,
									*pDB_SpectrumAnalyzerSpuriousState				=	NULL,
									*pszGethSpectrumAnalyzerStateNumber				=	NULL; 
									
	double							lfDB_Spurious_Start_Band_Freq					=	0.0,
									lfDB_Spurious_Stop_Band_Freq					=	0.0,
									lfDB_Spurious_Carrier_Isolation_Freq			=	0.0; 
									
	int								hSpectrumAnalyzer								=	0;
									
	char							*pCurrentTestPrefix								=	NULL;

	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;
	
	int								bDB_UseMaxHold									=	0,
									bDB_FrequencySecured							=	0, 
									bDB_SimpleSpuriousTest							=	0,
									bDB_UseTestFrequencySetting						=	0,
									bDB_UseInternalFrequencyLoop					=	0;
	
	int								bDB_EnableToStoreMeasureData					=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "TX Spurious Test.");
	
	GET_TEST_PREFIX(pCurrentTestPrefix);  
	
	GET_SA_EQUIPMENT_HANDLE(hSpectrumAnalyzer);
	
	PATH_GET_CURRENT_INDEX( iCurrentPathIndex );
		
	GET_SEGMENT_LOOP_NUM(iCurrentTemperatureSegmentLoop);	
	GET_SEGMENT_NUMBER(iCurrentTemperatureSegment);
	
	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	bDB_UseTestFrequencySetting = GetVarInt( clb, VARTYPE_TEST , "UseTestFrequencySetting" );   
	
	bDB_UseInternalFrequencyLoop = GetVarInt( clb, VARTYPE_TEST , "UsePathIndexForFrequency" );
	
	GET_FREQUENCIES_LOOP_INFO_SINGLE( lfCurrentFrequency , bDB_FrequencySecured , iDB_FrequencyIndex );
	
	if ( bDB_UseTestFrequencySetting )
	{
		iDB_NumberOfFrequencies = GetVarDoubleArray( clb, VARTYPE_TEST , "Frequnecies" , &pvlfDB_Frequencies);
		
		if ( iDB_FrequencyIndex < iDB_NumberOfFrequencies )
			lfCurrentFrequency = pvlfDB_Frequencies[iDB_FrequencyIndex];
		else
			lfCurrentFrequency = pvlfDB_Frequencies[0];
	}
	
	lfDB_MeasureDelay = GetVarDouble( clb, VARTYPE_TEST , "Measure_Delay" );

	bDB_SimpleSpuriousTest = GetVarInt( clb, VARTYPE_TEST , "SimpleSpuriousTest" );
	
	if ( bDB_SimpleSpuriousTest == 0 )
	{
		lfDB_Spurious_Start_Band_Freq = GetVarDouble( clb, VARTYPE_TEST , "Spurious_Start_Band_Freq" );
		lfDB_Spurious_Stop_Band_Freq = GetVarDouble( clb, VARTYPE_TEST , "Spurious_Stop_Band_Freq" );
	
		lfDB_Spurious_Carrier_Isolation_Freq = GetVarDouble( clb, VARTYPE_TEST , "Spurious_Carrier_Isolation_Freq" );  
	
		if ( lfDB_Spurious_Carrier_Isolation_Freq > 0.0 )
		{
			pDB_SpectrumAnalyzerCarrierState = GetVarString( clb , VARTYPE_EQUIPMENT_USE , "Carrier_State_File" , hSpectrumAnalyzer  );           
			pDB_SpectrumAnalyzerSpuriousState = GetVarString( clb , VARTYPE_EQUIPMENT_USE , "Spurious_State_File" , hSpectrumAnalyzer  );           
		}
	}
	
	if ( pDB_SpectrumAnalyzerCarrierState == NULL )
	{
		pDB_SpectrumAnalyzerCarrierState = GetVarString( clb, VARTYPE_EQUIPMENT_USE , "State_FileName" , hSpectrumAnalyzer );  	
	}
	
	lfMaxSpurious = 1000.0;
	
	iDB_NumberOfSweeps = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Number_Of_Sweeps" , hSpectrumAnalyzer );
	lfDB_SweepTimeout = GetVarDouble( clb, VARTYPE_EQUIPMENT_USE , "Sweep_Timeout" , hSpectrumAnalyzer );
	bDB_UseMaxHold = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "UseMaxHold" , hSpectrumAnalyzer ); 

	if ( iDB_NumberOfSweeps < 1 )
		iDB_NumberOfSweeps = 1;
	
	CHK_STDERR( DRV_SpectrumAnalyzer_GetLastStateFileName( hSpectrumAnalyzer , &pszGethSpectrumAnalyzerStateNumber ));
	
	if (( pszGethSpectrumAnalyzerStateNumber == NULL ) || ( strcmp(pszGethSpectrumAnalyzerStateNumber,pDB_SpectrumAnalyzerCarrierState)))
	{
		DRV_SpectrumAnalyzer_Reset(hSpectrumAnalyzer);  
		
		sprintf( szFormatedString , "Spectrum Analyzer :: Loading state (%s)" , pDB_SpectrumAnalyzerCarrierState );  
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
		CHK_STDERR( DRV_SpectrumAnalyzer_Recall ( hSpectrumAnalyzer , pDB_SpectrumAnalyzerCarrierState )); 
	}
	
	CHK_STDERR( DRV_SpectrumAnalyzer_SetSweepMode( hSpectrumAnalyzer , 0 ));
	
	CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold ( hSpectrumAnalyzer , 0 ));

	CHK_STDERR( DRV_SpectrumAnalyzer_WaitForIntrumentDone ( hSpectrumAnalyzer , lfDB_SweepTimeout ));
	
	if ( bDB_UseMaxHold )
	{
		LOG(CLB_LINE_NORMAL, "Spectrum Analyzer :: SetMaxhold On" );
		
		CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold ( hSpectrumAnalyzer , iDB_NumberOfSweeps )); 
	}
	
	CHK_STDERR( DRV_SpectrumAnalyzer_RestartMeasure ( hSpectrumAnalyzer ));
	
	WAIT("",lfDB_MeasureDelay); 
	
	LOG(CLB_LINE_NORMAL, "Spectrum Analyzer :: Waiting for sweep..." );  
	
	START_TIMEOUT( "" , lfDB_SweepTimeout );
	
	CHK_STDERR( DRV_SpectrumAnalyzer_GetSweep ( hSpectrumAnalyzer , lfDB_SweepTimeout ));
	CHK_STDERR( DRV_SpectrumAnalyzer_SearchMarkerPeak( hSpectrumAnalyzer , 1 )); 

	STOP_TIMEOUT;
	
	CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerFrequency( hSpectrumAnalyzer , 1 , &lfMarkerFrequency )); 
	CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerAmplitude( hSpectrumAnalyzer , 1 , &lfMarkerAmplitude )); 

	if ( bDB_UseMaxHold )
	{
		LOG(CLB_LINE_NORMAL, "Spectrum Analyzer :: SetMaxhold Off" );
		CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold( hSpectrumAnalyzer , 0 ));
	}
	
	CHK_STDERR( DRV_SpectrumAnalyzer_SetSweepMode( hSpectrumAnalyzer , 1 ));
	
	PATH_GET_LOSS_VALUE( lfMarkerFrequency , lfLossFactor );

	if ( bDB_FrequencySecured ) 
		sprintf( szFormatedString , "Loss Factor at is %0.5lf dBm" , lfLossFactor );
	else
		sprintf( szFormatedString , "Loss Factor at (%0.3lf MHz) = %0.5lf dBm" , ( lfMarkerFrequency / 1E6 ) , lfLossFactor );

	LOG(CLB_LINE_NORMAL, szFormatedString ); 

	if ( bDB_FrequencySecured )
		sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf dBm" , lfMarkerAmplitude );   
	else
		sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf MHz / %0.3lf dBm" , (lfMarkerFrequency / 1E6) , lfMarkerAmplitude );   
	
	LOG(CLB_LINE_NORMAL, szFormatedString ); 

	CHK_STDERR( DRV_SpectrumAnalyzer_GetReferenceLevel( hSpectrumAnalyzer , &lfReferenceLevel ));  
		
	CHK_STDERR( DRIVER_MANAGER_CALIBRATION_Gel_3D_Factor( hSpectrumAnalyzer , lfReferenceLevel , lfMarkerFrequency , &lfSpectrumAnalyzerLossFactor ));  

	if ( lfSpectrumAnalyzerLossFactor < 0.0 )
	{
		sprintf( szFormatedString , "Spectrum Analyzer :: Factor = %0.3lf dBm" , lfSpectrumAnalyzerLossFactor );  
		LOG(CLB_LINE_NORMAL,szFormatedString);

		lfLossFactor += lfSpectrumAnalyzerLossFactor;
	}

	lfCarrierPower = lfMarkerAmplitude - lfLossFactor;

	sprintf( szFormatedString , "%0.3lf GHz" , ( lfMarkerFrequency / 1E9 ));
	RESULT_CONST_NUM( 1 , CLB_RESULT_DOUBLE , szFormatedString , lfCarrierPower , "result"); 
	
	if ( pDB_SpectrumAnalyzerSpuriousState )
	{
		sprintf( szFormatedString , "Loading spectrum analyzer state (%s)" , pDB_SpectrumAnalyzerSpuriousState );   //1
		LOG(CLB_LINE_NORMAL, szFormatedString ); 

		CHK_STDERR( DRV_SpectrumAnalyzer_Recall ( hSpectrumAnalyzer , pDB_SpectrumAnalyzerSpuriousState )); 

		CHK_STDERR( DRV_SpectrumAnalyzer_GetStartStopFrequency ( hSpectrumAnalyzer , &lfStartFrequency , &lfStopFrequency ));     
	
		lfSpanFrequency = lfStopFrequency - lfStartFrequency;
		lfCenterFrequency = lfDB_Spurious_Start_Band_Freq - (lfSpanFrequency / 2.0 );
	
		do
		{
			lfCenterFrequency += lfSpanFrequency;
		
			if ( lfCenterFrequency > lfDB_Spurious_Stop_Band_Freq )
				break;
		
			if ( ( lfCenterFrequency + (lfSpanFrequency / 2.0 ) > lfDB_Spurious_Stop_Band_Freq )) 
			{
				lfCenterFrequency = lfDB_Spurious_Stop_Band_Freq - (lfSpanFrequency / 2.0 );  	
			}
		
			if ((( lfCenterFrequency + (lfSpanFrequency / 2.0 ) > ( pvlfDB_Frequencies[iFrequencyIndex] - lfDB_Spurious_Carrier_Isolation_Freq ))) && (( lfCenterFrequency - (lfSpanFrequency / 2.0 ) < ( pvlfDB_Frequencies[iFrequencyIndex] + lfDB_Spurious_Carrier_Isolation_Freq ))))
			{
				if ( lfCenterFrequency < pvlfDB_Frequencies[iFrequencyIndex] )
					lfCenterFrequency = ( pvlfDB_Frequencies[iFrequencyIndex] - lfDB_Spurious_Carrier_Isolation_Freq ) - (lfSpanFrequency / 2.0 );  	
				else
					lfCenterFrequency = ( pvlfDB_Frequencies[iFrequencyIndex] + lfDB_Spurious_Carrier_Isolation_Freq ) + (lfSpanFrequency / 2.0 );  	
			}
		
			sprintf( szFormatedString , "Set spectrum analyzer frequency %0.3lf GHz" , ( lfCenterFrequency / 1E9) );   
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
			CHK_STDERR( DRV_SpectrumAnalyzer_SetFrequency ( hSpectrumAnalyzer , lfCenterFrequency )); 
	
			if ( bDB_UseMaxHold )
			{
				LOG(CLB_LINE_NORMAL, "Spectrum Analyzer :: SetMaxhold On" );
	
				CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold ( hSpectrumAnalyzer , iDB_NumberOfSweeps )); 
			}
		
			WAIT("",lfDB_MeasureDelay); 

			LOG(CLB_LINE_NORMAL, "Spectrum Analyzer :: Waiting for sweep..." );  
			
			START_TIMEOUT( "" , lfDB_SweepTimeout ); 
			
			CHK_STDERR( DRV_SpectrumAnalyzer_GetSweep ( hSpectrumAnalyzer , lfDB_SweepTimeout ));
			CHK_STDERR( DRV_SpectrumAnalyzer_SearchMarkerPeak( hSpectrumAnalyzer , 1 )); 

			STOP_TIMEOUT;
			
			CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerFrequency( hSpectrumAnalyzer , 1 , &lfMarkerFrequency )); 
			CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerAmplitude( hSpectrumAnalyzer , 1 , &lfMarkerAmplitude )); 

			LOG(CLB_LINE_NORMAL, "Release spectrum analyzer MaxHold" );
			CHK_STDERR( DRV_SpectrumAnalyzer_SetMaxHold( hSpectrumAnalyzer , 0 ));
		
			sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf GHz / %0.3lf dBm" , (lfMarkerFrequency / 1E9) , lfMarkerAmplitude );   
			LOG(CLB_LINE_NORMAL, szFormatedString ); 

			PATH_GET_LOSS_VALUE( lfMarkerFrequency , lfLossFactor );	

			sprintf( szFormatedString , "Path Loss Factor = %0.5lf dBm" , lfLossFactor );
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
			DRV_SpectrumAnalyzer_GetReferenceLevel( hSpectrumAnalyzer , &lfReferenceLevel );
			DRIVER_MANAGER_CALIBRATION_Gel_3D_Factor( hSpectrumAnalyzer , lfReferenceLevel , lfMarkerFrequency , &lfSpectrumAnalyzerLossFactor );  
	
			sprintf( szFormatedString , "Spectrum Analyzer Loss Factor = %0.5lf dBm (%0.3lf dB)" , lfSpectrumAnalyzerLossFactor , lfReferenceLevel );
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
			lfLossFactor += lfSpectrumAnalyzerLossFactor;
	
			lfCurrentSpur = lfCarrierPower - ( lfMarkerAmplitude - lfLossFactor );
		
			if ( lfCurrentSpur < lfMaxSpurious )
			{
				lfMaxSpurious = lfCurrentSpur;
				lfMaxSpuriousFreq = lfMarkerFrequency;
			}
		
		} while ( lfCenterFrequency + (lfSpanFrequency / 2.0 ) < lfDB_Spurious_Stop_Band_Freq );    
	
		sprintf( szFormatedString , "%0.3lf GHz" , ( lfMaxSpuriousFreq / 1E9 ));
		RESULT_CONST_NUM( 2 , CLB_RESULT_DOUBLE , szFormatedString , lfMaxSpurious , "result" ); 
	}
		
CATCH

	UPDATE_ERROR(StdError);
	
	if ( StdError.pszErrorDescription )
	{
		LOG_END(CLB_LINE_WARNING, (char*)StdError.pszErrorDescription );
	}

	iTestError = -1;
	
FINALLY

	FREE(pvlfDB_Frequencies);
	
	FREE(pszGethSpectrumAnalyzerStateNumber);
	FREE(pDB_SpectrumAnalyzerCarrierState);
	FREE(pDB_SpectrumAnalyzerSpuriousState);
	
	return iTestError;
}
