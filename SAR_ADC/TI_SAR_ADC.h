#ifndef __TI_SAR_ADC_H__
#define __TI_SAR_ADC_H__
#include "SAR_ADC.h"
#include <random>
#include <vector>
#include <string>

using namespace std;
//===========================TI SAR ADC===========================//
// TI SAR ADC is based class--> SAR_ADC
//Just use M SAR_ADC to compose TI SAR ADC
//Because class array cannot use variable, so I use const int to 
//show the number of channel

const int channel_num = 4;

class TI_SARADC
{
public:
	SARADC SingleChannel[channel_num];		//32 single channels
	SARADC ReferenceChannel;				//Reference channel for calibration			
	


	int now_channel;						//Which channel this convert use
	int ref_correspondence_channel;			//The channel referened by ref
	int n;									//Bit of singlechannel
	int n_radix;
	double radix;							//Radix num of nonbinary ADC

	double REF_AD;							//Reference AD value
	string REF_AD_String;
	double NOW_AD_Calibration;			//Now AD_Value
	string NOW_AD_Cali_String;
	//double NOW_AD_Calibration;				


	
	//Member of offset calibration:
	double Offset_CorretedValue[channel_num];
	double Offset_Residual[channel_num];
	double Offset_Calibration_Step;
	
	//Member of cap calibration
	vector<vector<double>> TI_Cap_Weight;
	vector<double> TI_Cap_Offset;
	vector<double> REF_Cap_Weight;
	double Cap_Calibration_Step;
	vector<double> Cap_Cali_Residual;


public:
	TI_SARADC();
	void Init_Single_Channel(int n_in,			
						double Vref_high_in,
						double Vref_low_in,
						double Vcm_in, 
						double Cunit_in,
						double radix);

	void Init_Ref_Channel();

	void Init_TI_SARADC(int n_in, int n_radix_in, double radix_in);

	double TI_Transcoding(string AD_value);

	double REF_Transcoding(string AD_value);

	double AD_Convert(double Vin, double Vip);

    void Ref_AD_Get(double Vin, double Vip, int R_P_Choose);		//R_P_Choose is for Reverse of Positive choose

	void Init_MisMatch();

	void MisMatch_Calibration();

	void Init_Calibration();
	
	void OffsetMisMatch_Calibration(double TI_Channel_AD, double REF_Channel_AD);

	void CapMisMatch_Calibration(string TI_Channel_AD_string, string REF_Channel_AD_string);

};




#endif
