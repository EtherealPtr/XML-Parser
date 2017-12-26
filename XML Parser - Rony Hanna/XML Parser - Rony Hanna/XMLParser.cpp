#include "XMLParser.h"

extern string filename;

XMLParser::XMLParser() :
	m_isContained(false)
{ /* Do Nothing Else */ }

XMLParser::~XMLParser() { /* Do Nothing */ }

// -----------------------------
// @Author: Rony Hanna
// @Description: Function that reads the data within the tags of an XML file
// -----------------------------
std::vector<string> XMLParser::readXml(string line, string xmlTag)
{
	// Open the well-formed XML file 
	ifstream file(filename + ".xml", std::ios::in | std::ios::binary);

	// Prepare vector of strings
	vector<string> read_xml_file;

	// Check if the file has opened successfully
	if (file.is_open())
	{
		// Parse the XML file and do not read the tags
		while (getline(file, line))
		{
			// If the tag is found
			if ((line.find(xmlTag)) != std::string::npos)
			{
				int start = 0, end = 0;
				bool erase = false;
				int index = 0;
				int sum = 0;

				// Remove all tabs 
				line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

				// Read the line
				string read = line.substr(0, line.length());
				string readBetween;

				// Remove any whitespace
				index = read.find('<');
				for (unsigned int i = 0; i < read.size(); ++i)
				{
					if (i < index)
					{
						if (read[i] == ' ')
						{
							read.erase(i, index);
						}
					}
				}

				for (unsigned int i = 0; i < read.size(); ++i)
				{
					if (read[i] == '<')
					{
						start = read.find('<');
					}
					if (read[i] == '>')
					{
						end = read.find('>') + 1;
						erase = true;
					}
					if (erase == true)
					{
						read.erase(start, end);
						sum = end - start;
						erase = false;
						break;
					}
				}

				int c = 0;
				readBetween.resize(read.size() - sum - 2);
				for (unsigned int i = 0; i < read.size(); ++i)
				{
					if (read[i] == '<')
					{
						break;
					}
					else
					{
						readBetween[c] = read[i];
						c++;
					}
				}

				// Store the line inside the vector 
				read_xml_file.push_back(readBetween);
			}
		}

		// Close the file
		file.close();
	}
	else
	{
		std::cerr << "ERROR: Unable to open XML file.\n";
	}

	// Return the vector containing the favourite game 
	return read_xml_file;
}

// -----------------------------
// @Author: Rony Hanna
// @Description: Function that reads the Tags of each line in an XML file 
// -----------------------------
std::vector<string> XMLParser::readXml(string line)
{
	// Open the well-formed XML file 
	ifstream file(filename + ".xml", std::ios::in | std::ios::binary);

	// Prepare vector of strings
	vector<string> read_xml_file;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			if ((line.find("<?") != std::string::npos))
			{
				continue;
			}

			// Read tag
			if ((line.find("</") && line.find(">")))
			{
				string read = line.substr(line.find("</") + 2, line.length() - (line.find(">") + 2));

				if (line.find('>'))
				{
					char temp = '>';
					for (unsigned int i = 0; i < read.size(); ++i)
					{
						if (read[i] == temp)
						{
							read.erase(read.size() - 2, read.size() - 1);
						}
					}
				}

				for (std::vector<std::string>::const_iterator iter = read_xml_file.begin(); iter != read_xml_file.end(); ++iter)
				{
					if (*iter == read)
					{
						m_isContained = true;
					}
				}

				if (!m_isContained)
					read_xml_file.push_back(read);

				m_isContained = false;
			}
		}

		// Close the file
		file.close();
	}
	else
	{
		std::cerr << "ERROR: Unable to open XML file.\n";
	}

	return read_xml_file;
}