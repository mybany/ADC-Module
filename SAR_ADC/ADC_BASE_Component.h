#ifndef __ADC_BASE_COMPONENT_H__
#define __ADC_BASE_COMPONENT_H__
#include<vector>
#include<math.h>
#include<string>
#include<fstream>
using namespace std;


//===========================AD Setting===========================//
//This class is for setting AD parameter
//Before AD-Convertion, you need to set the Analog Information
class AD_Setting
{
public:
//For Normally ADC
	double Fin;
	double Fs;
	double Period;
	long int Samp_Times;
	double Start_Time;
	double Vcm;
	double Vref_high;
	double Vref_low;
	
//Math paraments
	const double PI = 3.14159265358979323846;

public:
	AD_Setting();
	void Init_AD_Setting(double Vcm_in,	
				double Vref_high_in,
				double Vref_low_in,
				double Fin_in,		
				double Fs_in,					
				int Samp_Times_in,				
				double Start_Times_in);

	double GetVinValue(double t);
	double GetVipValue(double t);
};


//===========================Base ADC===========================//
//This Base ADC is a Numerical ADC and it uses math way to AD-Convert
class BaseADC
{
public:
	int n;							//Resolution of ADC
	double Vref;
	
public:
	BaseADC();
	BaseADC(int n_in = 10, double Vref_in = 0.9);
	string ADC_getvalue(double Vin);
};






#endif



