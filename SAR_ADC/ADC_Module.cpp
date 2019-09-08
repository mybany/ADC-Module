// SAR_ADC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
#include <iostream>
#include <random>
#include "ADC_BASE_Component.h"
#include "SAR_ADC.h"
#include "TI_SAR_ADC.h"
#include "Pipelined_SAR_ADC.h"
#include "MyDebug.h"
#define channel_num 4


void MT4005B(void);
void MT4006(void);

int main()
{
	MT4006();
	
}

void MT4006()
{
	AD_Setting AD_Signal;
	double Main_Clock = 1 / 50e6;
	AD_Signal.Init_AD_Setting(1, 0.89*2, 0, 50e6 * 251 / 1024, 50e6, 1e4, 2e-9);


	SARADC testAD;
	testAD.InitSARADC(8, 2, 0, 1, 4e-15, 2);
	testAD.InitCapArray();

	vector<string> AD_Result(AD_Signal.Samp_Times);

	for (long int i = 0; i < AD_Signal.Samp_Times; i++)
	{
		double time = AD_Signal.Start_Time + Main_Clock * i;
		double Vin = AD_Signal.GetVinValue(time);
		double Vip = AD_Signal.GetVipValue(time);

		AD_Result[i] = testAD.AD_Convert(Vin, Vip);
	}

	TXT_Output_String(AD_Result);
}




void MT4005B()
{
	AD_Setting AD_Signal;
	double Main_Clock = 1 / 6.4e9;
	double Ref_Clock = 1 / (6.4e9 / (channel_num + 1));

	//AD_Setting AD_Signal_for_Reference;
	AD_Signal.Init_AD_Setting(0.5, 0.89*0.7, 0.3, 6.4e9 * 251 / 1024, 6.4e9, 2000, 2e-9);


	TI_SARADC MT2801;
	MT2801.Init_Single_Channel(13, 0.7, 0.3, 0.5, 1, 1.88);
	MT2801.Init_TI_SARADC(12, 13, 1.88);
	MT2801.Init_Ref_Channel();
	MT2801.Init_MisMatch();
	MT2801.Init_Calibration();

	int points = 1e4;
	vector<double> TI_SARADC_Result(points);
	vector<double> Parament1;

	default_random_engine randomengine;
	randomengine.seed(123);
	uniform_int_distribution<int> randomregion(1, 100);
	normal_distribution<double> jitter_random(0.24e-12, 3e-12);
	double jitter = 0;
	for (long int i = 0; i < points; i++)
	{
		double main_time = AD_Signal.Start_Time + i * Main_Clock;
		//jitter = jitter_random(randomengine);
		double vin = AD_Signal.GetVinValue(main_time + jitter);
		double vip = AD_Signal.GetVipValue(main_time);
		TI_SARADC_Result[i] = MT2801.AD_Convert(vin, vip);
		if (i % (channel_num + 1) == 0)
		{
			int period = i / (channel_num + 1);
			int R_P_Choose = randomregion(randomengine);
			double vin_ref = AD_Signal.GetVinValue(AD_Signal.Start_Time + period * Ref_Clock);
			double vip_ref = AD_Signal.GetVipValue(AD_Signal.Start_Time + period * Ref_Clock);
			MT2801.Ref_AD_Get(vin_ref, vip_ref, R_P_Choose);
			MT2801.CapMisMatch_Calibration(MT2801.NOW_AD_Cali_String, MT2801.REF_AD_String);
			Parament1.push_back(MT2801.Cap_Cali_Residual[1]);
			cout << points - i << endl;
		}
	}
	TXT_Output(TI_SARADC_Result);
	Parament1_Output(Parament1);
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
