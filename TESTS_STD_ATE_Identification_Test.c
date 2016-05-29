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


TSTE_TEST ( STD_ID_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;

	char							*pGetCommandName								=	NULL,
									*pGetValue										=	NULL,
									*pCalibrationTable								=	NULL;
									
	int								iGetValueLength									=	0,
									iGetValueSize									=	0;

	char							szFormatedString[512]							=	{0},
									szSerialNumber[LOW_STRING]						=	{0},
									szVersionString[LOW_STRING]						=	{0},
									szUserSerialNumber[LOW_STRING]					=	{0},
									szVersionNameString[LOW_STRING]					=	{0},
									szVersionCompareString[LOW_STRING]				=	{0}; 		
	
	int								hUnitUnderTest									=	0;
									
	int								iVersionIndex									=	0;
	
	char							*pDB_SerialNumberCommand						=	NULL,
									*pDB_VersionCommand								=	NULL,
									*pDB_Version_Name_String						=	NULL, 
									*pDB_Version_Compare_String						=	NULL,
									*pDB_Version_Contructor_String					=	NULL, 
									*pDB_Serial_Number_Contructor_String			=	NULL; 
									
	double							lfDB_GetVersionDelay							=	0.0,
									lfDB_GetSerialNumberDelay						=	0.0;
	
	int								iDB_NumberOfVersionsToGet						=	0;
	
	int								bDB_EnableToStoreMeasureData					=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Identification Test."); 
	
	GET_UUT_EQUIPMENT_HANDLE(hUnitUnderTest);	
	
	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	//------------------- Serial number ----------------------------------------------------//
	pDB_SerialNumberCommand = GetVarString( clb, VARTYPE_TEST , "Get_Serial_Number_Command" );  
		
	if ( pDB_SerialNumberCommand && ( strlen(pDB_SerialNumberCommand)))
	{
		lfDB_GetSerialNumberDelay = GetVarDouble( clb, VARTYPE_TEST , "GetSerialNumberDelay" );  
		
		DRV_StandardDevice_SetValue( hUnitUnderTest , pDB_SerialNumberCommand , NULL , 0 , 1 );   
		DelayWithEventProcessing(lfDB_GetSerialNumberDelay);
		DRV_StandardDevice_GetValue( hUnitUnderTest , &pGetCommandName , (void**)&pGetValue , &iGetValueLength , &iGetValueSize );  
	
		pDB_Serial_Number_Contructor_String = GetVarString( clb, VARTYPE_TEST , "Serial_Number_Contructor_String" ); 
		
		if ( pGetValue )
		{
			if ( pDB_Serial_Number_Contructor_String )
			{
				sprintf( szSerialNumber , pDB_Serial_Number_Contructor_String , pGetValue );
			}
			else
			{
				memcpy( szSerialNumber , pGetValue , (iGetValueLength*iGetValueSize));
			}
		}
	}
	
	GET_SERIAL_NUMBER(szUserSerialNumber);
	
	RESULT( CLB_RESULT_DOUBLE , szSerialNumber , (strcmp(szUserSerialNumber,szSerialNumber) == 0 ) , "Serial Number Compare" );

	iDB_NumberOfVersionsToGet = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Versions_To_Get" );
	
	for( iVersionIndex = 0; iVersionIndex < iDB_NumberOfVersionsToGet; iVersionIndex++ )
	{
		memset( szVersionCompareString , 0 , LOW_STRING );
		memset( szVersionNameString , 0 , LOW_STRING ); 
		
		FREE(pDB_VersionCommand);
		FREE(pDB_Version_Contructor_String);
		
		sprintf( szFormatedString , "Get_Version_%d_Command" , (iVersionIndex+1) );
		pDB_VersionCommand = GetVarString( clb, VARTYPE_TEST , szFormatedString );  
		
		if ( pDB_VersionCommand && ( strlen(pDB_VersionCommand)))
		{
			sprintf( szFormatedString , "Get_Version_%d_Delay" , (iVersionIndex+1) );
			lfDB_GetVersionDelay = GetVarDouble( clb, VARTYPE_TEST , szFormatedString );  
		
			DRV_StandardDevice_SetValue( hUnitUnderTest , pDB_VersionCommand , NULL , 0 , 1 );   
			DelayWithEventProcessing(lfDB_GetSerialNumberDelay);
			DRV_StandardDevice_GetValue( hUnitUnderTest , &pGetCommandName , (void**)&pGetValue , &iGetValueLength , &iGetValueSize );  

			sprintf( szFormatedString , "Get_Version_%d_Contructor_String" , (iVersionIndex+1) );
			pDB_Version_Contructor_String = GetVarString( clb, VARTYPE_TEST , szFormatedString ); 
		
			if ( pGetValue )
			{
				if ( pDB_Serial_Number_Contructor_String )
				{
					sprintf( szVersionString , pDB_Version_Contructor_String , pGetValue );
				}
				else
				{
					memcpy( szVersionString , pGetValue , (iGetValueLength*iGetValueSize));
				}
			}
		}
	
		sprintf( szFormatedString , "Compare_Version_%d_String" , (iVersionIndex+1) );
		pDB_Version_Compare_String = GetVarString( clb, VARTYPE_TEST , szFormatedString ); 
			
		if ( pDB_Version_Compare_String )
			strcpy( szVersionCompareString , pDB_Version_Compare_String );

		sprintf( szFormatedString , "Version_%d_Name" , (iVersionIndex+1) );
		pDB_Version_Name_String = GetVarString( clb, VARTYPE_TEST , szFormatedString ); 
		
		if ( pDB_Version_Name_String )
			strcpy( szVersionNameString , pDB_Version_Name_String );
		
		RESULT( CLB_RESULT_DOUBLE , szVersionString , (strcmp(szVersionCompareString,szVersionString) == 0 ) , szVersionNameString );
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

	FREE(pGetValue);
	FREE(pGetCommandName);
	FREE(pCalibrationTable);
	
	FREE(pDB_SerialNumberCommand);
	FREE(pDB_VersionCommand);
	
	FREE(pDB_Version_Compare_String);
	FREE(pDB_Version_Name_String);
	
	FREE(pDB_Version_Contructor_String);
	FREE(pDB_Serial_Number_Contructor_String);
	
	return iTestError;
}

