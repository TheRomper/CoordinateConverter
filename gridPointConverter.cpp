/*
 * gpctes.cpp
 *
 *  Created on: Apr 12, 2014
 *      Author: Administrator
 */

//============================================================================
// Name        : GridpointConverter
// Author      : Andrea Brenner-Billet
// Version     : V2.0
// File		   : gridPointConverter.cpp
// Description : Simple tool for converting coordinates between coordinate systems
//============================================================================
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include "gridPointConverter.h"

using namespace std;

Temp::Temp(double xIn, double yIn, double tempIn) {
	adjustedTemp = 0;
	x = xIn;
	y = yIn;
	temp = tempIn;
}

gpctest::gpctest() {
	for (int i = 0; i < sizeX; i++)  //setting init values for error checking
		for (int j = 0; j < sizeY; j++) {
			nonAdjustedTemps[i][j] = -10000000000;
			adjustedTemps[i][j] = 0;
		}

	maxDiff = MAXDIFF;
	tooLargeDiff = TOOLARGEDIFF;
	weight = WEIGHT;

	sizeX = SIZEX;
	sizeY = SIZEY;

	file1Weight = FILE1WEIGHT;
	file2Weight = FILE2WEIGHT;

	longLow = LONGLOW;
	longUp = LONGUP;

	latLow = LATLOW;
	latUp = LATUP;

	fillOp = FILL;
	fillSize = FILLSIZE;

	gpsToOdv = GPSTOODV;

	drawGreenland = DRAWGREENLAND;
	drawIceland = DRAWICELAND;

	multipleFiles = MULTIPLEFILES;

//	init();
//
//	if (fillOp == false) {
//
//		read();
//		roundGridpoint();
//		smooth();
//		if (gpsToOdv == false)  {
//
//			icelandOutline();
//			if (drawGreenland)
//				greenlandOutline();
//			write();
//		} else {
//			goFill(fillSize);
//			writeFill(fillSize);
//		}
//
//	} else {
//		fill(fillSize);
//
//	}

	menu();
}

gpctest::~gpctest() {
	// TODO Auto-generated destructor stub
}

void gpctest::menu() {

	int quit = 1;
	int option;

	while (quit != 0) {
		cout << "Choose type of conversion" << endl;
		cout << "1. GPS to gridpoint" << endl;
		cout << "2. Gridpoint to GPS (for OceanDataView)" << endl;
		cout << "3. GPS to gridpoint to GPS (for OceanDataView)" << endl;
		cout << "4. Help" << endl;
		cout << " Press 0 to quit" << endl;

		cin >> option;

		switch (option) {
		case 1:
			startGPStoGP();
			break;
		case 2:
			startGPtoGPS();
			break;
		case 3:
			startGPStoGPtoGPS();
			break;
		case 4:
			help();
			break;
		case 0:
			quit = 0;
			break;
		}
	}
}

