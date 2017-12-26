#pragma once
#ifndef __XMLParser_H__
#define __XMLParser_H__

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>

using std::string;
using std::vector;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::cerr;

class XMLParser
{
public:
	XMLParser();
	~XMLParser();

	std::vector<string> readXml(string line, string xmltag);
	std::vector<string> readXml(string line);

private:
	bool m_isContained;
};

#endif // !__XMLParser_H__