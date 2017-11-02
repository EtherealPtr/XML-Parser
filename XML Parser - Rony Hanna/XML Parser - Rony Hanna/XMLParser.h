#pragma once
#ifndef __XMLParser_H__
#define __XMLParser_H__

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

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