void gpctest::startGPStoGP() {

	int option;
	int loopOptions = 1;
	char mFiles;

	//Setting up options
	cout << "Using multiple input files? (y or n)" << endl;
	cin >> mFiles;
	if (mFiles == 'y') {
		multipleFiles = 1;
	} else {
		multipleFiles = 0;
	}

	cout << endl;

	while (loopOptions) {

		cout << "Here are the current options:" << endl;
		cout << "1. Maximum GPS Difference: " << maxDiff << endl;
		cout << "2. Maximum smoothing difference: " << tooLargeDiff << endl;
		cout << "3. Weight of point: " << weight << endl;
		cout << "4. Gridpoint size X: " << sizeX << endl;
		cout << "5. Gridpoint size Y: " << sizeY << endl;
		cout << "6. Minimum Longitude: " << longLow << endl;
		cout << "7. Maximum Longitude: " << longUp << endl;
		cout << "8. Minimum Latitude: " << latLow << endl;
		cout << "9. Maximum Latitude: " << latUp << endl;

		if (drawIceland)
			cout << "10. Draw Iceland: True" << endl;
		else
			cout << "10. Draw Iceland: False" << endl;
		if (drawGreenland)
			cout << "11. Draw Greenland: True" << endl;
		else
			cout << "11. Draw Greenland: False" << endl;

		if (multipleFiles) {
			cout << "12. Multiple Input Files: True" << endl;
			cout << "13. File 1 weight: " << file1Weight << endl;
			cout << "14. File 2 weight: " << file2Weight << endl;
		} else
			cout << "12. Multiple Input Files: False" << endl;

		cout
				<< "-->To change a variable, enter list number and press enter, otherwise press 0 to start"
				<< endl << endl;

		cin >> option;

		switch (option) {
		case 1:
			cout
					<< "This is the maximum GPS distance from a point that will be factored into its temperature value during conversion to gripoints"
					<< endl;
			cin >> maxDiff;
			break;
		case 2:
			cout
					<< "This is the maximum gridpoint difference used during the smoothing of temperatures"
					<< endl;
			cin >> tooLargeDiff;
			break;
		case 3:
			cout
					<< "This is the weight that a point holds in comparison to it's neighbors during smoothing"
					<< endl;
			cin >> weight;
			break;
		case 4:
			cout << "The X range of gridpoints" << endl;
			cin >> sizeX;
			break;
		case 5:
			cout << "The Y range of gridpoints" << endl;
			cin >> sizeY;
			break;
		case 6:
			cout << "The minimum longitude" << endl;
			cin >> longLow;
			break;
		case 7:
			cout << "The maximum longitude" << endl;
			cin >> longUp;
			break;
		case 8:
			cout << "The minimum latitude" << endl;
			cin >> latLow;
			break;
		case 9:
			cout << "The maximum latitude" << endl;
			cin >> latUp;
			break;
		case 10:
			cout << "Set area of Iceland to 999 degrees (y or no)" << endl;
			char iceTemp;
			cin >> iceTemp;
			if (iceTemp == 'y')
				drawIceland = 1;
			else
				drawIceland = 0;
			break;
		case 11:
			cout << "Set area of Greenland to 999 degrees (y or no)" << endl;
			char greenTemp;
			cin >> greenTemp;
			if (greenTemp == 'y')
				drawGreenland = 1;
			else
				drawGreenland = 0;
			break;
		case 12:
			cout << "Use multiple input files (y or no)" << endl;
			char mulFiles;
			cin >> mulFiles;
			if (mulFiles == 'y')
				multipleFiles = 1;
			else
				multipleFiles = 0;
			break;
		case 13:
			cout
					<< "The weight that file 1 holds in comparison to the second file"
					<< endl;
			cin >> file1Weight;
			break;
		case 14:
			cout
					<< "The weight that file 2 holds in comparison to the first file"
					<< endl;
			cin >> file2Weight;
			break;

		case 0:
			loopOptions = 0;
			break;
		}

	}

	init();
	read();
	roundGridpoint();
	smooth();
	if (drawIceland)
		icelandOutline();

	if (drawGreenland)
		greenlandOutline();

	write();

}

