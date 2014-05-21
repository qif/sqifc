//============================================================================
// Name        : SQIF
// Author      : Quoc-Sang Phan <dark2bright@gmail.com>
// Version     : 0.1
// Copyright   : No
// Description : Symbolic Quantitative Information Flow
//============================================================================

#include "config.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<
			int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(
			std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

config::config(const char*filename) {

	string line;
	size_t found;
	ifstream infile(filename);
	if (infile.is_open()) {
		while (infile.good()) {
			getline(infile, line);
			if (line.find("#") != string::npos)
				continue;
			found = line.find("=");
			if (found == string::npos)
				continue;
			else {
				string key = line.substr(0, found);
				string entry = line.substr(found + 1, line.size() - 1);
				insert(pair<string, string> (trim(key), trim(entry)));
			}
		}
		infile.close();
	} else {
		cout << "Unable to open config file" << endl << endl;
		return;
	}

	if (find("cbmc") == end())
		cout << "Config error: cbmc not found" << endl;
	if (find("driver") == end())
		cout << "Config error: driver not found" << endl;
	if (find("tmp") == end())
		cout << "Config error: tmp not found" << endl;
	if (find("size") == end())
		cout << "Config error: data size not found" << endl;
}

void config::printEntries() {
	map<string, string>::iterator it;
	for (it = begin(); it != end(); ++it) {
		cout << it->first << " : " << it->second << endl;
	}
}

const char* config::getCBMCpath() {
	return find("cbmc")->second.c_str();
}

const char* config::getDriverPath() {
	return find("driver")->second.c_str();
}

const char* config::getTmpPath() {
	return find("tmp")->second.c_str();
}

int config::getSize() {
	return atoi(find("size")->second.c_str());
}

int config::getPolicy() {
	if (find("policy") == end())
		return 32; // default policy: 5 bits
	return atoi(find("policy")->second.c_str());
}

config::~config() {
}

