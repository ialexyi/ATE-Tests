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

TSTE_TEST ( STD_POWER_CHANGE_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	char							szFormatedString[512]							=	{0},
									szFormatedVariable[512]							=	{0};
	
	int								iDB_PowerSupply_NumberOfChannel					=	0;
	
	int								iDB_NumberOfMeasures							=	0,
									iMeasureIndex									=	0,
									iIndex											=	0;
	
	double							lfDB_MeasureDelay								=	0.0,
									lfDB_PowerUpDelay								=	0.0,  
									lfDB_PowerDownDelay								=	0.0;
									
	tsPowerSupplyItem				*ptPowerSupplyList								=	NULL;
									
	int								*viDB_PowerSupply_Channel_List					=	NULL;
	
	double							lfVoltage										=	0.0,
									lfCurrent										=	0.0,
									lfFrequency										=	0.0,
									lfPhase											=	0.0,
									lfAverageValue									=	0.0;   
									
	int								bIsItOutputStar						  			=	0;
	
	int								iCurrentChannel									=	0;
	
	char							*pCurrentTestPrefix								=	NULL;

	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;
	
	int								iCurrentPathIndex								=	0;
	
	int								bDB_Power_UP_or_DOWN							=	0, 
									bDB_Change_Power_Settings						=	0, 
									bDB_In_Test_Configuration						=	0,
									bDB_EnableToStoreMeasureData					=	0;

	char							*pszPowerSupplyName								=	NULL;
	
	int								vartype											=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Power Change.");
	
	GET_TEST_PREFIX(pCurrentTestPrefix); 
		
	PATH_GET_CURRENT_INDEX( iCurrentPathIndex );
		
	GET_SEGMENT_LOOP_NUM(iCurrentTemperatureSegmentLoop);	
	GET_SEGMENT_NUMBER(iCurrentTemperatureSegment);
	
	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" );
	
	bDB_Change_Power_Settings = GetVarInt( clb, VARTYPE_TEST , "Change_Power_Settings" );
	
	bDB_Power_UP_or_DOWN = GetVarInt( clb, VARTYPE_TEST , "Power_UP_or_DOWN" ); 
	
	bDB_In_Test_Configuration = GetVarInt( clb, VARTYPE_TEST , "In_Test_Configuration" );
	
	if ( bDB_In_Test_Configuration )
		vartype = VARTYPE_TEST;
	else
		vartype = VARTYPE_UUTVER;
		
	iDB_PowerSupply_NumberOfChannel = GetVarIntArray( clb, vartype , "PowerSupply_Channel_List" , &viDB_PowerSupply_Channel_List ); 
	
	if (( iDB_PowerSupply_NumberOfChannel == 0 ) || ( viDB_PowerSupply_Channel_List == NULL ))
	{
		iDB_PowerSupply_NumberOfChannel = GetVarInt( clb, vartype , "PowerSupply_NumberOfFeeds" ); 
	}
	
	CALLOC_ERR( ptPowerSupplyList , iDB_PowerSupply_NumberOfChannel , sizeof(tsPowerSupplyItem));
	
	//---------------------------- Get variables from DB -----------------------------------------------------//
	iDB_NumberOfMeasures = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Measures" );
	
	lfDB_MeasureDelay = GetVarDouble( clb, VARTYPE_TEST , "Measure_Delay" );
	
	if ( bDB_Power_UP_or_DOWN )
		lfDB_PowerUpDelay = GetVarDouble( clb, VARTYPE_TEST , "Power_UP_Delay" ); 
	else
		lfDB_PowerDownDelay = GetVarDouble( clb, VARTYPE_TEST , "Power_DOWN_Delay" );

	//--------------------------------------------------------------------------------------------------------//
	
	if ( bDB_Change_Power_Settings )
	{	 
		for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
		{
			iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex];  
		
			sprintf( szFormatedString , "Feed_%d_VoltagePreset" , iCurrentChannel ); 
			ptPowerSupplyList[iIndex].lfSetup_Voltage = GetVarDouble( clb, vartype , szFormatedString ); 
		
			sprintf( szFormatedString , "Feed_%d_CurrentPreset" , iCurrentChannel ); 
			ptPowerSupplyList[iIndex].lfSetup_Current = GetVarDouble( clb, vartype , szFormatedString ); 
	
			sprintf( szFormatedString , "Feed_%d_OverVoltage" , iCurrentChannel );
			ptPowerSupplyList[iIndex].lfSetup_OverVoltage = GetVarDouble( clb, vartype , szFormatedString );

			sprintf( szFormatedString , "Feed_%d_MaxCurrent" , iCurrentChannel );
			ptPowerSupplyList[iIndex].lfSetup_MaxCurrent = GetVarDouble( clb, vartype , szFormatedString ); 
												   
			sprintf( szFormatedString , "Feed_%d_IsItAcPowerSupply" , iCurrentChannel );
			ptPowerSupplyList[iIndex].bIsItAcPowerSupply = GetVarInt( clb, vartype , szFormatedString );    
		
			sprintf( szFormatedString , "Feed_%d_Range" , iCurrentChannel );
			ptPowerSupplyList[iIndex].iSetup_Range = GetVarDouble( clb, vartype , szFormatedString ); 
		
			FREE(pszPowerSupplyName);
		
			sprintf( szFormatedString , "Feed_%d_PS_Use_Name" , iCurrentChannel );
			pszPowerSupplyName = GetVarString( clb, vartype , szFormatedString  ); 

			if ( pszPowerSupplyName && ( strlen(pszPowerSupplyName) > 1 ))
			{
				GET_PS_EQUIPMENT_HANDLE_BY_ALIAS( pszPowerSupplyName , ptPowerSupplyList[iIndex].hPowerSupplyHandle );  	
			}
			else
			{
				 BREAK_CURRENT_SEQUENCE;	
			}

			sprintf( szFormatedString , "Feed_%d_ChannelNumber" , iCurrentChannel ); 
		
			ptPowerSupplyList[iIndex].iChannelNumber = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , szFormatedString , ptPowerSupplyList[iIndex].hPowerSupplyHandle );
		
			CHK_STDERR( DRV_PowerSupply_SetOutputState( ptPowerSupplyList[iIndex].hPowerSupplyHandle , ptPowerSupplyList[iIndex].iChannelNumber  , CLEAR ));
		}
	}
	else
	{
		for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
		{
			iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex];  
		
			FREE(pszPowerSupplyName);
		
			sprintf( szFormatedString , "Feed_%d_PS_Use_Name" , iCurrentChannel );
			pszPowerSupplyName = GetVarString( clb, vartype , szFormatedString  ); 

			if ( pszPowerSupplyName && ( strlen(pszPowerSupplyName) > 1 ))
			{
				GET_PS_EQUIPMENT_HANDLE_BY_ALIAS( pszPowerSupplyName , ptPowerSupplyList[iIndex].hPowerSupplyHandle );  	
			}
			else
			{
				 BREAK_CURRENT_SEQUENCE;	
			}

			sprintf( szFormatedString , "Feed_%d_ChannelNumber" , iCurrentChannel ); 
		
			ptPowerSupplyList[iIndex].iChannelNumber = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , szFormatedString , ptPowerSupplyList[iIndex].hPowerSupplyHandle );
		}
	}
	
