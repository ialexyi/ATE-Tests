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


TSTE_TEST ( STD_NETWORK_PHASE_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	int								iSpecValuesCount								=	0,
									iActualyPointsNumber							=	0; 
										
	double							*plfTrace										=	NULL,
									*plfFrequency									=	NULL;

	double							**plfTraceArray									=	NULL;
																  
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
									lfDB_MeasureTimeout								=	0.0,
									lfDB_StateRecallDelay							=	0.0,
									lfDB_ElectricalMeasureDelay						=	0.0,
									lfDB_ElectricalDelayRepairStep					=	0.0; 
									
	int								iDB_TraceNumber									=	0,  
									iDB_Average_Count								=	0, 
									iDB_NumberOfPoints								=	0,
									iDB_NumberOfWindows								=	0, 
									iDB_NumberOfTraceNumbers						=	0, 
									iDB_PhaseRepairTryNumber						=	0,
									iDB_NumberOfMathFunctions						=	0,
									iDB_GroupDelayTraceNumber						=	0,
									iDB_NumberOfStoreToMemoryItems					=	0; 
									
	int								*vbDB_StoreTraceAsReference						=	NULL; 
	
	int								*viDB_TraceNumbers								=	NULL; 
	
	int								*piTraceCount									=	NULL;
									
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
									iAverageIndex									=	0,
									iFrequencyIndex									=	0,
									iTraceResultIndex								=	0,
									iCurrentPathIndex								=	0,
									iTryElecticalDelayIndex							=	0;
									
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
									bDB_AbsoluteDelta								=	0, 
									bDB_Decrease360									=	0, 
									bDB_BuildOutputPlotAsGraph						=	0, 
									bDB_UseTestFrequencySetting						=	0,
									bDB_EnableGroupDelayMeasureTraceResult			=	0, 
									bDB_ElectricalDelayFromCenterFreq				=	0; 
									
	double							*plfElecticalDelay								=	NULL,
									*plfTotalDeltaResult							=	NULL; 
									
	double							**plfElecticalDelayTrace						=	NULL,
									**plfElecticalDelayTraceFreq					=	NULL; 
									
	int								*piElecticalDelayCount							=	NULL,
									*piElecticalDelayTraceCounter					=	NULL; 

	int								bDB_LoadStateByPathName							=	0,
									bDB_EnableToStoreMeasureData					=	0;
	
	double							lfSearchPhaseTakeover							=	0.0; 
									
	int								bPhaseTakeoverExists							=	0;
	
	int								iSearchPhaseTakeoverCounter						=	0;
	
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

	bDB_EnableGroupDelayMeasureTraceResult = GetVarInt( clb, VARTYPE_TEST , "EnableGroupDelayMeasureTraceResult" );  
	
	iDB_NumberOfPoints = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Points" );

	iDB_Average_Count = GetVarInt( clb, VARTYPE_TEST , "Average_Count" ); 
	
	iDB_PhaseRepairTryNumber  = GetVarInt( clb, VARTYPE_TEST , "PhaseRepairTryNumber" ); 
	
	bDB_ElectricalDelayFromCenterFreq = GetVarInt( clb, VARTYPE_TEST , "GetElectricalDelayFromCenterFreq" );  

	bDB_AbsoluteDelta = GetVarInt( clb, VARTYPE_TEST , "DeltaResult_IsAbsoluteValue" );
	bDB_Decrease360 = GetVarInt( clb, VARTYPE_TEST , "DeltaResult_Decrease360" );  
	
	lfDB_MeasureTimeout = GetVarDouble( clb, VARTYPE_TEST , "Measure_Timeout" ); 
	
	lfDB_Delay = GetVarDouble( clb, VARTYPE_TEST , "Delay" ); 
	lfDB_ElectricalMeasureDelay = GetVarDouble( clb, VARTYPE_TEST , "ElectricalMeasureDelay" );
	
	lfDB_ElectricalDelayRepairStep = GetVarDouble( clb, VARTYPE_TEST , "ElectricalDelayRepairStep" );    
	
	iDB_NumberOfTraceNumbers = GetVarIntArray( clb, VARTYPE_TEST , "GroupDelay_Trace_Number" , &viDB_TraceNumbers );

	if ( iDB_NumberOfTraceNumbers == 1 )
		iDB_GroupDelayTraceNumber = viDB_TraceNumbers[0];
	else
		iDB_GroupDelayTraceNumber = viDB_TraceNumbers[iCurrentPathIndex];
	
	FREE(viDB_TraceNumbers);
	
	iDB_NumberOfTraceNumbers = GetVarIntArray( clb, VARTYPE_TEST , "Trace_Number" , &viDB_TraceNumbers );

	if ( iDB_NumberOfTraceNumbers == 1 )
		iDB_TraceNumber = viDB_TraceNumbers[0];
	else
		iDB_TraceNumber = viDB_TraceNumbers[iCurrentPathIndex];
	
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

	sprintf( szFormatedString , "Network Analyzer :: Trace Number (%d)" , iDB_GroupDelayTraceNumber );  
	LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
	DRV_NetworkAnalyzer_SelectTrace ( hNetworkAnalyzer , iDB_GroupDelayTraceNumber );
	
	WAIT("" , lfDB_ElectricalMeasureDelay );
	
	LOG(CLB_LINE_NORMAL, "Reading Electrical Delay Trace...");  
	
	CALLOC_ERR( plfTraceArray , (iDB_Average_Count+1) , sizeof(double*));
	CALLOC_ERR( plfTrace , (iDB_NumberOfPoints+1) , sizeof(double)); 
	CALLOC_ERR( piTraceCount , (iDB_NumberOfPoints+1) , sizeof(int));
	CALLOC_ERR( plfElecticalDelay , (iDB_NumberOfPoints+1) , sizeof(double));
	CALLOC_ERR( piElecticalDelayCount , (iDB_NumberOfPoints+1) , sizeof(int));
	
	CALLOC_ERR( piElecticalDelayTraceCounter , (iDB_NumberOfWindows+1) , sizeof(int));     
	CALLOC_ERR( plfElecticalDelayTrace , (iDB_NumberOfWindows+1) , sizeof(double*)); 
	CALLOC_ERR( plfElecticalDelayTraceFreq , (iDB_NumberOfWindows+1) , sizeof(double*)); 
	
	CALLOC_ERR( plfTotalDeltaResult , (iDB_NumberOfWindows+1) , sizeof(double));   
	
	for ( iWindowIndex = 0; iWindowIndex < iDB_NumberOfWindows; iWindowIndex++ ) 
	{
		CALLOC_ERR( plfElecticalDelayTrace[iWindowIndex] , (iDB_NumberOfPoints+1) , sizeof(double*)); 
		CALLOC_ERR( plfElecticalDelayTraceFreq[iWindowIndex] , (iDB_NumberOfPoints+1) , sizeof(double*)); 		
	}
	
	for( iAverageIndex = 0; (( iAverageIndex < iDB_Average_Count ) || ( iAverageIndex == 0 )); iAverageIndex++ )
	{
		CHK_STDERR( DRV_NetworkAnalyzer_GetTrace ( hNetworkAnalyzer , 0 , 0 , 0 , lfDB_MeasureTimeout , &(plfFrequency) , &(plfTraceArray[iAverageIndex]) , &iActualyPointsNumber ));
		
		for ( iPointIndex = 0; iPointIndex < iActualyPointsNumber; iPointIndex++ ) 
		{
			plfTrace[iPointIndex] += plfTraceArray[iAverageIndex][iPointIndex];
			piTraceCount[iPointIndex]++;
		}
	}
	
	if ( iDB_Average_Count > 1 )
	{
		for ( iPointIndex = 0; iPointIndex < iActualyPointsNumber; iPointIndex++ ) 
		{
			plfTrace[iPointIndex] /= piTraceCount[iPointIndex];
		}
	}

	for ( iWindowIndex = 0; iWindowIndex < iDB_NumberOfWindows; iWindowIndex++ ) 
	{
		for ( iPointIndex = 0; iPointIndex < iActualyPointsNumber; iPointIndex++ ) 
		{
			if (( plfFrequency[iPointIndex] >= pvlfStartFrequency[iWindowIndex] ) && ( plfFrequency[iPointIndex] <= pvlfStopFrequency[iWindowIndex] ))
			{
				plfElecticalDelayTrace[iWindowIndex][piElecticalDelayTraceCounter[iWindowIndex]] = plfTrace[iPointIndex];
				plfElecticalDelayTraceFreq[iWindowIndex][piElecticalDelayTraceCounter[iWindowIndex]] = plfFrequency[iPointIndex];
					
				piElecticalDelayTraceCounter[iWindowIndex]++;
			}
				
			// do next conditional at avrage of window or at center frequency of window
			if (((( plfFrequency[iPointIndex] >= pvlfStartFrequency[iWindowIndex] ) && ( plfFrequency[iPointIndex] <= pvlfStopFrequency[iWindowIndex] )) && ( bDB_ElectricalDelayFromCenterFreq == 0 )) || ( bDB_ElectricalDelayFromCenterFreq && (((( pvlfStopFrequency[iWindowIndex] - pvlfStartFrequency[iWindowIndex] ) / 2.0 ) + pvlfStartFrequency[iWindowIndex] ) <= plfFrequency[iPointIndex] ) && ( piElecticalDelayCount[iWindowIndex] == 0 )))
			{
				plfElecticalDelay[iWindowIndex] += plfTrace[iPointIndex];
				piElecticalDelayCount[iWindowIndex]++;
			}
		}
	}
	
	for ( iWindowIndex = 0; iWindowIndex < iDB_NumberOfWindows; iWindowIndex++ ) 
	{
		if ( piElecticalDelayCount[iWindowIndex] > 1 )
			plfElecticalDelay[iWindowIndex] /= piElecticalDelayCount[iWindowIndex];
	}
	
	for ( iWindowIndex = 0; iWindowIndex < iDB_NumberOfWindows; iWindowIndex++ ) 
	{
		FREE(plfTrace);
		FREE(plfFrequency);
		
		if ( bDB_EnableGroupDelayMeasureTraceResult )
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
				ADD_TRACE_TO_RESULT_GRAPH( iResultCounter , pGraphItem , plfElecticalDelayTrace[iWindowIndex] ,piElecticalDelayTraceCounter[iWindowIndex],0,piElecticalDelayTraceCounter[iWindowIndex],plfElecticalDelayTraceFreq[iWindowIndex][0],plfElecticalDelayTraceFreq[iWindowIndex][piElecticalDelayTraceCounter[iWindowIndex]-1],"Group Delay",VAL_GREEN);
				FREE(plfResultTrace);
	
				RESULT_CONST_NUM( iResultCounter , CLB_RESULT_GRAPH,"", pGraphItem , "Delay Trace" );
			}
		}
	
		sprintf( szFormatedString , "Network Analyzer :: Trace Number (%d)" , iDB_TraceNumber );  
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
		DRV_NetworkAnalyzer_SelectTrace ( hNetworkAnalyzer , iDB_TraceNumber );  
		
		if ( iDB_PhaseRepairTryNumber < 1 )
			iDB_PhaseRepairTryNumber = 1;
		
		for ( iTryElecticalDelayIndex = 0; iTryElecticalDelayIndex < iDB_PhaseRepairTryNumber; iTryElecticalDelayIndex++ )
		{
			if ( iTryElecticalDelayIndex )
				plfElecticalDelay[iWindowIndex] += lfDB_ElectricalDelayRepairStep;
			
			sprintf( szFormatedString , "Network Analyzer :: Set Electrical Delay Time %0.3lf nSec" , ( plfElecticalDelay[iWindowIndex] * 1E9 ));  
			LOG(CLB_LINE_NORMAL, szFormatedString ); 		
		
			CHK_STDERR( DRV_NetworkAnalyzer_SetElectricalDelayTime( hNetworkAnalyzer , iDB_TraceNumber , plfElecticalDelay[iWindowIndex] ));
			
			if ( bDB_EnableToStoreMeasureData )
			{
				sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_ElecticalDelay" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
				SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfElecticalDelay[iWindowIndex] );
			}

			WAIT("" , lfDB_Delay );
	
			LOG(CLB_LINE_NORMAL, "Reading Phase Trace...");  
	
			CHK_STDERR( DRV_NetworkAnalyzer_GetTrace ( hNetworkAnalyzer , 0 , 0 , 0 , lfDB_MeasureTimeout , &(plfFrequency) , &(plfTrace) , &iActualyPointsNumber ));
	
			lfSearchPhaseTakeover = 0.0;
			bPhaseTakeoverExists = 0;
			iSearchPhaseTakeoverCounter = 0;
		
			for ( iPointIndex = 0; iPointIndex < iActualyPointsNumber; iPointIndex++ ) 
			{
				if (( plfFrequency[iPointIndex] >= pvlfStartFrequency[iWindowIndex] ) && ( plfFrequency[iPointIndex] <= pvlfStopFrequency[iWindowIndex] ))
				{
					lfSearchPhaseTakeover += plfTrace[iPointIndex];
					iSearchPhaseTakeoverCounter++;
				
					if (( fabs( plfTrace[iPointIndex] - ( lfSearchPhaseTakeover / (double)iSearchPhaseTakeoverCounter )) > 180.0 ) && ( iSearchPhaseTakeoverCounter > 1 ))
					{
						bPhaseTakeoverExists = 1;
						break;
					}
				}
			}						 
		
			if ( bPhaseTakeoverExists == 0 )
				break;
		}
		
		iResultCounter++; 
		GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
			
		if ( bResultExists )
		{
			RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, "", plfElecticalDelay[iWindowIndex] , "Electical Delay" );  
		}
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Measure_Trace" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) ); 
			SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfTrace , iActualyPointsNumber ); 
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Measure_Trace_Freq" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) ); 
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
	
		lfAverageValue = 0;
		lfMinimumValue = 0;
		lfMaximumValue = 0;
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
		
		if ( bResultExists ) 
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
		
		if ( bResultExists )
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
		
		if ( bResultExists )
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
		
		if ( bResultExists )
		{
			sprintf( szFormatedString , "%0.3lf MHz" , (lfMinimumFreq / 1E6) ); 
			RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, szFormatedString, lfMinimumValue , "Minimum Value" ); 
		}
		
		iResultCounter++; 
		GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
		
		sprintf( szFormatedString , "Max Value = %lf | Min Value = %lf" , lfMaximumValue , lfMinimumValue );  
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
		lfDeltaValue = lfMaximumValue - lfMinimumValue;
		
		sprintf( szFormatedString , "Delta ( Max - Min ) Value = %lf" , lfDeltaValue );  
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
		if ( bDB_Decrease360 )
		{
			while ( fabs(lfDeltaValue) > 180.0 )	
			{
				lfDeltaValue = lfDeltaValue - 360.0;	
			};
			
			sprintf( szFormatedString , "Delta Value ( After 360 Norm ) = %lf" , lfDeltaValue );  
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		}
		
		if ( bDB_AbsoluteDelta )
		{
			lfDeltaValue = fabs(lfDeltaValue);	
			
			sprintf( szFormatedString , "Delta Value ( After Absolute ) = %lf" , lfDeltaValue );  
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		}
															
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Delta" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDeltaValue );
		}
		
		if ( bResultExists )
		{
			sprintf( szFormatedString , "%0.3lf MHz" , (lfMinimumFreq / 1E6) ); 
			RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, szFormatedString, lfDeltaValue , "Delta" ); 
		}
		
		plfTotalDeltaResult[iWindowIndex] = lfDeltaValue;
		
		iResultCounter++; 
		GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
		
		lfDeltaValue /= 2.0;
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_HalfDelta" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfDeltaValue );
		}
		
		if ( bResultExists )
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
	
	//--------------------------- Total statistics about delta ----------------//
	iAverageCounter = 0; 
	lfAverageValue = 0.0;
	lfMinimumValue = 0.0;
	lfMaximumValue = 0.0;
	
	for ( iWindowIndex = 0; iWindowIndex < iDB_NumberOfWindows; iWindowIndex++ ) 
	{
		lfAverageValue += plfTotalDeltaResult[iWindowIndex];
		iAverageCounter++;
			
		if (( plfTotalDeltaResult[iWindowIndex] < lfMinimumValue ) || ( lfMinimumValue == 0.0 ))
			lfMinimumValue = plfTotalDeltaResult[iWindowIndex];
		
		if (( plfTotalDeltaResult[iWindowIndex] > lfMaximumValue ) || ( lfMaximumValue == 0.0 ))
			lfMaximumValue = plfTotalDeltaResult[iWindowIndex];
	}
	
	iResultCounter++; 
	GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
	
	lfAverageValue /= (double)iAverageCounter;
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Average_Delta" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfAverageValue ); 
	}
	
	if ( bResultExists  )
	{
		RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, "", lfAverageValue , "Average Delta" ); 
	}
	
	iResultCounter++; 
	GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Maximum_Delta" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfMaximumValue );
	}
	
	if ( bResultExists  )
	{
		RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, "", lfMaximumValue , "Maximum Delta" );  
	}
	
	iResultCounter++; 
	GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Minimum_Delta" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfMinimumValue );
	}
	
	if ( bResultExists  )
	{
		RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, "", lfMinimumValue , "Minimum Delta" ); 
	}	
	
	//--------------------------- Total statistics about half delta ----------------//
	lfAverageValue /= 2.0;
	lfMaximumValue /= 2.0;
	lfMinimumValue /= 2.0;
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Average_Half_Delta" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfAverageValue ); 
	}
	
	if ( bResultExists  )
	{
		RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, "", lfAverageValue , "Average Half Delta" ); 
	}
	
	iResultCounter++; 
	GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Maximum_Half_Delta" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfMaximumValue );
	}
	
	if ( bResultExists  )
	{
		RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, "", lfMaximumValue , "Maximum Half Delta" );  
	}
	
	iResultCounter++; 
	GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
	
	if ( bDB_EnableToStoreMeasureData )
	{
		sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Window%d_Minimum_Half_Delta" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iWindowIndex+1) );
		SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfMinimumValue );
	}
	
	if ( bResultExists  )
	{
		RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, "", lfMinimumValue , "Minimum Half Delta" ); 
	}	
		
	//---------------------------------------------------------------------------------------------------//
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
	FREE_LIST(plfTraceArray , iDB_Average_Count );
	FREE_LIST( plfElecticalDelayTrace , iDB_NumberOfWindows ); 
	FREE_LIST( plfElecticalDelayTraceFreq , iDB_NumberOfWindows ); 
	
	FREE( piTraceCount );
	
	FREE( pCurrentTestPrefix );
	
	FREE(viDB_TraceNumbers); 
	
	FREE( plfElecticalDelay );
	FREE( piElecticalDelayCount );
	
	FREE( plfTotalDeltaResult );
	
	FREE( pFrequencyList );
	FREE( pSecuredListInfo );
	FREE( vbDB_StoreTraceAsReference );
	FREE( pszNetworkAnalyzerStateNumber ); 
	FREE( pszGetNetworkAnalyzerStateNumber );
	
	FREE( pviWindowFrequenciesCount );
	FREE( piElecticalDelayTraceCounter );
	
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

