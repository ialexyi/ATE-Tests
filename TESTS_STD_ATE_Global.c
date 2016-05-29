//==============================================================================
// Include files
#include <windows.h> 
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


void AddCalculateMasureItem( tsMeasureItem *ptMeasureItem )
{
	ptMeasureItem->iMeasureCount++;
	ptMeasureItem->lfAverageCalc += ptMeasureItem->lfLastValue;
	
	ptMeasureItem->lfAverage = ptMeasureItem->lfAverageCalc / (double)ptMeasureItem->iMeasureCount; 
	
	if (( ptMeasureItem->lfLastValue < ptMeasureItem->lfMinValue ) || ( ptMeasureItem->iMeasureCount == 1 ))
		ptMeasureItem->lfMinValue = ptMeasureItem->lfLastValue; 
	
	if (( ptMeasureItem->lfLastValue > ptMeasureItem->lfMaxValue ) || ( ptMeasureItem->iMeasureCount == 1 ))   
		ptMeasureItem->lfMaxValue = ptMeasureItem->lfLastValue; 
	
	return;
}
