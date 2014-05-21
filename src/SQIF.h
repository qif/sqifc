//============================================================================
// Name        : SQIFC
// Author      : Quoc-Sang Phan <dark2bright@gmail.com>
// Version     : 0.1
// Copyright   : No
// Description : Symbolic Quantitative Information Flow Analysis for C
//============================================================================

#ifndef SQIF_H_
#define SQIF_H_

#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"

// state of a propositional variable
#define UNCHECKED 0 // no information
#define INVALID   1 // can be false
#define SAT	  2 // can be true
#define UNSAT     3 // always false
#define VALID     4 // always true
#define NONFIXED  5 // either true or false

using namespace std;

class sqif {
	config* pConf;
	int call; // index for the file to be generated
	int K; // data size, e.g. K = 32 for integer
	int N;
	int p[256]; // 64 is the biggest data type
	int policy; // maximum leakage to find
	// file processing
	string src;
	string asst;
	size_t found;
private:
	int check(int, bool);
	void doBCP();
	void createAssertion(const char*);
	bool isSAT(const char* pc);
	void symCount(const char* pc, int i);
	void symCountBCP(const char* pc, int i);
	void symCountBCPnew(const char* pc, int i);
public:
	void symbolicQIF();
	sqif(config*);
	virtual ~sqif();
};

#endif /* SQIF_H_ */
