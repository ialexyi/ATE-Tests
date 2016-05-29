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


TSTE_TEST ( STD_NETWORK_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	int								iSpecValuesCount								=	0,
									iActualyPointsNumber							=	0; 
										
	double							*plfTrace										=	NULL,
									*plfFrequency									=	NULL;
									
	double							*pvlfStartFrequency								=	NULL,
									*pvlfStopFrequency								=	NULL;
									
	double							*plfResultTrace									=	NULL,
									*plfResultFreqTrace								=	NULL;
	
	double							*plfBuildingTrace								=	NULL,
									*plfBuildingFreqTrace							=	NULL;
	
	double							**pvlfWindowFrequencies							=	NULL;
	
	int								*pviWindowFrequenciesCount						=	NULL;
	
	double							lfFullBandStartFrequency						=	0.0, 
									lfFullBandStopFrequency							=	0.0;
		
	double							lfDB_Delay										=	0.0,
									lfDB_StateRecallDelay							=	0.0, 
									lfDB_MeasureTimeout								=	0.0;
	
	int								iDB_TraceNumber									=	0,  
									iDB_NumberOfPoints								=	0,
									iDB_NumberOfWindows								=	0,
									iDB_NumberOfMathFunctions						=	0,
									iDB_NumberOfTraceNumbers						=	0,  
									iDB_NumberOfStoreToMemoryItems					=	0; 
									
	int								*vbDB_StoreTraceAsReference						=	0;
	
	char							*pszNetworkAnalyzerStateNumber					=	NULL,
									*pszGetNetworkAnalyzerStateNumber				=	NULL; 
									
	char							**pszNetworkAnalyzerMathFunction				=	NULL;
	
	char							*pCurrentPathName								=	NULL,
									*pCurrentTestPrefix								=	NULL;
	
	double							*pFrequencyList									=	NULL;
	
	int								*pSecuredListInfo								=	NULL;
	
	int								iNumberOfPoints									=	0,
									iTraceLength									=	0;
	
	char							szFormatedString[LOW_STRING]					=	{0},
									szFormatedVariable[LOW_STRING]					=	{0};
	
	void							*pGraphItem										=	NULL;
	
	int								iPointIndex										=	0, 
									iWindowIndex									=	0,
									iFrequencyIndex									=	0,
									iTraceResultIndex								=	0,
									iCurrentPathIndex								=	0; 
										
	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;
	
	double							lfDeltaValue									=	0.0,
									lfAverageValue									=	0.0,
									lfMinimumValue									=	0.0,
									lfMaximumValue									=	0.0,
									lfMinimumFreq									=	0.0, 
									lfMaximumFreq									=	0.0;
	
	int								iAverageCounter									=	0,
									iResultCounter									=	0;  
										
	int								hNetworkAnalyzer								=	0;
	
	int								bResultExists									=	0,
									bDB_EnableMeasureTraceResult					=	0, 
									bDB_BuildOutputPlotAsGraph						=	0, 
									bDB_UseTestFrequencySetting						=	0,
									bDB_ClearAverageBeforeMeasure					=	0; 
									
	int								*viDB_TraceNumbers								=	NULL; 
	
	int								bDB_LoadStateByPathName							=	0,
									bDB_EnableToStoreMeasureData					=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Network Test."); 
	
	GET_TEST_PREFIX(pCurrentTestPrefix);
	
	PATH_GET_CURRENT_INDEX(iCurrentPathIndex); 
	
	GET_SEGMENT_LOOP_NUM(iCurrentTemperatureSegmentLoop);	
	GET_SEGMENT_NUMBER(iCurrentTemperatureSegment);
	
	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	bDB_LoadStateByPathName = GetVarInt( clb, VARTYPE_TEST , "LoadStateByPathName" ); 

	GET_NA_EQUIPMENT_HANDLE(hNetworkAnalyzer);
	
	bDB_UseTestFrequencySetting = GetVarInt( clb, VARTYPE_TEST , "UseTestFrequencySetting" );   
	bDB_BuildOutputPlotAsGraph = GetVarInt( clb, VARTYPE_TEST , "BuildOutputPlotAsGraph" );  

	bDB_EnableMeasureTraceResult = GetVarInt( clb, VARTYPE_TEST , "EnableMeasureTraceResult" );     
	
	bDB_ClearAverageBeforeMeasure = GetVarInt( clb, VARTYPE_TEST , "ClearAverageBeforeMeasure" );
	
	iDB_NumberOfPoints = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Points" );

	lfDB_MeasureTimeout = GetVarDouble( clb, VARTYPE_TEST , "Measure_Timeout" ); 
	
	lfDB_Delay = GetVarDouble( clb, VARTYPE_TEST , "Delay" ); 
	
	iDB_NumberOfTraceNumbers = GetVarIntArray( clb, VARTYPE_TEST , "Trace_Number" , &viDB_TraceNumbers );

	if ( iDB_NumberOfTraceNumbers == 1 )
		iDB_TraceNumber = viDB_TraceNumbers[0];
	else
		iDB_TraceNumber = viDB_TraceNumbers[iCurrentPathIndex];
	
	FREE(viDB_TraceNumbers);
	
	if ( bDB_UseTestFrequencySetting )
	{
		iDB_NumberOfWindows = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Windows" );  
	
		CALLOC_ERR( pvlfStartFrequency , iDB_NumberOfWindows , sizeof(double));
		CALLOC_ERR( pvlfStopFrequency , iDB_NumberOfWindows , sizeof(double));
	
		CALLOC_ERR( pvlfWindowFrequencies , iDB_NumberOfWindows , sizeof(double*)); 
		CALLOC_ERR( pviWindowFrequenciesCount , iDB_NumberOfWindows , sizeof(int)); 
	
		for ( iWindowIndex = 0; iWindowIndex < iDB_NumberOfWindows; iWindowIndex++ ) 
		{
			sprintf( szFormatedString , "Windows_%d_StartFrequency" , ( iWindowIndex + 1 ));
			pvlfStartFrequency[iWindowIndex] = GetVarDouble( clb, VARTYPE_TEST , szFormatedString );  
			sprintf( szFormatedString , "Windows_%d_StopFrequency" , ( iWindowIndex + 1 ));
			pvlfStopFrequency[iWindowIndex] = GetVarDouble( clb, VARTYPE_TEST , szFormatedString ); 
			sprintf( szFormatedString , "Windows_%d_MeasureFrequencies" , ( iWindowIndex + 1 ));
			pviWindowFrequenciesCount[iWindowIndex] = GetVarDoubleArray( clb, VARTYPE_TEST , szFormatedString, &(pvlfWindowFrequencies[iWindowIndex])); 
		}
	}
	else
	{
		iDB_NumberOfWindows = 1;	
		GET_FREQUENCIES_LOOP_INFO_LIST( pFrequencyList , pSecuredListInfo , iNumberOfPoints );
	}
	
	lfDB_StateRecallDelay = GetVarDouble( clb, VARTYPE_EQUIPMENT_USE , "StateRecallDelay" , hNetworkAnalyzer );
	
	if ( bDB_LoadStateByPathName == 0 )
	{
		pszNetworkAnalyzerStateNumber = GetVarString( clb, VARTYPE_EQUIPMENT_USE , "StateFile" , hNetworkAnalyzer );
	}
	else
	{
		PATH_GET_CURRENT_PATH_NAME(pCurrentPathName);
		
		if ( pCurrentPathName )
		{
			pszNetworkAnalyzerStateNumber = GetVarString( clb, VARTYPE_EQUIPMENT_USE , pCurrentPathName , hNetworkAnalyzer );  
			
			FREE(pCurrentPathName);
		}
	}
	
	if ( pszNetworkAnalyzerStateNumber )
	{
		CHK_STDERR( DRV_NetworkAnalyzer_GetLastStateFileName( hNetworkAnalyzer , &pszGetNetworkAnalyzerStateNumber ));
	
		if (( pszGetNetworkAnalyzerStateNumber == NULL ) || ( strcmp(pszGetNetworkAnalyzerStateNumber,pszNetworkAnalyzerStateNumber)))
		{
			DRV_NetworkAnalyzer_Reset(hNetworkAnalyzer);  
		
			sprintf( szFormatedString , "Loading network analyzer state (%s)" , pszNetworkAnalyzerStateNumber );   //1
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
			CHK_STDERR( DRV_NetworkAnalyzer_ReCallRegister( hNetworkAnalyzer , pszNetworkAnalyzerStateNumber )); 
		
			WAIT("" , lfDB_StateRecallDelay );
		}
	}
	
	CHK_STDERR( DRV_NetworkAnalyzer_GetPointsNum( hNetworkAnalyzer , &iActualyPointsNumber ));  
		
	IF (( iActualyPointsNumber != iDB_NumberOfPoints ) , "Wrong number of points" );
	
	sprintf( szFormatedString , "Network Analyzer :: Has %d Points." , iActualyPointsNumber );  
	LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
	LOG(CLB_LINE_NORMAL, "Obtaining Network Analyzer Information..."); 
	
	CHK_STDERR( DRV_NetworkAnalyzer_GetFrequency( hNetworkAnalyzer , &lfFullBandStartFrequency, &lfFullBandStopFrequency )); 
	
	pszNetworkAnalyzerMathFunction = GetVarStringArray( clb, VARTYPE_TEST , "NetworkAnalyzer_MathFunction" , &iDB_NumberOfMathFunctions , hNetworkAnalyzer ); 
	
	if ( pszNetworkAnalyzerMathFunction && pszNetworkAnalyzerMathFunction[0] && ( strlen(pszNetworkAnalyzerMathFunction[0])))
	{   
		if (( iDB_NumberOfMathFunctions > 1 ) && ( iCurrentPathIndex < iDB_NumberOfMathFunctions ))
		{
			sprintf( szFormatedString , "Set network analyzer math function (%c)" , pszNetworkAnalyzerMathFunction[iCurrentPathIndex][0] );   
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
			CHK_STDERR( DRV_NetworkAnalyzer_SelectMathFunction ( hNetworkAnalyzer , iDB_TraceNumber , pszNetworkAnalyzerMathFunction[iCurrentPathIndex][0] ))
		}
		else
		{
			sprintf( szFormatedString , "Set network analyzer math function (%c)" , pszNetworkAnalyzerMathFunction[0][0] );   
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
			CHK_STDERR( DRV_NetworkAnalyzer_SelectMathFunction ( hNetworkAnalyzer , iDB_TraceNumber , pszNetworkAnalyzerMathFunction[0][0] ))
		}  
	}

	sprintf( szFormatedString , "Network Analyzer :: Trace Number (%d)" , iDB_TraceNumber );  
	LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
	DRV_NetworkAnalyzer_SelectTrace ( hNetworkAnalyzer , iDB_TraceNumber ); 
	
	if ( bDB_ClearAverageBeforeMeasure ) 
	{
		DRV_NetworkAnalyzer_ClearAvarage( hNetworkAnalyzer , iDB_TraceNumber ); 
	}
	
	WAIT("" , lfDB_Delay );
	
	LOG(CLB_LINE_NORMAL, "Reading Network Analyzer Trace...");  
	
	CHK_STDERR( DRV_NetworkAnalyzer_GetTrace ( hNetworkAnalyzer , 0 , 0 , 0 , lfDB_MeasureTimeout , &(plfFrequency) , &(plfTrace) , &iActualyPointsNumber ));
	
	if ( bDB_EnableMeasureTraceResult )
	{
		iResultCounter++; 
		GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
	
		if ( bResultExists ) 
		{
			GET_SPECS_VALUE_COUNT_NUMBER(iResultCounter,iSpecValuesCount);
		
			if (( iSpecValuesCount > 3 ) && ( iSpecValuesCount != iActualyPointsNumber ))
			{
				sprintf( szFormatedString , "Result Nr.%d Requireds %d Points but actualy it is %d Points." , iResultCounter , iSpecValuesCount , iActualyPointsNumber );  
				LOG(CLB_LINE_WARNING, szFormatedString ); 
			}
			
			GET_RESULT_GRAPH_ITEM( iResultCounter , pGraphItem);
			ADD_TRACE_TO_RESULT_GRAPH( iResultCounter , pGraphItem ,plfTrace,iActualyPointsNumber,0,iActualyPointsNumber,plfFrequency[0],plfFrequency[iActualyPointsNumber-1],"Trace",VAL_GREEN);
			FREE(plfResultTrace);
		
			RESULT_CONST_NUM( iResultCounter , CLB_RESULT_GRAPH,"", pGraphItem , "Measure Trace" );
		}
	}			
				
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Measure_Trace" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex );
		SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfTrace , iActualyPointsNumber ); 
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Measure_Trace_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex );
		SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfFrequency , iActualyPointsNumber ); 
	}
	
	iDB_NumberOfStoreToMemoryItems = GetVarIntArray( clb, VARTYPE_TEST , "Trace_Store_To_Mem_As_Reference" , &vbDB_StoreTraceAsReference );
	
	if ( vbDB_StoreTraceAsReference &&  iDB_NumberOfStoreToMemoryItems )
	{
		if ((( iDB_NumberOfStoreToMemoryItems == 1 ) && ( vbDB_StoreTraceAsReference[0] )) || (( iDB_NumberOfStoreToMemoryItems > 1 ) && ( vbDB_StoreTraceAsReference[iCurrentPathIndex])))
		{
			LOG(CLB_LINE_NORMAL, "Store Network Analyzer Trace to memory.");  
		
			CHK_STDERR( DRV_NetworkAnalyzer_SetTraceToMathMemory (  hNetworkAnalyzer , iDB_TraceNumber ));  
		}
	}
	
	LOG(CLB_LINE_NORMAL, "Analyzing Data...");  
	
	for ( iWindowIndex = 0; iWindowIndex < iDB_NumberOfWindows; iWindowIndex++ ) 
	{
		lfAverageValue = 0.0;
		lfMinimumValue = 0.0;
		lfMaximumValue = 0.0;
		iAverageCounter = 0;
	
		FREE_CALLOC_ERR( plfResultTrace , iActualyPointsNumber , sizeof(double));
		FREE_CALLOC_ERR( plfResultFreqTrace , iActualyPointsNumber , sizeof(double));
		
		for ( iPointIndex = 0; iPointIndex < iActualyPointsNumber; iPointIndex++ ) 
		{
			if (( plfFrequency[iPointIndex] >= pvlfStartFrequency[iWindowIndex] ) && ( plfFrequency[iPointIndex] <= pvlfStopFrequency[iWindowIndex] ))
			{
				plfResultTrace[iAverageCounter] = plfTrace[iPointIndex];
				plfResultFreqTrace[iAverageCounter] = plfFrequency[iPointIndex]; 
				
				lfAverageValue += plfTrace[iPointIndex];
				iAverageCounter++;
			
				if (( plfTrace[iPointIndex] < lfMinimumValue ) || ( lfMinimumValue == 0.0 ))
				{
					lfMinimumValue = plfTrace[iPointIndex];
					lfMinimumFreq = plfFrequency[iPointIndex];
				}
				
				if (( plfTrace[iPointIndex] > lfMaximumValue ) || ( lfMaximumValue == 0.0 ))
				{
					lfMaximumValue = plfTrace[iPointIndex];
					lfMaximumFreq = plfFrequency[iPointIndex]; 
				}
			}
		}
	
		iResultCounter++; 
		GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Trace" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
			SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfResultTrace , iAverageCounter ); 
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Trace_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
			SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfResultFreqTrace , iAverageCounter ); 
		}
		
		if ( bResultExists  ) 
		{
			if ( iAverageCounter > 2 )
			{
				GET_SPECS_VALUE_COUNT_NUMBER(iResultCounter,iSpecValuesCount);
		
				if (( iSpecValuesCount > 3 ) && ( iSpecValuesCount != iActualyPointsNumber ))
				{
					sprintf( szFormatedString , "Result Nr.%d Requireds %d Points but actualy it is %d Points." , iResultCounter , iSpecValuesCount , iActualyPointsNumber );  
					LOG(CLB_LINE_WARNING, szFormatedString ); 
				}
				
				GET_RESULT_GRAPH_ITEM( iResultCounter , pGraphItem);
				ADD_TRACE_TO_RESULT_GRAPH( iResultCounter , pGraphItem ,plfResultTrace,iAverageCounter,0,iAverageCounter,pvlfStartFrequency[iWindowIndex],pvlfStopFrequency[iWindowIndex],"Trace",VAL_GREEN);
				FREE(plfResultTrace);
		
				RESULT_CONST_NUM( iResultCounter , CLB_RESULT_GRAPH,"", pGraphItem , "Trace" );
			}
			else
			{
				sprintf( szFormatedString , "Window%d" , (iWindowIndex+1));
				RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, szFormatedString, plfResultTrace[0] , "Average Value" ); 	
			}
		}

		iResultCounter++; 
		GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
		
		lfAverageValue /= (double)iAverageCounter;
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Average" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfAverageValue ); 
		}
		
		if ( bResultExists  )
		{
			sprintf( szFormatedString , "Window%d" , (iWindowIndex+1));
			RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, szFormatedString, lfAverageValue , "Average Value" ); 
		}
		
		iResultCounter++; 
		GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Maximum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfMaximumValue );
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Maximum_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfMaximumFreq );
		}
		
		if ( bResultExists  )
		{
			sprintf( szFormatedString , "%0.3lf MHz" , (lfMaximumFreq / 1E6) ); 
			RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, szFormatedString, lfMaximumValue , "Maximum Value" );  
		}
		
		iResultCounter++; 
		GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Minimum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfMinimumValue );
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Minimum_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfMinimumFreq );
		}
		
		if ( bResultExists  )
		{
			sprintf( szFormatedString , "%0.3lf MHz" , (lfMinimumFreq / 1E6) ); 
			RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, szFormatedString, lfMinimumValue , "Minimum Value" ); 
		}
		
		iResultCounter++; 
		GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
		
		lfDeltaValue = lfMaximumValue - lfMinimumValue;
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Delta" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDeltaValue );
		}
		
		if ( bResultExists  )
		{
			sprintf( szFormatedString , "%0.3lf MHz" , (lfMinimumFreq / 1E6) ); 
			RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, szFormatedString, lfDeltaValue , "Delta" ); 
		}
		
		iResultCounter++; 
		GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
		
		lfDeltaValue /= 2.0;
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_HalfDelta" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDeltaValue );
		}
		
		if ( bResultExists  )
		{
			sprintf( szFormatedString , "%0.3lf MHz" , (lfMinimumFreq / 1E6) ); 
			RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, szFormatedString, lfDeltaValue , "HalfDelta" ); 
		}
		
		if ( bDB_UseTestFrequencySetting )
		{
			if (( pviWindowFrequenciesCount[iWindowIndex] > 1 ) || ( pvlfWindowFrequencies[iWindowIndex][0] != 0.0 )) 
			{
				if ( pviWindowFrequenciesCount[iWindowIndex] )
				{
					FREE_CALLOC( plfBuildingTrace , pviWindowFrequenciesCount[iWindowIndex] , sizeof(double));
					FREE_CALLOC( plfBuildingFreqTrace , pviWindowFrequenciesCount[iWindowIndex] , sizeof(double));
				}
				
				for ( iFrequencyIndex = 0; iFrequencyIndex < pviWindowFrequenciesCount[iWindowIndex]; iFrequencyIndex++ )
				{
					for ( iPointIndex = 0; iPointIndex < iActualyPointsNumber; iPointIndex++ ) 
						if ( plfFrequency[iPointIndex] >= pvlfWindowFrequencies[iWindowIndex][iFrequencyIndex] ) 
							break;
			
					if ( bDB_BuildOutputPlotAsGraph == 0 )
					{
						iResultCounter++; 
						GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
					}
					
					if ( bDB_EnableToStoreMeasureData )
					{
						sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Frequency_%d_Value" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) , (iFrequencyIndex+1));
						SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfTrace[iPointIndex] );
						sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Frequency_%d_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) , (iFrequencyIndex+1));
						SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , pvlfWindowFrequencies[iWindowIndex][iFrequencyIndex] );
					}
					
					if ( bDB_BuildOutputPlotAsGraph == 0 )
					{
						if ( bResultExists  )
						{
							sprintf( szFormatedString , "%0.3lf MHz" , (pvlfWindowFrequencies[iWindowIndex][iFrequencyIndex] / 1E6) ); 
							RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, szFormatedString, plfTrace[iPointIndex] , "Marker Value" ); 
						}
					}
					else
					{
						plfBuildingTrace[iFrequencyIndex] = plfTrace[iPointIndex];
						plfBuildingFreqTrace[iFrequencyIndex] = pvlfWindowFrequencies[iWindowIndex][iFrequencyIndex];
					}
				}
				
				if ( bDB_BuildOutputPlotAsGraph )
				{
					iTraceLength = pviWindowFrequenciesCount[iWindowIndex];
					
					iResultCounter++; 
					GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
		
					if ( bDB_EnableToStoreMeasureData )
					{
						sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_OutputTrace" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
						SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfBuildingTrace , pviWindowFrequenciesCount[iWindowIndex] ); 
						sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_OutputTrace_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
						SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfBuildingFreqTrace , pviWindowFrequenciesCount[iWindowIndex] ); 
					}
					
					if ( bResultExists  )
					{
						if ( iTraceLength >= 3 )
						{	 
							GET_SPECS_VALUE_COUNT_NUMBER(iResultCounter,iSpecValuesCount);
		
							if (( iSpecValuesCount > 3 ) && ( iSpecValuesCount != iActualyPointsNumber ))
							{
								sprintf( szFormatedString , "Result Nr.%d Requireds %d Points but actualy it is %d Points." , iResultCounter , iSpecValuesCount , iActualyPointsNumber );  
								LOG(CLB_LINE_WARNING, szFormatedString ); 
							}
			
							GET_RESULT_GRAPH_ITEM( iResultCounter , pGraphItem);
							ADD_TRACE_TO_RESULT_GRAPH( iResultCounter , pGraphItem,plfBuildingTrace,iTraceLength,0,iTraceLength,plfBuildingFreqTrace[0],plfBuildingFreqTrace[iTraceLength-1],"Trace",VAL_GREEN);
							FREE(plfResultTrace);
	
							RESULT_CONST_NUM( iResultCounter , CLB_RESULT_GRAPH,"", pGraphItem , "Output Trace" );
						}
						else
						{
							for ( iTraceResultIndex = 0; iTraceResultIndex < iTraceLength; iTraceResultIndex++ )
							{
								sprintf( szFormatedString , "%0.3lf MHz" , (plfBuildingFreqTrace[iTraceResultIndex] / 1E6) ); 
								RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, szFormatedString, plfBuildingTrace[iTraceResultIndex] , "Marker Value" ); 	
							}
						}
					}
				}
			}
		}
	}
	
	if ( bDB_UseTestFrequencySetting == 0 )  
	{
		if ( iNumberOfPoints )
		{
			FREE_CALLOC( plfBuildingTrace , iNumberOfPoints , sizeof(double));
			FREE_CALLOC( plfBuildingFreqTrace , iNumberOfPoints , sizeof(double));
		}
		
		for ( iFrequencyIndex = 0; iFrequencyIndex < iNumberOfPoints; iFrequencyIndex++ )
		{
			for ( iPointIndex = 0; iPointIndex < iActualyPointsNumber; iPointIndex++ ) 
				if ( plfFrequency[iPointIndex] >= pFrequencyList[iFrequencyIndex] ) 
					break;
	
			if ( bDB_BuildOutputPlotAsGraph == 0 )
			{
				iResultCounter++; 
				GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
			}

			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Frequency_%d_Value" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) , (iFrequencyIndex+1));
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfTrace[iPointIndex] );
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Frequency_%d_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) , (iFrequencyIndex+1));
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , pFrequencyList[iFrequencyIndex] );
			}
			
			if ( bDB_BuildOutputPlotAsGraph == 0 )
			{
				if ( bResultExists  )
				{
					sprintf( szFormatedString , "%0.3lf MHz" , (pFrequencyList[iFrequencyIndex] / 1E6) ); 
					RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, szFormatedString, plfTrace[iPointIndex] , "Marker Value" ); 
				}
			}
			else
			{
				plfBuildingTrace[iFrequencyIndex] = plfTrace[iPointIndex];
				plfBuildingFreqTrace[iFrequencyIndex] = pFrequencyList[iFrequencyIndex];
			}	
		}
		
		if ( bDB_BuildOutputPlotAsGraph )
		{
			iTraceLength = iNumberOfPoints;
			
			iResultCounter++; 
			GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);

			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_OutputTrace" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
				SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfBuildingTrace , pviWindowFrequenciesCount[iWindowIndex] ); 
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_OutputTrace_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
				SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfBuildingFreqTrace , pviWindowFrequenciesCount[iWindowIndex] ); 
			}
			
			if ( bResultExists  )
			{
				if ( iTraceLength >= 3 )
				{	 
					GET_SPECS_VALUE_COUNT_NUMBER(iResultCounter,iSpecValuesCount);
		
					if (( iSpecValuesCount > 3 ) && ( iSpecValuesCount != iActualyPointsNumber ))
					{
						sprintf( szFormatedString , "Result Nr.%d Requireds %d Points but actualy it is %d Points." , iResultCounter , iSpecValuesCount , iActualyPointsNumber );  
						LOG(CLB_LINE_WARNING, szFormatedString ); 
					}
			
					GET_RESULT_GRAPH_ITEM( iResultCounter , pGraphItem);
					ADD_TRACE_TO_RESULT_GRAPH( iResultCounter , pGraphItem,plfBuildingTrace,iTraceLength,0,iTraceLength,plfBuildingFreqTrace[0],plfBuildingFreqTrace[iTraceLength-1],"Trace",VAL_GREEN);
					FREE(plfResultTrace);

					RESULT_CONST_NUM( iResultCounter , CLB_RESULT_GRAPH,"", pGraphItem , "Output Trace" );
				}
				else
				{
					for ( iTraceResultIndex = 0; iTraceResultIndex < iTraceLength; iTraceResultIndex++ )
					{
						sprintf( szFormatedString , "%0.3lf MHz" , (plfBuildingFreqTrace[iTraceResultIndex] / 1E6) ); 
						RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, szFormatedString, plfBuildingTrace[iTraceResultIndex] , "Marker Value" ); 	
					}
				}
			}
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

	FREE_LIST(pszNetworkAnalyzerMathFunction,iDB_NumberOfMathFunctions);
	FREE_LIST(pvlfWindowFrequencies , iDB_NumberOfWindows );
	
	FREE( pCurrentTestPrefix );
	
	FREE( pFrequencyList );
	FREE( pSecuredListInfo );
	FREE( vbDB_StoreTraceAsReference );
	FREE( pszNetworkAnalyzerStateNumber ); 
	FREE( pszGetNetworkAnalyzerStateNumber );
	
	FREE( pviWindowFrequenciesCount );
	
	FREE( pvlfStartFrequency );
	FREE( pvlfStopFrequency );
	FREE( plfResultTrace );
	FREE( plfResultFreqTrace );
	
	FREE( plfBuildingTrace );
	FREE( plfBuildingFreqTrace );
	
	FREE( plfTrace );
	FREE( plfFrequency ); 
	
	return iTestError;
}

