// Include files
#include "Windows.h"
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


TSTE_TEST ( STD_RF_ON_OFF_TEST )
{
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	char							szFormatedString[512]							=	{0},
									szFormatedVariable[512]							=	{0};
	
	int								bDB_RF_ON										=	0;
	
	int								iDB_Channel										=	0;
	
	char							*pCurrentTestPrefix								=	NULL;

	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;	
	
	int								iCurrentPathIndex								=	0;
	
	double							*pvlfDB_Powers									=	NULL,
									*pvlfDB_Frequencies								=	NULL; 

	double							lfCurrentFrequency								=	0.0,
									lfLossFactor									=	0.0,  
									lfCurrentPower									=	0.0, 
									lfSignalGeneratorLossFactor						=	0.0;
									
	int								iDB_NumberOfFrequnecies							=	0,
									iDB_FrequencyIndex								=	0, 
									iDB_NumberOfPowers								=	0;
									
	int								bDB_UseTestFrequencySetting						=	0,
									bDB_UseInternalFrequencyLoop					=	0,
									bDB_FrequencySecured							=	0; 
									
	int								hSignalGenerator								=	0;
	
	int								bDB_EnableToStoreMeasureData					=	0;
	
	bDB_RF_ON = GetVarInt( clb, VARTYPE_TEST , "RF_ON" );
	
	GET_SG_EQUIPMENT_HANDLE(hSignalGenerator);
	
	if ( bDB_RF_ON )
	{
		LOG(CLB_LINE_HEADER_TEST, "RF ON Test."); 
	}
	else
	{
		LOG(CLB_LINE_HEADER_TEST, "RF OFF Test."); 
	}
	
	iDB_Channel = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Channel" , hSignalGenerator );
	
	bDB_UseTestFrequencySetting = GetVarInt( clb, VARTYPE_TEST , "UseTestFrequencySetting" );   
	
	bDB_UseInternalFrequencyLoop = GetVarInt( clb, VARTYPE_TEST , "UsePathIndexForFrequency" );
	
	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	GET_FREQUENCIES_LOOP_INFO_SINGLE( lfCurrentFrequency , bDB_FrequencySecured , iDB_FrequencyIndex );
	
	LOG(CLB_LINE_NORMAL, "Signal Generator :: Power Off" );
	CHK_STDERR( DRV_SignalGenerator_RF_Off ( hSignalGenerator , iDB_Channel ));  
	
	LOG(CLB_LINE_NORMAL, "Signal Generator ::Reset" );
	CHK_STDERR( DRV_SignalGenerator_Reset ( hSignalGenerator )); 
		
	if ( bDB_RF_ON )
	{
		GET_TEST_PREFIX(pCurrentTestPrefix); 
		
		PATH_GET_CURRENT_INDEX( iCurrentPathIndex );
		
		if ( bDB_UseInternalFrequencyLoop )
			iDB_FrequencyIndex = iCurrentPathIndex;
			
		GET_SEGMENT_LOOP_NUM(iCurrentTemperatureSegmentLoop);	
		GET_SEGMENT_NUMBER(iCurrentTemperatureSegment);
	
		if ( bDB_UseTestFrequencySetting )
		{
			iDB_NumberOfFrequnecies = GetVarDoubleArray( clb, VARTYPE_TEST , "Frequnecies" , &pvlfDB_Frequencies);
			
			if ( iDB_FrequencyIndex < iDB_NumberOfFrequnecies )
				lfCurrentFrequency = pvlfDB_Frequencies[iDB_FrequencyIndex];
			else
				lfCurrentFrequency = pvlfDB_Frequencies[0];
		}
		
		iDB_NumberOfPowers = GetVarDoubleArray( clb, VARTYPE_TEST , "Powers" , &pvlfDB_Powers); 
		
		PATH_GET_LOSS_VALUE( lfCurrentFrequency , lfLossFactor ); 
	
		sprintf( szFormatedString , "Loss Factor = %0.5lf dBm" , lfLossFactor );
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
		if ( iDB_FrequencyIndex < iDB_NumberOfPowers )
			lfCurrentPower = pvlfDB_Powers[iDB_FrequencyIndex];
		else
			lfCurrentPower = pvlfDB_Powers[0];
		
		CHK_STDERR( DRIVER_MANAGER_CALIBRATION_Gel_3D_Factor( hSignalGenerator , lfCurrentPower , lfCurrentFrequency , &lfSignalGeneratorLossFactor ));  
			
		if ( lfSignalGeneratorLossFactor < 0.0 )
		{
			sprintf( szFormatedString , "Signal Generator Factor = %0.3lf dBm" , lfSignalGeneratorLossFactor );  
			LOG(CLB_LINE_NORMAL,szFormatedString);
		}
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_RealPower" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , iDB_FrequencyIndex );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfCurrentPower ); 
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_RealPower_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , iDB_FrequencyIndex );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfCurrentFrequency );
		}
		
		lfCurrentPower = lfCurrentPower - lfLossFactor - lfSignalGeneratorLossFactor;
		
		sprintf( szFormatedString , "Signal Generator :: Set Power to %0.3lf dBm" , lfCurrentPower );  
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_PresetPower" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , iDB_FrequencyIndex );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfCurrentPower ); 
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Frequency_%d_PresetPower_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , iDB_FrequencyIndex );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfCurrentFrequency );
		}
		
		CHK_STDERR( DRV_SignalGenerator_SetPower ( hSignalGenerator , 0 , lfCurrentPower )); 
			
		if ( bDB_FrequencySecured == 0 )
			sprintf( szFormatedString , "Signal Generator :: Set Frequency Nr - %d" , ( iDB_FrequencyIndex + 1 ));  
		else
			sprintf( szFormatedString , "Signal Generator :: Set Frequency to %0.3lf MHz" , ( lfCurrentFrequency / 1E6 ));  

		LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
		CHK_STDERR( DRV_SignalGenerator_SetFrequency ( hSignalGenerator , 0 , lfCurrentFrequency )); 
		
		LOG(CLB_LINE_NORMAL, "Signal Generator :: RF On" );  
			
		CHK_STDERR( DRV_SignalGenerator_RF_On( hSignalGenerator , 0 ));
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

	FREE( pvlfDB_Powers );
	FREE( pvlfDB_Frequencies );
	
	return iTestError;
}


