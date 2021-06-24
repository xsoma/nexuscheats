#pragma once
#include <iostream>
#include <windows.h>
#include <WinUser.h>
#include <WinBase.h>
#include <tchar.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "Advapi32.lib")
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <WinInet.h>
#include <iomanip>
#include <cstdio>
#include <TlHelp32.h>
#include <Psapi.h>
#include <wtsapi32.h>
#define use using namespace std;
//using namespace std;


std::string replaceAll(std::string subject, const std::string& search,
	const std::string& replace) {
	use;
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string getHwid()
{
	DWORD lVolSerialNbr = 0;
	char sHDSerial[255] = "";
	GetVolumeInformationA("C:\\", 0, 0, &lVolSerialNbr, 0, 0, 0, NULL);
	_ultoa_s(lVolSerialNbr, sHDSerial, 10);
	std::string c(sHDSerial);
	return c;
}

int Exit()
{
	int a = 0, b = 0;
	__asm int 0;
	return a / b;
}

std::string DownloadString(std::string URL) {
	use;
	HINTERNET interwebs = InternetOpenA("NexusAgentHttp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}

std::vector<std::string> split_string(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;

	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();

	} while (pos < str.length() && prev < str.length());

	return tokens;
}
