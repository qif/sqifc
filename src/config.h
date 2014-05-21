//============================================================================
// Name        : SQIFC
// Author      : Quoc-Sang Phan <dark2bright@gmail.com>
// Version     : 0.1
// Copyright   : No
// Description : Symbolic Quantitative Information Flow Analysis for C
//============================================================================

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <map>

using namespace std;

class config: map<string, string> {
public:
	config(const char*);
	const char* getCBMCpath();
	const char* getDriverPath();
	const char* getTmpPath();
	int getSize();
	int getPolicy();
	void printEntries();
	virtual ~config();
};

#endif /* CONFIG_H_ */
