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
#include "TESTS_RFM_ATE_SupportLibrary.h"

//==============================================================================
// Constants

//==============================================================================
// Types

typedef enum { SWITCH_MODE_TYPE_I , SWITCH_MODE_TYPE_Q , SWITCH_MODE_TYPE_PHASE , SWITCH_MODE_TYPE_SENTINEL } teSwitchModeType;

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions


int		FillCubeItem( tsSingleCubeItem *ptDataItem , int iMinimumAdd )
{
	if ( ptDataItem->leftTop.bFilled )
	{
		if ( ptDataItem->rigthBottom.bFilled )
		{
			if ( ptDataItem->rigthTop.bFilled == 0 )
			{
				ptDataItem->rigthTop.bMeasured = 0;
				ptDataItem->rigthTop.bFilled = 1;
				ptDataItem->rigthTop.I = ptDataItem->rigthBottom.I;
				ptDataItem->rigthTop.Q = ptDataItem->leftTop.Q;
			}
			
			if ( ptDataItem->leftBottom.bFilled == 0 )
			{
				ptDataItem->leftBottom.bMeasured = 0;
				ptDataItem->leftBottom.bFilled = 1;
				ptDataItem->leftBottom.I = ptDataItem->leftTop.I;
				ptDataItem->leftBottom.Q = ptDataItem->rigthBottom.Q;
			}
			
			if ( ptDataItem->center.bFilled == 0 )
			{
				ptDataItem->center.bMeasured = 0;
				ptDataItem->center.bFilled = 1;
				ptDataItem->center.I = ptDataItem->leftTop.I + (( ptDataItem->rigthBottom.I - ptDataItem->leftTop.I) / 2 ); 
				ptDataItem->center.Q = ptDataItem->rigthBottom.Q + (( ptDataItem->leftTop.Q - ptDataItem->rigthBottom.Q ) / 2 );
			}
			
			//---------- Open Cube back -------------------------//
			if (( ptDataItem->leftTop.I == ptDataItem->center.I ) || ( ptDataItem->center.I == ptDataItem->rigthTop.I ))
			{
				ptDataItem->center.I = ptDataItem->leftTop.I + iMinimumAdd;
				ptDataItem->rigthTop.I = ptDataItem->center.I + iMinimumAdd;
				ptDataItem->rigthBottom.I = ptDataItem->rigthTop.I;
			}
			
			if (( ptDataItem->leftTop.Q == ptDataItem->center.Q ) || ( ptDataItem->center.Q == ptDataItem->rigthBottom.Q ))
			{
				ptDataItem->center.Q = ptDataItem->leftTop.Q - iMinimumAdd;
				ptDataItem->rigthTop.Q = ptDataItem->center.Q - iMinimumAdd;
				ptDataItem->rigthBottom.Q = ptDataItem->rigthTop.Q;
			}
			
			return 0;
		}
		
		if ( ptDataItem->center.bFilled )
		{
			if ( ptDataItem->rigthBottom.bFilled == 0 )
			{
				ptDataItem->rigthBottom.bMeasured = 0;
				ptDataItem->rigthBottom.bFilled = 1;
				ptDataItem->rigthBottom.I = ptDataItem->center.I + ( ptDataItem->center.I - ptDataItem->leftTop.I );
				ptDataItem->rigthBottom.Q = ptDataItem->center.Q - ( ptDataItem->leftTop.Q - ptDataItem->center.Q );  
			}
			
			if ( ptDataItem->rigthTop.bFilled == 0 )
			{
				ptDataItem->rigthTop.bMeasured = 0;
				ptDataItem->rigthTop.bFilled = 1;
				ptDataItem->rigthTop.I = ptDataItem->rigthBottom.I;
				ptDataItem->rigthTop.Q = ptDataItem->leftTop.Q;
			}
			
			if ( ptDataItem->leftBottom.bFilled == 0 )
			{
				ptDataItem->leftBottom.bMeasured = 0;
				ptDataItem->leftBottom.bFilled = 1;
				ptDataItem->leftBottom.I = ptDataItem->leftTop.I;
				ptDataItem->leftBottom.Q = ptDataItem->rigthBottom.Q;
			}
			
			//---------- Open Cube back -------------------------//
			if (( ptDataItem->leftTop.I == ptDataItem->center.I ) || ( ptDataItem->center.I == ptDataItem->rigthTop.I ))
			{
				ptDataItem->center.I = ptDataItem->leftTop.I + iMinimumAdd;
				ptDataItem->rigthTop.I = ptDataItem->center.I + iMinimumAdd;
				ptDataItem->rigthBottom.I = ptDataItem->rigthTop.I;
			}
			
			if (( ptDataItem->leftTop.Q == ptDataItem->center.Q ) || ( ptDataItem->center.Q == ptDataItem->rigthBottom.Q ))
			{
				ptDataItem->center.Q = ptDataItem->leftTop.Q - iMinimumAdd;
				ptDataItem->rigthTop.Q = ptDataItem->center.Q - iMinimumAdd;
				ptDataItem->rigthBottom.Q = ptDataItem->rigthTop.Q;
			}
			
			return 0;
		}
	}

	if ( ptDataItem->rigthTop.bFilled )
	{
		if ( ptDataItem->leftBottom.bFilled )
		{
			if ( ptDataItem->leftTop.bFilled == 0 )
			{
				ptDataItem->leftTop.bMeasured = 0;
				ptDataItem->leftTop.bFilled = 1;
				ptDataItem->leftTop.I = ptDataItem->leftBottom.I;
				ptDataItem->leftTop.Q = ptDataItem->rigthBottom.Q;
			}
			
			if ( ptDataItem->rigthBottom.bFilled == 0 )
			{
				ptDataItem->rigthBottom.bMeasured = 0;
				ptDataItem->rigthBottom.bFilled = 1;
				ptDataItem->rigthBottom.I = ptDataItem->rigthTop.I;
				ptDataItem->rigthBottom.Q = ptDataItem->leftBottom.Q;
			}
			
			if ( ptDataItem->center.bFilled == 0 )
			{
				ptDataItem->center.bMeasured = 0;
				ptDataItem->center.bFilled = 1;
				ptDataItem->center.I = ptDataItem->leftTop.I + (( ptDataItem->rigthBottom.I - ptDataItem->leftTop.I) / 2 ); 
				ptDataItem->center.Q = ptDataItem->rigthBottom.Q + (( ptDataItem->leftTop.Q - ptDataItem->rigthBottom.Q ) / 2 );
			}
			
			//---------- Open Cube back -------------------------//
			if (( ptDataItem->leftTop.I == ptDataItem->center.I ) || ( ptDataItem->center.I == ptDataItem->rigthTop.I ))
			{
				ptDataItem->center.I = ptDataItem->rigthTop.I - iMinimumAdd;
				ptDataItem->leftTop.I = ptDataItem->center.I - iMinimumAdd;
				ptDataItem->leftBottom.I = ptDataItem->leftTop.I;
			}
			
			if (( ptDataItem->leftTop.Q == ptDataItem->center.Q ) || ( ptDataItem->center.Q == ptDataItem->rigthBottom.Q ))
			{
				ptDataItem->center.Q = ptDataItem->leftTop.Q - iMinimumAdd;
				ptDataItem->rigthBottom.Q = ptDataItem->center.Q - iMinimumAdd;
				ptDataItem->leftBottom.Q = ptDataItem->rigthBottom.Q;
			}
			
			return 0;
		}
		
		if ( ptDataItem->center.bFilled )
		{
			if ( ptDataItem->leftBottom.bFilled == 0 )
			{
				ptDataItem->leftBottom.bMeasured = 0;
				ptDataItem->leftBottom.bFilled = 1;
				ptDataItem->leftBottom.I = ptDataItem->center.I - ( ptDataItem->rigthTop.I - ptDataItem->center.I );
				ptDataItem->leftBottom.Q = ptDataItem->center.Q - ( ptDataItem->rigthTop.Q - ptDataItem->center.Q );  
			}
			
			if ( ptDataItem->leftTop.bFilled == 0 )
			{
				ptDataItem->leftTop.bMeasured = 0;
				ptDataItem->leftTop.bFilled = 1;
				ptDataItem->leftTop.I = ptDataItem->leftBottom.I;
				ptDataItem->leftTop.Q = ptDataItem->rigthBottom.Q;
			}
			
			if ( ptDataItem->rigthBottom.bFilled == 0 )
			{
				ptDataItem->rigthBottom.bMeasured = 0;
				ptDataItem->rigthBottom.bFilled = 1;
				ptDataItem->rigthBottom.I = ptDataItem->rigthTop.I;
				ptDataItem->rigthBottom.Q = ptDataItem->leftBottom.Q;
			}
			
			//---------- Open Cube back -------------------------//
			if (( ptDataItem->leftTop.I == ptDataItem->center.I ) || ( ptDataItem->center.I == ptDataItem->rigthTop.I ))
			{
				ptDataItem->center.I = ptDataItem->rigthTop.I - iMinimumAdd;
				ptDataItem->leftTop.I = ptDataItem->center.I - iMinimumAdd;
				ptDataItem->leftBottom.I = ptDataItem->leftTop.I;
			}
			
			if (( ptDataItem->leftTop.Q == ptDataItem->center.Q ) || ( ptDataItem->center.Q == ptDataItem->rigthBottom.Q ))
			{
				ptDataItem->center.Q = ptDataItem->leftTop.Q - iMinimumAdd;
				ptDataItem->rigthBottom.Q = ptDataItem->center.Q - iMinimumAdd;
				ptDataItem->leftBottom.Q = ptDataItem->rigthBottom.Q;
			}
			
			return 0;
		}
	}
	
	if ( ptDataItem->center.bFilled )
	{
		if ( ptDataItem->leftBottom.bFilled )
		{
			if ( ptDataItem->rigthTop.bFilled == 0 )
			{
				ptDataItem->rigthTop.bMeasured = 0;
				ptDataItem->rigthTop.bFilled = 1;
				ptDataItem->rigthTop.I = ptDataItem->center.I + ( ptDataItem->center.I - ptDataItem->leftBottom.I );
				ptDataItem->rigthTop.Q = ptDataItem->center.Q + ( ptDataItem->center.Q - ptDataItem->leftBottom.Q );  
			}
			
			if ( ptDataItem->leftTop.bFilled == 0 )
			{
				ptDataItem->leftTop.bMeasured = 0;
				ptDataItem->leftTop.bFilled = 1;
				ptDataItem->leftTop.I = ptDataItem->leftBottom.I;
				ptDataItem->leftTop.Q = ptDataItem->rigthBottom.Q;
			}
			
			if ( ptDataItem->rigthBottom.bFilled == 0 )
			{
				ptDataItem->rigthBottom.bMeasured = 0;
				ptDataItem->rigthBottom.bFilled = 1;
				ptDataItem->rigthBottom.I = ptDataItem->rigthTop.I;
				ptDataItem->rigthBottom.Q = ptDataItem->leftBottom.Q;
			}
			
			//---------- Open Cube back -------------------------//
			if (( ptDataItem->leftTop.I == ptDataItem->center.I ) || ( ptDataItem->center.I == ptDataItem->rigthTop.I ))
			{
				ptDataItem->rigthTop.I = ptDataItem->center.I + iMinimumAdd;
				ptDataItem->rigthBottom.I = ptDataItem->rigthTop.I;
				ptDataItem->leftTop.I = ptDataItem->center.I - iMinimumAdd;
				ptDataItem->leftBottom.I = ptDataItem->leftTop.I;
			}
			
			if (( ptDataItem->leftTop.Q == ptDataItem->center.Q ) || ( ptDataItem->center.Q == ptDataItem->rigthBottom.Q ))
			{
				ptDataItem->leftTop.Q = ptDataItem->center.Q + iMinimumAdd;
				ptDataItem->rigthTop.Q = ptDataItem->leftTop.Q; 
				ptDataItem->leftBottom.Q = ptDataItem->center.Q - iMinimumAdd;
				ptDataItem->rigthBottom.Q = ptDataItem->leftBottom.Q;
			}
			
			return 0;
		}
		
		if ( ptDataItem->rigthBottom.bFilled )
		{
			if ( ptDataItem->leftTop.bFilled == 0 )
			{
				ptDataItem->leftTop.bMeasured = 0;
				ptDataItem->leftTop.bFilled = 1;
				ptDataItem->leftTop.I = ptDataItem->center.I - ( ptDataItem->rigthBottom.I - ptDataItem->center.I );
				ptDataItem->leftTop.Q = ptDataItem->center.Q + ( ptDataItem->center.Q - ptDataItem->rigthBottom.Q );  
			}
			
			if ( ptDataItem->rigthTop.bFilled == 0 )
			{
				ptDataItem->rigthTop.bMeasured = 0;
				ptDataItem->rigthTop.bFilled = 1;
				ptDataItem->rigthTop.I = ptDataItem->rigthBottom.I;
				ptDataItem->rigthTop.Q = ptDataItem->leftTop.Q;
			}
			
			if ( ptDataItem->leftBottom.bFilled == 0 )
			{
				ptDataItem->leftBottom.bMeasured = 0;
				ptDataItem->leftBottom.bFilled = 1;
				ptDataItem->leftBottom.I = ptDataItem->leftTop.I;
				ptDataItem->leftBottom.Q = ptDataItem->rigthBottom.Q;
			}
			
			//---------- Open Cube back -------------------------//
			if (( ptDataItem->leftTop.I == ptDataItem->center.I ) || ( ptDataItem->center.I == ptDataItem->rigthTop.I ))
			{
				ptDataItem->rigthTop.I = ptDataItem->center.I + iMinimumAdd;
				ptDataItem->rigthBottom.I = ptDataItem->rigthTop.I;
				ptDataItem->leftTop.I = ptDataItem->center.I - iMinimumAdd;
				ptDataItem->leftBottom.I = ptDataItem->leftTop.I;
			}
			
			if (( ptDataItem->leftTop.Q == ptDataItem->center.Q ) || ( ptDataItem->center.Q == ptDataItem->rigthBottom.Q ))
			{
				ptDataItem->leftTop.Q = ptDataItem->center.Q + iMinimumAdd;
				ptDataItem->rigthTop.Q = ptDataItem->leftTop.Q; 
				ptDataItem->leftBottom.Q = ptDataItem->center.Q - iMinimumAdd;
				ptDataItem->rigthBottom.Q = ptDataItem->leftBottom.Q;
			}
			
			return 0;
		}
	
		return -1;
	}
	
	return -1;
}

