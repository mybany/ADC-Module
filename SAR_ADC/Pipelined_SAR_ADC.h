#ifndef __PIPELINE_SAR_ADC_H__
#define __PIPELINE_SAR_ADC_H__
#include "SAR_ADC.h"
#include <random>
#include <vector>
#include <string>

using namespace std;
//===========================Pipelined SAR ADC===========================//
//For convenient, this module use 2-radix between CorseADC and Fine ADC
//If do not want to this, you have to change code!


class Pipelined_SAR_ADC
{
public:
	SARADC CoarseADC;
	SARADC FineADC;

public:
	void InitPipelined_SAR_ADC( int first_n_in,
								int second_n_in,
								double Vref_high_in,
								double Vref_low_in,
								double first_Cunit_in,
								double second_Cunit_in);

	


};


#endif

