#include "ExcelParser.h"
#include <algorithm>
#include <iostream>
#include <sstream>

using std::ifstream;
using std::vector;
using std::string;

extern string filename;

ExcelParser::ExcelParser()
{
}

ExcelParser::~ExcelParser()
{
}

std::vector<std::string> ExcelParser::readCsvTags(std::string _line)
{
	// Open the excisting csv file
	ifstream file(filename + ".csv", std::ios::in | std::ios::binary);

	// Prepare vector of strings
	vector<string> read_csv_file;
	std::string theLine;

	// Check if the file has opened successfully
	if (file.is_open())
	{
		// Parse the CSV file  
		while (std::getline(file, _line))
		{
			theLine = _line;
			break;
		}

		std::istringstream ss(theLine);
		std::string token;

		while (std::getline(ss, token, ','))
		{
			token.insert(token.begin(), '<');
			token.insert(token.end(), '>');
			read_csv_file.push_back(token);
		}

		// Close the file
		file.close();
	}
	else
	{
		std::cerr << "ERROR: Unable to open XML file.\n";
	}

	// Return the vector containing the excel row
	return read_csv_file;
}

std::vector<std::string> ExcelParser::readCsv(std::string _line, int& _iteration)
{
	// Open the excisting csv file
	ifstream file(filename + ".csv", std::ios::in | std::ios::binary);

	// Prepare vector of strings
	vector<string> read_csv_file;
	std::string theLine;
	bool doOnce = true;

	// Check if the file has opened successfully
	if (file.is_open())
	{
		// Parse the CSV file  
		while (std::getline(file, _line))
		{
			std::string read = _line;

			if (doOnce)
			{
				for (unsigned int i = 0; i < read.size(); ++i)
				{
					if (read[i] == ',')
					{
						++_iteration;
					}

					doOnce = false;
				}
			}

			// Read the data
			theLine = _line;
			std::istringstream ss(theLine);
			std::string token;

			while (std::getline(ss, token, ','))
			{
				read_csv_file.push_back(token);
			}
		}

		// Close the file
		file.close();
	}
	else
	{
		std::cerr << "ERROR: Unable to open XML file.\n";
	}

	// Return the vector containing the excel row
	return read_csv_file;
}