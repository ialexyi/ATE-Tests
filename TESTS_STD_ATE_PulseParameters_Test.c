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


TSTE_TEST ( STD_PULSE_PARAM_TEST )
{	
	STD_ERROR						StdError										=	{0};
	
	int								iTestError										=	0;
	
	int								iDB_MeasureChannel								=	0, 
									iDB_NumberOfMeasures							=	0,
									iDB_NumberOfPulseLoops							=	0, 
									iDB_ResultBlockQuantity							=	0;
	
	int								iMeasureIndex									=	0,
									iPulseLoopIndex									=	0,   
									iRepeatLoopIndex								=	0,
									iResultOffsetIndex								=	0;
										
	char							*pszScopeStateFileName							=	NULL;
	
	double							*pvlfOffsets									=	NULL;
	
	char							szFormatedString[512]							=	{0},
									szFormatedVariable[512]							=	{0};
									
	tsMeasureItem					tMeasureDelay									=	{0.0},
									tMeasureRise									=	{0.0},
									tMeasureFall									=	{0.0}, 
									tMeasureWidth									=	{0.0}, 
									tMeasureSourceWidth								=	{0.0},
									tMeasureRearDelay								=	{0.0};

	double							lfDB_Signal_Threshold							=	0.0,
									lfDB_DelayLossFactor							=	0.0;  
										
	double							lfLeadJitter 									=	0.0,
									lfTrailJitter									=	0.0;
	
	double							lfSignalAmplitude								=	0.0;
	
	int								hOscilloscope									=	0;
									
	char							*pCurrentTestPrefix								=	NULL;

	int								iCurrentTemperatureSegmentLoop					=	0,
									iCurrentTemperatureSegment						=	0;
	
	int								iCurrentPathIndex								=	0;
	
	int								bDB_EnableToStoreMeasureData					=	0;
	
	LOG(CLB_LINE_HEADER_TEST, "Pulse Parameters Test."); 
	
	GET_SC_EQUIPMENT_HANDLE(hOscilloscope);	
	
	PATH_GET_CURRENT_INDEX( iCurrentPathIndex );
		
	GET_SEGMENT_LOOP_NUM(iCurrentTemperatureSegmentLoop);	
	GET_SEGMENT_NUMBER(iCurrentTemperatureSegment);
	
	bDB_EnableToStoreMeasureData = GetVarInt( clb, VARTYPE_TEST , "EnableToStoreMeasureData" ); 
	
	iDB_MeasureChannel = GetVarInt( clb, VARTYPE_EQUIPMENT_USE , "Measure_Channel" , hOscilloscope );
	
	iDB_NumberOfMeasures = GetVarInt( clb, VARTYPE_TEST , "Number_Of_Measures" ); 
	
	iDB_ResultBlockQuantity = GetVarInt( clb, VARTYPE_TEST , "Result_Block_Quantity" ); 
	
	lfDB_Signal_Threshold = GetVarDouble( clb, VARTYPE_TEST , "Measure_Signal_Threshold" );   
	
	lfDB_DelayLossFactor = GetVarDouble( clb, VARTYPE_TEST , "Measure_Delay_LossFactor" ); 

	sprintf( szFormatedString , "Delay Loss Factor = %0.3lf nSec" , ( lfDB_DelayLossFactor * 1E9 ));
	LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
	pszScopeStateFileName = GetVarString( clb, VARTYPE_EQUIPMENT_USE , "State_FileName" , hOscilloscope );  

	sprintf( szFormatedString , "Loading scope state (%s)" , pszScopeStateFileName );   //pulse_param.scp
	LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
	CHK_STDERR( DRV_Oscilloscope_MemoryState_Recall( hOscilloscope , pszScopeStateFileName ));
	CHK_STDERR( DRV_Oscilloscope_SweepContinuous ( hOscilloscope , 0 )); 
		
	CHK_STDERR( DRV_Oscilloscope_ConfigMeasureDelay ( hOscilloscope , 1 , 1 , 0 , iDB_MeasureChannel , 1 , 0 ));
			
	iDB_NumberOfPulseLoops = GetVarDoubleArray( clb, VARTYPE_EQUIPMENT_USE , "Pulse_Offset_List" , &pvlfOffsets , hOscilloscope ); 
		
	for ( iPulseLoopIndex = 0; iPulseLoopIndex < iDB_NumberOfPulseLoops; iPulseLoopIndex++ )
	{ 
		sprintf( szFormatedString , "Pulse Check, loop - %d" , iPulseLoopIndex+1 );
		LOG(CLB_LINE_NORMAL, szFormatedString ); 
	
		CHK_STDERR( DRV_Oscilloscope_HorizontalOffset( hOscilloscope  , pvlfOffsets[iPulseLoopIndex] ));  
	
		memset( &tMeasureDelay , 0 , sizeof(tsMeasureItem));
		memset( &tMeasureRise , 0 , sizeof(tsMeasureItem));
		memset( &tMeasureFall , 0 , sizeof(tsMeasureItem));
		memset( &tMeasureWidth , 0 , sizeof(tsMeasureItem));
		memset( &tMeasureSourceWidth , 0 , sizeof(tsMeasureItem));
		memset( &tMeasureRearDelay , 0 , sizeof(tsMeasureItem)); 
		
		for ( iMeasureIndex = 0; iMeasureIndex < iDB_NumberOfMeasures; iMeasureIndex++ )
		{
			sprintf( szFormatedString , "Measure - %d" , iMeasureIndex+1 );
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
			CHK_STDERR_CONTINUE( DRV_Oscilloscope_WaitForEvent( hOscilloscope , 5.0 )); 
			
			LOG(CLB_LINE_NORMAL, "Reading Pulse Parameters" ); 
			
			CHK_STDERR_CONTINUE( DRV_Oscilloscope_MeasureWidthPositive ( hOscilloscope , 1, 5.0 , &(tMeasureSourceWidth.lfLastValue) )); 
			
			if ( tMeasureSourceWidth.lfLastValue < 0.1 )
			{
				AddCalculateMasureItem(&tMeasureSourceWidth);
			
				CHK_STDERR_CONTINUE( DRV_Oscilloscope_MeasureVoltagePk2Pk ( hOscilloscope , iDB_MeasureChannel , 5.0 , &lfSignalAmplitude )); 
					
				if ( lfSignalAmplitude < lfDB_Signal_Threshold )
					continue;
				
				CHK_STDERR_CONTINUE( DRV_Oscilloscope_MeasureDelay ( hOscilloscope , iDB_MeasureChannel , 5.0 , &(tMeasureDelay.lfLastValue) )); 
				
				tMeasureDelay.lfLastValue = fabs(tMeasureDelay.lfLastValue) - lfDB_DelayLossFactor; 
				
				if (tMeasureDelay.lfLastValue < 1.0 )
					AddCalculateMasureItem(&tMeasureDelay);
				else
					tMeasureDelay.lfLastValue = 0;
					
				CHK_STDERR_CONTINUE( DRV_Oscilloscope_MeasureRiseTime ( hOscilloscope , iDB_MeasureChannel, 5.0 , &(tMeasureRise.lfLastValue) ));  
				
				if (tMeasureRise.lfLastValue < 1.0 )
					AddCalculateMasureItem(&tMeasureRise);
				else
					tMeasureRise.lfLastValue = 0;
				
				CHK_STDERR_CONTINUE( DRV_Oscilloscope_MeasureFallTime ( hOscilloscope , iDB_MeasureChannel, 5.0 , &(tMeasureFall.lfLastValue) ));  
				
				if (tMeasureFall.lfLastValue < 1.0 )
					AddCalculateMasureItem(&tMeasureFall);
				else
					tMeasureFall.lfLastValue = 0;
				
				CHK_STDERR_CONTINUE( DRV_Oscilloscope_MeasureWidthPositive ( hOscilloscope , iDB_MeasureChannel, 5.0 , &(tMeasureWidth.lfLastValue) ));     
				
				if (tMeasureWidth.lfLastValue < 1.0 )
					AddCalculateMasureItem(&tMeasureWidth);
				else
					tMeasureWidth.lfLastValue = 0;
				
				if (( tMeasureDelay.lfLastValue > 0.0 ) && ( tMeasureWidth.lfLastValue > 0.0 ) && ( tMeasureSourceWidth.lfLastValue > 0.0 ))
					tMeasureRearDelay.lfLastValue = tMeasureDelay.lfLastValue + (tMeasureWidth.lfLastValue-tMeasureSourceWidth.lfLastValue); 
				
				if (tMeasureRearDelay.lfLastValue < 1.0 )
					AddCalculateMasureItem(&tMeasureRearDelay); 
				else
					tMeasureRearDelay.lfLastValue = 0;
				
				sprintf( szFormatedString , "Delay (%4.1lfns,%4.1lfns) Edge (%4.1lfns,%4.1lfns) Width (%4.1lfns,%4.1lfns)" , ((tMeasureDelay.lfLastValue) * 1E9) , ((tMeasureRearDelay.lfLastValue) * 1E9) , ((tMeasureRise.lfLastValue) * 1E9) , ((tMeasureFall.lfLastValue) * 1E9) , ((tMeasureWidth.lfLastValue) * 1E9) , ((tMeasureSourceWidth.lfLastValue) * 1E9) );
				LOG(CLB_LINE_NORMAL, szFormatedString ); 
			}
			
			LOG(CLB_LINE_NORMAL, "Init Trigger" );
			
			CHK_STDERR( DRV_Oscilloscope_InitTrigger ( hOscilloscope ));
		}

		LOG(CLB_LINE_NORMAL, "Measure Finished." );  
		
		lfLeadJitter = (tMeasureDelay.lfMaxValue-tMeasureDelay.lfMinValue);
		lfTrailJitter = (tMeasureRearDelay.lfMaxValue-tMeasureRearDelay.lfMinValue); 
			
		iResultOffsetIndex = iRepeatLoopIndex * iDB_ResultBlockQuantity;
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Average" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureDelay.lfAverage ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureDelay.lfAverage , "Average Delay" );
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Maximum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureDelay.lfMaxValue ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureDelay.lfMaxValue , "Maximum Delay" );
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Minimum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureDelay.lfMinValue ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureDelay.lfMinValue , "Minimum Delay" );
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Average" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureRise.lfAverage ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureRise.lfAverage , "Average Rise" ); 
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Maximum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureRise.lfMaxValue ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureRise.lfMaxValue , "Maximum Rise" ); 
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Minimum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureRise.lfMinValue ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureRise.lfMinValue , "Minimum Rise" ); 
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Average" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureFall.lfAverage ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureFall.lfAverage , "Average Fall" ); 
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Maximum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureFall.lfMaxValue ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureFall.lfMaxValue , "Maximum Fall" );
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Minimum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureFall.lfMinValue ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureFall.lfMinValue , "Minimum Fall" );
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Average" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureWidth.lfAverage ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureWidth.lfAverage , "Average Width" ); 
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Maximum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureWidth.lfMaxValue ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureWidth.lfMaxValue , "Maximum Width" );
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Minimum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureWidth.lfMinValue ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureWidth.lfMinValue , "Minimum Width");
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Average" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureSourceWidth.lfAverage ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureSourceWidth.lfAverage , "Source Average Width" );
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Maximum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureSourceWidth.lfMaxValue ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureSourceWidth.lfMaxValue , "Source Maximum Width" );
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_Minimum" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , tMeasureSourceWidth.lfMinValue ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , tMeasureSourceWidth.lfMinValue , "Source Minimum Width" );
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_LeadJitter" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfLeadJitter ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , lfLeadJitter , "Lead Jitter" );
		
		if ( bDB_EnableToStoreMeasureData )
		{
			sprintf( szFormatedVariable , "%s_TSeg_%d_TLoop_%d_Path_%d_Pulse%d_TrailJitter" , pCurrentTestPrefix , iCurrentTemperatureSegment , iCurrentTemperatureSegmentLoop , iCurrentPathIndex , (iPulseLoopIndex+1) );
			SetVarDouble( clb, VARTYPE_MEASURE_STORE , szFormatedVariable , lfTrailJitter ); 
		}
		
		RESULT_CONST_NUM( ++iResultOffsetIndex , CLB_RESULT_DOUBLE , "" , lfTrailJitter , "Trail Jitter" );
		
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

	FREE(pszScopeStateFileName); 
	FREE(pvlfOffsets);
	
	return iTestError;
}