//---------------------------- Config power supply -----------------------------------------------------//
	
	if ( bDB_Change_Power_Settings )
	{
		LOG(CLB_LINE_NORMAL, "Configurating power supply..." );
	
		for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
		{
			iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex];
	
			if ( ptPowerSupplyList[iIndex].iSetup_Range >= 0 )
			{
				CHK_STDERR( DRV_PowerSupply_SetRange ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , ptPowerSupplyList[iIndex].iChannelNumber , ptPowerSupplyList[iIndex].iSetup_Range ));
			}
	
			if ( ptPowerSupplyList[iIndex].lfSetup_OverVoltage > 0.0 )
			{
				CHK_STDERR( DRV_PowerSupply_SetVoltageProtectionOver ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 	ptPowerSupplyList[iIndex].iChannelNumber , ptPowerSupplyList[iIndex].lfSetup_OverVoltage ));   
			}
		
			CHK_STDERR( DRV_PowerSupply_SetOutputVoltage ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 			ptPowerSupplyList[iIndex].iChannelNumber , ptPowerSupplyList[iIndex].lfSetup_Voltage )); 
			CHK_STDERR( DRV_PowerSupply_SetOutputCurrent ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 			ptPowerSupplyList[iIndex].iChannelNumber , ptPowerSupplyList[iIndex].lfSetup_Current )); 

			CHK_STDERR( DRV_PowerSupply_GetOutputVoltageDefined ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 	ptPowerSupplyList[iIndex].iChannelNumber , &lfVoltage ));
			CHK_STDERR( DRV_PowerSupply_GetOutputCurrentDefined ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 	ptPowerSupplyList[iIndex].iChannelNumber , &lfCurrent ));  
	
			sprintf( szFormatedString , "Can't set %0.1lfV power supply, voltage preset." , ptPowerSupplyList[iIndex].lfSetup_Voltage );
			IF (( lfVoltage != ptPowerSupplyList[iIndex].lfSetup_Voltage ) ,  szFormatedString );
		
			sprintf( szFormatedString , "Can't set %0.1lfV power supply, current preset." , ptPowerSupplyList[iIndex].lfSetup_Voltage );
			IF (( lfCurrent != ptPowerSupplyList[iIndex].lfSetup_Current ) ,  szFormatedString );
		
			// If power supply is AC case
			if( ptPowerSupplyList[iIndex].bIsItAcPowerSupply )
			{
				// Get  Variables from DB
				sprintf( szFormatedString , "Feed_%d_Frequency" , iCurrentChannel );
				ptPowerSupplyList[iIndex].lfSetup_Frequency = GetVarDouble( clb, vartype , szFormatedString ); 
			
				sprintf( szFormatedString , "Feed_%d_IsItOutputStar" , iCurrentChannel );
				ptPowerSupplyList[iIndex].bIsItOutputStar = GetVarInt( clb, vartype , szFormatedString );
			
				sprintf( szFormatedString , "Feed_%d_Phase" , iCurrentChannel );
				ptPowerSupplyList[iIndex].lfSetup_Phase = GetVarDouble( clb, vartype , szFormatedString );
			
				// Set Config for AC PowerSupply	     
			
				CHK_STDERR( DRV_PowerSupply_SetFrequency ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 			ptPowerSupplyList[iIndex].iChannelNumber , ptPowerSupplyList[iIndex].lfSetup_Frequency)); 
				CHK_STDERR( DRV_PowerSupply_SetOutputStarState( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 		ptPowerSupplyList[iIndex].iChannelNumber , ptPowerSupplyList[iIndex].bIsItOutputStar));
				CHK_STDERR( DRV_PowerSupply_SetStarPhase( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 			ptPowerSupplyList[iIndex].iChannelNumber , ptPowerSupplyList[iIndex].lfSetup_Phase));
			
				// Verify config for AC PowerSupply
			
				CHK_STDERR( DRV_PowerSupply_GetFrequency ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 	ptPowerSupplyList[iIndex].iChannelNumber , &lfFrequency ));
				CHK_STDERR( DRV_PowerSupply_GetStarPhase ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 	ptPowerSupplyList[iIndex].iChannelNumber , &lfPhase ));  
				CHK_STDERR( DRV_PowerSupply_GetOutputStarState ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 	ptPowerSupplyList[iIndex].iChannelNumber , &bIsItOutputStar ));
	
				sprintf( szFormatedString , "Can't set %0.1lfHz power supply, frequency preset." , ptPowerSupplyList[iIndex].lfSetup_Frequency );
				IF (( lfFrequency != ptPowerSupplyList[iIndex].lfSetup_Frequency ) ,  szFormatedString );
		
				sprintf( szFormatedString , "Can't set %0.1lf° power supply, phase preset." , ptPowerSupplyList[iIndex].lfSetup_Phase );
				IF (( lfPhase != ptPowerSupplyList[iIndex].lfSetup_Phase ) ,  szFormatedString );
			
				if ( bIsItOutputStar != ptPowerSupplyList[iIndex].bIsItOutputStar )
				{
					if ( ptPowerSupplyList[iIndex].bIsItOutputStar )
						sprintf( szFormatedString , "Can't set power supply mode to Star.");
					else
						sprintf( szFormatedString , "Can't set power supply mode to Triangle.");
				}
			
				IF (( bIsItOutputStar != ptPowerSupplyList[iIndex].bIsItOutputStar ) ,  szFormatedString );
			}
		}
	}
	
	if ( bDB_Power_UP_or_DOWN )
	{
		LOG(CLB_LINE_NORMAL, "Power Up." );  
		//---------------------------- Power UP -----------------------------------------------------// 
	
		for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
		{
			iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex];  
		
			CHK_STDERR( DRV_PowerSupply_SetOutputState( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 	ptPowerSupplyList[iIndex].iChannelNumber , SET ));
		}
	
		WAIT("",lfDB_PowerUpDelay); 
	}
	else
	{
		LOG(CLB_LINE_NORMAL, "Power Down." );  
		//---------------------------- Power Down -----------------------------------------------------// 
	
		for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
		{
			iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex];  
		
			CHK_STDERR( DRV_PowerSupply_SetOutputState( ptPowerSupplyList[iIndex].hPowerSupplyHandle , 	ptPowerSupplyList[iIndex].iChannelNumber , CLEAR ));
		}
	
		WAIT("",lfDB_PowerDownDelay); 
	}
	
	if ( bDB_Change_Power_Settings )
	{
		//---------------------------- Get voltage parameters -----------------------------------------------------// 
		for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
		{
			iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex];  
		
			CHK_STDERR( DRV_PowerSupply_GetOutputVoltageActual ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , ptPowerSupplyList[iIndex].iChannelNumber , &lfVoltage )); 
			sprintf( szFormatedString , "Voltage Measure (%0.1lfV) : %0.3lf" , ptPowerSupplyList[iIndex].lfSetup_Voltage , lfVoltage );
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		}
	
	
		//---------------------------- Check max current -----------------------------------------------------// 
		for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
		{
			iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex];  
		
			CHK_STDERR( DRV_PowerSupply_GetOutputCurrentActual ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , ptPowerSupplyList[iIndex].iChannelNumber , &lfCurrent ));  
		
			sprintf( szFormatedString , "Current Overflow in %0.1lfV power supply." , ptPowerSupplyList[iIndex].lfSetup_Voltage );
			IF (( lfCurrent >= ptPowerSupplyList[iIndex].lfSetup_MaxCurrent ) ,  szFormatedString); 
		}
	}
	
	if ( iDB_NumberOfMeasures > 0 )
	{
		LOG(CLB_LINE_NORMAL, "Start current measure" );  
	
		for ( iMeasureIndex = 0; iMeasureIndex < iDB_NumberOfMeasures; iMeasureIndex++ )
		{
			for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
			{
				iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex];  
			
				CHK_STDERR( DRV_PowerSupply_GetOutputCurrentActual ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , ptPowerSupplyList[iIndex].iChannelNumber , &lfCurrent ));
	
				sprintf( szFormatedString , "Current Measure (%0.1lfV) : %0.3lf A" , ptPowerSupplyList[iIndex].lfSetup_Voltage , lfCurrent );
				LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
				ptPowerSupplyList[iIndex].lfMeasure_Current += lfCurrent;
		
				if (( lfCurrent > ptPowerSupplyList[iIndex].lfMeasure_MaxValue ) || ( iMeasureIndex == 0 ))
					ptPowerSupplyList[iIndex].lfMeasure_MaxValue = lfCurrent;
		
				if (( lfCurrent < ptPowerSupplyList[iIndex].lfMeasure_MinValue ) || ( iMeasureIndex == 0 ))  
					ptPowerSupplyList[iIndex].lfMeasure_MinValue = lfCurrent;
			
				ptPowerSupplyList[iIndex].iNumberOfMeasures++;
			}										 
																							 
			WAIT("",lfDB_MeasureDelay);
		}
	
		LOG(CLB_LINE_NORMAL, "Measure completed" );  	
	
		for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
		{
			iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex];  
		
			sprintf( szFormatedString , "%0.1lfV" , ptPowerSupplyList[iIndex].lfSetup_Voltage );
			
			lfAverageValue = (ptPowerSupplyList[iIndex].lfMeasure_Current / (double)(ptPowerSupplyList[iIndex].iNumberOfMeasures));
		
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Channel%d_Average" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iIndex+1) );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfAverageValue ); 
			}
		
			RESULT( CLB_RESULT_DOUBLE , szFormatedString , lfAverageValue , "Average Value" ); 
		
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Channel%d_Maximum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iIndex+1) );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , ptPowerSupplyList[iIndex].lfMeasure_MaxValue );
			}
		
			RESULT( CLB_RESULT_DOUBLE , szFormatedString , ptPowerSupplyList[iIndex].lfMeasure_MaxValue , "Maximum Value" );
		
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Channel%d_Minimum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iIndex+1) );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , ptPowerSupplyList[iIndex].lfMeasure_MinValue );
			}
		
			RESULT( CLB_RESULT_DOUBLE , szFormatedString , ptPowerSupplyList[iIndex].lfMeasure_MinValue , "Minimum Value"); 
		}
	}
	
CATCH

	UPDATE_ERROR(StdError);
	
	if ( StdError.pszErrorDescription )
	{
		LOG_END(CLB_LINE_WARNING, (char*)StdError.pszErrorDescription );
	}
	
	for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
	{
		iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex];  
		DRV_PowerSupply_SetOutputState( ptPowerSupplyList[iIndex].hPowerSupplyHandle , ptPowerSupplyList[iIndex].iChannelNumber , CLEAR );   
	}
	
	iTestError = -1;
	
FINALLY

	FREE(ptPowerSupplyList);
	FREE(viDB_PowerSupply_Channel_List);
	FREE(pCurrentTestPrefix);
	
	FREE(pszPowerSupplyName);
	
	return iTestError;
}
