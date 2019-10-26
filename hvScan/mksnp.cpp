#include <fstream>
#include <iostream>
#include <cmath>
#include <cfloat>
#include <string>
#include <sstream>

using namespace std;

// Global variables
string col1[6][1000];
int col2[6][1000], ctr;
double col3[6][1000] = {0};

string std_fname = "BAND_HV-aftersingleCheckout";
string std_ext   = ".snp";
string std_input;

// Forward-declaring functions here
void LoadHVtable();
const std::string currentDateTime();
// ==================================================================================
int main( int argc , char ** argv ){
	if(argc!=4){
		cout << "=====================\nRun this code by doing:\n./mksnp A B C" << endl;
		cout << "where A = change in PMT voltage [Volts]" << endl;
		cout << "      B = 1: all bars\n          2: long bars only\n          3: short bars only" << endl;
		cout << "      C = /path/to/input/file" << endl;
		cout << "=====================" << endl;
		exit(0);
	}

	std_input = argv[3];

	if( (atoi(argv[2])!=1)&&(atoi(argv[2])!=2)&&(atoi(argv[2])!=3) ){
		cout << "Input B has to be 1, 2, or 3. Run the code as: './code' for more info" << endl;
		exit(0);
	}
	
	cout << "=====================" << endl;
	string now = currentDateTime();
	cout << "--- " << now << " ---" << endl;
	double deltaV = stod(argv[1]);
	cout << "**************************************************************************" << endl;
	cout << "Will be changing current set voltage by " << deltaV << " V";
	string tmp_str1, tmp_str2;
	if     (atoi(argv[2])==1){tmp_str1 = " for ALL bars!!!"       ;	tmp_str2 =""      ;}
	else if(atoi(argv[2])==2){tmp_str1 = " for LONG bars ONLY!!!" ;	tmp_str2 ="_long" ;}
	else if(atoi(argv[2])==3){tmp_str1 = " for SHORT bars ONLY!!!";	tmp_str2 ="_short";}
	cout << tmp_str1 << endl;
	cout << "**************************************************************************" << endl;

	LoadHVtable();

	// -----------------------------------------------------
	// Looping over voltages to change their value
	for(int i = 0 ; i < ctr ; i++){
		if( (atoi(argv[2])==2&&(col1[0][i].substr(17,1)!="A"&&col1[0][i].substr(17,1)!="B"))||
		    (atoi(argv[2])==3&&(col1[0][i].substr(17,1)=="A"||col1[0][i].substr(17,1)=="B"))||
		    (atoi(argv[2])==1)
		){
			col3[0][i] += deltaV;
			if(col3[0][i]<0) col3[0][i] = 0;
		}
	}

	// -----------------------------------------------------
	// Saving new values into a file
	string st_tmp = std_input.substr(0,std_input.size()-4);
	string out_name = st_tmp + "_vset_" + argv[1] + "V" + tmp_str2 + std_ext;
	cout << "Changes will be saved to: " << out_name << endl;

	ofstream out;
	out.open(out_name);
	out << "--- Start BURT header" << endl;
	out << "Time:     Fri Feb  1 09:24:17 2019" << endl;
	out << "Login ID: clasrun (Online DAQ)"     << endl;
	out << "Eff  UID: 2508"                     << endl;
	out << "Group ID: 9998"                     << endl;
	out << "Keywords:"                          << endl;
	out << "Comments:"                          << endl;
	out << "Type:     Absolute"                 << endl;
	out << "Directory /home/clasrun"            << endl;
	out << "Req File: /usr/clas12/release/1.3.0/epics/tools/burtreq/BAND_HV.req" << endl;
	out << "--- End BURT header"                << endl;

	for(int i = 0 ; i < ctr ; i++){
		out << col1[0][i] << " " << col2[0][i] << " " << std::scientific;
		out << col3[0][i] << endl;
                out << col1[1][i] << " " << col2[1][i] << " " << std::scientific;
		out << col3[1][i] << endl;
                out << col1[2][i] << " " << col2[2][i] << " " << std::scientific;
		out << col3[2][i] << endl;
                out << col1[3][i] << " " << col2[3][i] << " " << std::scientific;
		out << col3[3][i] << endl;
                out << col1[4][i] << " " << col2[4][i] << " " << std::scientific;
		out << col3[4][i] << endl;
                out << col1[5][i] << " " << col2[5][i] << " " << std::scientific;
		out << col3[5][i] << endl;
	}

	out.close();

	return 0;
}
// ==================================================================================
void LoadHVtable(){
	string temp1;
	int    temp2;
	double temp3;
        string line;
        ifstream f;
        f.open(std_input);
	cout << "Loading file: '" << std_input << "'" << endl;
        if(f.fail()){cout << "Could not find input file '" << std_input << "'. Bailing out!" << endl; exit(0);}

        while(!f.eof()){

                getline(f, line);
                
		if(line[0]=='B'){
                        stringstream input(line);

                        input >> temp1;
			input >> temp2;
			input >> temp3;

			if     (temp1.substr(temp1.length() - 4)=="vset"){col1[0][ctr]=temp1;	col2[0][ctr]=temp2;	col3[0][ctr] = temp3;}
			else if(temp1.substr(temp1.length() - 4)=="vmax"){col1[1][ctr]=temp1;	col2[1][ctr]=temp2;	col3[1][ctr] = temp3;}
			else if(temp1.substr(temp1.length() - 4)=="iset"){col1[2][ctr]=temp1;	col2[2][ctr]=temp2;	col3[2][ctr] = temp3;}
			else if(temp1.substr(temp1.length() - 4)=="trip"){col1[3][ctr]=temp1;	col2[3][ctr]=temp2;	col3[3][ctr] = temp3;}
			else if(temp1.substr(temp1.length() - 4)==":rup"){col1[4][ctr]=temp1;	col2[4][ctr]=temp2;	col3[4][ctr] = temp3;}
			else if(temp1.substr(temp1.length() - 4)==":rdn"){col1[5][ctr]=temp1;	col2[5][ctr]=temp2;	col3[5][ctr] = temp3; ctr++;}
			else{
				cout << "Something went terribly wrong while loading the data. It's the end of the world! Bailing out." << endl;
				exit(0);
			}

		}
        }
	cout << "Loaded " << ctr << " values" << endl;
        f.close();

}
// ==================================================================================
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
