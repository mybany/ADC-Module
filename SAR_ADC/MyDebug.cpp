#include "MyDebug.h"


//===========================TXT Output===========================//
void TXT_Output(vector<double> AD_Result)
{
	ofstream out_file("E:\\WorkFile_E\\MatlabFile\\MT2801_Model\\AD_Result(test).txt");
	//out_file << "Hello World" << endl;
	out_file.precision(14);
	for (int i = 0; i < AD_Result.size(); i++)
	{
		out_file << AD_Result[i] << endl;
	}

}

void TXT_Output_String(vector<string> AD_Result)
{
	ofstream out_file("E:\\WorkFile_E\\MatlabFile\\MT4006_Model\\AD_Result(test).txt");
	//out_file << "Hello World" << endl;
	//out_file.precision(14);
	for (int i = 0; i < AD_Result.size(); i++)
	{
		out_file << AD_Result[i] << endl;
	}
}


void Parament1_Output(vector<double> Parament)
{
	ofstream out_file("E:\\WorkFile_E\\MatlabFile\\MT2801_Model\\Parament1(test).txt");
	//out_file << "Hello World" << endl;
	out_file.precision(14);
	for (int i = 0; i < Parament.size(); i++)
	{
		out_file << Parament[i] << endl;
	}
}



void Temp_Output(double input)
{
	ofstream out_file("E:\\WorkFile_E\\MatlabFile\\MT2801_Model\\tempdata.txt", ios::app);
	out_file.precision(14);
	out_file << input << endl;
}