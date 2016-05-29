#ifndef __TESTS_STD_ATE_SupportLibrary_H__
#define __TESTS_STD_ATE_SupportLibrary_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"
#include "GlobalDefines.h"

//==============================================================================
// Constants

//==============================================================================
// Types
typedef struct
{
	int			I,Q,PH;
	
	int			bFilled;
	
	double		lfMeasure,
				lfOriginalMeasure;
	
	int			bMeasured;
	
	int			iDCA_Value,
				iVVA_Value,
				iATT_Value,
				iPowerDetector;
	
	double		lfMaximumValue,
				lfMinimumValue;
				
} tsSinglePointItem;

typedef struct SINGLE_CUBE_ITEM
{
	tsSinglePointItem		leftTop,
							rigthTop,
							center,
							leftBottom,
							rigthBottom;
	
	double					lfLowerResult,
							lfAverageResult;
	
	struct SINGLE_CUBE_ITEM	**vtCubeDataArray; 
	int						iNumberOfCubeDataItems;

} tsSingleCubeItem;

typedef enum { CORRECT_TYPE_ATT_ONLY , CORRECT_TYPE_DCA_ONLY , CORRECT_TYPE_VVA_ONLY , CORRECT_TYPE_ATT_VVA , CORRECT_TYPE_DCA_VVA , CORRECT_TYPE_IQ , CORRECT_TYPE_PHASE_IQ , CORRECT_TYPE_PHASE_IPH , CORRECT_TYPE_PHASE_QPH , CORRECT_TYPE_SENTINEL } teCorrectionType;


typedef struct
{
	double					lfMeasureDelay,
							lfSpectrumTimeout;
	
	int						iSpectrumNumberOfSweeps;
	
	int						bDeltaSearch,
							bSingleMeasure,
							bQuickSearch,
							bSearchLowerOnly,
							bFrequencySecured,
							bUseDoubleConvergence;
	
	double					lfNeededValue,
							lfQuickAccuracy,
							lfLossFactor;
	
	int						hUnitUnderTest,
							hSpectrumAnalyzer;
							
	int						iConvergenceLimit; 

} tsConfigurationItem;

typedef struct
{
	double					lfMeasureDelay,
							lfSpectrumTimeout;
	
	double					lfStartValue,
							lfStopValue;
	
	double					lfNeededValue,
							lfLossFactor;
	
	int						hUnitUnderTest,
							hSpectrumAnalyzer;
							
	double					lfAccuracy; 
	
	int						iSpectrumNumberOfSweeps;
	
	int						bFrequencySecured;

} tsConfigInterpolationItem;

typedef struct
{
	double					lfMeasureDelay,
							lfSpectrumTimeout;
	
	double					lfStart_I_Value,
							lfStop_I_Value;
	
	double					lfStart_Q_Value,
							lfStop_Q_Value;
	
	double					lfStart_PH_Value,
							lfStop_PH_Value;
	
	double					lfNeededValue,
							lfLossFactor;
	
	int						hUnitUnderTest,
							hSpectrumAnalyzer;
							
	double					lfAccuracy; 
	
	int						iSpectrumNumberOfSweeps;
	
	int						bFrequencySecured;

} tsConfigIQItem;

//==============================================================================
// External variables

//==============================================================================
// Global functions

void				recursiveFreeSingleCubeItem( tsSingleCubeItem *ptItem );

int					FillCubeItem( tsSingleCubeItem *ptDataItem , int iMinimumAdd );

STD_ERROR			UUT_IQ_RecursiveConvergence( sClb* clb , int iLevelCount , teCorrectionType correctionType , tsConfigurationItem config , tsSingleCubeItem *ptDataItem , tsSinglePointItem *ptLowerPoint , int *pStatus );

STD_ERROR			InterpolationConvergence( sClb* clb , teCorrectionType correctionType , tsConfigInterpolationItem config , tsSinglePointItem *ptDataItem , int *pbStatus );

STD_ERROR			IQ_Convergence( sClb* clb , teCorrectionType correctionType , tsConfigIQItem config , tsSinglePointItem *ptDataItem , int *pbStatus );

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __TESTS_STD_ATE_SupportLibrary_H__ */
