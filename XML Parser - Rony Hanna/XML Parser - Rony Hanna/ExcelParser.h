#pragma once
#ifndef __EXCELPARSER_H__
#define __EXCELPARSER_H__

#include <vector>
#include <string>

class ExcelParser
{
public:
	ExcelParser();
	~ExcelParser();

	std::vector<std::string> readCsv(std::string _line, int& _iteration);
	std::vector<std::string> readCsvTags(std::string _line);
};

#endif // !__EXCELPARSER_H__