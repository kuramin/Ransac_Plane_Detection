#pragma once

#include "Scan_Point.h"

#include <fstream>
#include <iomanip>
#include <vector>

class TXTWriter
{

public:
	static void WritePointXYZ(std::string path, const std::vector<Scan_Point> &points)
	{
		if (points.size() == 0)
			return;

		std::ofstream file;
		file.open(path, std::ios::out);

		file << std::fixed << std::showpoint;
		file << std::setprecision(3);

		for (const Scan_Point &p : points)
		{
			file << p << endl;
		}

		file.close();
	}

	static void WritePointPTX(std::string path, const std::vector<Scan_Point> &points)
	{
		if (points.size() == 0)
			return;

		std::ofstream file;
		file.open(path, std::ios::out);

		file << std::fixed << std::showpoint;
		file << std::setprecision(3);

		// necessary lines of ptx-file
		file << "1" << endl << points.size() << endl;
		file << "0 0 0" << endl;
		file << "1 0 0" << endl;
		file << "0 1 0" << endl;
		file << "0 0 1" << endl;
		file << "1 0 0 0" << endl;
		file << "0 1 0 0" << endl;
		file << "0 0 1 0" << endl;
		file << "0 0 0 1" << endl;

		for (const Scan_Point &p : points)
		{
			file << p << endl;
		}

		file.close();
	}

	static void WritePointPointersPTX(std::string path, const std::vector<Scan_Point*> &points)
	{
		if (points.size() == 0)
			return;

		std::ofstream file;
		file.open(path, std::ios::out);

		file << std::fixed << std::showpoint;
		file << std::setprecision(3);

		// necessary lines of ptx-file
		file << "1" << endl << points.size() << endl;
		file << "0 0 0" << endl;
		file << "1 0 0" << endl;
		file << "0 1 0" << endl;
		file << "0 0 1" << endl;
		file << "1 0 0 0" << endl;
		file << "0 1 0 0" << endl;
		file << "0 0 1 0" << endl;
		file << "0 0 0 1" << endl;

		for (const Scan_Point* p : points)
		{
			file << *p << endl;
		}

		file.close();
	}
};