#include "ADC_BASE_Component.h"

//===========================AD Setting===========================//
AD_Setting::AD_Setting()
{
	Fin = 0;
	Fs = 0;
	Period = 0;
	Samp_Times = 0;
	Start_Time = 0;
	Vcm = 0;
}

void AD_Setting::Init_AD_Setting(double Vcm_in,	
								double Vref_high_in,
								double Vref_low_in,
								double Fin_in,						
								double Fs_in,						
								int Samp_Times_in,					
								double Start_Times_in)
{
	Vcm = Vcm_in;
	Vref_high = Vref_high_in;
	Vref_low = Vref_low_in;
	Fin = Fin_in;
	Fs = Fs_in;
	Period = 1 / Fs_in;
	Samp_Times = Samp_Times_in;
	Start_Time = Start_Times_in;
}

double AD_Setting::GetVipValue(double t)
{
	return Vcm + 0.5 * (Vref_high - Vref_low) * sin(2 * PI * Fin * t);
}

double AD_Setting::GetVinValue(double t)
{
	return Vcm - 0.5 * (Vref_high - Vref_low) * sin(2 * PI * Fin * t);
}




//===========================Base ADC===========================//
BaseADC::BaseADC()
{
	n = 0;					
	Vref = 0;
}

BaseADC::BaseADC(int n_in, double Vref_in)
{
	n = n_in;
	Vref = Vref_in;
}

string BaseADC::ADC_getvalue(double Vin)
{
	string AD_Result = "";
	double LSB = Vref / (pow(2, n));
	string code = "";
	int value = (int)(Vin / LSB + 0.5);
	
	for (int i = 0; i < n; i++)
	{
		code = (value % 2) + 48;
		AD_Result = AD_Result + code;
		value = value / 2;
	}
	
	AD_Result = string(AD_Result.rbegin(), AD_Result.rend());

	return AD_Result;
}








