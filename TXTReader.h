#pragma once

#include "Util.h"
#include "Scan_Point.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class TXTReader
{

public:
	static vector<Scan_Point> Read_point_PTX_to_vector(const string path)
	{
		vector<Scan_Point> result;
		ifstream file(path, ios::in);
		string fileInput_point;
		vector<string> coords;
		while (file && !file.eof())
		{
			getline(file, fileInput_point);
			if (!fileInput_point.empty())
			{
				coords = util::Util::Split(fileInput_point, " ", false);
				/*if (coords.size() == 4)
				{
					result.push_back(Scan_Point(atof(coords[0].c_str()), 
												atof(coords[1].c_str()), 
												atof(coords[2].c_str()), 
												atof(coords[3].c_str())));
				}*/
				if (coords.size() == 7)
				{
					result.push_back(Scan_Point(atof(coords[0].c_str()), 
												atof(coords[1].c_str()),
												atof(coords[2].c_str()),
												atof(coords[3].c_str()),
												atof(coords[4].c_str()), 
												atof(coords[5].c_str()), 
												atof(coords[6].c_str())));
				}
			}
		}
		return result;
	}

	static vector<vector<Scan_Point>> Read_point_PTX_to_matrix(string path)
	{
		ifstream file(path, ios::in);
		string fileInput_point, str_col_num, str_row_num;
		vector<string> coords;
		getline(file, str_col_num); // read the first 2 lines with size of the image
		getline(file, str_row_num);
		const int col_num = atoi(str_col_num.c_str());
		const int row_num = atoi(str_row_num.c_str());

		vector<vector<Scan_Point>> result;
		for (int i = 0; i < col_num; i++) {
			result.push_back(vector<Scan_Point>());
			for (int j = 0; j < row_num; j++) {
				result[i].push_back(Scan_Point(0,0,0,255,0,0,0));
			}
		}

		// read lines of projection description, react only if end of file found there
		for (int k = 0; k < 8; k++) {
			getline(file, fileInput_point);
			if (!file || file.eof()) {
				cout << "Unexpected end of file" << endl;
				break;
			}
		}

		// now read main body of the file
		int counter = 0, i = 0, j = 0;
		while (file && !file.eof())
		{
			getline(file, fileInput_point);
			i = counter / row_num;
			j = counter % row_num;
			if (i < col_num) {
				if (!fileInput_point.empty()) {
					counter++;
					coords = util::Util::Split(fileInput_point, " ", false);
					if (coords.size() == 7) {
						result[i][j] = Scan_Point(atof(coords[0].c_str()),
							atof(coords[1].c_str()),
							atof(coords[2].c_str()),
							atof(coords[3].c_str()),
							atof(coords[4].c_str()),
							atof(coords[5].c_str()),
							atof(coords[6].c_str()));
					}
				}
			}
			else break;
		}
		if (counter < row_num * col_num) cout << "Too short file" << endl;
		else cout << "File read successfully" << endl;
		if (!fileInput_point.empty()) cout << "But file is too long and a part of it is not used" << endl;
		
		return result;
	}

	static vector<vector<Scan_Point*>> Read_point_PTX_to_matrix_of_pointers(string path)
	{
		ifstream file(path, ios::in);
		string fileInput_point, str_col_num, str_row_num;
		vector<string> coords;
		getline(file, str_col_num); // read the first 2 lines with size of the image
		getline(file, str_row_num);
		const int col_num = atoi(str_col_num.c_str());
		const int row_num = atoi(str_row_num.c_str());

		// initialize resulting matrix
		vector<vector<Scan_Point*>> result;
		for (int i = 0; i < col_num; i++) {
			result.push_back(vector<Scan_Point*>());
			for (int j = 0; j < row_num; j++) {
				result[i].push_back(new Scan_Point(0, 0, 0, 255, 0, 0, 0));
			}
		}

		// read lines of projection description, react only if end of file found there
		for (int k = 0; k < 8; k++) {
			getline(file, fileInput_point);
			if (!file || file.eof()) {
				cout << "Unexpected end of file" << endl;
				break;
			}
		}

		// now read main body of the file
		int counter = 0, i = 0, j = 0;
		while (file && !file.eof())
		{
			getline(file, fileInput_point);
			i = counter / row_num; // define i and j from current value of counter
			j = counter % row_num;
			if (i < col_num) { // only if result matrix is not full, do
				if (!fileInput_point.empty()) { // if line is not empty
					counter++; // increase counter only if line is non-empty line and result matrix is not full
					coords = util::Util::Split(fileInput_point, " ", false);
					if (coords.size() == 4) { // if line consists of 4 numbers
						result[i][j] = new Scan_Point(atof(coords[0].c_str()), // create Scan_Point and assign pointer on it to of result[i][j]
							atof(coords[1].c_str()),
							atof(coords[2].c_str()),
							round(255*atof(coords[3].c_str())));
					}
					if (coords.size() == 7) { // if line consists of 7 numbers
						result[i][j] = new Scan_Point(atof(coords[0].c_str()), // create Scan_Point and assign pointer on it to of result[i][j]
							atof(coords[1].c_str()),
							atof(coords[2].c_str()),
							round(255*atof(coords[3].c_str())),
							atof(coords[4].c_str()),
							atof(coords[5].c_str()),
							atof(coords[6].c_str()));
					}
				}
			}
			else break; // if result matrix is full, break
		}
		if (counter < row_num * col_num) cout << "Too short file" << endl; // if eof was found too early 
		else cout << "File read successfully" << endl; 

		// if result matrix is full but line is not empty, say that file is too long
		if (!fileInput_point.empty()) cout << "But file is too long and a part of it is not used" << endl;

		return result;
	}
};