void gpctest::startGPtoGPS() {

	int option;
	int loopOptions = 1;

	cout << endl;

	while (loopOptions) {

		cout << "Here are the current options:" << endl;
		cout << "1. Gridpoint size X: " << sizeX << endl;
		cout << "2. Gridpoint size Y: " << sizeY << endl;
		cout << "3. Minimum Longitude: " << longLow << endl;
		cout << "4. Maximum Longitude: " << longUp << endl;
		cout << "5. Minimum Latitude: " << latLow << endl;
		cout << "6. Maximum Latitude: " << latUp << endl;
		cout << "7. Fill Size: " << fillSize << endl;

		cout
				<< "-->To change a variable, enter list number and press enter, otherwise press 0 to start"
				<< endl << endl;

		cin >> option;

		switch (option) {
		case 1:
			cout << "The X range of gridpoints" << endl;
			cin >> sizeX;
			break;
		case 2:
			cout << "The Y range of gridpoints" << endl;
			cin >> sizeY;
			break;
		case 3:
			cout << "The minimum longitude" << endl;
			cin >> longLow;
			break;
		case 4:
			cout << "The maximum longitude" << endl;
			cin >> longUp;
			break;
		case 5:
			cout << "The minimum latitude" << endl;
			cin >> latLow;
			break;
		case 6:
			cout << "The maximum latitude" << endl;
			cin >> latUp;
			break;
		case 7:
			cout << "This is the amount of points to fill in between gridpoints"
					<< endl;
			cin >> fillSize;
			break;

		case 0:
			loopOptions = 0;
			break;
		}

	}
	init();
	fill(fillSize);
}

void gpctest::startGPStoGPtoGPS() {
	int option;
	int loopOptions = 1;
	char mFiles;

	//Setting up options
	cout << "Using multiple input files? (y or n)" << endl;
	cin >> mFiles;
	if (mFiles == 'y') {
		multipleFiles = 1;
	} else {
		multipleFiles = 0;
	}

	cout << endl;

	while (loopOptions) {

		cout << "Here are the current options:" << endl;
		cout << "1. Maximum GPS Difference: " << maxDiff << endl;
		cout << "2. Maximum smoothing difference: " << tooLargeDiff << endl;
		cout << "3. Weight of point: " << weight << endl;
		cout << "4. Gridpoint size X: " << sizeX << endl;
		cout << "5. Gridpoint size Y: " << sizeY << endl;
		cout << "6. Minimum Longitude: " << longLow << endl;
		cout << "7. Maximum Longitude: " << longUp << endl;
		cout << "8. Minimum Latitude: " << latLow << endl;
		cout << "9. Maximum Latitude: " << latUp << endl;

		if (drawIceland)
			cout << "10. Draw Iceland: True" << endl;
		else
			cout << "10. Draw Iceland: False" << endl;
		if (drawGreenland)
			cout << "11. Draw Greenland: True" << endl;
		else
			cout << "11. Draw Greenland: False" << endl;

		cout << "12. Fill Size: " << fillSize << endl;

		if (multipleFiles) {
			cout << "13. Multiple Input Files: True" << endl;
			cout << "14. File 1 weight: " << file1Weight << endl;
			cout << "15. File 2 weight: " << file2Weight << endl;
		} else
			cout << "13. Multiple Input Files: False" << endl;

		cout
				<< "-->To change a variable, enter list number and press enter, otherwise press 0 to start"
				<< endl << endl;

		cin >> option;

		switch (option) {
		case 1:
			cout
					<< "This is the maximum GPS distance from a point that will be factored into its temperature value during conversion to gripoints"
					<< endl;
			cin >> maxDiff;
			break;
		case 2:
			cout
					<< "This is the maximum gridpoint difference used during the smoothing of temperatures"
					<< endl;
			cin >> tooLargeDiff;
			break;
		case 3:
			cout
					<< "This is the weight that a point holds in comparison to it's neighbors during smoothing"
					<< endl;
			cin >> weight;
			break;
		case 4:
			cout << "The X range of gridpoints" << endl;
			cin >> sizeX;
			break;
		case 5:
			cout << "The Y range of gridpoints" << endl;
			cin >> sizeY;
			break;
		case 6:
			cout << "The minimum longitude" << endl;
			cin >> longLow;
			break;
		case 7:
			cout << "The maximum longitude" << endl;
			cin >> longUp;
			break;
		case 8:
			cout << "The minimum latitude" << endl;
			cin >> latLow;
			break;
		case 9:
			cout << "The maximum latitude" << endl;
			cin >> latUp;
			break;
		case 10:
			cout << "Set area of Iceland to 999 degrees (y or no)" << endl;
			char iceTemp;
			cin >> iceTemp;
			if (iceTemp == 'y')
				drawIceland = 1;
			else
				drawIceland = 0;
			break;
		case 11:
			cout << "Set area of Greenland to 999 degrees (y or no)" << endl;
			char greenTemp;
			cin >> greenTemp;
			if (greenTemp == 'y')
				drawGreenland = 1;
			else
				drawGreenland = 0;
			break;
		case 12:
			cout << "This is the amount of points to fill in between gridpoints"
					<< endl;
			cin >> fillSize;
			break;
		case 13:
			cout << "Use multiple input files (y or no)" << endl;
			char mulFiles;
			cin >> mulFiles;
			if (mulFiles == 'y')
				multipleFiles = 1;
			else
				multipleFiles = 0;
			break;
		case 14:
			cout
					<< "The weight that file 1 holds in comparison to the second file"
					<< endl;
			cin >> file1Weight;
			break;
		case 15:
			cout
					<< "The weight that file 2 holds in comparison to the first file"
					<< endl;
			cin >> file2Weight;
			break;

		case 0:
			loopOptions = 0;
			break;
		}

	}
	init();
	read();
	roundGridpoint();
	smooth();
	goFill(fillSize);
	writeFill(fillSize);

}

