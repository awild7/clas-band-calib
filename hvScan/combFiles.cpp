#include <fstream>
#include <iostream>
#include <cmath>
#include <cfloat>
#include <string>
#include <sstream>
#include <map>

using namespace std;

// Forward-declaring functions:
void LoadCalibfile(string file_input, std::map<string,double> &input_map_mean, std::map<string,double> &input_map_sigma);
void LoadHVfile(string file_input, std::map<string,double> &input_map);
// ===================================================================================================
int main(int argc,char ** argv){


  if(argc<4) {
			cout << "not enough input files given " << endl;
      cout << "Usage:" << endl;
			cout << " /calib_snp_combine <file1> <file2> <output>" << endl;
			cout << " where <file1> is from Calibration suite with HV fit values and " << endl;
			cout << " <file2> has the HV setting information. " << endl;
			cout << "<output> is the name of the outputfile." << endl;
		}
	else {
 	  string calib_input = argv[1];
		cout << "Calibration fit file " << calib_input << " is used." << endl;
		string hv_input = argv[2];
		cout << "HV setting file " << hv_input << " is used." << endl;
		string outputfile = argv[3];
		cout << "Output file will be " << outputfile << endl;

    std::map<string,double> mean_values;
		std::map<string,double> sigma_values;
    std::map<string,double> HV_values;

  	LoadCalibfile(calib_input, mean_values, sigma_values);
		LoadHVfile(hv_input, HV_values);

		ofstream tab1;
		tab1.open(outputfile);
		tab1 << "# SECTOR LAYER COMPONENT ORDER HV MEAN SIGMA" << endl;

    for (auto itr = mean_values.begin(); itr != mean_values.end(); ++itr) {
			if (sigma_values.count(itr->first) && HV_values.count(itr->first) )
			{
        string key = itr->first;
		//	cout << key.at(0) << " " <<key.at(1) << " " << key.at(2) << " " << key.at(3) << " " <<  HV_values.at(itr->first) << " " << itr->second << " " << sigma_values.at(itr->first) << " " <<  endl;
			tab1 << key.at(0) << " " << key.at(1) << " " << key.at(2) << " " << key.at(3) << " " ;
			tab1 << HV_values.at(itr->first) << " " << itr->second << " " << sigma_values.at(itr->first) << endl;

			}


    }
  tab1.close();

  }
	cout << "*****************************************" << endl;
	// ---------------------------------------------
	return 0;
}
// ===================================================================================================
void LoadCalibfile(string file_input, std::map<string,double> &input_map_mean, std::map<string,double> &input_map_sigma)
{
	//Calib File structure:
	//SECTOR LAYER COMPONENT ORDER MEAN SIGMA
	string sector;
	string layer;
	string component;
	string order;
	double mean;
	double sigma;
	string key;
	ifstream f;
	string line;
  f.open(file_input);

	if(f.fail()){cout << "Could not find input file " << file_input << " . Bailing out!" << endl; exit(0);}

	while(!f.eof()){
   	getline(f, line);
		if(line[0]!='#'){
			stringstream input(line);
			input >> sector;
			input >> layer;
			input >> component;
			input >> order;
			input >> mean;
			input >> sigma;
			key = sector + layer + component + order;

      input_map_mean.insert( std::pair<string, double>(key, mean));
			input_map_sigma.insert( std::pair<string, double>(key, sigma));
			//cout << sector << " " << layer << " " << component << " " << order << " key " << key << " mean: " << mean << " sigma: " << sigma <<  endl;
		}
	}

	f.close();
}

void LoadHVfile( string file_input, std::map<string,double> &input_map)
{
	//HV File structure:
	//SECTOR LAYER COMPONENT ORDER HVvalue
	string sector;
	string layer;
	string component;
	string order;
	double hv_value;
	string key;
	string line;
	ifstream f;
	f.open(file_input);

	if(f.fail()){cout << "Could not find input file " << file_input << " . Bailing out!" << endl; exit(0);}
	while(!f.eof()){
   	getline(f, line);
		if(line[0]!='#'){
			stringstream input(line);
			input >> sector;
			input >> layer;
			input >> component;
			input >> order;
			input >> hv_value;
			key = sector + layer + component + order;

      input_map.insert( std::pair<string, double>(key, hv_value));
			//cout << sector << " " << layer << " " << component << " " << order << " key " << key << " mean: " << mean << " sigma: " << sigma <<  endl;
		}
	}

	f.close();
}
