#include "XMLParser.h"
#include "ExcelParser.h"
#include <Windows.h>

char* g_szClassName = "MainWindowClass";

#define TO_EXCEL_BTN 1
#define TO_XML_BTN 2
#define TEXT_INPUT_ID 3
#define SUBMIT_BUTTON 4

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
char filePath[255];
HWND hEdit;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CREATE:
	{
		char* button01_ID = "BUTTON";
		char* button02_ID = "BUTTON";
		char* button3_ID = "BUTTON";
		char* button01_text = "Generate CSV/Excel File";
		char* button02_text = "Generate XML File";
		char* button03_text = "Submit";
		HWND button01 = CreateWindowEx(NULL, button01_ID, button01_text, BS_DEFPUSHBUTTON | WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 10, 200, 50, hwnd, (HMENU)TO_EXCEL_BTN, NULL, NULL);
		HWND button02 = CreateWindowEx(NULL, button02_ID, button02_text, BS_DEFPUSHBUTTON | WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 100, 200, 50, hwnd, (HMENU)TO_XML_BTN, NULL, NULL);
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "Enter file name then press submit (default file is FavouriteGames)",
			WS_CHILD | WS_VISIBLE, 10, 220, 450, 20, hwnd, (HMENU)TEXT_INPUT_ID, GetModuleHandle(NULL), NULL);
		 
		HWND button3 = CreateWindowEx(NULL, button3_ID, button03_text, BS_DEFPUSHBUTTON | WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 250, 450, 40, hwnd, (HMENU)SUBMIT_BUTTON, NULL, NULL);

		textfield = CreateWindow("STATIC", "Copyright (c) 2017 Rony Hanna - Media Design School", WS_VISIBLE | WS_CHILD, 30, 430, 400, 20, hwnd, NULL, NULL, NULL);
	}
	break;
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case TO_EXCEL_BTN:
			ConvertToCSV();
			MessageBox(NULL, "An Excel/CSV file has been successfully generated.", "Operation Complete", MB_OK | MB_ICONINFORMATION);
			break;

		case TO_XML_BTN:
			ConvertToXML();
			MessageBox(NULL, "An XML file has been successfully generated.", "Operation Complete", MB_OK | MB_ICONINFORMATION);
			break;
			 
		case SUBMIT_BUTTON:
			int stat = 0;
			char* t = &filePath[0];
			stat = GetWindowText(hEdit, t, 20); 
		 
			std::string s(filePath);
			filename.clear();
			filename = s;

			char buffer[100];
			string name = filename;
			sprintf_s(buffer, "The file %s is now read to be parsed.", name.c_str());
			MessageBox(NULL, buffer, "Ready", MB_OK | MB_ICONINFORMATION);
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
	wc.hbrBackground = (HBRUSH)(LTGRAY_BRUSH);
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;

	if (!RegisterClassEx(&wc))
	{
		char* Error01 = "Could not register class";
		char* Error01_Caption = "Error";
		MessageBox(NULL, Error01, Error01_Caption, MB_OK | MB_ICONERROR);
	}

	char* WindowTitle = "XML Parser Tool - Rony Hanna (c) 2017 Media Design School";

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		char* Error02 = "Could not create window!";
		char* Error02_Caption = "Error";
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
 