STD_ERROR			UUT_IQ_RecursiveConvergence( sClb* clb , int iLevelCount , teCorrectionType correctionType , tsConfigurationItem config , tsSingleCubeItem *ptDataItem , tsSinglePointItem *ptLowerPoint , int *pStatus )
{
	tsSinglePointItem				**vtDataPointsArray								=	NULL,
									tAverageLowerPoint								=	{0};
	
	int								index											=	0,
									iSweepIndex										=	0,
									iLowerIndex										=	0,
									iAverageLowerIndex								=	0; 
										
	STD_ERROR						StdError										=	{0};

	char							szFormatedString[256]							=	{0};
	
	double							lfMarkerAmplitude								=	0.0,
									lfMarkerFrequency								=	0.0,  
									lfCurrentValue									=	0.0,  
									lfLowerMeasure									=	0.0,
									lfAverage										=	0.0;
	
	tsSingleCubeItem				*ptNewLeftTopCubeDataItem						=	NULL,
									*ptNewRigthTopCubeDataItem						=	NULL,
									*ptNewLeftBottomCubeDataItem					=	NULL,
									*ptNewRigthBottomCubeDataItem					=	NULL,
									*ptNewCenterCubeDataItem						=	NULL;
									
	int								iATT_Value										=	0,
									iDCA_Value										=	0,
									iVVA_Value										=	0,
									iPHASE_Value									=	0;
	
	int								viI_Value[2]									=	{0},
									viQ_Value[2]									=	{0};
	
	int								vbItemAlreadyExists[5]							=	{0};
	
	static int						iLast_I_Value									=	0,
									iLast_Q_Value									=	0;
	
	int								bNoItemToMeasureExists							=	0;
	
	int								iMeasureItemCounter								=	0;
	
	IF ((( ptDataItem == NULL ) || ( ptLowerPoint == NULL ) || ( pStatus == NULL )) , "Found NULL pointer arguments." );
	
	if ( iLevelCount == 0 )
	{
		ptLowerPoint->lfMeasure = 1E9;
		
		iLast_I_Value = 1E9;
		iLast_Q_Value = 1E9;
	}
	
	CALLOC_ERR( vtDataPointsArray , 5 , sizeof(tsSinglePointItem*));
	
	if ( ptDataItem->vtCubeDataArray == NULL )
	{
		CALLOC_ERR( ptDataItem->vtCubeDataArray , 6 , sizeof(tsSingleCubeItem*));   
		ptDataItem->iNumberOfCubeDataItems = 6;
	}
	
	vtDataPointsArray[0] = &(ptDataItem->leftTop);  
	vtDataPointsArray[1] = &(ptDataItem->rigthTop);
	vtDataPointsArray[2] = &(ptDataItem->center);
	vtDataPointsArray[3] = &(ptDataItem->leftBottom);
	vtDataPointsArray[4] = &(ptDataItem->rigthBottom);
	
	if ( fabs( ptDataItem->leftTop.Q - ptDataItem->leftBottom.Q ) < config.iConvergenceLimit )
	{
		RETURN;  
	}
	
	do
	{
		bNoItemToMeasureExists = 1;
		iMeasureItemCounter++;
		
		for ( index = 0 ; index < 5 ; index++ )
			if ( (vtDataPointsArray[index])->bMeasured == 0 ) 
			{
				bNoItemToMeasureExists = 0;
				
				if ( iMeasureItemCounter < 2 )
				{
					if (( (vtDataPointsArray[index])->I != iLast_I_Value ) && ( (vtDataPointsArray[index])->Q != iLast_Q_Value ))
						continue;
				}
				
				switch ( correctionType )
				{
					 case CORRECT_TYPE_ATT_VVA:
					
						iATT_Value = (vtDataPointsArray[index])->I;
						iVVA_Value = (vtDataPointsArray[index])->Q;
					
						//------------------- Set VVA and DCA Correction  -------------------------------------//
						if ( iLast_I_Value != (vtDataPointsArray[index])->I )
						{
							sprintf( szFormatedString , "UUT :: Set TX ATT = %d" , iATT_Value ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
						
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "TX_ATT" , &iATT_Value , 1 , sizeof(int)));  
							iMeasureItemCounter = 0;
						}
						
						if ( iLast_Q_Value != (vtDataPointsArray[index])->Q )
						{
							sprintf( szFormatedString , "UUT :: Set TX VVA = %d" , iVVA_Value ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
						
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "TX_VVA" , &iVVA_Value , 1 , sizeof(int))); 
							iMeasureItemCounter = 0;
						}
						
						break;
				
					case CORRECT_TYPE_DCA_VVA:
					
						iDCA_Value = (vtDataPointsArray[index])->I;
						iVVA_Value = (vtDataPointsArray[index])->Q;
					
						//------------------- Set VVA and DCA Correction  -------------------------------------//
						
						if ( iLast_I_Value != (vtDataPointsArray[index])->I )
						{
							sprintf( szFormatedString , "UUT :: Set TX DCA = %d" , iDCA_Value ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
				
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "TX_DCA" , &iDCA_Value , 1 , sizeof(int)));  
							iMeasureItemCounter = 0;
						}
						
						if ( iLast_Q_Value != (vtDataPointsArray[index])->Q )
						{
							sprintf( szFormatedString , "UUT :: Set TX VVA = %d" , iVVA_Value ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
				
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "TX_VVA" , &iVVA_Value , 1 , sizeof(int))); 
							iMeasureItemCounter = 0;
						}
						
						break;
					
					case CORRECT_TYPE_IQ:  // ( -1023 <> +1023 )				
					
						//------------------- Set I and Q Correction  -------------------------------------//
						
						if ( iLast_I_Value != (vtDataPointsArray[index])->I )
						{
							sprintf( szFormatedString , "UUT :: Set TX I offset = %d" , ((vtDataPointsArray[index])->I) ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
				
							if ( (vtDataPointsArray[index])->I < 0 )
								viI_Value[0] = 0x80 | (( -1 * ((vtDataPointsArray[index])->I) ) & 3 );
		    				else
								viI_Value[0] = (((vtDataPointsArray[index])->I) & 3 );
	
							viI_Value[1] = (int)( abs((vtDataPointsArray[index])->I) / 4.0 ) & 0xff;
						
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "I_OFFSET" , viI_Value , 2 , sizeof(int))); 
							iMeasureItemCounter = 0;
						}
						
						if ( iLast_Q_Value != (vtDataPointsArray[index])->Q )
						{
							sprintf( szFormatedString , "UUT :: Set TX Q offset = %d" , ((vtDataPointsArray[index])->Q) ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
							
							if ( (vtDataPointsArray[index])->Q < 0 )
								viQ_Value[0] = 0x80 | (( -1 * ((vtDataPointsArray[index])->Q) ) & 3 );
		    				else
								viQ_Value[0] = (((vtDataPointsArray[index])->Q) & 3 );
	
							viQ_Value[1] = (int)( abs((vtDataPointsArray[index])->Q) / 4.0 ) & 0xff;
						
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "Q_OFFSET" , viQ_Value , 2 , sizeof(int))); 
							iMeasureItemCounter = 0;
						}
						
						break;
					
					case CORRECT_TYPE_PHASE_IQ: // ( 0 <> 255 ) 
													
						//------------------- Set Tx Phase - I and Q  Correction  -------------------------------------//  
						
						if ( iLast_I_Value != (vtDataPointsArray[index])->I )
						{
							sprintf( szFormatedString , "UUT :: Set TX I gain = %d" , ((vtDataPointsArray[index])->I) ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
					
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "I_GAIN" , &((vtDataPointsArray[index])->I) , 1 , sizeof(int)));
							iMeasureItemCounter = 0;
						}
						
						if ( iLast_Q_Value != (vtDataPointsArray[index])->Q )
						{
							sprintf( szFormatedString , "UUT :: Set TX Q gain = %d" , ((vtDataPointsArray[index])->Q) ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
					
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "Q_GAIN" , &((vtDataPointsArray[index])->Q) , 1 , sizeof(int)));  
							iMeasureItemCounter = 0;
						}
						
						break;	
					
					case CORRECT_TYPE_PHASE_IPH: // ( -128 <> +128 )
			
						//------------------- Set Tx Phase - I and Phase  Correction  -------------------------------------//  
						
						if ( iLast_I_Value != (vtDataPointsArray[index])->I )
						{
							sprintf( szFormatedString , "UUT :: Set TX I gain = %d" , ((vtDataPointsArray[index])->I) ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
					
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "I_GAIN" , &((vtDataPointsArray[index])->I) , 1 , sizeof(int)));
							iMeasureItemCounter = 0;
						}
						
						(vtDataPointsArray[index])->PH = (vtDataPointsArray[index])->Q;
						
						if ( iLast_Q_Value != (vtDataPointsArray[index])->Q )
						{
							sprintf( szFormatedString , "UUT :: Set TX Phase = %d" , ((vtDataPointsArray[index])->Q) ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
					
							iPHASE_Value = ((vtDataPointsArray[index])->Q) & 0x0FF; 
							
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "PHASE_BALANCE" , &iPHASE_Value , 1 , sizeof(int)));  
							iMeasureItemCounter = 0;
						}
						
						break;
					
					case CORRECT_TYPE_PHASE_QPH:  
					
						//------------------- Set Tx Phase - Phase and Q  Correction  -------------------------------------//  
						
						(vtDataPointsArray[index])->PH = (vtDataPointsArray[index])->I;
						
						if ( iLast_I_Value != (vtDataPointsArray[index])->I )
						{
							sprintf( szFormatedString , "UUT :: Set TX Phase = %d" , ((vtDataPointsArray[index])->I) ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
					
							iPHASE_Value = ((vtDataPointsArray[index])->I) & 0x0FF; 
						
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "PHASE_BALANCE" , &iPHASE_Value , 1 , sizeof(int)));
							iMeasureItemCounter = 0;
						}
						
						if ( iLast_Q_Value != (vtDataPointsArray[index])->Q )
						{
							sprintf( szFormatedString , "UUT :: Set TX Q = %d" , ((vtDataPointsArray[index])->Q) ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
						
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "Q_GAIN" , &((vtDataPointsArray[index])->Q) , 1 , sizeof(int)));  
							iMeasureItemCounter = 0;
						}
						
						break;
					
					case CORRECT_TYPE_VVA_ONLY: 
					case CORRECT_TYPE_ATT_ONLY:
					case CORRECT_TYPE_DCA_ONLY:
					default:
						break;

				}    
			
			if ( config.lfMeasureDelay > 0.0 )
			{
				WAIT( "" , config.lfMeasureDelay ); 
			}
		
			for ( iSweepIndex = 0; iSweepIndex < config.iSpectrumNumberOfSweeps; iSweepIndex++ )
			{
				sprintf( szFormatedString , "Sweep - %d" , iSweepIndex+1 );
				LOG(CLB_LINE_NORMAL, szFormatedString ); 

				DRV_SpectrumAnalyzer_GetSweep ( config.hSpectrumAnalyzer , config.lfSpectrumTimeout ); 
			}

			CHK_STDERR( DRV_SpectrumAnalyzer_WaitForIntrumentDone ( config.hSpectrumAnalyzer , config.lfSpectrumTimeout ));  
			
			CHK_STDERR( DRV_SpectrumAnalyzer_SearchMarkerPeak( config.hSpectrumAnalyzer , 1 ));

			CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerFrequency( config.hSpectrumAnalyzer , 1 , &lfMarkerFrequency )); 
			CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerAmplitude( config.hSpectrumAnalyzer , 1 , &lfMarkerAmplitude )); 

			lfMarkerAmplitude -= config.lfLossFactor;
			
			if ( config.bFrequencySecured )
				sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf dBm" , lfMarkerAmplitude );   
			else
				sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf GHz / %0.3lf dBm" , (lfMarkerFrequency / 1E9) , lfMarkerAmplitude );   
	
			LOG(CLB_LINE_NORMAL, szFormatedString ); 
			
			(vtDataPointsArray[index])->lfMeasure = lfMarkerAmplitude; 	
			(vtDataPointsArray[index])->lfOriginalMeasure = lfMarkerAmplitude;
			
			(vtDataPointsArray[index])->bMeasured = 1;
			
			iLast_I_Value = vtDataPointsArray[index]->I;
			iLast_Q_Value = vtDataPointsArray[index]->Q;
		}
		
	} while( bNoItemToMeasureExists == 0 );
	
	lfAverage = 0.0;
	
	for ( index = 0 ; index < 5 ; index++ ) 
	{
		if ( config.bDeltaSearch )
		{
			lfCurrentValue = fabs((vtDataPointsArray[index])->lfOriginalMeasure - config.lfNeededValue );
			(vtDataPointsArray[index])->lfMeasure = lfCurrentValue;
		}
		else
			lfCurrentValue = (vtDataPointsArray[index])->lfMeasure;
			
		lfAverage += lfCurrentValue;

		if (( lfCurrentValue < lfLowerMeasure ) || ( index == 0 ))
		{
			lfLowerMeasure = lfCurrentValue;   
			iLowerIndex = index;
		}
	}
	
	ptDataItem->lfAverageResult = lfAverage / 5.0;
	ptDataItem->lfLowerResult = lfLowerMeasure;
	
	if ( lfLowerMeasure < ptLowerPoint->lfMeasure )
	{
		*ptLowerPoint = *(vtDataPointsArray[iLowerIndex]);
		
		if ( config.bQuickSearch )
		{
			if ( lfLowerMeasure <= config.lfQuickAccuracy )
			{
				*pStatus = 1;
				RETURN;
			}
		}
		else
		{
			*pStatus = 1;
			RETURN;
		}
	}
	
	if (!config.bSingleMeasure)
	{
		if ( ptDataItem->vtCubeDataArray[0] == NULL )
		{
			CALLOC_ERR( ptNewCenterCubeDataItem , 1 , sizeof( tsSingleCubeItem ));
		}
		else
		{
			ptNewCenterCubeDataItem = ptDataItem->vtCubeDataArray[0];  
			vbItemAlreadyExists[0] = 1;
		}
		
		if ( ptDataItem->vtCubeDataArray[1] == NULL )
		{
			CALLOC_ERR( ptNewLeftTopCubeDataItem , 1 , sizeof( tsSingleCubeItem ));	
		}
		else
		{
			ptNewLeftTopCubeDataItem = ptDataItem->vtCubeDataArray[1];
			vbItemAlreadyExists[1] = 1;
		}
		
		if ( ptDataItem->vtCubeDataArray[2] == NULL )
		{
			CALLOC_ERR( ptNewRigthTopCubeDataItem , 1 ,	sizeof( tsSingleCubeItem ));
		}
		else
		{
			ptNewRigthTopCubeDataItem = ptDataItem->vtCubeDataArray[2];
			vbItemAlreadyExists[2] = 1;
		}
		
		if ( ptDataItem->vtCubeDataArray[3] == NULL )
		{
			CALLOC_ERR( ptNewLeftBottomCubeDataItem , 1 , sizeof( tsSingleCubeItem ));
		}
		else
		{
			ptNewRigthBottomCubeDataItem = ptDataItem->vtCubeDataArray[3];
			vbItemAlreadyExists[3] = 1;
		}
		
		if ( ptDataItem->vtCubeDataArray[4] == NULL )
		{
			CALLOC_ERR( ptNewRigthBottomCubeDataItem , 1 , sizeof( tsSingleCubeItem ));
		}
		else
		{
			ptNewLeftBottomCubeDataItem = ptDataItem->vtCubeDataArray[4];
			vbItemAlreadyExists[4] = 1;
		}
		
		do
		{
			// *00
			// 000
			// 000
			if ( vbItemAlreadyExists[0] == 0 )
			{
				ptNewLeftTopCubeDataItem->leftTop = ptDataItem->leftTop;
				ptNewLeftTopCubeDataItem->rigthBottom = ptDataItem->center;
	
				FillCubeItem( ptNewLeftTopCubeDataItem , 0 );
			}
		
			config.bSingleMeasure = 1;
			UUT_IQ_RecursiveConvergence( clb , (iLevelCount+1) , correctionType , config , ptNewLeftTopCubeDataItem , ptLowerPoint , pStatus );
	
			if ( config.bQuickSearch && (*pStatus) ) 
				break;
			
			// 00*
			// 000
			// 000
			if ( vbItemAlreadyExists[1] == 0 )
			{
				ptNewRigthTopCubeDataItem->leftTop = ptNewLeftTopCubeDataItem->rigthTop;
				ptNewRigthTopCubeDataItem->rigthTop = ptDataItem->rigthTop;
				ptNewRigthTopCubeDataItem->leftBottom = ptDataItem->center;
	
				FillCubeItem( ptNewRigthTopCubeDataItem , 0 ); 
			}
		
			config.bSingleMeasure = 1;
			UUT_IQ_RecursiveConvergence( clb , (iLevelCount+1) , correctionType , config , ptNewRigthTopCubeDataItem , ptLowerPoint , pStatus );
			
			if ( config.bQuickSearch && (*pStatus) ) 
				break;
			
			// 000
			// 000
			// 00*
			if ( vbItemAlreadyExists[2] == 0 )
			{
				ptNewRigthBottomCubeDataItem->rigthTop = ptNewRigthTopCubeDataItem->rigthBottom;
				ptNewRigthBottomCubeDataItem->leftTop = ptDataItem->center;
				ptNewRigthBottomCubeDataItem->rigthBottom = ptDataItem->rigthBottom;
	
				FillCubeItem( ptNewRigthBottomCubeDataItem , 0 ); 
			}
		
			config.bSingleMeasure = 1;
			UUT_IQ_RecursiveConvergence( clb , (iLevelCount+1) , correctionType , config , ptNewRigthBottomCubeDataItem , ptLowerPoint , pStatus );
	
			if ( config.bQuickSearch && (*pStatus) ) 
				break;
			
			// 000
			// 000
			// *00
			if ( vbItemAlreadyExists[3] == 0 )
			{
				ptNewLeftBottomCubeDataItem->rigthTop = ptDataItem->center;
				ptNewLeftBottomCubeDataItem->leftBottom = ptDataItem->leftBottom;
				ptNewLeftBottomCubeDataItem->leftTop = ptNewLeftTopCubeDataItem->leftBottom;
				ptNewLeftBottomCubeDataItem->rigthBottom = ptNewRigthBottomCubeDataItem->leftBottom;
	
				FillCubeItem( ptNewLeftBottomCubeDataItem , 0 ); 
			}
		
			config.bSingleMeasure = 1;
			UUT_IQ_RecursiveConvergence( clb , (iLevelCount+1) , correctionType , config , ptNewLeftBottomCubeDataItem , ptLowerPoint , pStatus );
	
			if ( config.bQuickSearch && (*pStatus) ) 
				break;
			
			// 000
			// 0*0
			// 000
			if ( vbItemAlreadyExists[4] == 0 )
			{
				ptNewCenterCubeDataItem->rigthTop = ptNewRigthTopCubeDataItem->center;
				ptNewCenterCubeDataItem->leftBottom = ptNewLeftBottomCubeDataItem->center;
				ptNewCenterCubeDataItem->leftTop = ptNewLeftTopCubeDataItem->center;
				ptNewCenterCubeDataItem->rigthBottom = ptNewRigthBottomCubeDataItem->center;
				ptNewCenterCubeDataItem->center = ptDataItem->center;  
			}
		
			config.bSingleMeasure = 1;
			UUT_IQ_RecursiveConvergence( clb , (iLevelCount+1) , correctionType , config , ptNewCenterCubeDataItem , ptLowerPoint , pStatus );
			
		} while(0);
		
		if ( config.bQuickSearch && (*pStatus) ) 
		{
			RETURN;
		}
		
		if ( vbItemAlreadyExists[0] == 0 )
		{
			ptDataItem->vtCubeDataArray[0] = ptNewCenterCubeDataItem;
		}
		
		if ( vbItemAlreadyExists[1] == 0 )
		{
			ptDataItem->vtCubeDataArray[1] = ptNewLeftTopCubeDataItem;
			//------- duplicated --------------------//
			ptDataItem->vtCubeDataArray[5] = ptNewLeftTopCubeDataItem;
		}
		
		if ( vbItemAlreadyExists[2] == 0 )
		{
			ptDataItem->vtCubeDataArray[2] = ptNewRigthTopCubeDataItem;
		}
		
		if ( vbItemAlreadyExists[3] == 0 )
		{
			ptDataItem->vtCubeDataArray[3] = ptNewRigthBottomCubeDataItem;
		}
		
		if ( vbItemAlreadyExists[4] == 0 )
		{
			ptDataItem->vtCubeDataArray[4] = ptNewLeftBottomCubeDataItem;
		}

		lfAverage = ptDataItem->vtCubeDataArray[0]->lfAverageResult;
		lfLowerMeasure = ptDataItem->vtCubeDataArray[0]->lfLowerResult;   

		for ( index = 0 ; index < 5 ; index++ ) 
		{
			if ( ptDataItem->vtCubeDataArray[index]->lfAverageResult < lfAverage )
			{
				lfAverage = ptDataItem->vtCubeDataArray[index]->lfAverageResult;
				iAverageLowerIndex = index; 
			}
	
			if ( ptDataItem->vtCubeDataArray[index]->lfLowerResult < lfLowerMeasure )
			{
				lfLowerMeasure = ptDataItem->vtCubeDataArray[index]->lfLowerResult;
				iLowerIndex = index; 
			}
		}
		
		if ( config.bSearchLowerOnly == 0 )
		{
			if ( iLowerIndex != iAverageLowerIndex ) 
				if ( ptDataItem->vtCubeDataArray[iAverageLowerIndex]->lfLowerResult == lfLowerMeasure )
					iLowerIndex = iAverageLowerIndex;
		}
		
		config.bSingleMeasure = 0;
		
		if ( config.bUseDoubleConvergence )
		{   
			if ( iLowerIndex != iAverageLowerIndex ) 
				UUT_IQ_RecursiveConvergence( clb , (iLevelCount+1) , correctionType , config , ptDataItem->vtCubeDataArray[iAverageLowerIndex] , &tAverageLowerPoint , pStatus ); 
		}
		
		UUT_IQ_RecursiveConvergence( clb , (iLevelCount+1) , correctionType , config , ptDataItem->vtCubeDataArray[iLowerIndex] , ptLowerPoint , pStatus );
		
		if ( config.bUseDoubleConvergence )
		{
			if ( iLowerIndex != iAverageLowerIndex )
				if ( ptLowerPoint->lfMeasure > tAverageLowerPoint.lfMeasure )
					memcpy( ptLowerPoint , &tAverageLowerPoint , sizeof(tsSinglePointItem));
		}
	}
	
