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


TSTE_TEST ( STD_MATH_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	int								iActualyPointsNumber							=	0; 
										
	double							*plfTrace										=	NULL,
									*plfFrequency									=	NULL;
									
	double							*pvlfStartFrequency								=	NULL,
									*pvlfStopFrequency								=	NULL;
									
	double							*plfResultTrace									=	NULL;
	
	double							**pvlfWindowFrequencies							=	NULL;
	
	int								*pviWindowFrequenciesCount						=	NULL;
	
	double							lfFullBandStartFrequency						=	0.0, 
									lfFullBandStopFrequency							=	0.0;
		
	double							lfDB_MeasureTimeout								=	0.0;
	
	int								iDB_TraceNumber									=	0,  
									iDB_NumberOfPoints								=	0,
									iDB_NumberOfWindows								=	0,
									iDB_NumberOfMathFunctions						=	0,
									iDB_NumberOfStoreToMemoryItems					=	0; 
									
	int								*vbDB_StoreTraceAsReference						=	0;
	
	char							*pszNetworkAnalyzerStateNumber					=	NULL,
									*pszGetNetworkAnalyzerStateNumber				=	NULL; 
									
	char							**pszNetworkAnalyzerMathFunction				=	NULL;
	
	char							*pCurrentTestPrefix								=	NULL;
	
	char							szFormatedString[LOW_STRING]					=	{0},
									szTemporaryString[LOW_STRING]					=	{0},
									szFormatedVariable[LOW_STRING]					=	{0};
	
	void							*pGraphItem										=	NULL;
	
	int								iPointIndex										=	0, 
									iWindowIndex									=	0,
									iFrequencyIndex									=	0,
									iCurrentPathIndex								=	0,
									iActionIndex									=	0; 
										
	double							lfDeltaValue									=	0.0,
									lfAverageValue									=	0.0,
									lfMinimumValue									=	0.0,
									lfMaximumValue									=	0.0,
									lfMinimumFreq									=	0.0, 
									lfMaximumFreq									=	0.0;
	
	int								iAverageCounter									=	0,
									iResultCounter									=	0;  
										
	int								hNetworkAnalyzer								=	0;
	
	int								iTraceColor										=	0;
	
	int								bResultExists									=	0,
									bResultAlreadyAdded								=	0;
	
	int								iResultVectorLength								=	0;
	
	int								iNumber_Of_A_Factors							=	0,
									iNumber_Of_B_Factors							=	0,
									iNumber_Of_Operations							=	0,
									iNumber_Of_InterOper							=	0, 
									iNumberOfActions								=	0;
	
	char							**pA_FactorsList								=	NULL,
									**pB_FactorsList								=	NULL,
									**pOperationsList								=	NULL,
									**pInterOperList								=	NULL; 
									
	double							*plfA_FactorVector								=	NULL, 
									*plfB_FactorVector								=	NULL; 
									 	
	double							*plfA_FactorFrequencyVector     				=	NULL,
									*plfB_FactorFrequencyVector						=	NULL;
	
	double							*plfTempCalculationVector						=	NULL,
									*plfCurrentCalculationVector					=	NULL,
									*plfCurrentCalculationVectorFrequency			=	NULL;
									
	double							*plfTempXVector									=	NULL,
									*plfTempYVector									=	NULL;
	
	double							*plfA_FreqVector								=	NULL,
									*plfB_FreqVector								=	NULL;
									
	int								iA_FactorVectorLength							=	0,
									iB_FactorVectorLength							=	0,
									iA_FactorFrequencyVectorLength					=	0,  
									iB_FactorFrequencyVectorLength					=	0,  
									iCalculationVectorLength						=	0,
									iCurrentVectorLength							=	0;
									
	int								vTraceColors[]									=	{ VAL_RED , VAL_GREEN , VAL_BLUE , VAL_CYAN , VAL_MAGENTA , VAL_YELLOW , VAL_DK_RED , VAL_DK_BLUE , VAL_DK_GREEN , VAL_DK_CYAN , VAL_DK_MAGENTA , VAL_DK_YELLOW , VAL_LT_GRAY , VAL_DK_GRAY };
			
	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;

	int								bDB_FrequencySecured							=	0; 

	double							lfCurrentFrequency								=	0.0;

	int								iDB_FrequencyIndex								=	0;
	
	int								bDB_EnableToStoreMeasureData					=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Mathematics Test."); 
	
	GET_TEST_PREFIX(pCurrentTestPrefix);
	
	PATH_GET_CURRENT_INDEX(iCurrentPathIndex); 

	GET_SEGMENT_LOOP_NUM(iCurrentTemperatureSegmentLoop);
	GET_SEGMENT_NUMBER(iCurrentTemperatureSegment);
	
	GET_FREQUENCIES_LOOP_INFO_SINGLE( lfCurrentFrequency , bDB_FrequencySecured , iDB_FrequencyIndex );

	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	do
	{
		bResultAlreadyAdded = 0;
		
		FREE( plfCurrentCalculationVector );
		FREE( plfCurrentCalculationVectorFrequency );
		
		iResultCounter++; 
		GET_RESULT_DISABLE_STATUS(iResultCounter,bResultExists);
		
		if ( bResultExists < 0 )
			break;
		
		sprintf( szFormatedVariable , "ResultCalc_%d_A_Factor" , iResultCounter );
		pA_FactorsList = GetVarStringArray( clb, VARTYPE_TEST , szFormatedVariable , &iNumber_Of_A_Factors );
		
		sprintf( szFormatedVariable , "ResultCalc_%d_Operations" , iResultCounter );
		pOperationsList = GetVarStringArray( clb, VARTYPE_TEST , szFormatedVariable , &iNumber_Of_Operations );
	
		sprintf( szFormatedVariable , "ResultCalc_%d_B_Factor" , iResultCounter );
		pB_FactorsList = GetVarStringArray( clb, VARTYPE_TEST , szFormatedVariable , &iNumber_Of_B_Factors );
		
		sprintf( szFormatedVariable , "ResultCalc_%d_InterOper" , iResultCounter );
		pInterOperList = GetVarStringArray( clb, VARTYPE_TEST , szFormatedVariable , &iNumber_Of_InterOper );
		
		if (( pA_FactorsList == NULL ) || ( pB_FactorsList == NULL ) || ( pOperationsList == NULL ))
			continue;
		
		if (( iNumber_Of_A_Factors == 0 ) || ( iNumber_Of_B_Factors == 0 ) || ( iNumber_Of_Operations == 0 ))
			continue;
		
		iNumberOfActions = iNumber_Of_A_Factors;
		
		if ( iNumber_Of_B_Factors < iNumberOfActions )
			iNumberOfActions = iNumber_Of_B_Factors;
		
		if ( iNumber_Of_Operations < iNumberOfActions )
			iNumberOfActions = iNumber_Of_Operations;
				
		if (( iNumber_Of_InterOper > 0 ) && ( iNumber_Of_InterOper < iNumberOfActions ))
			iNumberOfActions = iNumber_Of_InterOper;
		else
			iNumberOfActions = 1;
		
		if ( bResultExists  )
		{
			GET_SPECS_VALUE_COUNT(iResultVectorLength);
			
			if ( iResultVectorLength > 1 )
			{
				GET_CURRENT_RESULT_GRAPH_ITEM(pGraphItem);
			}
			else
			{
				pGraphItem = NULL;
			}
			
			for ( iActionIndex = 0; iActionIndex < iNumberOfActions; iActionIndex++ )
			{
				RemoveSurroundingWhiteSpace(pA_FactorsList[iActionIndex]); 
				RemoveSurroundingWhiteSpace(pB_FactorsList[iActionIndex]); 
				RemoveSurroundingWhiteSpace(pOperationsList[iActionIndex]); 
				StringUpperCase (pOperationsList[iActionIndex]);
				
				if ( strstr( pA_FactorsList[iActionIndex] , "TSeg_%d_TLoop_%d_Path_%d" ) )
				{
					sprintf( szFormatedString , pA_FactorsList[iActionIndex] , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex );	
				}
				else
				{
					if ( strstr( pA_FactorsList[iActionIndex] , "TSeg_%d_TLoop_%d" ) )
						sprintf( szFormatedString , pA_FactorsList[iActionIndex] , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop );
					else
						if ( strstr( pA_FactorsList[iActionIndex] , "TLoop_%d_Path_%d" ) )
							sprintf( szFormatedString , pA_FactorsList[iActionIndex] , iCurrentTemperatureSegmentLoop , iCurrentPathIndex );
						else
							if (( strstr( pA_FactorsList[iActionIndex] , "TSeg_%d" ) ) && ( strstr( pA_FactorsList[iActionIndex] , "Path_%d" ) ))
								sprintf( szFormatedString , pA_FactorsList[iActionIndex] , iCurrentTemperatureSegment , iCurrentPathIndex );
							else
							{
								if ( strstr( pA_FactorsList[iActionIndex] , "TSeg_%d" )) 
									sprintf( szFormatedString , pA_FactorsList[iActionIndex] , iCurrentTemperatureSegment );
								
								if ( strstr( pA_FactorsList[iActionIndex] , "Path_%d" ) )
									sprintf( szFormatedString , pA_FactorsList[iActionIndex] , iCurrentPathIndex );
								
								if ( strstr( pA_FactorsList[iActionIndex] , "TLoop_%d" ))
									sprintf( szFormatedString , pA_FactorsList[iActionIndex] , iCurrentTemperatureSegmentLoop );
							}
				}													 
				
				if ( strstr( szFormatedString , "_Frequency_%d" ) ) 
				{
					sprintf( szTemporaryString , szFormatedString , iDB_FrequencyIndex );
					strcpy( szFormatedString , szTemporaryString );
				}
				
				iA_FactorVectorLength = GetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedString , &plfA_FactorVector );
		
				if (( plfA_FactorVector == NULL ) || ( iA_FactorVectorLength == 0 ))
					break;
		
				sprintf( szFormatedVariable , "%s_Freq" , szFormatedString );
				iA_FactorFrequencyVectorLength = GetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , &plfA_FactorFrequencyVector );
				
				if (( plfA_FactorFrequencyVector == NULL ) || ( iA_FactorFrequencyVectorLength != iA_FactorVectorLength ))
					break;
				
				if (( iCalculationVectorLength != iA_FactorVectorLength ) && ( iCalculationVectorLength > 1 ) && ( iA_FactorVectorLength > 1 ))
				{
					APPLY_LINEAR_INTERPOLATION( plfA_FactorFrequencyVector , plfA_FactorVector , iA_FactorVectorLength , iCalculationVectorLength  , plfA_FactorFrequencyVector[0] , plfA_FactorFrequencyVector[(iA_FactorVectorLength-1)] , plfTempXVector , plfTempYVector );
				
					FREE( plfA_FactorVector );
					FREE( plfA_FreqVector );
				
					plfA_FreqVector = plfTempXVector;
					plfA_FactorVector = plfTempYVector;
				}
				else
				{
					plfA_FreqVector = plfA_FactorFrequencyVector;
					plfA_FactorVector = plfA_FactorVector;	
				}
				
				if ( strstr( pB_FactorsList[iActionIndex] , "TSeg_%d_TLoop_%d_Path_%d" ) )
				{
					sprintf( szFormatedString , pB_FactorsList[iActionIndex] , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex );	
				}
				else
				{
					if ( strstr( pB_FactorsList[iActionIndex] , "TSeg_%d_TLoop_%d" ) )
						sprintf( szFormatedString , pB_FactorsList[iActionIndex] , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop );
					else
						if ( strstr( pB_FactorsList[iActionIndex] , "TLoop_%d_Path_%d" ) )
							sprintf( szFormatedString , pB_FactorsList[iActionIndex] , iCurrentTemperatureSegmentLoop , iCurrentPathIndex );
						else
							if (( strstr( pB_FactorsList[iActionIndex] , "TSeg_%d" ) ) && ( strstr( pB_FactorsList[iActionIndex] , "Path_%d" ) ))
								sprintf( szFormatedString , pB_FactorsList[iActionIndex] , iCurrentTemperatureSegment , iCurrentPathIndex );
							else
							{
								if ( strstr( pB_FactorsList[iActionIndex] , "TSeg_%d" )) 
									sprintf( szFormatedString , pB_FactorsList[iActionIndex] , iCurrentTemperatureSegment );
								
								if ( strstr( pB_FactorsList[iActionIndex] , "Path_%d" ) )
									sprintf( szFormatedString , pB_FactorsList[iActionIndex] , iCurrentPathIndex );
								
								if ( strstr( pB_FactorsList[iActionIndex] , "TLoop_%d" ))
									sprintf( szFormatedString , pB_FactorsList[iActionIndex] , iCurrentTemperatureSegmentLoop );
							}
				}
				
				if ( strstr( szFormatedString , "_Frequency_%d" ) ) 
				{
					sprintf( szTemporaryString , szFormatedString , iDB_FrequencyIndex );
					strcpy( szFormatedString , szTemporaryString );
				}
				
				iB_FactorVectorLength = GetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedString , &plfB_FactorVector ); 
		
				if (( plfB_FactorVector == NULL ) || ( iB_FactorVectorLength == 0 ))
					break;
				
				sprintf( szFormatedVariable , "%s_Freq" , szFormatedString );
				iB_FactorFrequencyVectorLength = GetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , &plfB_FactorFrequencyVector );
				
				if (( plfB_FactorFrequencyVector == NULL ) || ( iB_FactorFrequencyVectorLength != iB_FactorVectorLength ))
					break;
				
				if (( iCalculationVectorLength != iB_FactorVectorLength ) && ( iCalculationVectorLength > 1 ) && ( iB_FactorVectorLength > 1 ))
				{
					APPLY_LINEAR_INTERPOLATION( plfB_FactorFrequencyVector , plfB_FactorVector , iB_FactorVectorLength , iCalculationVectorLength  , plfB_FactorFrequencyVector[0] , plfB_FactorFrequencyVector[(iB_FactorVectorLength-1)] , plfTempXVector , plfTempYVector );
				
					FREE( plfB_FactorVector );
					FREE( plfB_FreqVector );
				
					plfB_FreqVector = plfTempXVector;
					plfB_FactorVector = plfTempYVector;
				}
				else
				{
					plfB_FreqVector = plfB_FactorFrequencyVector;
					plfB_FactorVector = plfB_FactorVector;	
				}
				
				iCalculationVectorLength = iA_FactorVectorLength; 
				
				if ( iA_FactorVectorLength > iB_FactorVectorLength ) 
					iCalculationVectorLength = iB_FactorVectorLength; 
				
				FREE_CALLOC( plfTempCalculationVector , iCalculationVectorLength , sizeof(double));
				
				if ( plfCurrentCalculationVector == NULL )
				{
					CALLOC( plfCurrentCalculationVector , iCalculationVectorLength , sizeof(double));
					CALLOC( plfCurrentCalculationVectorFrequency , iCalculationVectorLength , sizeof(double));  
					
					memcpy( plfCurrentCalculationVectorFrequency , plfA_FreqVector , (sizeof(double)*iA_FactorVectorLength));
				}
				
				if ( iCalculationVectorLength == 1 )
				{   // Single Result
					
					switch(pOperationsList[iActionIndex][0])
					{
						case '+':	plfTempCalculationVector[0] = plfA_FactorVector[0] + plfB_FactorVector[0];
									break;
									
						case '-':   plfTempCalculationVector[0] = plfA_FactorVector[0] - plfB_FactorVector[0];
									break;
									
						case '*':   plfTempCalculationVector[0] = plfA_FactorVector[0] * plfB_FactorVector[0];
									break;
									
						case '/':   plfTempCalculationVector[0] = plfA_FactorVector[0] / plfB_FactorVector[0];
									break;
									
						default:	plfTempCalculationVector[0] = plfA_FactorVector[0] + plfB_FactorVector[0];
									break;			
					}
				}
				else
				{   // Trace Result
					do
					{
						if ( strstr( pOperationsList[iActionIndex] , "+" ))
						{
							for( iPointIndex = 0; iPointIndex < iA_FactorVectorLength; iPointIndex++ )
							{
								if ( iA_FactorVectorLength == iB_FactorVectorLength )
									plfTempCalculationVector[iPointIndex] = plfA_FactorVector[iPointIndex] + plfB_FactorVector[iPointIndex]; 
								else
									plfTempCalculationVector[iPointIndex] = plfA_FactorVector[iPointIndex] + plfB_FactorVector[0]; 
							}
						
							break;
						}
						
						if ( strstr( pOperationsList[iActionIndex] , "-" ))
						{
							for( iPointIndex = 0; iPointIndex < iA_FactorVectorLength; iPointIndex++ )
							{
								if ( iA_FactorVectorLength == iB_FactorVectorLength )
									plfTempCalculationVector[iPointIndex] = plfA_FactorVector[iPointIndex] - plfB_FactorVector[iPointIndex]; 
								else
									plfTempCalculationVector[iPointIndex] = plfA_FactorVector[iPointIndex] - plfB_FactorVector[0]; 
							}
						
							break;
						}
					
						if ( strstr( pOperationsList[iActionIndex] , "*" ))
						{
							for( iPointIndex = 0; iPointIndex < iA_FactorVectorLength; iPointIndex++ )
							{
								if ( iA_FactorVectorLength == iB_FactorVectorLength )
									plfTempCalculationVector[iPointIndex] = plfA_FactorVector[iPointIndex] * plfB_FactorVector[iPointIndex]; 
								else
									plfTempCalculationVector[iPointIndex] = plfA_FactorVector[iPointIndex] * plfB_FactorVector[0]; 
							}
						
							break;
						}
					
						if ( strstr( pOperationsList[iActionIndex] , "/" ))
						{
							for( iPointIndex = 0; iPointIndex < iA_FactorVectorLength; iPointIndex++ )
							{
								if ( iA_FactorVectorLength == iB_FactorVectorLength )
									plfTempCalculationVector[iPointIndex] = plfA_FactorVector[iPointIndex] / plfB_FactorVector[iPointIndex]; 
								else
									plfTempCalculationVector[iPointIndex] = plfA_FactorVector[iPointIndex] / plfB_FactorVector[0]; 
							}
						
							break;
						}
						
						if ( strstr( pOperationsList[iActionIndex] , "MAX" ))
						{
							plfTempCalculationVector[0] = plfA_FactorVector[0];
						
							for( iPointIndex = 0; iPointIndex < iA_FactorVectorLength; iPointIndex++ ) 
								if ( plfA_FactorVector[iPointIndex] > plfTempCalculationVector[0] )
									plfTempCalculationVector[0] = plfA_FactorVector[iPointIndex];
							
							iCalculationVectorLength = 1;
							
							break;
						}
						
						if ( strstr( pOperationsList[iActionIndex] , "MIN" ))
						{
							plfTempCalculationVector[0] = plfA_FactorVector[0];
						
							for( iPointIndex = 0; iPointIndex < iA_FactorVectorLength; iPointIndex++ ) 
								if ( plfA_FactorVector[iPointIndex] < plfTempCalculationVector[0] )
									plfTempCalculationVector[0] = plfA_FactorVector[iPointIndex];
						
							iCalculationVectorLength = 1;
							
							break;
						}
						
						if ( strstr( pOperationsList[iActionIndex] , "CENTER" ))
						{
							plfTempCalculationVector[0] = plfA_FactorVector[((iA_FactorVectorLength/2)+1)];
							
							iCalculationVectorLength = 1;
							
							break;
						}
						
						if ( strstr( pOperationsList[iActionIndex] , "DELTA" ))
						{
							lfMaximumValue = plfA_FactorVector[0];
							lfMinimumValue = plfA_FactorVector[0];
							
							for( iPointIndex = 0; iPointIndex < iA_FactorVectorLength; iPointIndex++ ) 
							{
								if ( plfA_FactorVector[iPointIndex] > lfMaximumValue )
									lfMaximumValue = plfA_FactorVector[iPointIndex];
						
								if ( plfA_FactorVector[iPointIndex] > lfMinimumValue )
									lfMinimumValue = plfA_FactorVector[iPointIndex];
							}
							
							plfTempCalculationVector[0] = ( lfMaximumValue - lfMinimumValue );
							
							iCalculationVectorLength = 1; 
							
							break;
						}
						
						if ( strstr( pOperationsList[iActionIndex] , "FREQUENCY" ))
						{
							GET_POINT_LINEAR_INTERPOLATION( plfA_FreqVector , plfA_FactorVector , iA_FactorVectorLength , plfB_FreqVector[0] , plfTempCalculationVector[0] );
						
							iCalculationVectorLength = 1; 
						
							break;
						}
						
						if ( strstr( pOperationsList[iActionIndex] , "ADD" ))
						{
							sprintf( szFormatedVariable , "Trace%d" , (iActionIndex+1) );                 
							
							if ( iActionIndex < sizeof(vTraceColors))
								iTraceColor = vTraceColors[iActionIndex];
							
							ADD_TRACE_TO_CURRENT_RESULT_GRAPH(pGraphItem,plfTempCalculationVector,iCalculationVectorLength,0,iCalculationVectorLength,plfA_FreqVector[0],plfA_FreqVector[iCalculationVectorLength-1], szFormatedVariable , iTraceColor );
						
							if ( bDB_EnableToStoreMeasureData )
							{
								sprintf( szFormatedVariable , "%s_Result%d_Trace%d" , pCurrentTestPrefix , iResultCounter , (bResultAlreadyAdded + 1));
								SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfTempCalculationVector , iCalculationVectorLength ); 
								sprintf( szFormatedVariable , "%s_Result%d_Trace%d_Freq" , pCurrentTestPrefix , iResultCounter , (bResultAlreadyAdded + 1));
								SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfA_FreqVector , iCalculationVectorLength ); 
							}
							
							bResultAlreadyAdded++;
							
							break;
						}
						
					} while(0);
				}
				
				if ( pInterOperList && ( iActionIndex < iNumber_Of_InterOper ))
				{
					RemoveSurroundingWhiteSpace(pInterOperList[iActionIndex]);
					StringUpperCase (pInterOperList[iActionIndex]);
					
					if ( iActionIndex == 0 )
					{
						memcpy( plfCurrentCalculationVector , plfTempCalculationVector , (iCalculationVectorLength * (sizeof(double))));
						
						if ( iCurrentVectorLength == 0 )
							iCurrentVectorLength = iCalculationVectorLength; 
					}

					if ( iActionIndex || (( strlen( pInterOperList[iActionIndex] ) > 1 ) && ( iCurrentVectorLength > 1 )))
					{
						if ( iCurrentVectorLength == 1 )
						{   // Single Result
					
							switch(pInterOperList[iActionIndex][0])
							{
								case '+':	plfCurrentCalculationVector[0] += plfTempCalculationVector[0];
											break;
									
								case '-':   plfCurrentCalculationVector[0] -= plfTempCalculationVector[0];
											break;
									
								case '*':   plfCurrentCalculationVector[0] *= plfTempCalculationVector[0];
											break;
									
								case '/':   plfCurrentCalculationVector[0] /= plfTempCalculationVector[0];
											break;
									
								default:	plfCurrentCalculationVector[0] += plfTempCalculationVector[0];
											break;			
							}
						}
						else
						{   // Trace Result
							do
							{
								if ( strstr( pInterOperList[iActionIndex] , "+" ))
								{
									for( iPointIndex = 0; iPointIndex < iCurrentVectorLength; iPointIndex++ )
									{
										if ( iCurrentVectorLength == iCalculationVectorLength )
											plfCurrentCalculationVector[iPointIndex] += plfTempCalculationVector[iPointIndex]; 
										else
											plfCurrentCalculationVector[iPointIndex] += plfTempCalculationVector[0]; 
									}
						
									break;
								}
						
								if ( strstr( pInterOperList[iActionIndex] , "-" ))
								{
									for( iPointIndex = 0; iPointIndex < iCurrentVectorLength; iPointIndex++ )
									{
										if ( iCurrentVectorLength == iCalculationVectorLength )
											plfCurrentCalculationVector[iPointIndex] -= plfTempCalculationVector[iPointIndex]; 
										else
											plfCurrentCalculationVector[iPointIndex] -= plfTempCalculationVector[0]; 
									}
						
									break;
								}
					
								if ( strstr( pInterOperList[iActionIndex] , "*" ))
								{
									for( iPointIndex = 0; iPointIndex < iCurrentVectorLength; iPointIndex++ )
									{
										if ( iCurrentVectorLength == iCalculationVectorLength )
											plfCurrentCalculationVector[iPointIndex] *= plfTempCalculationVector[iPointIndex]; 
										else
											plfCurrentCalculationVector[iPointIndex] *= plfTempCalculationVector[0]; 
									}
						
									break;
								}
					
								if ( strstr( pInterOperList[iActionIndex] , "/" ))
								{
									for( iPointIndex = 0; iPointIndex < iCurrentVectorLength; iPointIndex++ )
									{
										if ( iCurrentVectorLength == iCalculationVectorLength )
											plfCurrentCalculationVector[iPointIndex] /= plfTempCalculationVector[iPointIndex]; 
										else
											plfCurrentCalculationVector[iPointIndex] /= plfTempCalculationVector[0]; 
									}
						
									break;
								}
						
								if ( strstr( pInterOperList[iActionIndex] , "MAX" ))
								{
									lfMaximumValue = plfCurrentCalculationVector[0];
						
									for( iPointIndex = 0; iPointIndex < iCurrentVectorLength; iPointIndex++ ) 
										if ( plfCurrentCalculationVector[iPointIndex] > lfMaximumValue )
											lfMaximumValue = plfCurrentCalculationVector[iPointIndex];
							
									plfCurrentCalculationVector[iPointIndex] = lfMaximumValue;
									
									iCurrentVectorLength = 1;
							
									break;
								}
						
								if ( strstr( pInterOperList[iActionIndex] , "MIN" ))
								{
									lfMinimumValue = plfCurrentCalculationVector[0];
						
									for( iPointIndex = 0; iPointIndex < iCurrentVectorLength; iPointIndex++ ) 
										if ( plfCurrentCalculationVector[iPointIndex] < lfMinimumValue )
											lfMinimumValue = plfCurrentCalculationVector[iPointIndex];

									plfCurrentCalculationVector[iPointIndex] = lfMinimumValue;
									
									iCurrentVectorLength = 1;
							
									break;
								}
						
								if ( strstr( pInterOperList[iActionIndex] , "CENTER" ))
								{
									plfCurrentCalculationVector[0] = plfCurrentCalculationVector[((iCurrentVectorLength/2)+1)];
							
									iCurrentVectorLength = 1;
							
									break;
								}
						
								if ( strstr( pInterOperList[iActionIndex] , "DELTA" ))
								{
									lfMaximumValue = plfCurrentCalculationVector[0];
									lfMinimumValue = plfCurrentCalculationVector[0];
							
									for( iPointIndex = 0; iPointIndex < iCurrentVectorLength; iPointIndex++ ) 
									{
										if ( plfCurrentCalculationVector[iPointIndex] > lfMaximumValue )
											lfMaximumValue = plfCurrentCalculationVector[iPointIndex];
						
										if ( plfCurrentCalculationVector[iPointIndex] > lfMinimumValue )
											lfMinimumValue = plfCurrentCalculationVector[iPointIndex];
									}
							
									plfCurrentCalculationVector[0] = ( lfMaximumValue - lfMinimumValue );
							
									iCurrentVectorLength = 1; 
							
									break;
								}
						
								if ( strstr( pInterOperList[iActionIndex] , "FREQUENCY" ))
								{
									GET_POINT_LINEAR_INTERPOLATION( plfCurrentCalculationVectorFrequency , plfCurrentCalculationVector , iCurrentVectorLength , plfTempCalculationVector[0] , plfTempCalculationVector[0] );
						
									iCurrentVectorLength = 1; 
						
									break;
								}
						
								if ( strstr( pOperationsList[iActionIndex] , "ADD" ))
								{
									sprintf( szFormatedVariable , "Trace%d" , (iActionIndex+1) );                 
							
									if ( iActionIndex < sizeof(vTraceColors))
										iTraceColor = vTraceColors[iActionIndex];
							
									ADD_TRACE_TO_CURRENT_RESULT_GRAPH(pGraphItem,plfCurrentCalculationVector,iCurrentVectorLength,0,iCurrentVectorLength,plfCurrentCalculationVectorFrequency[0],plfCurrentCalculationVectorFrequency[iCurrentVectorLength-1],"Trace",VAL_GREEN);
						
									if ( bDB_EnableToStoreMeasureData )
									{
										sprintf( szFormatedVariable , "%s_Result%d_Trace%d" , pCurrentTestPrefix , iResultCounter , (bResultAlreadyAdded + 1));
										SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfCurrentCalculationVector , iCurrentVectorLength ); 
										sprintf( szFormatedVariable , "%s_Result%d_Trace%d_Freq" , pCurrentTestPrefix , iResultCounter , (bResultAlreadyAdded + 1));
										SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfCurrentCalculationVectorFrequency , iCurrentVectorLength ); 
									}
									
									bResultAlreadyAdded++;
							
									break;
								}
						
							} while(0);
						}
						
					}
				}
				
				if ( iCalculationVectorLength < iCurrentVectorLength )
					iCurrentVectorLength = iCalculationVectorLength;
			}
			
			if ( iCurrentVectorLength > 1 )
			{
				if ( bResultAlreadyAdded == 0 )
				{
					if ( bDB_EnableToStoreMeasureData )
					{
						sprintf( szFormatedVariable , "%s_Result%d_Trace%d" , pCurrentTestPrefix , iResultCounter , (bResultAlreadyAdded + 1));
						SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfCurrentCalculationVector , iCurrentVectorLength ); 
						sprintf( szFormatedVariable , "%s_Result%d_Trace%d_Freq" , pCurrentTestPrefix , iResultCounter , (bResultAlreadyAdded + 1));
						SetVarDoubleArray( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfCurrentCalculationVectorFrequency , iCurrentVectorLength ); 
					}
					
					ADD_TRACE_TO_CURRENT_RESULT_GRAPH(pGraphItem,plfCurrentCalculationVector,iCurrentVectorLength,0,iCurrentVectorLength,plfCurrentCalculationVectorFrequency[0],plfCurrentCalculationVectorFrequency[iCurrentVectorLength-1],"Trace",VAL_GREEN);
				}
		
				RESULT_CONST_NUM( iResultCounter , CLB_RESULT_GRAPH,"", pGraphItem , "Trace Result" );
			}
			else
			{
				sprintf( szFormatedVariable , "%s_Result%d" , pCurrentTestPrefix , iResultCounter );
				
				if ( plfCurrentCalculationVector )
				{
					if ( bDB_EnableToStoreMeasureData )
					{
						SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , plfCurrentCalculationVector[0] ); 
					}
					
					RESULT_CONST_NUM( iResultCounter , CLB_RESULT_DOUBLE, "", plfCurrentCalculationVector[0]  , "Single result" ); 
				}
			}
		}

		FREE_LIST( pA_FactorsList  , iNumber_Of_A_Factors  );  
		FREE_LIST( pB_FactorsList  , iNumber_Of_B_Factors  );  
		FREE_LIST( pOperationsList , iNumber_Of_Operations );
		FREE_LIST( pInterOperList  , iNumber_Of_InterOper  );  
		
	} while(1);
	
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
	
	FREE_LIST( pA_FactorsList , iNumber_Of_A_Factors );
	FREE_LIST( pB_FactorsList , iNumber_Of_B_Factors );
	FREE_LIST( pOperationsList , iNumber_Of_Operations );
	FREE_LIST( pInterOperList , iNumber_Of_InterOper );  
	
	FREE( plfA_FactorVector );
	FREE( plfB_FactorVector );
	
	FREE( plfTempCalculationVector );
	FREE( plfCurrentCalculationVector );

	FREE( plfCurrentCalculationVectorFrequency );
	 
	FREE( pCurrentTestPrefix );
	
	FREE( vbDB_StoreTraceAsReference );
	FREE( pszNetworkAnalyzerStateNumber ); 
	FREE( pszGetNetworkAnalyzerStateNumber );
	
	FREE( pviWindowFrequenciesCount );
	
	FREE( pvlfStartFrequency );
	FREE( pvlfStopFrequency );
	FREE( plfResultTrace );
	
	FREE( plfTrace );
	FREE( plfFrequency ); 
	
	return iTestError;
}

