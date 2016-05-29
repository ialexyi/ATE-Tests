// Include files
#include "Windows.h"
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


TSTE_TEST ( STD_POWER_DOWN_TEST )
{
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	char							szFormatedString[512]							=	{0},
									szFormatedVariable[512]							=	{0};
	
	int								iDB_PowerSupply_NumberOfChannel					=	0;
	
	int								iDB_NumberOfMeasures							=	0,
									iMeasureIndex									=	0,
									iIndex											=	0;
	
	double							lfDB_MeasureDelay								=	0.0;
									
	tsPowerSupplyItem				*ptPowerSupplyList								=	NULL;
									
	double							lfVoltage										=	0.0,
									ltCurrent										=	0.0,
									lfAverageValue									=	0.0;
	
	int								iCurrentChannel									=	0;
	
	char							*pCurrentTestPrefix								=	NULL;

	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;	
	
	int								iCurrentPathIndex								=	0;
	
	int								bDB_EnableToStoreMeasureData					=	0;

	char							*pszPowerSupplyName								=	NULL;

	int								*viDB_PowerSupply_Channel_List					=	NULL;
	
	LOG(CLB_LINE_HEADER_TEST, "Power Down Test."); 
	
	GET_TEST_PREFIX(pCurrentTestPrefix); 
		
	PATH_GET_CURRENT_INDEX( iCurrentPathIndex );
		
	GET_SEGMENT_LOOP_NUM(iCurrentTemperatureSegmentLoop);	
	GET_SEGMENT_NUMBER(iCurrentTemperatureSegment);
	
	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	iDB_PowerSupply_NumberOfChannel = GetVarIntArray( clb, VARTYPE_UUTVER , "PowerSupply_Channel_List" , &viDB_PowerSupply_Channel_List ); 
	
	if (( iDB_PowerSupply_NumberOfChannel == 0 ) || ( viDB_PowerSupply_Channel_List == NULL ))
	{
		iDB_PowerSupply_NumberOfChannel = GetVarInt( clb, VARTYPE_UUTVER , "PowerSupply_NumberOfFeeds" ); 
	}
	
	CALLOC_ERR( ptPowerSupplyList , iDB_PowerSupply_NumberOfChannel , sizeof(tsPowerSupplyItem));
	
	//---------------------------- Get variables from DB -----------------------------------------------------//
	iDB_NumberOfMeasures = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Measures" );
	
	lfDB_MeasureDelay = GetVarDouble( clb, VARTYPE_TEST , "Measure_Delay" );
	
	//--------------------------------------------------------------------------------------------------------//
	
	for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
	{
		iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex]; 
		
		sprintf( szFormatedString , "Feed_%d_VoltagePreset" , iCurrentChannel ); 
		ptPowerSupplyList[iIndex].lfSetup_Voltage = GetVarDouble( clb, VARTYPE_UUTVER , szFormatedString ); 
		
		sprintf( szFormatedString , "Feed_%d_CurrentPreset" , iCurrentChannel ); 
		ptPowerSupplyList[iIndex].lfSetup_Current = GetVarDouble( clb, VARTYPE_UUTVER , szFormatedString ); 
	
		sprintf( szFormatedString , "Feed_%d_OverVoltage" , iCurrentChannel );
		ptPowerSupplyList[iIndex].lfSetup_OverVoltage = GetVarDouble( clb, VARTYPE_UUTVER , szFormatedString );

		sprintf( szFormatedString , "Feed_%d_MaxCurrent" , iCurrentChannel );
		ptPowerSupplyList[iIndex].lfSetup_MaxCurrent = GetVarDouble( clb, VARTYPE_UUTVER , szFormatedString ); 

		FREE(pszPowerSupplyName);
		
		sprintf( szFormatedString , "Feed_%d_PS_Use_Name" , iCurrentChannel );
		pszPowerSupplyName = GetVarString( clb, VARTYPE_UUTVER , szFormatedString  ); 

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
	
	//---------------------------- Get voltage parameters -----------------------------------------------------// 
	for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
	{
		iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex]; 
		
		CHK_STDERR( DRV_PowerSupply_GetOutputVoltageActual ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , ptPowerSupplyList[iIndex].iChannelNumber , &lfVoltage )); 
		sprintf( szFormatedString , "Voltage Measure (%0.1lfV) : %0.3lf" , ptPowerSupplyList[iIndex].lfSetup_Voltage , lfVoltage );
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
	}
	
	if ( iDB_NumberOfMeasures > 0 )
	{
		LOG(CLB_LINE_NORMAL, "Start current measure" );  
	
		for ( iMeasureIndex = 0; iMeasureIndex < iDB_NumberOfMeasures; iMeasureIndex++ )
		{
			for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
			{
				iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex]; 
			
				CHK_STDERR( DRV_PowerSupply_GetOutputCurrentActual ( ptPowerSupplyList[iIndex].hPowerSupplyHandle , ptPowerSupplyList[iIndex].iChannelNumber , &ltCurrent ));
	
				sprintf( szFormatedString , "Current Measure (%0.1lfV) : %0.3lf A" , ptPowerSupplyList[iIndex].lfSetup_Voltage , ltCurrent );
				LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
				ptPowerSupplyList[iIndex].lfMeasure_Current += ltCurrent;
		
				if (( ltCurrent > ptPowerSupplyList[iIndex].lfMeasure_MaxValue ) || ( iMeasureIndex == 0 ))  
					ptPowerSupplyList[iIndex].lfMeasure_MaxValue = ltCurrent;
		
				if (( ltCurrent < ptPowerSupplyList[iIndex].lfMeasure_MinValue ) || ( iMeasureIndex == 0 ))  
					ptPowerSupplyList[iIndex].lfMeasure_MinValue = ltCurrent;
			
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
		
			RESULT( CLB_RESULT_DOUBLE , szFormatedString , ptPowerSupplyList[iIndex].lfMeasure_MinValue , "Minimum Value" ); 
		}
	}
	
	LOG(CLB_LINE_NORMAL, "Power Down." ); 

CATCH

	UPDATE_ERROR(StdError);
	
	if ( StdError.pszErrorDescription )
	{
		LOG_END(CLB_LINE_WARNING, (char*)StdError.pszErrorDescription );
	}
	
	iTestError = -1;
	
FINALLY
	
	for( iIndex = 0; iIndex < iDB_PowerSupply_NumberOfChannel; iIndex++ )
	{
		iCurrentChannel = viDB_PowerSupply_Channel_List[iIndex]; 
		DRV_PowerSupply_SetOutputState( ptPowerSupplyList[iIndex].hPowerSupplyHandle , ptPowerSupplyList[iIndex].iChannelNumber , CLEAR );   
	}
	
	FREE(ptPowerSupplyList);
	
	FREE(viDB_PowerSupply_Channel_List);
	
	LOG_END(CLB_LINE_NORMAL, "Finalizing Test." );

	FREE(pszPowerSupplyName);
	
	return iTestError;
}


