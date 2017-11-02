#include "XMLParser.h"
#include "ExcelParser.h"
#include <Windows.h>

LPCWSTR g_szClassName = L"MainWindowClass";

#define TO_EXCEL_BTN 1
#define TO_XML_BTN 2

extern string filename = "FavouriteGames";

ofstream outputExcelFile;
string line;
vector<string> xml, xmlDoc[255];
vector<string> xmlTag;
XMLParser xmlFile;

ofstream outputXmlFile;
ExcelParser excelParser;
vector<string> excelFile, excelFileTags, excelFileClosingTags, excelData, excelData2;
int startIndex = 0;
int sizeOfXml = 0;
int c = 0;

void ConvertToCSV()
{
	// Read XML tags
	xml = xmlFile.readXml(line);
	for (unsigned int i = 0; i < xml.size(); ++i)
	{
		std::string tag = xml.at(i);

		if (tag == "\r" || tag == "")
		{
			continue;
		}

		xmlTag.push_back(tag);
	}

	sizeOfXml = xmlTag.size();

	// Read data without tags
	for (unsigned int i = 0; i < sizeOfXml; ++i)
	{
		xmlDoc[i] = xmlFile.readXml(line, xmlTag[i]);
	}

	// Prepare the excel/csv file
	outputExcelFile.open(filename + ".csv");

	std::cout << "Parsing XML file...";

	// Read the tags into the file so that table in excel looks more organized
	for (unsigned int i = 0; i < xmlTag.size(); ++i)
	{
		outputExcelFile << xmlTag.at(i).append(",");
	}

	outputExcelFile << "\n";

	// Loop through and store the XML without tags
	for (unsigned int i = 0; i < xmlDoc[0].size(); ++i)
	{
		for (unsigned int j = 0; j < xmlTag.size(); ++j)
		{
			outputExcelFile << xmlDoc[j][i] << ",";
		}

		outputExcelFile << "\n";
	}

	outputExcelFile.close();
}
void ConvertToXML()
{
	excelFile = excelParser.readCsvTags(line);
	for (unsigned int i = 0; i < excelFile.size() - 1; ++i)
	{
		std::string tag = excelFile.at(i);
		excelFileTags.push_back(tag);
	}

	for (unsigned int i = 0; i < excelFileTags.size(); ++i)
	{
		std::string tag = excelFileTags.at(i);
		tag.insert(tag.begin() + 1, '/');
		excelFileClosingTags.push_back(tag);
	}

	startIndex = 0;
	excelData = excelParser.readCsv(line, startIndex);
	for (unsigned int i = startIndex + 1; i < excelData.size(); ++i)
	{
		std::string data = excelData.at(i);
		excelData2.push_back(data);
	}

	outputXmlFile.open("GeneratedXMLFile.xml");

	outputXmlFile << "<?xml version = \"1.0\" encoding = \"UTF-8\"?>";
	outputXmlFile << "\n<Root>\n";
	outputXmlFile << "\t<Body>\n";

	for (unsigned int i = 0; i < excelData2.size() - 1; ++i)
	{
		if (excelData2[i] == "\r")
		{
			outputXmlFile << "\t</Body>\n";
			outputXmlFile << "\t<Body>\n";
			continue;
		}

		outputXmlFile << "\t\t" << excelFileTags.at(c) << excelData2.at(i) << excelFileClosingTags.at(c) << "\n";
		c++;
		if (c >= excelFileTags.size())
		{
			c = 0;
		}
	}

	outputXmlFile << "\t</Body>\n";
	outputXmlFile << "</Root>\n";
	outputXmlFile.close();
}

HWND textfield;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CREATE:
	{
		LPCWSTR button01_ID = L"BUTTON";
		LPCWSTR button02_ID = L"BUTTON";
		LPCWSTR button01_text = L"Generate CSV/Excel File";
		LPCWSTR button02_text = L"Generate XML File";
		HWND button01 = CreateWindowEx(NULL, button01_ID, button01_text, BS_DEFPUSHBUTTON | WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 10, 200, 50, hwnd, (HMENU)TO_EXCEL_BTN, NULL, NULL);
		HWND button02 = CreateWindowEx(NULL, button02_ID, button02_text, BS_DEFPUSHBUTTON | WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 100, 200, 50, hwnd, (HMENU)TO_XML_BTN, NULL, NULL);
		
		textfield = CreateWindow(L"STATIC", L"Copyright (c) 2017 Rony Hanna", WS_VISIBLE | WS_CHILD, 20, 200, 220, 20, hwnd, NULL, NULL, NULL);
	}
	break;
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case TO_EXCEL_BTN:
			ConvertToCSV();
			break;

		case TO_XML_BTN:
			ConvertToXML();
			break;
		}
	}
	break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, Msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG msg;
	HWND hwnd;

	ZeroMemory(&wc, sizeof(wc));

	wc.style = 0;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIconSm = LoadIcon(NULL, IDC_ICON);
	wc.hIcon = LoadIcon(NULL, IDC_ICON);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;

	if (!RegisterClassEx(&wc))
	{
		LPCWSTR Error01 = L"Could not register class";
		LPCWSTR Error01_Caption = L"Error";
		MessageBox(NULL, Error01, Error01_Caption, MB_OK | MB_ICONERROR);
	}

	LPCWSTR WindowTitle = L"XML Parser Tool - Rony Hanna (c) 2017 Media Design School";

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		LPCWSTR Error02 = L"Could not create window!";
		LPCWSTR Error02_Caption = L"Error";
		MessageBox(NULL, Error02, Error02_Caption, MB_OK | MB_ICONERROR);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}