CATCH
FINALLY

	FREE(vtDataPointsArray);
	
	RETURN_STDERR;
}

void	recursiveFreeSingleCubeItem( tsSingleCubeItem *ptItem )
{
	int				index						=	0;

	if ( ptItem == NULL )
		return;
	
	if ( ptItem->vtCubeDataArray )
	{
		for ( index = 0; index < (ptItem->iNumberOfCubeDataItems-1) ; index++ )
		{
			if ( ptItem->vtCubeDataArray[index] )
			{
				recursiveFreeSingleCubeItem( ptItem->vtCubeDataArray[index] ); 
				
				FREE( ptItem->vtCubeDataArray[index] );
			}
		}
		
		FREE( ptItem->vtCubeDataArray );
	}
	
	memset( ptItem , 0 , sizeof(tsSingleCubeItem));
	
	return;
}


STD_ERROR	InterpolationConvergence( sClb* clb , teCorrectionType correctionType , tsConfigInterpolationItem config , tsSinglePointItem *ptDataItem , int *pbStatus )
{
	STD_ERROR						StdError										=	{0};
	
	int								iIndex											=	0, 
									iSweepIndex										=	0,
									iCheckRepetitionIndex							=	0;  
									
	double							lfNextValue										=	0.0,  
									lfStimulusValue									=	0.0,
									lfPreviousValue									=	0.0,
									lfMarkerAmplitude								=	0.0,
									lfMarkerFrequency								=	0.0;
	
	double							*pStimulusArray									=	NULL,
									*pMeasureArray									=	NULL;
	
	int								iVVA_Value										=	0,
									iATT_Value										=	0,
									iDCA_Value										=	0;
									
	double							vlfPreSetupValues[3]							=	{0};
	
	char							szFormatedString[LOW_STRING]					=	{0};
	
	int								iDierctRepetitionCounter						=	0;
	
	int								viIndirectRepetitionCounter[5]					=	{0};
	
	IF ((( ptDataItem == NULL )|| ( pbStatus == NULL )) , "Found NULL pointer arguments." );
		
	CALLOC_ERR( pStimulusArray , 1001 , sizeof(double));
	CALLOC_ERR( pMeasureArray , 1001 , sizeof(double));
	
	vlfPreSetupValues[0] = config.lfStartValue;
	vlfPreSetupValues[1] = (( config.lfStopValue - config.lfStartValue ) / 2.0 ) + config.lfStartValue;
	vlfPreSetupValues[2] = config.lfStopValue;
	
	for ( iIndex = 0; iIndex < 1001; iIndex++ )
	{
		if ( iIndex < 3 )
			lfNextValue = vlfPreSetupValues[iIndex];
		
		switch ( correctionType )
		{
			case CORRECT_TYPE_VVA_ONLY:
			
				iVVA_Value = lfNextValue;
			
				//------------------- Set VVA and DCA Correction  -------------------------------------//
				sprintf( szFormatedString , "UUT :: Set TX VVA = %d" , iVVA_Value ); 
				LOG( CLB_LINE_NORMAL , szFormatedString );
		
				CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "TX_VVA" , &iVVA_Value , 1 , sizeof(int))); 
			
				lfStimulusValue = iVVA_Value;
				
				break;  
			
			case CORRECT_TYPE_ATT_ONLY:
			
				iATT_Value = lfNextValue;
			
				//------------------- Set VVA and DCA Correction  -------------------------------------//
				sprintf( szFormatedString , "UUT :: Set TX ATT = %d" , iATT_Value , iVVA_Value ); 
				LOG( CLB_LINE_NORMAL , szFormatedString );
		
				CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "TX_ATT" , &iATT_Value , 1 , sizeof(int)));  
			
				lfStimulusValue = iATT_Value;
				
				break;
		
			case CORRECT_TYPE_DCA_ONLY:
			
				iDCA_Value = lfNextValue;
			
				//------------------- Set VVA and DCA Correction  -------------------------------------//
				sprintf( szFormatedString , "UUT :: Set TX DCA = %d" , iDCA_Value ); 
				LOG( CLB_LINE_NORMAL , szFormatedString );
		
				CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "TX_DCA" , &iDCA_Value , 1 , sizeof(int)));  
			
				lfStimulusValue = iDCA_Value;
				
				break;	
		}
		
		WAIT( "" , config.lfMeasureDelay ); 
		
		for ( iSweepIndex = 0; iSweepIndex < config.iSpectrumNumberOfSweeps; iSweepIndex++ )
		{
			sprintf( szFormatedString , "Sweep - %d" , iSweepIndex+1 );
			LOG(CLB_LINE_NORMAL, szFormatedString ); 

			DRV_SpectrumAnalyzer_GetSweep ( config.hSpectrumAnalyzer , config.lfSpectrumTimeout ); 
		}

		CHK_STDERR( DRV_SpectrumAnalyzer_WaitForIntrumentDone ( config.hSpectrumAnalyzer , config.lfSpectrumTimeout ));  
		
		CHK_STDERR( DRV_SpectrumAnalyzer_SearchMarkerPeak( config.hSpectrumAnalyzer , 1 ));

		CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerFrequency( config.hSpectrumAnalyzer , 1 , &lfMarkerFrequency )); 
		CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerAmplitude( config.hSpectrumAnalyzer , 1 , &lfMarkerAmplitude )); 

		lfMarkerAmplitude -= config.lfLossFactor;
		
		if ( config.bFrequencySecured )
			sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf dBm" , lfMarkerAmplitude );   
		else
			sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf GHz / %0.3lf dBm" , (lfMarkerFrequency / 1E9) , lfMarkerAmplitude );   

		LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
		if ( fabs( lfMarkerAmplitude - config.lfNeededValue ) < config.lfAccuracy )
		{
			*pbStatus = 1;
			break;
		}
		
		if ( ptDataItem )
		{
			if ( lfNextValue == config.lfStartValue )
				ptDataItem->lfMinimumValue = lfMarkerAmplitude;
		
			if ( lfNextValue == config.lfStopValue )
				ptDataItem->lfMaximumValue = lfMarkerAmplitude;
		}
		
		pMeasureArray[iIndex] = lfMarkerAmplitude; 	
		pStimulusArray[iIndex] = lfStimulusValue; 	
		
		lfPreviousValue = lfNextValue;
		
		if ( iIndex >= 2 )
		{
			GET_POINT_NON_LINEAR_INTERPOLATION( pMeasureArray , pStimulusArray , (iIndex+1) , config.lfNeededValue , lfNextValue )	
		
			if ( lfPreviousValue == lfNextValue )
				iDierctRepetitionCounter++;
		
			if ( iIndex > 5 )
			{
				for ( iCheckRepetitionIndex = 0; iCheckRepetitionIndex < 5; iCheckRepetitionIndex++ )
				{
					if ( pStimulusArray[iIndex-iCheckRepetitionIndex] == lfNextValue )	
						viIndirectRepetitionCounter[iCheckRepetitionIndex]++;
				}
			}
		}
		
		if ( lfNextValue >= config.lfStopValue )
		{
			lfNextValue = (( config.lfStopValue - lfPreviousValue ) / 2.0 ) + lfPreviousValue;
		}
		
		if ( lfNextValue <= config.lfStartValue )
		{
			lfNextValue = (( lfPreviousValue - config.lfStartValue ) / 2.0 ) + config.lfStartValue;
		}
		
		if ( iDierctRepetitionCounter > 10 )
		{
			RETURN; 
		}
		
		for ( iCheckRepetitionIndex = 0; iCheckRepetitionIndex < 5; iCheckRepetitionIndex++ )
			if ( viIndirectRepetitionCounter[iCheckRepetitionIndex] > 10 )
			{
				RETURN;
			}
	
		if (( lfNextValue > config.lfStopValue ) || ( lfNextValue < config.lfStartValue ))
			break;
	} 
	
