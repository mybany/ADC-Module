#include "TI_SAR_ADC.h"
#include <iostream>
//===========================TI SAR ADC===========================//
TI_SARADC::TI_SARADC()
{
	return;
}

void TI_SARADC::Init_Single_Channel(int n_in,
								double Vref_high_in,
								double Vref_low_in,
								double Vcm_in, 
								double Cunit_in,
								double radix_in)
{
	for (int i = 0; i < channel_num; i++)
	{
		SingleChannel[i].InitSARADC(n_in, Vref_high_in, Vref_low_in, Vcm_in, Cunit_in, radix_in);
		SingleChannel[i].InitCapArray();
	}
	n = n_in;
	radix = radix_in;
}

void TI_SARADC::Init_TI_SARADC(int n_in, int n_radix_in, double radix_in)
{
	now_channel = 0;
	n = n_in;
	n_radix = n_radix_in;
	radix = radix_in;
}


void TI_SARADC::Init_Ref_Channel()
{
	ReferenceChannel = SingleChannel[0];
	ref_correspondence_channel = channel_num;
}




void TI_SARADC::Init_MisMatch()
{
	
	default_random_engine randomengine;
	randomengine.seed(channel_num);

	//Offset MisMatch
	uniform_real_distribution<double> Offset_Mis_randomregion(-0.01, 0.01);
	for (int i = 0; i < channel_num; i++)
	{
		//SingleChannel[i].InitOffsetMisMatch(Offset_Mis_randomregion(randomengine));
	}
	//ReferenceChannel.InitOffsetMisMatch(Offset_Mis_randomregion(randomengine));



	//Cap MisMatch
	randomengine.seed(channel_num + 1);
	uniform_real_distribution<double> Cap_Mis_random(0.95, 1.05);
	vector<double> NCap_Mis_Ratio;
	vector<double> PCap_Mis_Ratio;
	NCap_Mis_Ratio.resize(n_radix);
	PCap_Mis_Ratio.resize(n_radix);
	for (int i = 0; i < channel_num; i++)
	{	
		for (int k = 0; k < n_radix; k++)
		{
			NCap_Mis_Ratio[k] = Cap_Mis_random(randomengine);
			PCap_Mis_Ratio[k] = Cap_Mis_random(randomengine);
		}
		SingleChannel[i].InitCapMismatch(NCap_Mis_Ratio, PCap_Mis_Ratio);
	}

	for (int k = 0; k < n_radix; k++)
	{
		NCap_Mis_Ratio[k] = (Cap_Mis_random(randomengine));
		PCap_Mis_Ratio[k] = (Cap_Mis_random(randomengine));
	}
	ReferenceChannel.InitCapMismatch(NCap_Mis_Ratio, PCap_Mis_Ratio);
}


void TI_SARADC::OffsetMisMatch_Calibration(double TI_Channel_AD, double REF_Channel_AD)
{

	Offset_Residual[ref_correspondence_channel] = (TI_Channel_AD - REF_Channel_AD);

	Offset_CorretedValue[ref_correspondence_channel] = Offset_CorretedValue[ref_correspondence_channel] - 2 * Offset_Calibration_Step * Offset_Residual[ref_correspondence_channel];

}


void TI_SARADC::CapMisMatch_Calibration(string TI_Channel_AD_string, string REF_Channel_AD_string)
{
	double Residual_Value = 0;

	Residual_Value = REF_AD - NOW_AD_Calibration;


	Cap_Cali_Residual[ref_correspondence_channel] = Residual_Value;

	for (int i = 0; i < n_radix; i++)
	{
		TI_Cap_Weight[ref_correspondence_channel][i] = TI_Cap_Weight[ref_correspondence_channel][i] + 2 * Cap_Calibration_Step * Residual_Value * (int(TI_Channel_AD_string[i] - 48));
		REF_Cap_Weight[i] = REF_Cap_Weight[i] - 2 * Cap_Calibration_Step * Residual_Value * (int(REF_Channel_AD_string[i] - 48)); 
	}

	TI_Cap_Offset[ref_correspondence_channel] = TI_Cap_Offset[ref_correspondence_channel] + 2 * Cap_Calibration_Step * Residual_Value;
}


void TI_SARADC::Init_Calibration()
{
	for (int i = 0; i < channel_num; i++)
	{
		Offset_CorretedValue[i] = 0;
		Offset_Residual[i] = 0;
	}

	Offset_Calibration_Step = 0.2;

	TI_Cap_Weight.resize(channel_num);
	TI_Cap_Offset.resize(channel_num);
	for (int i = 0; i < channel_num; i++)
	{
		TI_Cap_Weight[i].resize(n_radix);
		for (int k = 0; k < n_radix; k++)
		{
			TI_Cap_Weight[i][k] = pow(radix, n_radix - k - 1);
		}
		TI_Cap_Offset[i] = 0;
	}

	REF_Cap_Weight.resize(n_radix);
	for (int k = 0; k < n_radix; k++)
	{
		REF_Cap_Weight[k] = pow(radix, n_radix - k - 1);
	}


	Cap_Calibration_Step = 0.002;

	Cap_Cali_Residual.resize(channel_num);
}

void TI_SARADC::MisMatch_Calibration()
{
	OffsetMisMatch_Calibration(NOW_AD_Calibration, REF_AD);
}



//This function is for 2bit->10bit transforming
double TI_SARADC::TI_Transcoding(string AD_value)
{
	double value_dec = 0;
	for (int i = 0; i < n_radix; i++)
	{
		value_dec = value_dec + (AD_value[i] - 48) * TI_Cap_Weight[now_channel][i];
	}
	return value_dec;
}


double TI_SARADC::REF_Transcoding(string AD_value)
{
	double value_dec = 0;
	for (int i = 0; i < n_radix; i++)
	{
		value_dec = value_dec + (AD_value[i] - 48) * REF_Cap_Weight[i];
	}

	return value_dec;
}






double TI_SARADC::AD_Convert(double Vin, double Vip)
{
	//double AD_Result_Out = 0;
	//string AD_Value = "";
	
	NOW_AD_Cali_String = SingleChannel[now_channel].AD_Convert(Vin, Vip);;

	NOW_AD_Calibration = TI_Transcoding(NOW_AD_Cali_String);
	
	NOW_AD_Calibration = NOW_AD_Calibration + TI_Cap_Offset[now_channel];
	//NOW_AD_Calibration = NOW_AD_Calibration + Offset_CorretedValue[now_channel];

	now_channel = now_channel + 1;
	if (now_channel > (channel_num - 1)) 
	{
		now_channel = 0;
	}

	return NOW_AD_Calibration;
}


void TI_SARADC::Ref_AD_Get(double Vin, double Vip, int R_P_Choose)
{
	ref_correspondence_channel = ref_correspondence_channel + 1;
	if (ref_correspondence_channel >= channel_num)
	{
		ref_correspondence_channel = 0;
	}
	string AD_Value = "";

	if (R_P_Choose % 2 == 1)
		AD_Value = ReferenceChannel.AD_Convert(Vin, Vip);
	else
		AD_Value = ReferenceChannel.AD_Convert_Reverse(Vin, Vip);
	
	
	REF_AD_String = AD_Value;

	REF_AD = REF_Transcoding(AD_Value);


}