void gpctest::init() {

//initializing files

	cout << "Enter input filename: (do not include .dat)" << endl;
	cin >> inputFilename;
	inputFilename.append(".dat");

#if multipleFiles

	cout << "Enter 2nd input filename: (do not include .dat)" << endl;
	cin >> inputFilename2;
	inputFilename2.append(".dat");

#endif

	cout << "Enter output filename (do not include .dat): " << endl;

	cin >> outFileName;

//			const char* m = (const char*) MAXDIFF;
//			const char* t = (const char*) TOOLARGEDIFF;
//			const char* w = (const char*) WEIGHT;
//		outputFilename.append("(");
//		outputFilename.append(m);
//		outputFilename.append(",");
//		outputFilename.append(t);
//		outputFilename.append(",");
//	outputFilename.append(w);
//	outputFilename.append(")");
//outputFilename.append("temps");

//		char* mP=(char*)m;
//		char* tP=(char*)t;
//		char* wP=(char*)w;
//		outFileName.append(mP);
//		outFileName.append(tP);
//		outFileName.append(wP);
	outFileName.append(".dat");

	cout << endl << endl;

}

void gpctest::read() {
	ifstream initFile;
	initFile.open(inputFilename.c_str()); //opens file

	double x, y, temp;
	string line;

#if multipleFiles
	ifstream initFile2;
	initFile2.open(inputFilename2.c_str()); //opens file
#endif

	while (initFile.good()) {
		std::getline(initFile, line);

		initFile >> x >> y >> temp;
		if (x != -1 || y != -1) { //checks that no coordinates are outside of range
			Temp In(x, y, temp);
			initialNum.push_back(In);
			In.~Temp();
		}

	}

	initFile.close(); //remember to close the file
	cout << "File 1 read in successfully!" << endl << endl;

#if multipleFiles
	vector<Temp> initialNum2;
	double nonAdjustedTemps2[sizeX][sizeY];

	for (int i = 0; i < sizeX; i++)  //setting init values for error checking
	for (int j = 0; j < sizeY; j++)
	nonAdjustedTemps2[i][j] = -10000000000;

	while (initFile2.good()) {
		std::getline(initFile2, line);

		initFile2 >> x >> y >> temp;
		if (x != -1 || y != -1) { //checks that no coordinates are outside of range
			Temp In(x, y, temp);
			initialNum2.push_back(In);
			In.~Temp();
		}

	}
	initFile2.close(); //remember to close the file
	cout << "File 2 read in successfully!" << endl;

#endif
	cout << "_________________________________________________" << endl<<endl;

}

void gpctest::fill(int size) {

	readFill();
	goFill(size);
	writeFill(size);

}

