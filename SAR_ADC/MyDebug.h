#ifndef __MYDEBUG_H__
#define __MYDEBUG_H__
#include<vector>
#include<iostream>
#include<string>
#include<fstream>

using namespace std;

//===========================TXT Output===========================//
//This is not a class, it is a function to output the AD-Convertion Value
void TXT_Output(vector<double> AD_Result);
void Parament1_Output(vector<double> Parament);
void Temp_Output(double input);
void TXT_Output_String(vector<string> AD_Result);


#endif