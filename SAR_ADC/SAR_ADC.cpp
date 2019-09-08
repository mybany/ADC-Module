#include "SAR_ADC.h"

//===========================SAR ADC===========================//
SARADC::SARADC()
{
	n = 0;
	Vref_high = 0;
	Vref_low = 0;
	Vcm = 0;
	//CapArray.push_back(1e-12);
	Cunit = 1e-12;
	NCtot = 0;
	PCtot = 0;
}

void SARADC::InitSARADC(int n_in, double Vref_high_in, double Vref_low_in, double Vcm_in, double Cunit_in, double radix_in)
{
	n = n_in;
	Vref_high = Vref_high_in;
	Vref_low = Vref_low_in;
	Vcm = Vcm_in;
	Cunit = Cunit_in;
	for (int i = 0; i < n + 1; i++)
	{
		NCapArray.push_back(Cunit);
		PCapArray.push_back(Cunit);
	}
		
	PCtot = 0;
	NCtot = 0;
	radix = radix_in;
	ComparatorOffset = 0;
}


void SARADC::InitCapArray()
{
	for (int i = 0; i < n; i++)
	{
		PCapArray[i] = pow(radix, n - i - 1) * Cunit;
		NCapArray[i] = pow(radix, n - i - 1) * Cunit;
	}
	PCapArray[n - 1] = Cunit;
	PCapArray[n] = Cunit;
	NCapArray[n - 1] = Cunit;
	NCapArray[n] = Cunit;

	NCtot = 0;
	PCtot = 0;
	for (int i = 0; i < n + 1; i++)
	{
		NCtot = NCtot + NCapArray[i];
		PCtot = PCtot + PCapArray[i];
	}
}


void SARADC::InitOffsetMisMatch(double offset_in)
{
	ComparatorOffset = offset_in;
}

void SARADC::InitCapMismatch(vector<double> NCap_Mis_Ratio, vector<double> PCap_Mis_Ratio)
{
	double Nnew_cap_sum = 0;
	double Pnew_cap_sum = 0;
	for (int i = 0; i < n; i++)
	{
		NCapArray[i] = NCapArray[i] * NCap_Mis_Ratio[i];
		PCapArray[i] = PCapArray[i] * PCap_Mis_Ratio[i];
		Nnew_cap_sum = Nnew_cap_sum + NCapArray[i];
		Pnew_cap_sum = Pnew_cap_sum + PCapArray[i];
	}

	PCtot = Pnew_cap_sum;
	NCtot = Nnew_cap_sum;
}

void SARADC::InitTimeMismatch()
{

}





string SARADC::AD_Convert(double Vin, double Vip)
{
	vector<double> V_DAC_P_down(n + 1);
	vector<double> V_DAC_N_down(n + 1);
	V_DAC_P_down[0] = Vref_high;
	V_DAC_N_down[0] = Vref_low;

	string AD_Value = "";
	string code = "";

	for (int i = 1; i < n + 1; i++)
	{
		V_DAC_P_down[i] = Vref_low;
		V_DAC_N_down[i] = Vref_high;
	}

	double Pvalue = 0;
	double Nvalue = 0;

	double P_DAC_value = 0;
	double N_DAC_value = 0;

	for (int i = 0; i < n; i++)
	{
		Pvalue = 0;
		Nvalue = 0;
		for (int j = 0; j < n + 1; j++)
		{
			Pvalue = Pvalue + V_DAC_P_down[j] * PCapArray[j];
			Nvalue = Nvalue + V_DAC_N_down[j] * NCapArray[j];
		}

		//P_DAC_value = Pvalue / Ctot + (Vip - Vref_high) * CapArray[0] / Ctot + (Vip - Vref_low) * (Ctot - CapArray[0]) / Ctot;
		//N_DAC_value = Nvalue / Ctot + (Vin - Vref_low) * CapArray[0] / Ctot + (Vin - Vref_high) * (Ctot - CapArray[0]) / Ctot;

		P_DAC_value = Pvalue / PCtot + Vcm - Vip;
		N_DAC_value = Nvalue / NCtot + Vcm - Vin;

		P_DAC_Voltage = P_DAC_value;
		N_DAC_Voltage = N_DAC_value;

		if (N_DAC_value >= P_DAC_value + ComparatorOffset)
		//if(P_DAC_value - N_DAC_value + ComparatorOffset < 0)
		{
			code = 48 + 1;
			AD_Value = AD_Value + code;
			V_DAC_P_down[i + 1] = Vref_high;
			V_DAC_N_down[i + 1] = Vref_low;
		}

		else
		{
			code = 48 + 0;
			AD_Value = AD_Value + code;
			V_DAC_P_down[i] = Vref_low;
			V_DAC_P_down[i + 1] = Vref_high;
			V_DAC_N_down[i] = Vref_high;
			V_DAC_N_down[i + 1] = Vref_low;
		}
	}

	return AD_Value;

}

string SARADC::AD_Convert_Reverse(double Vin, double Vip)
{
	vector<double> V_DAC_P_down(n + 1);
	vector<double> V_DAC_N_down(n + 1);
	V_DAC_P_down[0] = Vref_low;
	V_DAC_N_down[0] = Vref_high;

	string AD_Value = "";
	string code = "";

	for (int i = 1; i < n + 1; i++)
	{
		V_DAC_P_down[i] = Vref_high;
		V_DAC_N_down[i] = Vref_low;
	}

	double Pvalue = 0;
	double Nvalue = 0;

	double P_DAC_value = 0;
	double N_DAC_value = 0;

	for (int i = 0; i < n; i++)
	{
		Pvalue = 0;
		Nvalue = 0;
		for (int j = 0; j < n + 1; j++)
		{
			Pvalue = Pvalue + V_DAC_P_down[j] * PCapArray[j];
			Nvalue = Nvalue + V_DAC_N_down[j] * NCapArray[j];
		}

		//P_DAC_value = Pvalue / Ctot + (Vip - Vref_high) * CapArray[0] / Ctot + (Vip - Vref_low) * (Ctot - CapArray[0]) / Ctot;
		//N_DAC_value = Nvalue / Ctot + (Vin - Vref_low) * CapArray[0] / Ctot + (Vin - Vref_high) * (Ctot - CapArray[0]) / Ctot;

		P_DAC_value = Pvalue / PCtot + Vcm + Vip;
		N_DAC_value = Nvalue / NCtot + Vcm + Vin;


		if (N_DAC_value > P_DAC_value - ComparatorOffset)
		//if(P_DAC_value - N_DAC_value - ComparatorOffset < 0)
		{
			code = 48 + 0;
			AD_Value = AD_Value + code;
			
			V_DAC_P_down[i] = Vref_high;
			V_DAC_P_down[i + 1] = Vref_low;
			V_DAC_N_down[i] = Vref_low;
			V_DAC_N_down[i + 1] = Vref_high;

			//V_DAC_P_down[i + 1] = Vref_high;
			//V_DAC_N_down[i + 1] = Vref_low;
		}

		else
		{
			code = 48 + 1;
			AD_Value = AD_Value + code;
			
			V_DAC_P_down[i + 1] = Vref_low;
			V_DAC_N_down[i + 1] = Vref_high;

			//V_DAC_P_down[i] = Vref_low;
			//V_DAC_P_down[i + 1] = Vref_high;
			//V_DAC_N_down[i] = Vref_high;
			//V_DAC_N_down[i + 1] = Vref_low;
		}
	}

	return AD_Value;
}