CATCH
FINALLY

	if ( ptDataItem )
	{
		ptDataItem->iATT_Value = iATT_Value;
		ptDataItem->iDCA_Value = iDCA_Value;
		ptDataItem->iVVA_Value = iVVA_Value;
		
		ptDataItem->lfOriginalMeasure = lfMarkerAmplitude;
		ptDataItem->lfMeasure = lfMarkerAmplitude;
	}
	
	FREE(pMeasureArray);
	FREE(pStimulusArray);
	
	RETURN_STDERR;
}


STD_ERROR	IQ_Convergence( sClb* clb , teCorrectionType correctionType , tsConfigIQItem config , tsSinglePointItem *ptDataItem , int *pbStatus )
{
	STD_ERROR						StdError										=	{0};
	
	int								iIndex											=	0, 
									iTryIndex										=	0,  
									iSweepIndex										=	0,
									iCheckRepetitionIndex							=	0;  
									
	double							lfNextValue										=	0.0,  
									lfOffsetValue									=	0.0,  
									lfStimulusValue									=	0.0,
									lfPreviousValue									=	0.0,
									lfMarkerAmplitude								=	0.0,
									lfMarkerFrequency								=	0.0;
	
	double							*pStimulusArray									=	NULL,
									*pMeasureArray									=	NULL;
	
	int								iVVA_Value										=	0,
									iATT_Value										=	0,
									iDCA_Value										=	0;
									
	double							vlfPreSetupValues[2]							=	{0};
	
	char							szFormatedString[LOW_STRING]					=	{0};
	
	int								iDierctRepetitionCounter						=	0;
	
	int								viIndirectRepetitionCounter[5]					=	{0};
	
	teSwitchModeType				tSwitchTriggerMode								=	0;

	int								bHasTrigger										=	0;
	
	int								viI_Value[2]									=	{0},
									viQ_Value[2]									=	{0};
	
	int								iPHASE_Value									=	0;
	
	int								iI_Value										=	0,
									iQ_Value										=	0,
									iPH_Value										=	0;
	
	int								iI_Index										=	0,
									iQ_Index										=	0,
									iPH_Index										=	0;
	
	int								iI_Maximum										=	0,
									iI_Minimum										=	0,
									iQ_Maximum										=	0,
									iQ_Minimum										=	0,
									iPH_Maximum										=	0,
									iPH_Minimum										=	0;
	
	int								iI_Setted_Size									=	0,
									iQ_Setted_Size									=	0,
									iPH_Setted_Size									=	0;
	
	int								*pbI_Setted										=	NULL,
									*pbQ_Setted										=	NULL,
									*pbPH_Setted									=	NULL;
	
	double							lfI_MinimumMeasure								=	0,
									lfQ_MinimumMeasure								=	0,
									lfPH_MinimumMeasure								=	0;
	
	int								iI_AtMinimumMeasure								=	0,
									iQ_AtMinimumMeasure								=	0,
									iPH_AtMinimumMeasure							=	0;
	
	int								iSwitchCounter									=	0;
	
	IF ((( ptDataItem == NULL )|| ( pbStatus == NULL )) , "Found NULL pointer arguments." );
		
	CALLOC_ERR( pStimulusArray , 1001 , sizeof(double));
	CALLOC_ERR( pMeasureArray , 1001 , sizeof(double));
	
	iI_Setted_Size = fabs(config.lfStop_I_Value-config.lfStart_I_Value) + 128;
	iQ_Setted_Size = fabs(config.lfStop_Q_Value-config.lfStart_Q_Value) + 128;    
	iPH_Setted_Size = fabs(config.lfStop_PH_Value-config.lfStart_PH_Value) + 128;    
	
	CALLOC_ERR( pbI_Setted , iI_Setted_Size , sizeof(int));
	CALLOC_ERR( pbQ_Setted , iQ_Setted_Size , sizeof(int));
	CALLOC_ERR( pbPH_Setted , iPH_Setted_Size , sizeof(int));
	
	do
	{   
		iI_Maximum = config.lfStop_I_Value;
		iI_Minimum = config.lfStart_I_Value;
		iQ_Maximum = config.lfStop_Q_Value;
		iQ_Minimum = config.lfStart_Q_Value;
		iPH_Maximum = config.lfStop_PH_Value;
		iPH_Minimum = config.lfStart_PH_Value;
	
		iDierctRepetitionCounter = 0;
		
		iSwitchCounter++;
		
		bHasTrigger = 0;
		
		memset( viIndirectRepetitionCounter , 0 , sizeof(viIndirectRepetitionCounter));
		
		switch ( tSwitchTriggerMode )
		{
			case SWITCH_MODE_TYPE_I:	
				
				vlfPreSetupValues[0] = iI_Minimum;
				vlfPreSetupValues[1] = iI_Maximum;
				break;
			
			case SWITCH_MODE_TYPE_Q:
				
				vlfPreSetupValues[0] = iQ_Minimum;
				vlfPreSetupValues[1] = iQ_Maximum;
				break;
				
			case SWITCH_MODE_TYPE_PHASE:
				
				vlfPreSetupValues[0] = iPH_Minimum;
				vlfPreSetupValues[1] = iPH_Maximum;
				break;
		}
		
		iI_Value = iI_AtMinimumMeasure;
		iQ_Value = iQ_AtMinimumMeasure;
		iPH_Value = iPH_AtMinimumMeasure; 
		
		memset( pbI_Setted , 0 , iI_Setted_Size * sizeof(int));
		memset( pbQ_Setted , 0 , iQ_Setted_Size * sizeof(int));
		memset( pbPH_Setted , 0 , iPH_Setted_Size * sizeof(int));
		
		for ( iIndex = 0; iIndex < 1001; iIndex++ )
		{
			switch ( correctionType )
			{
				case CORRECT_TYPE_IQ:  // ( -1023 <> +1023 )				
			
						if (( tSwitchTriggerMode == SWITCH_MODE_TYPE_I ) || ( iIndex == 0 ))
						{
							iI_Index = iI_Value - config.lfStart_I_Value;
							
							if ( iI_Index >= 0 )
								pbI_Setted[iI_Index] = 1;
							
							lfStimulusValue = iI_Value;
							//------------------- Set I Correction  -------------------------------------//
							sprintf( szFormatedString , "UUT :: Set TX I offset = %d" , iI_Value ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
				
							if ( iI_Value < 0 )
								viI_Value[0] = 0x80 | (( -1 * iI_Value ) & 3 );
		    				else
								viI_Value[0] = (iI_Value & 3 );
	
							viI_Value[1] = (int)( abs( iI_Value ) / 4.0 ) & 0xff;
					
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "I_OFFSET" , viI_Value , 2 , sizeof(int))); 
						}
						
						if (( tSwitchTriggerMode == SWITCH_MODE_TYPE_Q ) || ( iIndex == 0 ))      
						{
							iQ_Index = iQ_Value - config.lfStart_Q_Value;
							
							if ( iQ_Index >= 0 )
								pbQ_Setted[iQ_Index] = 1;
							
							lfStimulusValue = iQ_Value;
							//------------------- Set Q Correction  -------------------------------------//
							sprintf( szFormatedString , "UUT :: Set TX Q offset = %d" , iQ_Value ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
							
							if ( iQ_Value < 0 )
								viQ_Value[0] = 0x80 | (( -1 * iQ_Value ) & 3 );
		    				else
								viQ_Value[0] = (iQ_Value & 3 );
	
							viQ_Value[1] = (int)( abs(iQ_Value) / 4.0 ) & 0xff;
					
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "Q_OFFSET" , viQ_Value , 2 , sizeof(int))); 
						}
						
					break;  
			
				case CORRECT_TYPE_PHASE_IQ:  // ( -1023 <> +1023 )				
			
						if (( tSwitchTriggerMode == SWITCH_MODE_TYPE_I ) || ( iIndex == 0 ))
						{
							iI_Index = iI_Value - config.lfStart_I_Value;
							
							if ( iI_Index >= 0 )
								pbI_Setted[iI_Index] = 1;
							
							lfStimulusValue = iI_Value;
							//------------------- Set I Correction  -------------------------------------//
							sprintf( szFormatedString , "UUT :: Set TX I gain = %d" , iI_Value ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
							
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "I_GAIN" , &iI_Value , 1 , sizeof(int)));
						}
						
						if (( tSwitchTriggerMode == SWITCH_MODE_TYPE_Q ) || ( iIndex == 0 ))      
						{
							iQ_Index = iQ_Value - config.lfStart_Q_Value;
							
							if ( iQ_Index >= 0 )
								pbQ_Setted[iQ_Index] = 1;
							
							lfStimulusValue = iQ_Value;
							//------------------- Set Q Correction  -------------------------------------//
							sprintf( szFormatedString , "UUT :: Set TX Q gain = %d" , iQ_Value ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
							
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "Q_GAIN" , &iQ_Value , 1 , sizeof(int)));  
						}
						
						if (( tSwitchTriggerMode == SWITCH_MODE_TYPE_PHASE ) || ( iIndex == 0 ))      
						{
							iPH_Index = iPH_Value - config.lfStart_PH_Value;
							
							if ( iPH_Index >= 0 )
								pbPH_Setted[iPH_Index] = 1;
							
							lfStimulusValue = iPH_Value;
							//------------------- Set Q Correction  -------------------------------------//
							sprintf( szFormatedString , "UUT :: Set TX Phase = %d" , iPH_Value ); 
							LOG( CLB_LINE_NORMAL , szFormatedString );
							
							if ( iPH_Value < 0 )
				 				iPHASE_Value = ((~(-1 * iPH_Value )) & 0xff ) + 1;
							else
								iPHASE_Value = iPH_Value;
					
							iPHASE_Value &= 0x0FF;
							
							CHK_STDERR( DRV_StandardDevice_SetValue( config.hUnitUnderTest , "PHASE_BALANCE" , &iPHASE_Value , 1 , sizeof(int)));  
						}	  
			}
		
			WAIT( "" , config.lfMeasureDelay ); 
		
			for ( iSweepIndex = 0; iSweepIndex < config.iSpectrumNumberOfSweeps; iSweepIndex++ )
			{
				sprintf( szFormatedString , "Sweep - %d" , iSweepIndex+1 );
				LOG(CLB_LINE_NORMAL, szFormatedString ); 

				DRV_SpectrumAnalyzer_GetSweep ( config.hSpectrumAnalyzer , config.lfSpectrumTimeout ); 
			}

			CHK_STDERR( DRV_SpectrumAnalyzer_WaitForIntrumentDone ( config.hSpectrumAnalyzer , config.lfSpectrumTimeout ));  
		
			CHK_STDERR( DRV_SpectrumAnalyzer_SearchMarkerPeak( config.hSpectrumAnalyzer , 1 ));

			CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerFrequency( config.hSpectrumAnalyzer , 1 , &lfMarkerFrequency )); 
			CHK_STDERR( DRV_SpectrumAnalyzer_GetMarkerAmplitude( config.hSpectrumAnalyzer , 1 , &lfMarkerAmplitude )); 

			lfMarkerAmplitude -= config.lfLossFactor;
		
			switch ( tSwitchTriggerMode )
			{
				case SWITCH_MODE_TYPE_I:
				
					lfI_MinimumMeasure = lfMarkerAmplitude;
					iI_AtMinimumMeasure = iI_Value;
					break;
		
				case SWITCH_MODE_TYPE_Q: 	
					
					lfQ_MinimumMeasure = lfMarkerAmplitude;
					iQ_AtMinimumMeasure = iQ_Value;
					break;
					
				case SWITCH_MODE_TYPE_PHASE: 	
					
					lfPH_MinimumMeasure = lfMarkerAmplitude;
					iPH_AtMinimumMeasure = iPH_Value;
					break;	
			}
			
			if ( config.bFrequencySecured )
				sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf dBm" , lfMarkerAmplitude );   
			else
				sprintf( szFormatedString , "Get spectrum analyzer Marker %0.3lf GHz / %0.3lf dBm" , (lfMarkerFrequency / 1E9) , lfMarkerAmplitude );   

			LOG(CLB_LINE_NORMAL, szFormatedString ); 
		
			if ( fabs( lfMarkerAmplitude - config.lfNeededValue ) < config.lfAccuracy )
			{
				*pbStatus = 1;
				break;
			}

			pMeasureArray[iIndex] = lfMarkerAmplitude; 	
			pStimulusArray[iIndex] = lfStimulusValue; 	
		
			lfPreviousValue = lfNextValue;
		
			if ( iIndex < 2 )
			{
				lfNextValue = vlfPreSetupValues[iIndex];
			}
			else
			{
				switch ( tSwitchTriggerMode )
				{
					case SWITCH_MODE_TYPE_I:
					
								if ( iSwitchCounter >= 2 )
									lfOffsetValue = iI_AtMinimumMeasure;
									
								if ( ( iI_Maximum - lfPreviousValue ) >= ( lfPreviousValue - iI_Minimum ) )
								{
									lfNextValue = (( iI_Maximum - lfOffsetValue ) / 2.0 ) + lfOffsetValue; 	
								}
								else
								{
									lfNextValue = (( lfOffsetValue - iI_Minimum ) / 2.0 ) + iI_Minimum;  
								}
	
								break;
				
					case SWITCH_MODE_TYPE_Q:
					
								if ( iSwitchCounter >= 2 )
									lfOffsetValue = iQ_AtMinimumMeasure;
						
								if ( ( iQ_Maximum - lfPreviousValue ) >= ( lfPreviousValue - iQ_Minimum ) )
								{
									lfNextValue = (( iQ_Maximum - lfOffsetValue ) / 2.0 ) + lfOffsetValue; 	
								}
								else
								{
									lfNextValue = (( lfOffsetValue - iQ_Minimum ) / 2.0 ) + iQ_Minimum;  
								}
							
								break;
							
					case SWITCH_MODE_TYPE_PHASE:
					
								if ( iSwitchCounter >= 2 )
									lfOffsetValue = iPH_AtMinimumMeasure;
						
								if ( ( iPH_Maximum - lfPreviousValue ) >= ( lfPreviousValue - iPH_Minimum ) )
								{
									lfNextValue = (( iPH_Maximum - lfOffsetValue ) / 2.0 ) + lfOffsetValue; 	
								}
								else
								{
									lfNextValue = (( lfOffsetValue - iPH_Minimum ) / 2.0 ) + iPH_Minimum;  
								}
							
								break;			
			
				}
			}
			
			switch ( tSwitchTriggerMode )
			{
				case SWITCH_MODE_TYPE_I:
					
							for( iTryIndex = 0; iTryIndex < ( config.lfStop_I_Value - config.lfStart_I_Value ); iTryIndex++ )
							{
								iI_Index = lfNextValue - config.lfStart_I_Value;
				
								if (( iI_Index >= 0 ) && ( pbI_Setted[iI_Index] ))
								{
									if ( ( iI_Maximum - lfPreviousValue ) >= ( lfPreviousValue - iI_Minimum ) )
									{
										lfNextValue = (( iI_Maximum - iI_AtMinimumMeasure ) / 2.0 ) + iI_AtMinimumMeasure; 	
				
										iI_Maximum = lfNextValue;
									}
									else
									{
										lfNextValue = (( iI_AtMinimumMeasure - iI_Minimum ) / 2.0 ) + iI_Minimum;  
				
										iI_Minimum = lfNextValue;
									}
								}
								else
								{
									break;
								}
							}
							
							break;
				
				case SWITCH_MODE_TYPE_Q:
					
							for( iTryIndex = 0; iTryIndex < ( config.lfStop_Q_Value - config.lfStart_Q_Value ); iTryIndex++ )
							{
								iQ_Index = lfNextValue - config.lfStart_Q_Value;
				
								if (( iQ_Index >= 0 ) && ( pbQ_Setted[iQ_Index] ))
								{
									if ( ( iQ_Maximum - lfPreviousValue ) >= ( lfPreviousValue - iQ_Minimum ) )
									{
										lfNextValue = (( iQ_Maximum - iQ_AtMinimumMeasure ) / 2.0 ) + iQ_AtMinimumMeasure; 	
				
										iQ_Maximum = lfNextValue;
									}
									else
									{
										lfNextValue = (( iQ_AtMinimumMeasure - iQ_Minimum ) / 2.0 ) + iQ_Minimum;  
				
										iQ_Minimum = lfNextValue;
									}
								}
								else
								{
									break;
								}
							}
							
							break;
							
				case SWITCH_MODE_TYPE_PHASE:
					
							for( iTryIndex = 0; iTryIndex < ( config.lfStop_PH_Value - config.lfStart_PH_Value ); iTryIndex++ )
							{
								iPH_Index = lfNextValue - config.lfStart_PH_Value;
				
								if (( iPH_Index >= 0 ) && ( pbPH_Setted[iPH_Index] ))
								{
									if ( ( iPH_Maximum - lfPreviousValue ) >= ( lfPreviousValue - iPH_Minimum ) )
									{
										lfNextValue = (( iPH_Maximum - iPH_AtMinimumMeasure ) / 2.0 ) + iPH_AtMinimumMeasure; 	
				
										iPH_Maximum = lfNextValue;
									}
									else
									{
										lfNextValue = (( iPH_AtMinimumMeasure - iPH_Minimum ) / 2.0 ) + iPH_Minimum;  
				
										iPH_Minimum = lfNextValue;
									}
								}
								else
								{
									break;
								}
							}
							
							break;			
			
			}
				
			if ( lfPreviousValue == lfNextValue )
				iDierctRepetitionCounter++;
			
			if ( iIndex > 5 )
			{
				for ( iCheckRepetitionIndex = 0; iCheckRepetitionIndex < 5; iCheckRepetitionIndex++ )
				{
					if ( pStimulusArray[iIndex-iCheckRepetitionIndex] == lfNextValue )	
						viIndirectRepetitionCounter[iCheckRepetitionIndex]++;
				}
			}
			
			switch ( tSwitchTriggerMode )
			{
				case SWITCH_MODE_TYPE_I:
					
							if ( lfNextValue > config.lfStop_I_Value )
							{
								lfNextValue = (( config.lfStop_I_Value - iI_AtMinimumMeasure ) / 2.0 ) + iI_AtMinimumMeasure;
							}
		
							if ( lfNextValue < config.lfStart_I_Value )
							{
								lfNextValue = (( iI_AtMinimumMeasure - config.lfStart_I_Value ) / 2.0 ) + config.lfStart_I_Value;
							}
							
							break;
							
				case SWITCH_MODE_TYPE_Q:
					
							if ( lfNextValue > config.lfStop_Q_Value )
							{
								lfNextValue = (( config.lfStop_Q_Value - iI_AtMinimumMeasure ) / 2.0 ) + iI_AtMinimumMeasure;
							}
		
							if ( lfNextValue < config.lfStart_Q_Value )
							{
								lfNextValue = (( iI_AtMinimumMeasure - config.lfStart_Q_Value ) / 2.0 ) + config.lfStart_Q_Value;
							}
							
							break;
							
				case SWITCH_MODE_TYPE_PHASE:
					
							if ( lfNextValue > config.lfStop_PH_Value )
							{
								lfNextValue = (( config.lfStop_PH_Value - iI_AtMinimumMeasure ) / 2.0 ) + iI_AtMinimumMeasure;
							}
		
							if ( lfNextValue < config.lfStart_PH_Value )
							{
								lfNextValue = (( iI_AtMinimumMeasure - config.lfStart_PH_Value ) / 2.0 ) + config.lfStart_PH_Value;
							}
							
							break;
							
			}
		
			if ( iDierctRepetitionCounter >= 2 )
			{
				tSwitchTriggerMode++; 
				
				bHasTrigger = 1;  
			}
		
			for ( iCheckRepetitionIndex = 0; iCheckRepetitionIndex < 5; iCheckRepetitionIndex++ )
				if ( viIndirectRepetitionCounter[iCheckRepetitionIndex] > 2 )
				{
					tSwitchTriggerMode++; 
					
					bHasTrigger = 1;
				}
	
			if ( bHasTrigger )
			{
				switch ( correctionType )
				{
					case CORRECT_TYPE_IQ:
					
							if ( tSwitchTriggerMode >= SWITCH_MODE_TYPE_PHASE )
								tSwitchTriggerMode = 0;
					break;
				
					case CORRECT_TYPE_PHASE_IQ:
					
							if ( tSwitchTriggerMode >= SWITCH_MODE_TYPE_SENTINEL )
								tSwitchTriggerMode = 0;
					break;
				}
			}
			
			switch ( tSwitchTriggerMode )
			{
				case SWITCH_MODE_TYPE_I:
					
						if (( lfNextValue > config.lfStop_I_Value ) || ( lfNextValue < config.lfStart_I_Value ))
							bHasTrigger = 1;
						
						break;
						
				case SWITCH_MODE_TYPE_Q:
					
						if (( lfNextValue > config.lfStop_Q_Value ) || ( lfNextValue < config.lfStart_Q_Value ))
							bHasTrigger = 1;
						
						break;		
			
				case SWITCH_MODE_TYPE_PHASE:
					
						if (( lfNextValue > config.lfStop_PH_Value ) || ( lfNextValue < config.lfStart_PH_Value ))
							bHasTrigger = 1;
						
						break;
			}
			
			if ( bHasTrigger )
				break;
			
			switch ( tSwitchTriggerMode )
			{
				case SWITCH_MODE_TYPE_I:
					
					iI_Value = lfNextValue;
					break;
					
				case SWITCH_MODE_TYPE_Q:
					
					iQ_Value = lfNextValue;
					break;
					
				case SWITCH_MODE_TYPE_PHASE:
					
					iPH_Value = lfNextValue;
					break;
			}
		} 
		
	}while( *pbStatus == 0 );
	
CATCH
FINALLY

	if ( ptDataItem )
	{
		ptDataItem->iATT_Value = iATT_Value;
		ptDataItem->iDCA_Value = iDCA_Value;
		ptDataItem->iVVA_Value = iVVA_Value;
		
		ptDataItem->lfOriginalMeasure = lfMarkerAmplitude;
		ptDataItem->lfMeasure = lfMarkerAmplitude;
	}
	
	FREE(pMeasureArray);
	FREE(pStimulusArray);
	
	FREE(pbI_Setted);
	FREE(pbQ_Setted);
	FREE(pbPH_Setted);
	
	RETURN_STDERR;
}