void gpctest::goFill(int size) {
	cout << "Filling..." << endl ;
	double beginT, endT, diff, step;
	for (int i = 0; i < sizeX; i++)  // Filling in X values
		for (int j = 0; j < sizeY; j++) {

			if (i != sizeX  && j != sizeY ) {
				beginT = adjustedTemps[i][j];
				endT = adjustedTemps[i + 1][j];

				diff = abs(abs(endT) - abs(beginT));
				step = diff / (double) size;

				if (endT > beginT)
					for (int k = 0; k < size; k++) {
						//exArray[i*size + k][j*size] = beginT + step * k;

						exArray[i * size + k][j] = beginT + step * k;

					}
				else
					for (int k = 0; k < size; k++) {
//						exArray[i*size + k][j*size] = beginT - step * k;
						exArray[i * size + k][j] = beginT - step * k;

					}
			} else
				exArray[i * size][j] = adjustedTemps[i][j];
		}

//	for (int i = 0; i < sizeX * size; i++)  // Filling in Y values
//		for (int j = 0; j < sizeY; j++) {
//
//			if (i != (sizeX*size) - 1 && j != sizeY - 1) {
//				beginT = adjustedTemps[i][j];
//				endT = adjustedTemps[i][j + 1];
//
//				diff = abs(abs(endT) - abs(beginT));
//				step = diff /(double) size;
//
//				if (endT > beginT)
//					for (int k = 0; k < size; k++) {
//						exArray[i][j*size + k] = beginT + step * k;
//					}
//				else
//					for (int k = 0; k < size; k++) {
//						exArray[i][j*size + k] = beginT - step * k;
//					}
//			}
//		}

}

void gpctest::readFill() {

	ifstream initFile;
	initFile.open(inputFilename.c_str()); //opens file
	double temp;
	int lineNum = 0;
	string line;
	int x, y;
	while (initFile.good()) {
		std::getline(initFile, line);

		initFile >> temp;
		lineNum++;
		y = (lineNum - 1) / sizeX;
		x = ((lineNum - 1) % sizeX);
		if (temp == 999)
			adjustedTemps[x][y] = 0;
		else
			adjustedTemps[x][y] = temp;

//		cout << x << "  " << y << "  " << temp<<endl; //tester

	}

	initFile.close(); //remember to close the file
	cout << "File 1 read in successfully!" << endl;

	cout << "_________________________________________________" << endl<<endl;

}

void gpctest::writeFill(int size) {
	cout << "Writing to file..." << endl;
	double longitude, lat, posI, posJ;
	posI = 0;
	posJ = 0;
	longitude = 0;
	lat = 0;
	int longR = abs(longUp - longLow);
	int latR = abs(latUp - latLow);
	ofstream outfile;
//	try {

	outfile.open(outFileName.c_str());


		for (int j = 0; j < (sizeY); j++) //	for (int j = 0; j <( sizeY * size); j++)
			for (int i = 0; i < (sizeX * size); i++) //	for (int i = 0; i < (sizeX * size-(size-1)); i++)  //iterate through final temps
				if (i != sizeX * size  && j != sizeY ) { //		if (i != sizeX*size - (size-1) && j != sizeY - 1){
					posI = i;
					posJ = j;
//				longitude = longLow + ((posI / (sizeX*size-size)) * longR);
					longitude = longLow + ((posI / (sizeX * size)) * longR);

					//		lat = latLow + (posJ / (sizeY*size)) * latR;
					lat = latLow + (posJ / (sizeY)) * latR;

					cout << posI << "\t" << longitude << "\t" << posJ << "\t"
							<< lat << "\t" << exArray[i][j] << endl;

					outfile<< longitude << "\t"<< lat<< "\t"<< exArray[i][j]<<endl;

				}



//	} catch (int e) {
//		cout << e << endl;
//	}

	outfile.close();
	cout << "Write Successful!" << endl<<endl;

}

