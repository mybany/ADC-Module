#ifndef __SAR_ADC_H__
#define __SAR_ADC_H__
#include<vector>
#include<math.h>
#include<string>
using namespace std;
//===========================SAR ADC===========================//
//This SAR ADC is Normal Up Plate Sampling
//The Number of Cap is 11
//Vcm is not used in AD-Convertion
//You have to initial the CapArray before AD-Convertion
class SARADC
{
public:
	int n;							//Resolution of ADC
	double Vref_high;				//Reference Voltage
	double Vref_low;				//Reference Voltage
	double Vcm;						//Common Voltage
	vector<double> PCapArray;		//CapArray
	vector<double> NCapArray;
	double Cunit;					//Cunit
	double NCtot;					//Sum of Cap
	double PCtot;

	double radix;					//Radix of non binary ADC

	double P_DAC_Voltage;
	double N_DAC_Voltage;


	double ComparatorOffset;
	vector<double> CapOffset;


public:
	SARADC();
	
	void InitSARADC(int n_in, double Vref_high_in, double Vref_low_in, double Vcm_in, double Cunit_in, double radix_in);
	//If you do not want to use non binary ADC, just make radix=2.
	//However, if you want to use non binary ADC, please give the really num of n!  Not the ADC's n.

	void InitCapArray();
	
	void InitOffsetMisMatch(double offset_in);
	void InitCapMismatch(vector<double> NCap_Mis_Ratio, vector<double> PCap_Mis_Ratio);
	void InitTimeMismatch();
	
	string AD_Convert_Reverse(double Vin, double Vip);
	//The AD_Convert_Reverse is for Cap Mismatch Calibration
	//It is usually used in TI_SAR_ADC as REF channel
	//So if you do not need Cap Mismatch Calibration, you 
	//do not need this.


	string AD_Convert(double Vin, double Vip);


};













#endif