//x86 Minecraft Modpack Downloader
// (c) Keramis 2023
// https://github.com/keramis

#define CURL_STATICLIB
#define _CRT_SECURE_NO_WARNINGS //yes I know this is stupid idc

#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <string>
#include <thread>
#include "curl/curl.h"

//fetch curl library
#pragma comment (lib, "curl/libcurl_a.lib")

//we need these goddamnit
#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")

std::string findEntryValue(std::string& line)
{
	int last = line.find_last_of("\"");
	int secondToLast = line.find_last_of("\"", last - 1);

	return line.substr(secondToLast + 1, last - secondToLast - 1);
}

std::string getModName(std::string& line)
{
	int last = line.find_last_of("\"");
	int secondToLast = line.find_last_of("/");

	return line.substr(secondToLast + 1, last - secondToLast - 1);
}

static size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

std::string encodeURL(std::string line)
{
	int idx = 0;
	while ((idx = line.find(' ', idx)) != std::string::npos)
	{
		line.erase(line.begin() + idx);
		line.insert(idx, "%20");
	}
	return line;
}

void initializeDownload(std::vector<std::string>& links)
{
	//curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL* curl = curl_easy_init();

	if (!curl)
	{
		std::cerr << "Could not initialize cURL...\n";
		curl_global_cleanup();
		return;
	}

	for (std::string l : links)
	{
		FILE* fp = fopen(getModName(l).c_str(), "wb");
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		//we need to do this in case the mod link has a space in it (why the fuck does it do that?)
		std::string url_encoded = encodeURL(l);
		auto err = curl_easy_setopt(curl, CURLOPT_URL, url_encoded.c_str());
		err = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		//we need to do followLocation because it's a CDN
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		err = curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		std::cout << "Downloading " << getModName(l) << "\n";
		auto res = curl_easy_perform(curl);
		if (res)
		{
			std::cerr << "Error on CURL_PERFORM, link " << l << '\n';
			std::cerr << "Formatted linK: " << url_encoded << '\n';
			break;
		}
		fclose(fp);
	}

	curl_easy_cleanup(curl);
	//curl_global_cleanup();
}

#define testing false;

int main(int argc, char* argv[])
{
#if testing
	/*
	CURL* curl;
	FILE* fp;
	CURLcode res;
	std::string url = "https://edge.forgecdn.net/files/3968/312/FallingTree-1.19.2-3.10.0.jar";
	char outfilename[FILENAME_MAX] = "testjar.jar";
	curl = curl_easy_init();
	if (curl)
	{
		fp = fopen(outfilename, "wb");
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
	}*/

	std::cout << encodeURL(" l l ll dfd 2  1  3     11");

	return 0;

#else

	if (argc < 2)
	{
		std::cout << "No file provided.\n";
		Sleep(5000);
		exit(-1);
	}


	std::cout << "File provided. Reading...\n";
	//the file provided is vector1 (second argument)
	
	//open the file
	std::ifstream file(argv[1]);
	if (!file.is_open())
	{
		std::cout << "Could not open file...\n";
		Sleep(5000);
		exit(-1);
	}

	//parse all lines from file and add to vector
	std::cout << "Parsing lines...\n\n";
	std::vector<std::string> lines{};
	std::vector<std::string> downloadLinks{};
	std::string line;
	while (std::getline(file, line))
	{
		lines.push_back(line);
	}
	file.close();

	//search through the vector for links

	//the first "version" is the modpack version.
	//the second "version" is the minecraft version to play.
	int versionsCompleted = 0;
	for (auto& line : lines)
	{

		if (line.contains("version")) //print out version
		{
			//version looks like this:
			//"version":  "1.6.2.1",

			//prefix for the version
			std::string versub{ };
			if (!versionsCompleted)
				versub = "Modpack version: ";
			else
				versub = "Minecraft version: ";

			//display version
			std::cout << versub << findEntryValue(line) << '\n';
		}

		if (line.contains("author")) //print out author
		{
			std::cout << "Author: " << findEntryValue(line) << '\n';
		}

		if (line.contains("downloadUrl"))
		{
			downloadLinks.push_back(findEntryValue(line));
		}

	}

	std::cout << "\n\n ---- MODPACK EXTERNAL DOWNLOADER (c) Keramis 2023 ----\n\n";
	//give the user a chance to exit out
	std::cout << "INITIALIZING DOWNLOADS IN 5 SECONDS...\n\n";
	Sleep(5000);

	std::thread a(initializeDownload, std::ref(downloadLinks));
	a.detach();

	for (;;)
	{
		Sleep(5000);
	}
#endif
}