void gpctest::roundGridpoint() {
	cout << "Starting coordinate conversion..." << endl;

	int count = 0;
	int totalTemp = 0;

	for (int x1 = 0; x1 < sizeX; x1++) //iterate through non adjusted temps array
		for (int y1 = 0; y1 < sizeY; y1++) {
			for (it = initialNum.begin(); it != initialNum.end(); it++) //iterates through read in temps
					{

				double yTemp = abs((it -> y) - y1);
				double xTemp = abs(it -> x - x1);

				//if read-in x,y coordinates are <1 of desired coordinate
				if (xTemp < maxDiff)
					if (yTemp < maxDiff) {
						totalTemp += it-> temp;
						count++;
					}
			}

			if (count != 0)
				nonAdjustedTemps[x1][y1] = totalTemp / (double) count; //averaging temps

			//	cout<< "Total Temps for ("<<x1<<","<<y1<<") : "<<count<<endl;
			count = 0;
			totalTemp = 0;
		}

#if multipleFiles
	count = 0;
	totalTemp = 0;

	for (int x1 = 0; x1 < sizeX; x1++) //iterate through non adjusted temps array
	for (int y1 = 0; y1 < sizeY; y1++) {
		for (it = initialNum2.begin(); it != initialNum2.end(); it++) //iterates through read in temps
		{
			double yTemp = abs(it->y - y1);
			double xTemp = abs(it->x - x1);
			//if read-in x,y coordinates are <1 of desired coordinate
			if (xTemp < maxDiff)
			if (yTemp < maxDiff) {
				totalTemp += it->temp;
				count++;
			}
		}

		if (count != 0)
		nonAdjustedTemps2[x1][y1] = totalTemp / (double) count; //averaging temps

		//	cout<< "Total Temps for ("<<x1<<","<<y1<<") : "<<count<<endl;
		count = 0;
		totalTemp = 0;
	}
	cout << "______________________________________________" << endl << endl;

	cout<<"Starting addition of files..."<<endl;
	for (int x1 = 0; x1 < sizeX; x1++) //iterate through non adjusted temps array
	for (int y1 = 0; y1 < sizeY; y1++) {
		if(nonAdjustedTemps[x1][y1]==-10000000000)
		nonAdjustedTemps[x1][y1]=nonAdjustedTemps2[x1][y1];
		else if(nonAdjustedTemps2[x1][y1]==-10000000000)
		nonAdjustedTemps2[x1][y1]=nonAdjustedTemps[x1][y1];
		else
		nonAdjustedTemps[x1][y1]=(file1Weight*nonAdjustedTemps[x1][y1] + file2Weight*nonAdjustedTemps2[x1][y1])/(double)2.0;
	}
#endif
	cout << "______________________________________________" << endl << endl;

}
void gpctest::smooth() {

	cout << "Smoothing..." << endl ;

//smoothing out

	double initValue = 0;
	double totalTemp = 0;
	double totalTemp2 = 0;

	int count2 = 0;

	int startX, endX, startY, endY;
	for (int x1 = 0; x1 < sizeX; x1++) //iterate through non adjusted temps
		for (int y1 = 0; y1 < sizeY; y1++) {
//			if (nonAdjustedTemps[x1][y1] == -10000000000) //error check
//				cout << "Temp Null at (" << x1 << "," << y1 << ")" << endl;
			initValue = nonAdjustedTemps[x1][y1]; //stores init value for comparison

			startX = x1 - 1;
			endX = x1 + 1;
			startY = y1 - 1;
			endY = y1 + 1;

			if (x1 == 0) //if left edge
				startX = 0;
			if (x1 == sizeX - 1) //if right edge
				endX = sizeX - 1;
			if (y1 == 0) //if lower edge
				startY = 0;
			if (y1 == sizeY - 1) //if upper edge
				endY = sizeY - 1;

			for (int i = startX; i <= endX; i++) //iterate through surrounding points
				for (int j = startY; j <= endY; j++) {
					if (nonAdjustedTemps[i][j] == -10000000000)
						;  //error check
						   //						cout<<"Temp Null at"<<i<<","<<j<<endl;
					else {
//						if (abs(nonAdjustedTemps[i][j] - initValue)
//								> tooLargeDiff) //if too large of a difference
//							cout << "Too big of a difference b/t (" << x1 << ","
//									<< y1 << ")  and  (" << i << "," << j
//									<< ")  by: "
//									<< nonAdjustedTemps[i][j] - initValue
//									<< endl;
//						else //if no unassigned values or large difference found
//						{
							totalTemp2 += nonAdjustedTemps[i][j];
							count2++;
						//}
					}
				}
			if (count2 != 0) {
				if (nonAdjustedTemps[x1][y1] == -10000000000)
					adjustedTemps[x1][y1] = ((totalTemp / count2));
				else
					adjustedTemps[x1][y1] = ((totalTemp / count2)
							+ (nonAdjustedTemps[x1][y1] * weight))
							/ (weight + 1); //adjusting temp according to weight
			} else
//				cout << "count2=0 at  " << "(" << x1 << "," << y1 << ")"
//						<< endl;
				//			cout<<"("<<x1<<","<<y1<<"): "<<"Initial Temp: "<<initValue<<"  Adjusted Temp: "<<adjustedTemps[x1][y1]<<endl;
				count2 = 0;
			totalTemp2 = 0;
		}

	cout << endl << "Done with Smoothing!" << endl;
	cout << "______________________________________________" << endl<<endl;

}
void gpctest::icelandOutline() {

//setting iceland outline to 999 degrees

	cout << "Drawing Iceland..." << endl;

	adjustedTemps[25][28] = 999;
	adjustedTemps[25][29] = 999;

	adjustedTemps[26][22] = 999;
	adjustedTemps[26][23] = 999;
	for (int i = 27; i <= 30; i++)
		adjustedTemps[26][i] = 999;
	adjustedTemps[26][32] = 999;

	adjustedTemps[27][22] = 999;
	adjustedTemps[27][23] = 999;
	for (int i = 27; i <= 33; i++)
		adjustedTemps[27][i] = 999;

	for (int i = 22; i <= 24; i++)
		adjustedTemps[28][i] = 999;
	for (int i = 28; i <= 34; i++)
		adjustedTemps[28][i] = 999;

	for (int i = 22; i <= 24; i++)
		adjustedTemps[29][i] = 999;
	for (int i = 28; i <= 33; i++)
		adjustedTemps[29][i] = 999;

	for (int i = 22; i <= 24; i++)
		adjustedTemps[30][i] = 999;
	for (int i = 28; i <= 32; i++)
		adjustedTemps[30][i] = 999;
	for (int i = 34; i <= 36; i++)
		adjustedTemps[30][i] = 999;

	for (int i = 15; i <= 17; i++)
		adjustedTemps[31][i] = 999;
	for (int i = 22; i <= 24; i++)
		adjustedTemps[31][i] = 999;
	for (int i = 29; i <= 36; i++)
		adjustedTemps[31][i] = 999;

	for (int i = 15; i <= 17; i++)
		adjustedTemps[32][i] = 999;
	for (int i = 22; i <= 26; i++)
		adjustedTemps[32][i] = 999;
	for (int i = 28; i <= 36; i++)
		adjustedTemps[32][i] = 999;

	for (int i = 15; i <= 16; i++)
		adjustedTemps[33][i] = 999;
	for (int i = 20; i <= 26; i++)
		adjustedTemps[33][i] = 999;
	for (int i = 28; i <= 35; i++)
		adjustedTemps[33][i] = 999;

	for (int i = 15; i <= 34; i++)
		adjustedTemps[34][i] = 999;

	for (int i = 15; i <= 33; i++)
		adjustedTemps[35][i] = 999;

	for (int i = 15; i <= 32; i++)
		adjustedTemps[36][i] = 999;

	for (int i = 15; i <= 29; i++)
		adjustedTemps[37][i] = 999;
	adjustedTemps[37][31] = 999;
	adjustedTemps[37][32] = 999;

	for (int i = 15; i <= 29; i++)
		adjustedTemps[38][i] = 999;

	for (int i = 14; i <= 29; i++)
		adjustedTemps[39][i] = 999;

	for (int i = 13; i <= 29; i++)
		adjustedTemps[40][i] = 999;
	adjustedTemps[40][31] = 999;
	adjustedTemps[40][32] = 999;

	for (int i = 12; i <= 33; i++)
		for (int j = 41; j <= 42; j++)
			adjustedTemps[j][i] = 999;

	for (int i = 12; i <= 31; i++)
		adjustedTemps[43][i] = 999;

	for (int i = 12; i <= 32; i++)
		adjustedTemps[44][i] = 999;

	for (int i = 11; i <= 33; i++)
		for (int j = 45; j <= 49; j++)
			adjustedTemps[j][i] = 999;

	for (int i = 12; i <= 33; i++)
		adjustedTemps[50][i] = 999;

	for (int i = 13; i <= 33; i++)
		adjustedTemps[51][i] = 999;

	for (int i = 14; i <= 32; i++)
		adjustedTemps[52][i] = 999;

	for (int i = 14; i <= 34; i++)
		adjustedTemps[53][i] = 999;

	for (int i = 14; i <= 34; i++)
		adjustedTemps[54][i] = 999;

	for (int i = 14; i <= 34; i++)
		adjustedTemps[55][i] = 999;

	for (int i = 15; i <= 36; i++)
		adjustedTemps[56][i] = 999;

	for (int i = 16; i <= 36; i++)
		adjustedTemps[57][i] = 999;

	for (int i = 17; i <= 36; i++)
		adjustedTemps[58][i] = 999;

	for (int i = 18; i <= 35; i++)
		adjustedTemps[59][i] = 999;

	for (int i = 18; i <= 34; i++)
		for (int j = 60; j <= 61; j++)
			adjustedTemps[j][i] = 999;

	for (int i = 18; i <= 35; i++)
		adjustedTemps[62][i] = 999;

	for (int i = 19; i <= 32; i++)
		adjustedTemps[63][i] = 999;
	for (int i = 34; i <= 35; i++)
		adjustedTemps[63][i] = 999;

	for (int i = 19; i <= 32; i++)
		adjustedTemps[64][i] = 999;

	for (int i = 21; i <= 29; i++)
		adjustedTemps[65][i] = 999;

	for (int i = 22; i <= 29; i++)
		adjustedTemps[66][i] = 999;

	for (int i = 23; i <= 29; i++)
		adjustedTemps[67][i] = 999;

	for (int i = 24; i <= 28; i++)
		adjustedTemps[68][i] = 999;

//writing to file
	cout << "______________________________________________" << endl << endl;

}

void gpctest::greenlandOutline() {

//setting iceland outline to 999 degrees

	cout << "Drawing Greenland..." << endl << endl;

	int k = 40;
	for (int i = 0; i <= 50; i++) {
		k++;
		for (int j = k; j <= sizeY; j++)
			adjustedTemps[i][j] = 999;
	}

	cout << "______________________________________________" << endl << endl;

}

void gpctest::write() {
	ofstream outfile;
	outfile.open(outFileName.c_str());
	cout << "Writing to file..." << endl;
	for (int j = 0; j < sizeY; j++)
		for (int i = 0; i < sizeX; i++)  //iterate through final temps
				{
			outfile << adjustedTemps[i][j] << endl;
		}

	outfile.close();

	cout << "Write Successful!" << endl<<endl;

}

void gpctest::help() {

}

int main() {

	gpctest *go;

	return 0;
}
